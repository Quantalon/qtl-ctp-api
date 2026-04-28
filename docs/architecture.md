# 架构设计

## 整体架构

`qtl-ctp-api` 是一个 C++ 原生扩展模块，通过 nanobind 将 CTP（综合交易平台）的 C++ API 暴露给 Python。整体分为三层：

```
┌─────────────────────────────────────────┐
│           Python 用户层                  │
│   class MyApi(MdApi): ...               │
└─────────────────────────────────────────┘
                    │
                    ▼  nanobind trampoline
┌─────────────────────────────────────────┐
│           C++ 封装层                     │
│   MdApi / TdApi + PyMdApi / PyTdApi    │
│   DispatchQueue 任务队列                │
│   gbk_to_utf8 编码转换                  │
└─────────────────────────────────────────┘
                    │
                    ▼  CTP 原生接口调用
┌─────────────────────────────────────────┐
│           CTP 原生库                     │
│   libthostmduserapi_se.so               │
│   libthosttraderapi_se.so               │
└─────────────────────────────────────────┘
```

## 核心设计模式

### 1. Trampoline 模式（回调桥接）

CTP 使用 SPI（Service Provider Interface）模式进行异步回调：用户需要继承 `CThostFtdcMdSpi` 或 `CThostFtdcTraderSpi`，重写虚函数来接收事件。

为了让 Python 子类能够重写这些 C++ 虚函数，项目使用了 nanobind 的 **Trampoline** 机制：

- **抽象基类** (`MdApi` / `TdApi`): 继承自 CTP 的 SPI 类，声明纯虚方法 `PyOnXxx`，并在 CTP 原始回调 (`OnXxx`) 中通过 `DispatchQueue` 异步调用 `PyOnXxx`
- **Trampoline 类** (`PyMdApi` / `PyTdApi`): 继承自 `MdApi` / `TdApi`，使用 `NB_TRAMPOLINE` 和 `NB_OVERRIDE_PURE_NAME` 将 `PyOnXxx` 桥接到 Python 中同名方法

示例代码（`md_api.h`）:

```cpp
class MdApi : public CThostFtdcMdSpi {
    // CTP 原始回调
    void OnFrontConnected() override;
    // Python 可调用的纯虚方法
    virtual void PyOnFrontConnected() = 0;
};

class PyMdApi final : public MdApi {
    NB_TRAMPOLINE(MdApi, 9);
    void PyOnFrontConnected() override {
        NB_OVERRIDE_PURE_NAME("OnFrontConnected", PyOnFrontConnected);
    }
};
```

在 Python 中：

```python
class MyApi(MdApi):
    def OnFrontConnected(self):
        print("连接成功")
```

当 CTP 触发 `OnFrontConnected` 时，调用链为：

```
CTP 内部线程 → MdApi::OnFrontConnected()
                              ↓
                     DispatchQueue::dispatch()
                              ↓
              工作线程 + GIL → PyMdApi::PyOnFrontConnected()
                              ↓
                     NB_OVERRIDE_PURE_NAME
                              ↓
                     Python MyApi.OnFrontConnected()
```

### 2. DispatchQueue 解耦模式

CTP 在其内部线程中同步调用 SPI 回调。如果 Python 端处理耗时，会导致 CTP 内部队列溢出崩溃。

解决方案：`DispatchQueue` 实现了一个 **生产者-消费者队列**：

- **生产者**: CTP 回调线程收到事件后，立即将 lambda 任务入队，然后立即返回
- **消费者**: 单独的 C++ 工作线程不断从队列取出任务执行，获取 Python GIL 后调用 Python 方法

这样将 CTP 的同步回调转换为异步处理，避免了阻塞 CTP 内部线程。

### 3. 数据转换模式

所有 CTP 结构体与 Python dict 之间的转换都是手写的（非自动生成）。转换规则：

- **字符串字段**: 通过 `gbk_to_utf8()` 从 GBK 转换为 UTF-8
- **数值字段**: 直接传递，浮点数字段通过 `adjust_number()` 处理最大值哨兵
- **字符数组**: 通过 `strncpy` 安全复制到定长字段
- **请求字段**: Python dict → C struct，通过 `set_field()` 模板函数填充

## 线程模型

```
主线程 (Python)
    │
    ├── 调用 MdApi::CreateApi() / Init() / RegisterFront()
    │
    └── 用户业务逻辑

CTP 内部线程
    │
    ├── 网络连接事件 → OnFrontConnected() → dispatch(lambda)
    ├── 行情推送 → OnRtnDepthMarketData() → dispatch(lambda)
    └── 响应回报 → OnRspUserLogin() → dispatch(lambda)

DispatchQueue 工作线程
    │
    └── 从队列取出 lambda → 获取 GIL → 调用 Python 回调
```

## 文件依赖关系

```
main.cpp
    ├── bind_consts.h ──→ bind_consts.cpp
    ├── bind_md_api.h ──→ bind_md_api.cpp ──→ md_api.h ──→ md_api.cpp
    │                                              │
    │                                              ├── dispatch_queue.h
    │                                              ├── utils.h ──→ iconv.hpp
    │                                              └── ThostFtdcMdApi.h (CTP)
    └── bind_td_api.h ──→ bind_td_api.cpp ──→ td_api.h ──→ td_api.cpp
                                                   │
                                                   ├── dispatch_queue.h
                                                   ├── utils.h
                                                   └── ThostFtdcTraderApi.h (CTP)
```
