# TdApi 交易接口

`TdApi` 封装了 CTP 交易接口 (`CThostFtdcTraderApi` / `CThostFtdcTraderSpi`)，提供交易、查询、报单、撤单等功能。

## 类定义

```cpp
class TdApi : public CThostFtdcTraderSpi {
private:
    std::unique_ptr<DispatchQueue> queue_;
    CThostFtdcTraderApi* api_;
// ...
};
```

## 生命周期方法

### CreateApi

```cpp
void CreateApi(const std::string &flow_path, bool is_production_mode = true);
```

创建交易 API 实例。

- `flow_path`: 流文件存放目录
- `is_production_mode`: 是否生产环境模式

### Release

```cpp
void Release();
```

释放 API 资源。

### Init

```cpp
void Init();
```

初始化并启动与交易前置机的连接线程。

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

## 连接与订阅方法

### RegisterFront

```cpp
void RegisterFront(const std::string &front_address);
```

注册交易前置机地址。

### SubscribePrivateTopic

```cpp
void SubscribePrivateTopic(int resume_type);
```

订阅私有流。`resume_type` 对应 `THOST_TE_RESUME_TYPE`：
- `THOST_TERT_RESTART` (0): 从本交易日开始重传
- `THOST_TERT_RESUME` (1): 从上次断开处续传
- `THOST_TERT_QUICK` (2): 只传新内容

### SubscribePublicTopic

```cpp
void SubscribePublicTopic(int resume_type);
```

订阅公共流。参数含义同上。

## 请求方法

### ReqAuthenticate

```cpp
int ReqAuthenticate(const nb::dict &data, int request_id);
```

发送客户端认证请求。

`data` 字段：

| 字段名 | 类型 | 说明 |
|--------|------|------|
| `BrokerID` | string | 经纪公司代码 |
| `UserID` | string | 用户代码 |
| `UserProductInfo` | string | 用户端产品信息 |
| `AuthCode` | string | 认证码 |
| `AppID` | string | App ID |

### ReqUserLogin

```cpp
int ReqUserLogin(const nb::dict &data, int request_id);
```

发送登录请求。字段同 MdApi 的 `ReqUserLogin`。

### ReqUserLogout

```cpp
int ReqUserLogout(const nb::dict &data, int request_id);
```

发送登出请求。

### ReqUserPasswordUpdate

```cpp
int ReqUserPasswordUpdate(const nb::dict &data, int request_id);
```

修改用户密码。

`data` 字段：

| 字段名 | 类型 | 说明 |
|--------|------|------|
| `BrokerID` | string | 经纪公司代码 |
| `UserID` | string | 用户代码 |
| `OldPassword` | string | 原密码 |
| `NewPassword` | string | 新密码 |

### ReqOrderInsert

```cpp
int ReqOrderInsert(const nb::dict &data, int request_id);
```

报单请求（下单）。

`data` 主要字段：

| 字段名 | 类型 | 说明 |
|--------|------|------|
| `BrokerID` | string | 经纪公司代码 |
| `InvestorID` | string | 投资者代码 |
| `InstrumentID` | string | 合约代码 |
| `OrderRef` | string | 报单引用 |
| `UserID` | string | 用户代码 |
| `OrderPriceType` | char | 报单价格类型 |
| `Direction` | char | 买卖方向 |
| `CombOffsetFlag` | string | 组合开平标志 |
| `CombHedgeFlag` | string | 组合投机套保标志 |
| `LimitPrice` | double | 价格 |
| `VolumeTotalOriginal` | int | 数量 |
| `TimeCondition` | char | 有效期类型 |
| `VolumeCondition` | char | 成交量类型 |
| `MinVolume` | int | 最小成交量 |
| `ContingentCondition` | char | 触发条件 |
| `StopPrice` | double | 止损价 |
| `ForceCloseReason` | char | 强平原因 |
| `IsAutoSuspend` | int | 自动挂起标志 |
| `ExchangeID` | string | 交易所代码 |
| `InvestUnitID` | string | 投资单元代码 |
| `AccountID` | string | 资金账号 |
| `CurrencyID` | string | 币种代码 |
| `ClientID` | string | 客户代码 |
| `MacAddress` | string | Mac 地址 |
| `IPAddress` | string | IP 地址 |
| `OrderMemo` | string | 报单备注 |
| `SessionReqSeq` | int | 会话请求序号 |

### ReqOrderAction

```cpp
int ReqOrderAction(const nb::dict &data, int request_id);
```

撤单请求。

`data` 主要字段：

| 字段名 | 类型 | 说明 |
|--------|------|------|
| `BrokerID` | string | 经纪公司代码 |
| `InvestorID` | string | 投资者代码 |
| `OrderActionRef` | int | 报单操作引用 |
| `OrderRef` | string | 报单引用 |
| `RequestID` | int | 请求编号 |
| `FrontID` | int | 前置机编号 |
| `SessionID` | int | 会话编号 |
| `ExchangeID` | string | 交易所代码 |
| `OrderSysID` | string | 报单编号（交易所） |
| `ActionFlag` | char | 操作标志（删除/修改） |
| `LimitPrice` | double | 价格 |
| `VolumeChange` | int | 数量变化 |
| `UserID` | string | 用户代码 |
| `InvestUnitID` | string | 投资单元代码 |
| `MacAddress` | string | Mac 地址 |
| `InstrumentID` | string | 合约代码 |
| `IPAddress` | string | IP 地址 |
| `OrderMemo` | string | 报单备注 |
| `SessionReqSeq` | int | 会话请求序号 |

### ReqSettlementInfoConfirm

```cpp
int ReqSettlementInfoConfirm(const nb::dict &data, int request_id);
```

投资者结算结果确认。

`data` 字段：`BrokerID`, `InvestorID`, `ConfirmDate`, `ConfirmTime`, `SettlementID`, `AccountID`, `CurrencyID`

### ReqQryOrder

```cpp
int ReqQryOrder(const nb::dict &data, int request_id);
```

查询报单。

`data` 字段：`BrokerID`, `InvestorID`, `ExchangeID`, `OrderSysID`, `InsertTimeStart`, `InsertTimeEnd`, `InvestUnitID`, `InstrumentID`

### ReqQryTrade

```cpp
int ReqQryTrade(const nb::dict &data, int request_id);
```

查询成交。

`data` 字段：`BrokerID`, `InvestorID`, `ExchangeID`, `TradeID`, `TradeTimeStart`, `TradeTimeEnd`, `InvestUnitID`, `InstrumentID`

### ReqQryInvestorPosition

```cpp
int ReqQryInvestorPosition(const nb::dict &data, int request_id);
```

查询投资者持仓。

`data` 字段：`BrokerID`, `InvestorID`, `ExchangeID`, `InvestUnitID`, `InstrumentID`

### ReqQryTradingAccount

```cpp
int ReqQryTradingAccount(const nb::dict &data, int request_id);
```

查询资金账户。

`data` 字段：`BrokerID`, `InvestorID`, `CurrencyID`, `BizType`, `AccountID`

### ReqQryInstrument

```cpp
int ReqQryInstrument(const nb::dict &data, int request_id);
```

查询合约。

`data` 字段：`ExchangeID`, `InstrumentID`, `ExchangeInstID`, `ProductID`

### ReqQrySettlementInfo

```cpp
int ReqQrySettlementInfo(const nb::dict &data, int request_id);
```

查询投资者结算结果。

`data` 字段：`BrokerID`, `InvestorID`, `TradingDay`, `AccountID`, `CurrencyID`

## 回调方法（Python 侧重写）

### 连接相关

```python
def OnFrontConnected(self):
    """前置机连接成功"""

def OnFrontDisconnected(self, reason):
    """前置机断开连接"""

def OnHeartBeatWarning(self, time_lapse):
    """心跳超时警告"""
```

### 登录认证相关

```python
def OnRspAuthenticate(self, data, error, request_id, is_last):
    """客户端认证响应
    
    data 字段: BrokerID, UserID, UserProductInfo, AppID, AppType
    """

def OnRspUserLogin(self, data, error, request_id, is_last):
    """登录响应
    
    data 字段: TradingDay, LoginTime, BrokerID, UserID, SystemName,
              FrontID, SessionID, MaxOrderRef,
              SHFETime, DCETime, CZCETime, FFEXTime, INETime, GFEXTime,
              SysVersion, LoginDRIdentityID, UserDRIdentityID,
              LastLoginTime, ReserveInfo
    """

def OnRspUserLogout(self, data, error, request_id, is_last):
    """登出响应"""
```

### 交易相关

```python
def OnRspOrderInsert(self, data, error, request_id, is_last):
    """报单响应"""

def OnRspOrderAction(self, data, error, request_id, is_last):
    """报单操作响应（撤单）"""

def OnRspSettlementInfoConfirm(self, data, error, request_id, is_last):
    """结算确认响应"""
```

### 查询相关

```python
def OnRspQryOrder(self, data, error, request_id, is_last):
    """查询报单响应
    
    data 包含完整报单字段：OrderRef, OrderStatus, OrderSysID,
    VolumeTraded, VolumeTotal, InsertDate, InsertTime,
    FrontID, SessionID, InstrumentID 等 40+ 字段
    """

def OnRspQryTrade(self, data, error, request_id, is_last):
    """查询成交响应"""

def OnRspQryInvestorPosition(self, data, error, request_id, is_last):
    """查询持仓响应
    
    data 包含：Position, YdPosition, TodayPosition, LongFrozen,
    ShortFrozen, OpenVolume, CloseVolume, PositionCost, UseMargin,
    FrozenMargin, Commission, CloseProfit, PositionProfit 等
    """

def OnRspQryTradingAccount(self, data, error, request_id, is_last):
    """查询资金账户响应
    
    data 包含：PreBalance, Available, Balance, WithdrawQuota,
    CurrMargin, FrozenMargin, FrozenCash, FrozenCommission,
    Commission, CloseProfit, PositionProfit 等
    """

def OnRspQryInstrument(self, data, error, request_id, is_last):
    """查询合约响应
    
    data 包含：InstrumentID, InstrumentName, ExchangeID, ProductID,
    ProductClass, VolumeMultiple, PriceTick, DeliveryYear,
    DeliveryMonth, StrikePrice, OptionsType 等
    """

def OnRspQrySettlementInfo(self, data, error, request_id, is_last):
    """查询结算结果响应
    
    data 包含：TradingDay, SettlementID, BrokerID, InvestorID,
    SequenceNo, Content(bytes), AccountID, CurrencyID
    """
```

### 回报推送

```python
def OnRtnOrder(self, data):
    """报单回报（状态变化推送）
    
    当报单状态发生变化时，CTP 主动推送。
    字段同 OnRspQryOrder 的 data。
    """

def OnRtnTrade(self, data):
    """成交回报
    
    当报单成交时，CTP 主动推送。
    字段：BrokerID, InvestorID, OrderRef, TradeID, Direction,
    OffsetFlag, HedgeFlag, Price, Volume, TradeDate, TradeTime,
    InstrumentID, ExchangeID 等
    """

def OnErrRtnOrderInsert(self, data, error):
    """报单录入错误回报"""

def OnErrRtnOrderAction(self, data, error):
    """报单操作错误回报"""

def OnRtnInstrumentStatus(self, data):
    """合约交易状态通知
    
    data 字段: ExchangeID, SettlementGroupID, InstrumentStatus,
    TradingSegmentSN, EnterTime, EnterReason, ExchangeInstID, InstrumentID
    """
```

### 错误响应

```python
def OnRspError(self, error, request_id, is_last):
    """通用错误响应
    
    error: dict，包含 ErrorID 和 ErrorMsg
    """
```
