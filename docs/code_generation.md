# 代码生成

项目中大量重复性绑定代码是通过代码生成脚本自动生成的，位于 `scripts/generator/` 目录。

## 生成器位置

```
scripts/generator/
├── generate.py          # 主生成脚本
├── templates/           # Jinja2 模板目录
│   ├── bind_consts.cpp.tpl
│   ├── bind_md_api.cpp.tpl
│   ├── bind_td_api.cpp.tpl
│   ├── md_api.h.tpl
│   ├── md_api.cpp.tpl
│   ├── td_api.h.tpl
│   └── td_api.cpp.tpl
└── requirements.txt     # 生成器依赖
```

## 使用方法

```bash
cd scripts/generator
pip install -r requirements.txt
python generate.py
```

生成的文件会输出到 `../../src/` 目录，覆盖以下文件：
- `src/bind_consts.cpp`
- `src/bind_md_api.cpp`
- `src/bind_td_api.cpp`
- `src/md_api.h`
- `src/md_api.cpp`
- `src/td_api.h`
- `src/td_api.cpp`

## 生成原理

生成器使用 `CppHeaderParser` 库解析 CTP 头文件：

- `ThostFtdcUserApiDataType.h` — 数据类型定义
- `ThostFtdcUserApiStruct.h` — 结构体定义
- `ThostFtdcMdApi.h` — 行情 API 接口定义
- `ThostFtdcTraderApi.h` — 交易 API 接口定义

然后使用 Jinja2 模板引擎渲染生成 C++ 代码。

## 配置导出方法

在 `generate.py` 中，通过列表控制需要导出的方法：

```python
# MdApi 回调方法
self.exported_md_on_methods = [
    'OnFrontConnected',
    'OnFrontDisconnected',
    'OnHeartBeatWarning',
    'OnRspUserLogin',
    'OnRspUserLogout',
    'OnRspError',
    'OnRspSubMarketData',
    'OnRspUnSubMarketData',
    'OnRtnDepthMarketData',
]

# MdApi 请求方法
self.exported_md_req_methods = [
    'ReqUserLogin',
    'ReqUserLogout',
]

# TdApi 回调方法
self.exported_td_on_methods = [
    'OnFrontConnected',
    'OnFrontDisconnected',
    'OnHeartBeatWarning',
    'OnRspAuthenticate',
    'OnRspUserLogin',
    'OnRspUserLogout',
    'OnRspOrderInsert',
    'OnRspOrderAction',
    'OnRspSettlementInfoConfirm',
    'OnRspQryOrder',
    'OnRspQryTrade',
    'OnRspQryInvestorPosition',
    'OnRspQryTradingAccount',
    'OnRspQryInstrument',
    'OnRspError',
    'OnRtnOrder',
    'OnRtnTrade',
    'OnErrRtnOrderInsert',
    'OnErrRtnOrderAction',
    'OnRtnInstrumentStatus',
    'OnRspQrySettlementInfo',
]

# TdApi 请求方法
self.exported_td_req_methods = [
    'ReqAuthenticate',
    'ReqUserLogin',
    'ReqUserLogout',
    'ReqUserPasswordUpdate',
    'ReqOrderInsert',
    'ReqOrderAction',
    'ReqSettlementInfoConfirm',
    'ReqQryOrder',
    'ReqQryTrade',
    'ReqQryInvestorPosition',
    'ReqQryTradingAccount',
    'ReqQryInstrument',
    'ReqQrySettlementInfo',
]
```

**修改导出列表后必须重新运行 `python generate.py`**。

## 为何需要代码生成

1. **减少手写重复代码**: CTP API 有数百个结构体和回调，手写所有字段映射容易出错且维护困难
2. **精确控制导出范围**: 只导出实际需要的子集，减小二进制体积和编译时间
3. **CTP 版本升级**: 当 CTP 发布新版本时，只需更新头文件并重新生成即可
4. **一致性保证**: 头文件、绑定代码、类声明由同一数据源生成，避免不一致

## 注意事项

- 生成器解析的是 `libs/ctp/include/` 中的 CTP 头文件
- 生成的是 **框架代码**，具体的字段转换逻辑（如 `gbk_to_utf8`, `adjust_number`）可能需要手动微调
- 当前项目中，部分生成后的文件可能经过人工修改，重新生成时需注意不要覆盖自定义改动
- 建议在版本控制中同时保留生成脚本和生成结果，方便追踪差异
