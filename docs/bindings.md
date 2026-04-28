# 绑定层说明

绑定层负责使用 nanobind 将 C++ 类和方法暴露给 Python，形成最终的 `_ctp` 原生扩展模块。

## 模块入口

### main.cpp

```cpp
#include <nanobind/nanobind.h>
#include "bind_consts.h"
#include "bind_md_api.h"
#include "bind_td_api.h"

namespace nb = nanobind;

NB_MODULE(_ctp, m) {
    bind_consts(m);
    bind_md_api(m);
    bind_td_api(m);
}
```

`NB_MODULE(_ctp, m)` 定义了名为 `_ctp` 的 Python 扩展模块。模块初始化时依次注册：
1. 常量 (`bind_consts`)
2. 行情 API (`bind_md_api`)
3. 交易 API (`bind_td_api`)

## Python 包入口

### src/qtl_ctp_api/__init__.py

```python
from ._ctp import consts, MdApi, TdApi
```

Python 包 `qtl_ctp_api` 从 `_ctp` 模块导入三个公共对象：
- `consts`: CTP 常量集合
- `MdApi`: 行情 API 类
- `TdApi`: 交易 API 类

用户使用方式：

```python
import qtl_ctp_api as ctp

print(ctp.consts.THOST_FTDC_EXP_Normal)

class MyMdApi(ctp.MdApi):
    def OnFrontConnected(self):
        print("connected")
```

## 常量绑定

### bind_consts.cpp / bind_consts.h

`bind_consts.h`:

```cpp
void bind_consts(nb::module_ &);
```

`bind_consts.cpp` 通过 nanobind 将 CTP 头文件中定义的所有宏常量导出为 Python 属性，注册到 `m.consts` 子模块下。

示例导出形式：

```cpp
nb::module_ consts = m.def_submodule("consts");
consts.attr("THOST_FTDC_EXP_Normal") = THOST_FTDC_EXP_Normal;
consts.attr("THOST_FTDC_ICT_TaxNo") = THOST_FTDC_ICT_TaxNo;
// ... 数百个常量
```

这些常量包括：
- 交易所代码 (`THOST_FTDC_EIDT_...`)
- 产品类型 (`THOST_FTDC_PC_...`)
- 买卖方向 (`THOST_FTDC_D_...`)
- 开平标志 (`THOST_FTDC_OF_...`)
- 投机套保标志 (`THOST_FTDC_HF_...`)
- 报单状态 (`THOST_FTDC_OST_...`)
- 价格类型 (`THOST_FTDC_OPT_...`)
- 有效期类型 (`THOST_FTDC_TC_...`)
- 成交量类型 (`THOST_FTDC_VC_...`)
- 触发条件 (`THOST_FTDC_CC_...`)
- 强平原因 (`THOST_FTDC_FCC_...`)
- 持仓类型 (`THOST_FTDC_PT_...`)
- 持仓日期类型 (`THOST_FTDC_PSD_...`)
- 委托来源 (`THOST_FTDC_OSRC_...`)
- 报单提交状态 (`THOST_FTDC_OSS_...`)
- 成交类型 (`THOST_FTDC_TRDT_...`)
- 价格来源 (`THOST_FTDC_PSRC_...`)
- 交易角色 (`THOST_FTDC_ER_...`)
- 组合类型 (`THOST_FTDC_COMBT_...`)
- 产品生命周期状态 (`THOST_FTDC_IP_...`)
- 期权类型 (`THOST_FTDC_CP_...`)
- 合约交易状态 (`THOST_FTDC_IS_...`)
- 等...

## MdApi 绑定

### bind_md_api.cpp / bind_md_api.h

```cpp
void bind_md_api(nb::module_ &m) {
    nb::class_<MdApi, PyMdApi> md_api(m, "MdApi");
    md_api
        .def(nb::init<>())
        .def("CreateApi", &MdApi::CreateApi,
             nb::arg("flow_path"), nb::arg("is_production_mode") = true)
        .def("Release", &MdApi::Release)
        .def("Init", &MdApi::Init)
        .def("Join", &MdApi::Join)
        .def_static("GetApiVersion", &MdApi::GetApiVersion)
        .def("GetTradingDay", &MdApi::GetTradingDay)
        .def("RegisterFront", &MdApi::RegisterFront)
        .def("SubscribeMarketData", &MdApi::SubscribeMarketData)
        .def("UnSubscribeMarketData", &MdApi::UnSubscribeMarketData)
        // 请求方法
        .def("ReqUserLogin", &MdApi::ReqUserLogin)
        .def("ReqUserLogout", &MdApi::ReqUserLogout)
        // 回调方法（绑定到 PyOnXxx）
        .def("OnFrontConnected", &MdApi::PyOnFrontConnected)
        .def("OnFrontDisconnected", &MdApi::PyOnFrontDisconnected)
        // ... 更多回调
        ;
}
```

关键点：
- `nb::class_<MdApi, PyMdApi>`: 第二个模板参数 `PyMdApi` 是 trampoline 类，使 Python 子类能够重写虚方法
- `.def(nb::init<>())`: 暴露默认构造函数
- `nb::arg(...) = true`: 为 `CreateApi` 的 `is_production_mode` 提供默认参数
- `.def_static(...)`: 静态方法绑定
- 回调方法绑定到 `PyOnXxx` 而非 `OnXxx`，因为 `PyOnXxx` 是纯虚方法，实际执行的是 trampoline 路由逻辑

## TdApi 绑定

### bind_td_api.cpp / bind_td_api.h

结构与 `bind_md_api` 相同：

```cpp
void bind_td_api(nb::module_ &m) {
    nb::class_<TdApi, PyTdApi> td_api(m, "TdApi");
    td_api
        .def(nb::init<>())
        .def("CreateApi", &TdApi::CreateApi,
             nb::arg("flow_path"), nb::arg("is_production_mode") = true)
        // ... 生命周期方法
        // ... 请求方法 (ReqAuthenticate, ReqUserLogin, ReqOrderInsert ...)
        // ... 回调方法 (OnFrontConnected, OnRtnOrder, OnRtnTrade ...)
        ;
}
```

## 绑定设计原则

1. **只暴露必要方法**: 通过代码生成控制导出的 API 范围，减小二进制体积
2. **默认参数透传**: C++ 默认参数通过 `nb::arg(...).operator=()` 暴露给 Python
3. **回调绑定到纯虚方法**: 不直接绑定 `OnXxx`，而是绑定 `PyOnXxx`，确保 trampoline 机制生效
4. **类型自动转换**: nanobind 自动处理 `std::string` ↔ `str`, `std::vector<std::string>` ↔ `list[str]` 等转换
