# MdApi 行情接口

`MdApi` 封装了 CTP 行情接口 (`CThostFtdcMdApi` / `CThostFtdcMdSpi`)，提供市场数据订阅、登录等功能。

## 类定义

```cpp
class MdApi : public CThostFtdcMdSpi {
private:
    std::unique_ptr<DispatchQueue> queue_;
    CThostFtdcMdApi* api_;
// ...
};
```

## 生命周期方法

### CreateApi

```cpp
void CreateApi(const std::string &flow_path, bool is_production_mode = true);
```

创建行情 API 实例。

- `flow_path`: 流文件存放目录，用于存储通信过程中的临时数据
- `is_production_mode`: 是否生产环境模式

内部实现：
- 创建 `DispatchQueue` 实例
- 调用 `CThostFtdcMdApi::CreateFtdcMdApi()` 创建原生 API
- 注册 SPI (`RegisterSpi(this)`)

### Release

```cpp
void Release();
```

释放 API 资源。先注销 SPI，再调用原生 `Release()`，最后清空队列。

### Init

```cpp
void Init();
```

初始化并启动与行情前置机的连接线程。

### Join

```cpp
int Join();
```

阻塞等待 API 线程结束。

### GetApiVersion (静态)

```cpp
static std::string GetApiVersion();
```

获取 CTP API 版本号。

### GetTradingDay

```cpp
std::string GetTradingDay();
```

获取当前交易日。

## 连接方法

### RegisterFront

```cpp
void RegisterFront(const std::string &front_address);
```

注册行情前置机地址，例如 `tcp://180.166.103.21:55213`。

## 行情订阅方法

### SubscribeMarketData

```cpp
int SubscribeMarketData(const std::vector<std::string> &instrument_ids);
```

订阅指定合约的行情。

### UnSubscribeMarketData

```cpp
int UnSubscribeMarketData(const std::vector<std::string> &instrument_ids);
```

取消订阅指定合约的行情。

## 请求方法

### ReqUserLogin

```cpp
int ReqUserLogin(const nb::dict &data, int request_id);
```

发送登录请求。

`data` 字典支持的字段：

| 字段名 | 类型 | 说明 |
|--------|------|------|
| `TradingDay` | string | 交易日 |
| `BrokerID` | string | 经纪公司代码 |
| `UserID` | string | 用户代码 |
| `Password` | string | 密码 |
| `UserProductInfo` | string | 用户端产品信息 |
| `InterfaceProductInfo` | string | 接口端产品信息 |
| `ProtocolInfo` | string | 协议信息 |
| `MacAddress` | string | Mac 地址 |
| `OneTimePassword` | string | 动态密码 |
| `LoginRemark` | string | 登录备注 |
| `ClientIPPort` | int | 客户端 IP 端口 |
| `ClientIPAddress` | string | 客户端 IP 地址 |

### ReqUserLogout

```cpp
int ReqUserLogout(const nb::dict &data, int request_id);
```

发送登出请求。

`data` 字典支持的字段：

| 字段名 | 类型 | 说明 |
|--------|------|------|
| `BrokerID` | string | 经纪公司代码 |
| `UserID` | string | 用户代码 |

## 回调方法（Python 侧重写）

Python 子类需重写以下方法接收事件：

### OnFrontConnected

```python
def OnFrontConnected(self):
    """前置机连接成功"""
```

### OnFrontDisconnected

```python
def OnFrontDisconnected(self, reason):
    """前置机断开连接
    
    Args:
        reason: 断开原因代码
    """
```

### OnHeartBeatWarning

```python
def OnHeartBeatWarning(self, time_lapse):
    """心跳超时警告
    
    Args:
        time_lapse: 距离上次心跳的时间间隔
    """
```

### OnRspUserLogin

```python
def OnRspUserLogin(self, data, error, request_id, is_last):
    """登录响应
    
    Args:
        data: dict，包含以下字段：
            - TradingDay, LoginTime, BrokerID, UserID, SystemName
            - FrontID, SessionID, MaxOrderRef
            - SHFETime, DCETime, CZCETime, FFEXTime, INETime, GFEXTime
            - SysVersion, LoginDRIdentityID, UserDRIdentityID
            - LastLoginTime, ReserveInfo
        error: dict，包含 ErrorID 和 ErrorMsg
        request_id: 请求 ID
        is_last: 是否为最后一条响应
    """
```

### OnRspUserLogout

```python
def OnRspUserLogout(self, data, error, request_id, is_last):
    """登出响应
    
    Args:
        data: dict，包含 BrokerID, UserID
        error: dict，包含 ErrorID 和 ErrorMsg
    """
```

### OnRspError

```python
def OnRspError(self, error, request_id, is_last):
    """错误响应
    
    Args:
        error: dict，包含 ErrorID 和 ErrorMsg
    """
```

### OnRspSubMarketData

```python
def OnRspSubMarketData(self, data, error, request_id, is_last):
    """订阅行情响应
    
    Args:
        data: dict，包含 InstrumentID
        error: dict，包含 ErrorID 和 ErrorMsg
    """
```

### OnRspUnSubMarketData

```python
def OnRspUnSubMarketData(self, data, error, request_id, is_last):
    """取消订阅行情响应
    
    Args:
        data: dict，包含 InstrumentID
        error: dict，包含 ErrorID 和 ErrorMsg
    """
```

### OnRtnDepthMarketData

```python
def OnRtnDepthMarketData(self, data):
    """深度行情推送（实时 Tick 数据）
    
    Args:
        data: dict，包含以下字段：
            - TradingDay, ExchangeID, UpdateTime, UpdateMillisec
            - ActionDay, InstrumentID, ExchangeInstID
            - LastPrice, PreSettlementPrice, PreClosePrice, PreOpenInterest
            - OpenPrice, HighestPrice, LowestPrice, ClosePrice, SettlementPrice
            - UpperLimitPrice, LowerLimitPrice, PreDelta, CurrDelta
            - Volume, Turnover, OpenInterest, AveragePrice
            - BidPrice1 ~ BidPrice5, BidVolume1 ~ BidVolume5
            - AskPrice1 ~ AskPrice5, AskVolume1 ~ AskVolume5
            - BandingUpperPrice, BandingLowerPrice
    """
```

## 回调实现细节

所有 C++ 侧的 `OnXxx` 回调都遵循相同的模式：

1. 使用 `std::optional` 捕获指针参数（处理空指针）
2. 通过 `queue_->dispatch()` 将任务加入 `DispatchQueue`
3. 在工作线程中获取 GIL
4. 将 C struct 字段逐个转换为 `nb::dict`
5. 字符串字段通过 `gbk_to_utf8()` 转换
6. 浮点数字段通过 `adjust_number()` 处理哨兵值
7. 调用对应的 `PyOnXxx()` 纯虚方法

以 `OnRtnDepthMarketData` 为例，转换了 40+ 个字段的深度行情数据。
