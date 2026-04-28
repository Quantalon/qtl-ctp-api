# qtl-ctp-api 源码文档

本文档基于 `src/` 目录中的源代码，对 `qtl-ctp-api` 项目的内部实现进行详细说明。

## 文档目录

- [架构设计](architecture.md) — 项目整体架构、设计模式与数据流转
- [MdApi 行情接口](md_api.md) — 行情 API 的 C++ 实现与 Python 暴露方法
- [TdApi 交易接口](td_api.md) — 交易 API 的 C++ 实现与 Python 暴露方法
- [DispatchQueue 任务队列](dispatch_queue.md) — 线程安全的回调分发队列
- [工具函数](utils.md) — 字符编码转换、字段设置、数值处理等工具
- [绑定层说明](bindings.md) — nanobind 绑定入口与模块暴露
- [代码生成](code_generation.md) — 代码生成脚本的使用与原理

## 项目基本信息

- **语言**: C++20 + Python
- **绑定库**: nanobind
- **构建工具**: scikit-build-core + CMake
- **目标平台**: Linux only
- **CTP 版本**: v6.7.11_20250617
- **包版本**: 6.7.11

## 源码文件清单

| 文件 | 说明 |
|------|------|
| `src/main.cpp` | nanobind 模块入口，注册所有绑定 |
| `src/md_api.h` / `md_api.cpp` | 行情 API 实现 |
| `src/td_api.h` / `td_api.cpp` | 交易 API 实现 |
| `src/dispatch_queue.h` / `dispatch_queue.cpp` | 回调任务队列 |
| `src/utils.h` | 工具函数（编码转换、字段填充等） |
| `src/iconv.hpp` | iconv C++ 封装 |
| `src/bind_consts.cpp` / `bind_consts.h` | 常量绑定 |
| `src/bind_md_api.cpp` / `bind_md_api.h` | 行情 API 绑定 |
| `src/bind_td_api.cpp` / `bind_td_api.h` | 交易 API 绑定 |
| `src/qtl_ctp_api/__init__.py` | Python 包入口，导出公共 API |
