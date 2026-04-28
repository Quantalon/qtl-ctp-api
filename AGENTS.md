# qtl-ctp-api 项目指南

本文档面向 AI 编程助手，提供在 `qtl-ctp-api` 仓库中工作所需的全部上下文信息。

## 项目概述

`qtl-ctp-api` 是 CTP（综合交易平台）API 的 Python 封装库，用于连接国内期货 CTP 交易柜台。

- **语言**: C++20 + Python
- **绑定库**: nanobind（替代 pybind11，编译更快、二进制更小、运行时开销更低）
- **构建工具**: scikit-build-core + CMake
- **目标平台**: **仅 Linux**
- **CTP 版本**: v6.7.13_20260225
- **包版本**: 6.7.13
- **Python 最低版本**: 3.8
- **Wheel 稳定 ABI**: cp312（为 CPython 3.12+ 构建稳定 ABI wheel）

暴露两个核心 API：
- `MdApi` — 行情接口（订阅 Tick、登录登出）
- `TdApi` — 交易接口（报单、撤单、查询持仓/资金/合约/成交）

## 项目结构

```
qtl-ctp-api/
├── pyproject.toml              # Python 包与构建配置（scikit-build-core）
├── CMakeLists.txt              # CMake 构建配置
├── README.md                   # 面向用户的简介
├── CLAUDE.md                   # Claude Code 专用速查指南
├── AGENTS.md                   # 本文档
│
├── src/                        # C++ 源码与 Python 包
│   ├── main.cpp                # nanobind 模块入口（_ctp）
│   ├── md_api.h / md_api.cpp   # 行情 API 实现与 trampoline
│   ├── td_api.h / td_api.cpp   # 交易 API 实现与 trampoline
│   ├── dispatch_queue.h / .cpp # 回调任务队列
│   ├── utils.h                 # gbk_to_utf8、set_field、adjust_number
│   ├── iconv.hpp               # iconv 的 C++ RAII 封装
│   ├── bind_consts.h / .cpp    # 常量绑定
│   ├── bind_md_api.h / .cpp    # 行情 API nanobind 绑定
│   ├── bind_td_api.h / .cpp    # 交易 API nanobind 绑定
│   └── qtl_ctp_api/
│       └── __init__.py         # Python 包入口：from ._ctp import consts, MdApi, TdApi
│
├── libs/ctp/                   # CTP 原生库
│   ├── include/                # CTP 头文件（ThostFtdc*.h）
│   └── lib/                    # CTP 动态库（.so）
│
├── tests/                      # 测试脚本（非 pytest）
│   ├── test_basic.py           # 模块导入与常量测试
│   ├── test_md.py              # 行情连接测试（需 CTP 账号和网络）
│   ├── test_td.py              # 交易连接测试（需 CTP 账号和网络）
│   ├── test_td_private.py      # 包含真实账号信息，**不应提交到版本控制**
│   └── flow*.con               # CTP 流文件（运行时生成）
│
├── scripts/generator/          # 代码生成器
│   ├── generate.py             # 主生成脚本
│   ├── requirements.txt        # 生成器依赖（CppHeaderParser、Jinja2）
│   └── templates/              # Jinja2 模板（*.tpl）
│
├── docs/                       # 源码文档（中文）
│   ├── architecture.md         # 架构设计
│   ├── bindings.md             # 绑定层说明
│   ├── code_generation.md      # 代码生成
│   ├── dispatch_queue.md       # 任务队列
│   ├── md_api.md               # 行情接口
│   ├── td_api.md               # 交易接口
│   └── utils.md                # 工具函数
│
└── .github/workflows/
    └── publish-to-pypi.yml     # master 推送时自动发布 sdist 到 PyPI
```

## 构建与安装

### 标准安装（创建隔离构建环境）

```bash
pip install .
```

### 开发安装（更快，需预先安装构建依赖）

```bash
pip install nanobind scikit-build-core[pyproject]
pip install --no-build-isolation -ve .
```

### 开启自动重编译

导入时自动重新编译 C++ 改动：

```bash
pip install --no-build-isolation -ve . -Ceditable.rebuild=true
```

若未开启自动重编译，每次修改 C++ 源文件后需重新执行开发安装命令。

### CMake 直接构建警告

`CMakeLists.txt` 顶部明确警告：**不要直接运行 cmake**。它期望由 `scikit-build` 调用。直接运行几乎不会产生正确结果。

## 测试策略

本项目**不使用 pytest**。测试通过直接运行 Python 脚本完成：

```bash
# 基础测试（无需外部依赖）
python tests/test_basic.py

# 行情连接测试（需要有效的 CTP 经纪商账号和前置机地址）
python tests/test_md.py

# 交易连接测试（需要有效的 CTP 经纪商账号和前置机地址）
python tests/test_td.py
```

- `test_md.py` / `test_td.py` 中硬编码了示例前置机地址，但实际运行需要填写有效的 `broker_id`、`user_id`、`password` 等。
- `test_td_private.py` 包含真实账号信息，**已被 `.gitignore` 排除，不应提交到版本控制**。
- 测试运行时会在 `tests/` 目录下生成 `flow*.con` 流文件，这些也已被 `.gitignore` 排除。

## 代码风格与约定

### C++ 代码

- **标准**: C++20
- **缩进**: 4 空格
- **命名**:
  - 类名: `PascalCase`（`MdApi`、`DispatchQueue`）
  - 方法名: `PascalCase`（与 CTP 原始接口保持一致）
  - 私有成员: 下划线后缀（`lock_`、`queue_`）
  - 纯虚方法前缀: `Py`（`PyOnFrontConnected`）
- **头文件保护**: `#ifndef FILE_H / #define FILE_H / #endif`
- **现代 C++ 特性**: 使用 `std::optional`、`std::unique_ptr`、`std::thread`、`std::mutex`、`std::condition_variable`、lambda 表达式
- **字符编码**: 所有 CTP 字符串字段（GBK 编码）在传入 Python 前必须通过 `gbk_to_utf8()` 转为 UTF-8
- **数值处理**: 浮点数字段通过 `adjust_number()` 将 CTP 哨兵最大值（`numeric_limits::max()`）转为 0

### Python 代码

- 测试脚本和生成器使用标准 Python 风格
- 类名 `PascalCase`，方法名 `snake_case`
- 不使用类型注解（现有代码中没有）

## 核心架构

### 1. Trampoline 模式（Python 回调桥接）

CTP 通过 SPI（Service Provider Interface）虚函数回调事件。为了让 Python 子类能够重写这些 C++ 虚函数，项目使用 nanobind 的 trampoline 机制：

- **`MdApi` / `TdApi`** — 抽象基类，继承自 CTP 的 `CThostFtdcMdSpi` / `CThostFtdcTraderSpi`
  - 声明纯虚方法 `PyOnXxx`
  - 在 CTP 原始回调 (`OnXxx`) 中通过 `DispatchQueue` 异步调用 `PyOnXxx`
- **`PyMdApi` / `PyTdApi`** — trampoline 子类，继承自 `MdApi` / `TdApi`
  - 使用 `NB_TRAMPOLINE(MdApi, N)` 和 `NB_OVERRIDE_PURE_NAME` 将 `PyOnXxx` 桥接到 Python 中同名方法

Python 中重写 `OnFrontConnected`、`OnRtnOrder` 等即可接收事件。

### 2. DispatchQueue 解耦模式

CTP 在其内部线程中**同步调用** SPI 回调。如果 Python 端处理耗时，会导致 CTP 内部消息队列积压，最终引发队列溢出异常。

解决方案：`DispatchQueue` 实现了一个生产者-消费者队列：
- **生产者**: CTP 回调线程收到事件后，立即将 lambda 任务入队，然后立即返回
- **消费者**: 单独的 C++ 工作线程不断从队列取出任务执行，获取 Python GIL（`nb::gil_scoped_acquire`）后调用 Python 方法

所有回调按入队顺序依次执行，天然保证了事件顺序性。

### 3. 数据转换模式

所有 CTP 结构体与 Python `dict` 之间的转换都是手写的（非自动生成）。

- **请求**: Python `dict` → C struct，通过 `set_field()` 模板函数填充
  - `set_field(T &field, const nb::dict &input, const char *field_name)` — 数值字段
  - `set_field(T &field, const nb::dict &input, const char *field_name, size_t size)` — 字符串字段（使用 `strncpy`）
- **响应**: C struct → Python `dict`，手写逐字段映射
- **字符串**: 全部通过 `gbk_to_utf8()` 从 GBK 转 UTF-8（`src/utils.h` 中定义，使用线程局部 `iconvpp::converter`）
- **浮点数**: 通过 `adjust_number()` 将 CTP 哨兵最大值转为 0

### 4. 线程模型

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

## 代码生成

项目中大量重复性绑定代码是通过代码生成脚本自动生成的。

### 使用方法

```bash
cd scripts/generator
pip install -r requirements.txt
python generate.py
```

生成器解析 `libs/ctp/include/` 中的 CTP 头文件，通过 Jinja2 模板输出到 `src/`，覆盖以下文件：
- `src/bind_consts.cpp`
- `src/bind_md_api.cpp`
- `src/bind_td_api.cpp`
- `src/md_api.h`
- `src/md_api.cpp`
- `src/td_api.h`
- `src/td_api.cpp`

### 配置导出方法

在 `generate.py` 中，通过以下列表控制需要导出的方法：

- `exported_md_on_methods` — MdApi 回调方法
- `exported_md_req_methods` — MdApi 请求方法
- `exported_td_on_methods` — TdApi 回调方法
- `exported_td_req_methods` — TdApi 请求方法

**修改导出列表后必须重新运行 `python generate.py`**。

### 注意事项

- 生成器解析的是 `libs/ctp/include/` 中的 CTP 头文件
- 生成的是**框架代码**，具体的字段转换逻辑（如 `gbk_to_utf8`、`adjust_number`）可能需要手动微调
- 当前项目中，部分生成后的文件可能经过人工修改，重新生成时需注意不要覆盖自定义改动
- 建议在版本控制中同时保留生成脚本和生成结果，方便追踪差异

## CI / 发布

`.github/workflows/publish-to-pypi.yml`：
- 触发条件: `master` 分支推送时
- 运行环境: `ubuntu-24.04`
- 构建产物: 仅源码分发包（sdist）
- 二进制 wheel **不在 CI 中构建**
- 发布目标: PyPI（使用 `PYPI_API_TOKEN` secret）

## 安全与运维注意事项

1. **真实账号信息**: `tests/test_td_private.py` 包含真实 CTP 账号密码，已被 `.gitignore` 排除。务必确保此类文件不会被意外提交。
2. **流文件**: CTP 运行时会在 `flow_path` 目录生成 `.con` 流文件，这些文件包含会话状态，不应共享或提交。
3. **平台限制**: 项目仅面向 Linux。CTP 动态库（`.so`）和构建配置均针对 Linux，不要添加跨平台代码。
4. **GIL 管理**: 在 `DispatchQueue` 工作线程中调用 Python 回调前必须获取 GIL（`nb::gil_scoped_acquire`），否则会导致崩溃。
5. **数据复制**: CTP 传入的指针在回调返回后可能失效，所有数据必须在入队前通过 `std::optional` 复制到 lambda 中。

## 常用修改场景

### 新增一个 CTP 回调或请求方法

1. 修改 `scripts/generator/generate.py` 中对应的 `exported_*_methods` 列表
2. 运行 `cd scripts/generator && python generate.py`
3. 检查生成的代码，如有需要手动微调字段转换逻辑
4. 重新构建并测试

### 修改字段转换逻辑

如果某个字段的转换方式需要调整（例如新增字段、修改编码处理方式），直接编辑 `src/md_api.cpp` 或 `src/td_api.cpp` 中对应回调的实现。注意：这些修改在重新运行代码生成器时会被覆盖，如需持久化，应修改对应的 Jinja2 模板。

### 升级 CTP 版本

1. 替换 `libs/ctp/include/` 中的头文件和 `libs/ctp/lib/` 中的动态库
2. 更新 `pyproject.toml` 中的版本号
3. 运行代码生成器重新生成绑定代码
4. 检查新增/变更的字段和接口，手动调整转换逻辑
5. 构建并测试

## 依赖关系图

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
