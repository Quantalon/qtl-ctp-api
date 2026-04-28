# CLAUDE.md

本文档为 Claude Code (claude.ai/code) 提供在本仓库中工作时的指导信息。

## 项目概述

`qtl-ctp-api` 是 CTP（综合交易平台）API 的 Python 封装库，用于连接国内期货 CTP 交易柜台。

- **语言**: C++20 + Python
- **绑定库**: nanobind（替代 pybind11）
- **构建工具**: scikit-build-core + CMake
- **目标平台**: Linux only
- **CTP 版本**: v6.7.13_20260225
- **包版本**: 6.7.13

暴露两个核心 API：
- `MdApi` — 行情接口（订阅 Tick、登录登出）
- `TdApi` — 交易接口（报单、撤单、查询持仓/资金/合约/成交）

## 构建命令

标准安装（创建隔离构建环境）：

```bash
pip install .
```

开发安装（更快，需预先安装构建依赖）：

```bash
pip install nanobind scikit-build-core[pyproject]
pip install --no-build-isolation -ve .
```

开启自动重编译（导入时自动重新编译 C++ 改动）：

```bash
pip install --no-build-isolation -ve . -Ceditable.rebuild=true
```

若未开启自动重编译，每次修改 C++ 源文件后需重新执行上述开发安装命令。

## 测试

测试不使用 pytest，直接运行可执行脚本：

```bash
python tests/test_basic.py   # 模块导入与常量测试
python tests/test_md.py      # 行情连接测试（需 CTP 账号和网络）
python tests/test_td.py      # 交易连接测试（需 CTP 账号和网络）
```

`test_md.py` / `test_td.py` 需要有效的 CTP 经纪商账号和前置机地址。`test_td_private.py` 包含真实账号信息，**不应提交到版本控制**。

## 代码生成

绑定代码由 `scripts/generator/` 自动生成：

```bash
cd scripts/generator
pip install -r requirements.txt
python generate.py
```

生成器解析 `libs/ctp/include/` 中的 CTP 头文件，通过 Jinja2 模板输出到 `src/`：
- `bind_consts.cpp` / `bind_md_api.cpp` / `bind_td_api.cpp`
- `md_api.h` / `md_api.cpp`
- `td_api.h` / `td_api.cpp`

修改 `generate.py` 中的 `exported_md_on_methods`、`exported_td_on_methods` 等列表可控制导出范围。修改后需重新生成。

## 架构要点

### Trampoline 模式（Python 回调桥接）

CTP 通过 SPI 虚函数回调事件。项目使用 nanobind 的 `NB_TRAMPOLINE` 让 Python 子类能够重写这些回调：

- `MdApi` / `TdApi` — 抽象基类，声明 `PyOnXxx` 纯虚方法
- `PyMdApi` / `PyTdApi` — trampoline 子类，使用 `NB_OVERRIDE_PURE_NAME` 将调用路由到 Python 的同名方法

Python 中重写 `OnFrontConnected`、`OnRtnOrder` 等即可接收事件。

### DispatchQueue 解耦

CTP 在其内部线程同步调用回调。为防止 Python 处理耗时导致 CTP 队列溢出崩溃，所有回调只做一件事：**将 lambda 任务入队**，然后立即返回。

独立的 C++ 工作线程从 `DispatchQueue` 中消费任务，获取 GIL（`nb::gil_scoped_acquire`）后调用 Python 方法。这实现了 CTP 线程与 Python 执行的完全解耦。

### 数据转换

- **请求**: Python `dict` → C struct，通过 `set_field()` 模板填充
- **响应**: C struct → Python `dict`，手写逐字段映射
- **字符串**: 全部通过 `gbk_to_utf8()` 从 GBK 转 UTF-8（`src/utils.h` 中定义，使用线程局部 `iconvpp::converter`）
- **浮点数**: 通过 `adjust_number()` 将 CTP 哨兵最大值转为 0

### 关键文件

| 文件 | 作用 |
|------|------|
| `src/main.cpp` | nanobind 模块入口 (`_ctp`) |
| `src/md_api.cpp/h` | 行情 API 实现与 trampoline |
| `src/td_api.cpp/h` | 交易 API 实现与 trampoline |
| `src/dispatch_queue.cpp/h` | 回调任务队列 |
| `src/utils.h` | `gbk_to_utf8()`、`set_field()`、`adjust_number()` |
| `src/iconv.hpp` | `iconv` 的 C++ RAII 封装 |
| `src/bind_*.cpp/h` | nanobind 绑定代码 |
| `libs/ctp/include/` | CTP 头文件 |
| `libs/ctp/lib/` | CTP 动态库 (`.so`) |

## CI / 发布

`.github/workflows/publish-to-pypi.yml`：
- `master` 分支推送时触发
- 仅构建并发布源码分发包（sdist）到 PyPI
- 二进制 wheel 不在 CI 中构建
