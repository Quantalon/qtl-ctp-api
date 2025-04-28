# QTL CTP API

> qtl-ctp-api

又一个面向 Python 的 CTP 客户端 API 包

**Only for Linux**

## 特点

- 使用 nanobind 作为 C++ 到 Python 的绑定库，相对于 pybind11 ，nanobind 效率更高，其官方文档中所述：
    > More concretely, benchmarks show up to ~4× faster compile time, ~5× smaller binaries, and ~10× lower runtime overheads compared to pybind11. nanobind also outperforms Cython in important metrics (3-12× binary size reduction, 1.6-4× compilation time reduction, similar runtime performance).

- 在 C++ 端另外实现了一个任务队列，防止因为 Python 端处理不及时导致的 CTP 库内部运行时队列溢出异常。

- 使用 scikit_build + CMake 作为包构建工具，安装、编译、部署非常方便。

- 使用脚本生成 C++ 代码，其中可以配置需要实现绑定的方法，提高编译速度以及去掉运行时不必要的代码。

- 使用 iconv 进行字符编码转换。当前 libiconv 已经包含到 libc 中，无需再通过 locale 、操作 locale-gen 的方式进行转换。

- C++ 代码方面尽量按照新标准的 C++ 代码标准实现，利用现代 C++ 的各种优势。

- 只面向 Linux ，无需增加很多冗余的跨平台代码与配置。

## Install

*需要系统内有 C++ 编译相关工具，对于 Ubuntu Server 可以通过 `sudo apt install build-essential` 安装*

```
pip install qtl-ctp-api
```

## Quick Start

`./tests` 目录下有简单使用范例

- test_md.py 行情获取相关
- test_td.py 交易柜台相关

## CTP version

- CTP: v6.7.9

