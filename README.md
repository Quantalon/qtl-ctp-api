# QTL CTP API

> qtl-ctp-api

又一个面向 Python 的 CTP 客户端 API 包，用于连接国内期货公司 CTP 柜台进行期货交易市场相关操作。

**Only for Linux**

## 特点

- 使用 nanobind 作为 C++ 到 Python 的绑定库。相对于 pybind11，nanobind 在编译时间、二进制体积和运行时开销方面均有显著优势，官方基准测试表明：
    > 编译时间快约 4 倍，二进制体积小约 5 倍，运行时开销低约 10 倍。在二进制体积和编译时间方面，nanobind 也显著优于 Cython（二进制体积缩小 3-12 倍，编译时间缩短 1.6-4 倍，运行时性能相近）。

- 在 C++ 端实现了 DispatchQueue 任务队列，将 CTP 内部线程的同步回调转换为异步派发，防止因 Python 端处理耗时导致 CTP 内部消息队列溢出异常。

- 使用 scikit-build-core + CMake 作为包构建工具，支持标准 pip 安装和可编辑开发模式，安装、编译、部署非常方便。

- 使用脚本自动生成 C++ 绑定代码，可通过配置列表灵活控制需要导出的接口方法，减少编译时间和二进制体积。

- 使用 iconv 进行 GBK 到 UTF-8 的字符编码转换。libiconv 已包含于现代 libc 中，无需通过 locale 或 locale-gen 进行额外配置。

- C++ 代码基于 C++20 标准实现，充分利用 `std::optional`、`std::unique_ptr`、lambda 表达式、智能指针等现代 C++ 特性。

- 仅面向 Linux 平台，无需引入冗余的跨平台兼容代码与配置。

## Install

*需要系统内安装 C++ 编译工具链。对于 Ubuntu/Debian 系统，可通过以下命令安装：*

```bash
sudo apt install build-essential
```

```bash
pip install qtl-ctp-api
```

## Quick Start

`./tests` 目录下提供了使用范例：

- `test_md.py` —— 行情接口（MdApi）连接与订阅示例
- `test_td.py` —— 交易接口（TdApi）连接与报单示例

## CTP version

- CTP: v6.7.13_20260225
