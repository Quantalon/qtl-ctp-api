#include "td_api.h"


void TdApi::CreateApi(const std::string &flow_path) {
    queue_ = std::make_unique<DispatchQueue>();
    api_.reset(CThostFtdcTraderApi::CreateFtdcTraderApi(flow_path.c_str()));
    api_->RegisterSpi(this);
}

std::string TdApi::GetApiVersion() {
    return CThostFtdcTraderApi::GetApiVersion();
}

void TdApi::Init() {
    api_->Init();
}

int TdApi::Join() {
    return api_->Join();
}

std::string TdApi::GetTradingDay() {
    return api_->GetTradingDay();
}

void TdApi::RegisterFront(const std::string &front_address) {
    api_->RegisterFront(const_cast<char *>(front_address.c_str()));
}

void TdApi::SubscribePrivateTopic(int resume_type) {
    api_->SubscribePrivateTopic((THOST_TE_RESUME_TYPE)resume_type);
}

void TdApi::SubscribePublicTopic(int resume_type) {
    api_->SubscribePublicTopic((THOST_TE_RESUME_TYPE)resume_type);
}

int TdApi::ReqAuthenticate(const py::dict &data, int request_id) {
    CThostFtdcReqAuthenticateField request{};
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.UserID, data, "UserID", sizeof(request.UserID));
    set_str_field(request.UserProductInfo, data, "UserProductInfo", sizeof(request.UserProductInfo));
    set_str_field(request.AuthCode, data, "AuthCode", sizeof(request.AuthCode));
    set_str_field(request.AppID, data, "AppID", sizeof(request.AppID));
    return api_->ReqAuthenticate(&request, request_id);
}

int TdApi::ReqUserLogin(const py::dict &data, int request_id) {
    CThostFtdcReqUserLoginField request{};
    set_str_field(request.TradingDay, data, "TradingDay", sizeof(request.TradingDay));
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.UserID, data, "UserID", sizeof(request.UserID));
    set_str_field(request.Password, data, "Password", sizeof(request.Password));
    set_str_field(request.UserProductInfo, data, "UserProductInfo", sizeof(request.UserProductInfo));
    set_str_field(request.InterfaceProductInfo, data, "InterfaceProductInfo", sizeof(request.InterfaceProductInfo));
    set_str_field(request.ProtocolInfo, data, "ProtocolInfo", sizeof(request.ProtocolInfo));
    set_str_field(request.MacAddress, data, "MacAddress", sizeof(request.MacAddress));
    set_str_field(request.OneTimePassword, data, "OneTimePassword", sizeof(request.OneTimePassword));
    set_str_field(request.LoginRemark, data, "LoginRemark", sizeof(request.LoginRemark));
    set_int_field(request.ClientIPPort, data, "ClientIPPort");
    set_str_field(request.ClientIPAddress, data, "ClientIPAddress", sizeof(request.ClientIPAddress));
    return api_->ReqUserLogin(&request, request_id);
}

int TdApi::ReqUserLogout(const py::dict &data, int request_id) {
    CThostFtdcUserLogoutField request{};
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.UserID, data, "UserID", sizeof(request.UserID));
    return api_->ReqUserLogout(&request, request_id);
}

int TdApi::ReqOrderInsert(const py::dict &data, int request_id) {
    CThostFtdcInputOrderField request{};
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.InvestorID, data, "InvestorID", sizeof(request.InvestorID));
    set_str_field(request.OrderRef, data, "OrderRef", sizeof(request.OrderRef));
    set_str_field(request.UserID, data, "UserID", sizeof(request.UserID));
    set_char_field(request.OrderPriceType, data, "OrderPriceType");
    set_char_field(request.Direction, data, "Direction");
    set_str_field(request.CombOffsetFlag, data, "CombOffsetFlag", sizeof(request.CombOffsetFlag));
    set_str_field(request.CombHedgeFlag, data, "CombHedgeFlag", sizeof(request.CombHedgeFlag));
    set_double_field(request.LimitPrice, data, "LimitPrice");
    set_int_field(request.VolumeTotalOriginal, data, "VolumeTotalOriginal");
    set_char_field(request.TimeCondition, data, "TimeCondition");
    set_str_field(request.GTDDate, data, "GTDDate", sizeof(request.GTDDate));
    set_char_field(request.VolumeCondition, data, "VolumeCondition");
    set_int_field(request.MinVolume, data, "MinVolume");
    set_char_field(request.ContingentCondition, data, "ContingentCondition");
    set_double_field(request.StopPrice, data, "StopPrice");
    set_char_field(request.ForceCloseReason, data, "ForceCloseReason");
    set_int_field(request.IsAutoSuspend, data, "IsAutoSuspend");
    set_str_field(request.BusinessUnit, data, "BusinessUnit", sizeof(request.BusinessUnit));
    set_int_field(request.RequestID, data, "RequestID");
    set_int_field(request.UserForceClose, data, "UserForceClose");
    set_int_field(request.IsSwapOrder, data, "IsSwapOrder");
    set_str_field(request.ExchangeID, data, "ExchangeID", sizeof(request.ExchangeID));
    set_str_field(request.InvestUnitID, data, "InvestUnitID", sizeof(request.InvestUnitID));
    set_str_field(request.AccountID, data, "AccountID", sizeof(request.AccountID));
    set_str_field(request.CurrencyID, data, "CurrencyID", sizeof(request.CurrencyID));
    set_str_field(request.ClientID, data, "ClientID", sizeof(request.ClientID));
    set_str_field(request.MacAddress, data, "MacAddress", sizeof(request.MacAddress));
    set_str_field(request.InstrumentID, data, "InstrumentID", sizeof(request.InstrumentID));
    set_str_field(request.IPAddress, data, "IPAddress", sizeof(request.IPAddress));
    return api_->ReqOrderInsert(&request, request_id);
}

int TdApi::ReqOrderAction(const py::dict &data, int request_id) {
    CThostFtdcInputOrderActionField request{};
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.InvestorID, data, "InvestorID", sizeof(request.InvestorID));
    set_int_field(request.OrderActionRef, data, "OrderActionRef");
    set_str_field(request.OrderRef, data, "OrderRef", sizeof(request.OrderRef));
    set_int_field(request.RequestID, data, "RequestID");
    set_int_field(request.FrontID, data, "FrontID");
    set_int_field(request.SessionID, data, "SessionID");
    set_str_field(request.ExchangeID, data, "ExchangeID", sizeof(request.ExchangeID));
    set_str_field(request.OrderSysID, data, "OrderSysID", sizeof(request.OrderSysID));
    set_char_field(request.ActionFlag, data, "ActionFlag");
    set_double_field(request.LimitPrice, data, "LimitPrice");
    set_int_field(request.VolumeChange, data, "VolumeChange");
    set_str_field(request.UserID, data, "UserID", sizeof(request.UserID));
    set_str_field(request.InvestUnitID, data, "InvestUnitID", sizeof(request.InvestUnitID));
    set_str_field(request.MacAddress, data, "MacAddress", sizeof(request.MacAddress));
    set_str_field(request.InstrumentID, data, "InstrumentID", sizeof(request.InstrumentID));
    set_str_field(request.IPAddress, data, "IPAddress", sizeof(request.IPAddress));
    return api_->ReqOrderAction(&request, request_id);
}

int TdApi::ReqSettlementInfoConfirm(const py::dict &data, int request_id) {
    CThostFtdcSettlementInfoConfirmField request{};
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.InvestorID, data, "InvestorID", sizeof(request.InvestorID));
    set_str_field(request.ConfirmDate, data, "ConfirmDate", sizeof(request.ConfirmDate));
    set_str_field(request.ConfirmTime, data, "ConfirmTime", sizeof(request.ConfirmTime));
    set_int_field(request.SettlementID, data, "SettlementID");
    set_str_field(request.AccountID, data, "AccountID", sizeof(request.AccountID));
    set_str_field(request.CurrencyID, data, "CurrencyID", sizeof(request.CurrencyID));
    return api_->ReqSettlementInfoConfirm(&request, request_id);
}

int TdApi::ReqQryOrder(const py::dict &data, int request_id) {
    CThostFtdcQryOrderField request{};
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.InvestorID, data, "InvestorID", sizeof(request.InvestorID));
    set_str_field(request.ExchangeID, data, "ExchangeID", sizeof(request.ExchangeID));
    set_str_field(request.OrderSysID, data, "OrderSysID", sizeof(request.OrderSysID));
    set_str_field(request.InsertTimeStart, data, "InsertTimeStart", sizeof(request.InsertTimeStart));
    set_str_field(request.InsertTimeEnd, data, "InsertTimeEnd", sizeof(request.InsertTimeEnd));
    set_str_field(request.InvestUnitID, data, "InvestUnitID", sizeof(request.InvestUnitID));
    set_str_field(request.InstrumentID, data, "InstrumentID", sizeof(request.InstrumentID));
    return api_->ReqQryOrder(&request, request_id);
}

int TdApi::ReqQryTrade(const py::dict &data, int request_id) {
    CThostFtdcQryTradeField request{};
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.InvestorID, data, "InvestorID", sizeof(request.InvestorID));
    set_str_field(request.ExchangeID, data, "ExchangeID", sizeof(request.ExchangeID));
    set_str_field(request.TradeID, data, "TradeID", sizeof(request.TradeID));
    set_str_field(request.TradeTimeStart, data, "TradeTimeStart", sizeof(request.TradeTimeStart));
    set_str_field(request.TradeTimeEnd, data, "TradeTimeEnd", sizeof(request.TradeTimeEnd));
    set_str_field(request.InvestUnitID, data, "InvestUnitID", sizeof(request.InvestUnitID));
    set_str_field(request.InstrumentID, data, "InstrumentID", sizeof(request.InstrumentID));
    return api_->ReqQryTrade(&request, request_id);
}

int TdApi::ReqQryInvestorPosition(const py::dict &data, int request_id) {
    CThostFtdcQryInvestorPositionField request{};
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.InvestorID, data, "InvestorID", sizeof(request.InvestorID));
    set_str_field(request.ExchangeID, data, "ExchangeID", sizeof(request.ExchangeID));
    set_str_field(request.InvestUnitID, data, "InvestUnitID", sizeof(request.InvestUnitID));
    set_str_field(request.InstrumentID, data, "InstrumentID", sizeof(request.InstrumentID));
    return api_->ReqQryInvestorPosition(&request, request_id);
}

int TdApi::ReqQryTradingAccount(const py::dict &data, int request_id) {
    CThostFtdcQryTradingAccountField request{};
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.InvestorID, data, "InvestorID", sizeof(request.InvestorID));
    set_str_field(request.CurrencyID, data, "CurrencyID", sizeof(request.CurrencyID));
    set_char_field(request.BizType, data, "BizType");
    set_str_field(request.AccountID, data, "AccountID", sizeof(request.AccountID));
    return api_->ReqQryTradingAccount(&request, request_id);
}

int TdApi::ReqQryInstrument(const py::dict &data, int request_id) {
    CThostFtdcQryInstrumentField request{};
    set_str_field(request.ExchangeID, data, "ExchangeID", sizeof(request.ExchangeID));
    set_str_field(request.InstrumentID, data, "InstrumentID", sizeof(request.InstrumentID));
    set_str_field(request.ExchangeInstID, data, "ExchangeInstID", sizeof(request.ExchangeInstID));
    set_str_field(request.ProductID, data, "ProductID", sizeof(request.ProductID));
    return api_->ReqQryInstrument(&request, request_id);
}

void TdApi::OnFrontConnected() {
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        PyOnFrontConnected();
    });
}

void TdApi::OnFrontDisconnected(int reason) {
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        PyOnFrontDisconnected(reason);
    });
}

void TdApi::OnHeartBeatWarning(int time_lapse) {
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        PyOnHeartBeatWarning(time_lapse);
    });
}

void TdApi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcRspAuthenticateField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
            py_data["UserProductInfo"] = gbk_to_utf8(rsp_data.UserProductInfo);
            py_data["AppID"] = gbk_to_utf8(rsp_data.AppID);
            py_data["AppType"] = rsp_data.AppType;
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspAuthenticate(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcRspUserLoginField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["TradingDay"] = gbk_to_utf8(rsp_data.TradingDay);
            py_data["LoginTime"] = gbk_to_utf8(rsp_data.LoginTime);
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
            py_data["SystemName"] = gbk_to_utf8(rsp_data.SystemName);
            py_data["FrontID"] = rsp_data.FrontID;
            py_data["SessionID"] = rsp_data.SessionID;
            py_data["MaxOrderRef"] = gbk_to_utf8(rsp_data.MaxOrderRef);
            py_data["SHFETime"] = gbk_to_utf8(rsp_data.SHFETime);
            py_data["DCETime"] = gbk_to_utf8(rsp_data.DCETime);
            py_data["CZCETime"] = gbk_to_utf8(rsp_data.CZCETime);
            py_data["FFEXTime"] = gbk_to_utf8(rsp_data.FFEXTime);
            py_data["INETime"] = gbk_to_utf8(rsp_data.INETime);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspUserLogin(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspUserLogout(CThostFtdcUserLogoutField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcUserLogoutField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspUserLogout(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspOrderInsert(CThostFtdcInputOrderField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcInputOrderField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["InvestorID"] = gbk_to_utf8(rsp_data.InvestorID);
            py_data["OrderRef"] = gbk_to_utf8(rsp_data.OrderRef);
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
            py_data["OrderPriceType"] = rsp_data.OrderPriceType;
            py_data["Direction"] = rsp_data.Direction;
            py_data["CombOffsetFlag"] = gbk_to_utf8(rsp_data.CombOffsetFlag);
            py_data["CombHedgeFlag"] = gbk_to_utf8(rsp_data.CombHedgeFlag);
            py_data["LimitPrice"] = rsp_data.LimitPrice;
            py_data["VolumeTotalOriginal"] = rsp_data.VolumeTotalOriginal;
            py_data["TimeCondition"] = rsp_data.TimeCondition;
            py_data["GTDDate"] = gbk_to_utf8(rsp_data.GTDDate);
            py_data["VolumeCondition"] = rsp_data.VolumeCondition;
            py_data["MinVolume"] = rsp_data.MinVolume;
            py_data["ContingentCondition"] = rsp_data.ContingentCondition;
            py_data["StopPrice"] = rsp_data.StopPrice;
            py_data["ForceCloseReason"] = rsp_data.ForceCloseReason;
            py_data["IsAutoSuspend"] = rsp_data.IsAutoSuspend;
            py_data["BusinessUnit"] = gbk_to_utf8(rsp_data.BusinessUnit);
            py_data["RequestID"] = rsp_data.RequestID;
            py_data["UserForceClose"] = rsp_data.UserForceClose;
            py_data["IsSwapOrder"] = rsp_data.IsSwapOrder;
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["InvestUnitID"] = gbk_to_utf8(rsp_data.InvestUnitID);
            py_data["AccountID"] = gbk_to_utf8(rsp_data.AccountID);
            py_data["CurrencyID"] = gbk_to_utf8(rsp_data.CurrencyID);
            py_data["ClientID"] = gbk_to_utf8(rsp_data.ClientID);
            py_data["MacAddress"] = gbk_to_utf8(rsp_data.MacAddress);
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
            py_data["IPAddress"] = gbk_to_utf8(rsp_data.IPAddress);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspOrderInsert(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspOrderAction(CThostFtdcInputOrderActionField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcInputOrderActionField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["InvestorID"] = gbk_to_utf8(rsp_data.InvestorID);
            py_data["OrderActionRef"] = rsp_data.OrderActionRef;
            py_data["OrderRef"] = gbk_to_utf8(rsp_data.OrderRef);
            py_data["RequestID"] = rsp_data.RequestID;
            py_data["FrontID"] = rsp_data.FrontID;
            py_data["SessionID"] = rsp_data.SessionID;
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["OrderSysID"] = gbk_to_utf8(rsp_data.OrderSysID);
            py_data["ActionFlag"] = rsp_data.ActionFlag;
            py_data["LimitPrice"] = rsp_data.LimitPrice;
            py_data["VolumeChange"] = rsp_data.VolumeChange;
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
            py_data["InvestUnitID"] = gbk_to_utf8(rsp_data.InvestUnitID);
            py_data["MacAddress"] = gbk_to_utf8(rsp_data.MacAddress);
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
            py_data["IPAddress"] = gbk_to_utf8(rsp_data.IPAddress);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspOrderAction(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcSettlementInfoConfirmField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["InvestorID"] = gbk_to_utf8(rsp_data.InvestorID);
            py_data["ConfirmDate"] = gbk_to_utf8(rsp_data.ConfirmDate);
            py_data["ConfirmTime"] = gbk_to_utf8(rsp_data.ConfirmTime);
            py_data["SettlementID"] = rsp_data.SettlementID;
            py_data["AccountID"] = gbk_to_utf8(rsp_data.AccountID);
            py_data["CurrencyID"] = gbk_to_utf8(rsp_data.CurrencyID);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspSettlementInfoConfirm(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspQryOrder(CThostFtdcOrderField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcOrderField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["InvestorID"] = gbk_to_utf8(rsp_data.InvestorID);
            py_data["OrderRef"] = gbk_to_utf8(rsp_data.OrderRef);
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
            py_data["OrderPriceType"] = rsp_data.OrderPriceType;
            py_data["Direction"] = rsp_data.Direction;
            py_data["CombOffsetFlag"] = gbk_to_utf8(rsp_data.CombOffsetFlag);
            py_data["CombHedgeFlag"] = gbk_to_utf8(rsp_data.CombHedgeFlag);
            py_data["LimitPrice"] = rsp_data.LimitPrice;
            py_data["VolumeTotalOriginal"] = rsp_data.VolumeTotalOriginal;
            py_data["TimeCondition"] = rsp_data.TimeCondition;
            py_data["GTDDate"] = gbk_to_utf8(rsp_data.GTDDate);
            py_data["VolumeCondition"] = rsp_data.VolumeCondition;
            py_data["MinVolume"] = rsp_data.MinVolume;
            py_data["ContingentCondition"] = rsp_data.ContingentCondition;
            py_data["StopPrice"] = rsp_data.StopPrice;
            py_data["ForceCloseReason"] = rsp_data.ForceCloseReason;
            py_data["IsAutoSuspend"] = rsp_data.IsAutoSuspend;
            py_data["BusinessUnit"] = gbk_to_utf8(rsp_data.BusinessUnit);
            py_data["RequestID"] = rsp_data.RequestID;
            py_data["OrderLocalID"] = gbk_to_utf8(rsp_data.OrderLocalID);
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["ParticipantID"] = gbk_to_utf8(rsp_data.ParticipantID);
            py_data["ClientID"] = gbk_to_utf8(rsp_data.ClientID);
            py_data["TraderID"] = gbk_to_utf8(rsp_data.TraderID);
            py_data["InstallID"] = rsp_data.InstallID;
            py_data["OrderSubmitStatus"] = rsp_data.OrderSubmitStatus;
            py_data["NotifySequence"] = rsp_data.NotifySequence;
            py_data["TradingDay"] = gbk_to_utf8(rsp_data.TradingDay);
            py_data["SettlementID"] = rsp_data.SettlementID;
            py_data["OrderSysID"] = gbk_to_utf8(rsp_data.OrderSysID);
            py_data["OrderSource"] = rsp_data.OrderSource;
            py_data["OrderStatus"] = rsp_data.OrderStatus;
            py_data["OrderType"] = rsp_data.OrderType;
            py_data["VolumeTraded"] = rsp_data.VolumeTraded;
            py_data["VolumeTotal"] = rsp_data.VolumeTotal;
            py_data["InsertDate"] = gbk_to_utf8(rsp_data.InsertDate);
            py_data["InsertTime"] = gbk_to_utf8(rsp_data.InsertTime);
            py_data["ActiveTime"] = gbk_to_utf8(rsp_data.ActiveTime);
            py_data["SuspendTime"] = gbk_to_utf8(rsp_data.SuspendTime);
            py_data["UpdateTime"] = gbk_to_utf8(rsp_data.UpdateTime);
            py_data["CancelTime"] = gbk_to_utf8(rsp_data.CancelTime);
            py_data["ActiveTraderID"] = gbk_to_utf8(rsp_data.ActiveTraderID);
            py_data["ClearingPartID"] = gbk_to_utf8(rsp_data.ClearingPartID);
            py_data["SequenceNo"] = rsp_data.SequenceNo;
            py_data["FrontID"] = rsp_data.FrontID;
            py_data["SessionID"] = rsp_data.SessionID;
            py_data["UserProductInfo"] = gbk_to_utf8(rsp_data.UserProductInfo);
            py_data["StatusMsg"] = gbk_to_utf8(rsp_data.StatusMsg);
            py_data["UserForceClose"] = rsp_data.UserForceClose;
            py_data["ActiveUserID"] = gbk_to_utf8(rsp_data.ActiveUserID);
            py_data["BrokerOrderSeq"] = rsp_data.BrokerOrderSeq;
            py_data["RelativeOrderSysID"] = gbk_to_utf8(rsp_data.RelativeOrderSysID);
            py_data["ZCETotalTradedVolume"] = rsp_data.ZCETotalTradedVolume;
            py_data["IsSwapOrder"] = rsp_data.IsSwapOrder;
            py_data["BranchID"] = gbk_to_utf8(rsp_data.BranchID);
            py_data["InvestUnitID"] = gbk_to_utf8(rsp_data.InvestUnitID);
            py_data["AccountID"] = gbk_to_utf8(rsp_data.AccountID);
            py_data["CurrencyID"] = gbk_to_utf8(rsp_data.CurrencyID);
            py_data["MacAddress"] = gbk_to_utf8(rsp_data.MacAddress);
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
            py_data["ExchangeInstID"] = gbk_to_utf8(rsp_data.ExchangeInstID);
            py_data["IPAddress"] = gbk_to_utf8(rsp_data.IPAddress);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspQryOrder(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspQryTrade(CThostFtdcTradeField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcTradeField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["InvestorID"] = gbk_to_utf8(rsp_data.InvestorID);
            py_data["OrderRef"] = gbk_to_utf8(rsp_data.OrderRef);
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["TradeID"] = gbk_to_utf8(rsp_data.TradeID);
            py_data["Direction"] = rsp_data.Direction;
            py_data["OrderSysID"] = gbk_to_utf8(rsp_data.OrderSysID);
            py_data["ParticipantID"] = gbk_to_utf8(rsp_data.ParticipantID);
            py_data["ClientID"] = gbk_to_utf8(rsp_data.ClientID);
            py_data["TradingRole"] = rsp_data.TradingRole;
            py_data["OffsetFlag"] = rsp_data.OffsetFlag;
            py_data["HedgeFlag"] = rsp_data.HedgeFlag;
            py_data["Price"] = rsp_data.Price;
            py_data["Volume"] = rsp_data.Volume;
            py_data["TradeDate"] = gbk_to_utf8(rsp_data.TradeDate);
            py_data["TradeTime"] = gbk_to_utf8(rsp_data.TradeTime);
            py_data["TradeType"] = rsp_data.TradeType;
            py_data["PriceSource"] = rsp_data.PriceSource;
            py_data["TraderID"] = gbk_to_utf8(rsp_data.TraderID);
            py_data["OrderLocalID"] = gbk_to_utf8(rsp_data.OrderLocalID);
            py_data["ClearingPartID"] = gbk_to_utf8(rsp_data.ClearingPartID);
            py_data["BusinessUnit"] = gbk_to_utf8(rsp_data.BusinessUnit);
            py_data["SequenceNo"] = rsp_data.SequenceNo;
            py_data["TradingDay"] = gbk_to_utf8(rsp_data.TradingDay);
            py_data["SettlementID"] = rsp_data.SettlementID;
            py_data["BrokerOrderSeq"] = rsp_data.BrokerOrderSeq;
            py_data["TradeSource"] = rsp_data.TradeSource;
            py_data["InvestUnitID"] = gbk_to_utf8(rsp_data.InvestUnitID);
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
            py_data["ExchangeInstID"] = gbk_to_utf8(rsp_data.ExchangeInstID);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspQryTrade(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcInvestorPositionField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["InvestorID"] = gbk_to_utf8(rsp_data.InvestorID);
            py_data["PosiDirection"] = rsp_data.PosiDirection;
            py_data["HedgeFlag"] = rsp_data.HedgeFlag;
            py_data["PositionDate"] = rsp_data.PositionDate;
            py_data["YdPosition"] = rsp_data.YdPosition;
            py_data["Position"] = rsp_data.Position;
            py_data["LongFrozen"] = rsp_data.LongFrozen;
            py_data["ShortFrozen"] = rsp_data.ShortFrozen;
            py_data["LongFrozenAmount"] = rsp_data.LongFrozenAmount;
            py_data["ShortFrozenAmount"] = rsp_data.ShortFrozenAmount;
            py_data["OpenVolume"] = rsp_data.OpenVolume;
            py_data["CloseVolume"] = rsp_data.CloseVolume;
            py_data["OpenAmount"] = rsp_data.OpenAmount;
            py_data["CloseAmount"] = rsp_data.CloseAmount;
            py_data["PositionCost"] = rsp_data.PositionCost;
            py_data["PreMargin"] = rsp_data.PreMargin;
            py_data["UseMargin"] = rsp_data.UseMargin;
            py_data["FrozenMargin"] = rsp_data.FrozenMargin;
            py_data["FrozenCash"] = rsp_data.FrozenCash;
            py_data["FrozenCommission"] = rsp_data.FrozenCommission;
            py_data["CashIn"] = rsp_data.CashIn;
            py_data["Commission"] = rsp_data.Commission;
            py_data["CloseProfit"] = rsp_data.CloseProfit;
            py_data["PositionProfit"] = rsp_data.PositionProfit;
            py_data["PreSettlementPrice"] = rsp_data.PreSettlementPrice;
            py_data["SettlementPrice"] = rsp_data.SettlementPrice;
            py_data["TradingDay"] = gbk_to_utf8(rsp_data.TradingDay);
            py_data["SettlementID"] = rsp_data.SettlementID;
            py_data["OpenCost"] = rsp_data.OpenCost;
            py_data["ExchangeMargin"] = rsp_data.ExchangeMargin;
            py_data["CombPosition"] = rsp_data.CombPosition;
            py_data["CombLongFrozen"] = rsp_data.CombLongFrozen;
            py_data["CombShortFrozen"] = rsp_data.CombShortFrozen;
            py_data["CloseProfitByDate"] = rsp_data.CloseProfitByDate;
            py_data["CloseProfitByTrade"] = rsp_data.CloseProfitByTrade;
            py_data["TodayPosition"] = rsp_data.TodayPosition;
            py_data["MarginRateByMoney"] = rsp_data.MarginRateByMoney;
            py_data["MarginRateByVolume"] = rsp_data.MarginRateByVolume;
            py_data["StrikeFrozen"] = rsp_data.StrikeFrozen;
            py_data["StrikeFrozenAmount"] = rsp_data.StrikeFrozenAmount;
            py_data["AbandonFrozen"] = rsp_data.AbandonFrozen;
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["YdStrikeFrozen"] = rsp_data.YdStrikeFrozen;
            py_data["InvestUnitID"] = gbk_to_utf8(rsp_data.InvestUnitID);
            py_data["PositionCostOffset"] = rsp_data.PositionCostOffset;
            py_data["TasPosition"] = rsp_data.TasPosition;
            py_data["TasPositionCost"] = rsp_data.TasPositionCost;
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspQryInvestorPosition(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcTradingAccountField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["AccountID"] = gbk_to_utf8(rsp_data.AccountID);
            py_data["PreMortgage"] = rsp_data.PreMortgage;
            py_data["PreCredit"] = rsp_data.PreCredit;
            py_data["PreDeposit"] = rsp_data.PreDeposit;
            py_data["PreBalance"] = rsp_data.PreBalance;
            py_data["PreMargin"] = rsp_data.PreMargin;
            py_data["InterestBase"] = rsp_data.InterestBase;
            py_data["Interest"] = rsp_data.Interest;
            py_data["Deposit"] = rsp_data.Deposit;
            py_data["Withdraw"] = rsp_data.Withdraw;
            py_data["FrozenMargin"] = rsp_data.FrozenMargin;
            py_data["FrozenCash"] = rsp_data.FrozenCash;
            py_data["FrozenCommission"] = rsp_data.FrozenCommission;
            py_data["CurrMargin"] = rsp_data.CurrMargin;
            py_data["CashIn"] = rsp_data.CashIn;
            py_data["Commission"] = rsp_data.Commission;
            py_data["CloseProfit"] = rsp_data.CloseProfit;
            py_data["PositionProfit"] = rsp_data.PositionProfit;
            py_data["Balance"] = rsp_data.Balance;
            py_data["Available"] = rsp_data.Available;
            py_data["WithdrawQuota"] = rsp_data.WithdrawQuota;
            py_data["Reserve"] = rsp_data.Reserve;
            py_data["TradingDay"] = gbk_to_utf8(rsp_data.TradingDay);
            py_data["SettlementID"] = rsp_data.SettlementID;
            py_data["Credit"] = rsp_data.Credit;
            py_data["Mortgage"] = rsp_data.Mortgage;
            py_data["ExchangeMargin"] = rsp_data.ExchangeMargin;
            py_data["DeliveryMargin"] = rsp_data.DeliveryMargin;
            py_data["ExchangeDeliveryMargin"] = rsp_data.ExchangeDeliveryMargin;
            py_data["ReserveBalance"] = rsp_data.ReserveBalance;
            py_data["CurrencyID"] = gbk_to_utf8(rsp_data.CurrencyID);
            py_data["PreFundMortgageIn"] = rsp_data.PreFundMortgageIn;
            py_data["PreFundMortgageOut"] = rsp_data.PreFundMortgageOut;
            py_data["FundMortgageIn"] = rsp_data.FundMortgageIn;
            py_data["FundMortgageOut"] = rsp_data.FundMortgageOut;
            py_data["FundMortgageAvailable"] = rsp_data.FundMortgageAvailable;
            py_data["MortgageableFund"] = rsp_data.MortgageableFund;
            py_data["SpecProductMargin"] = rsp_data.SpecProductMargin;
            py_data["SpecProductFrozenMargin"] = rsp_data.SpecProductFrozenMargin;
            py_data["SpecProductCommission"] = rsp_data.SpecProductCommission;
            py_data["SpecProductFrozenCommission"] = rsp_data.SpecProductFrozenCommission;
            py_data["SpecProductPositionProfit"] = rsp_data.SpecProductPositionProfit;
            py_data["SpecProductCloseProfit"] = rsp_data.SpecProductCloseProfit;
            py_data["SpecProductPositionProfitByAlg"] = rsp_data.SpecProductPositionProfitByAlg;
            py_data["SpecProductExchangeMargin"] = rsp_data.SpecProductExchangeMargin;
            py_data["BizType"] = rsp_data.BizType;
            py_data["FrozenSwap"] = rsp_data.FrozenSwap;
            py_data["RemainSwap"] = rsp_data.RemainSwap;
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspQryTradingAccount(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspQryInstrument(CThostFtdcInstrumentField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcInstrumentField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["InstrumentName"] = gbk_to_utf8(rsp_data.InstrumentName);
            py_data["ProductClass"] = rsp_data.ProductClass;
            py_data["DeliveryYear"] = rsp_data.DeliveryYear;
            py_data["DeliveryMonth"] = rsp_data.DeliveryMonth;
            py_data["MaxMarketOrderVolume"] = rsp_data.MaxMarketOrderVolume;
            py_data["MinMarketOrderVolume"] = rsp_data.MinMarketOrderVolume;
            py_data["MaxLimitOrderVolume"] = rsp_data.MaxLimitOrderVolume;
            py_data["MinLimitOrderVolume"] = rsp_data.MinLimitOrderVolume;
            py_data["VolumeMultiple"] = rsp_data.VolumeMultiple;
            py_data["PriceTick"] = rsp_data.PriceTick;
            py_data["CreateDate"] = gbk_to_utf8(rsp_data.CreateDate);
            py_data["OpenDate"] = gbk_to_utf8(rsp_data.OpenDate);
            py_data["ExpireDate"] = gbk_to_utf8(rsp_data.ExpireDate);
            py_data["StartDelivDate"] = gbk_to_utf8(rsp_data.StartDelivDate);
            py_data["EndDelivDate"] = gbk_to_utf8(rsp_data.EndDelivDate);
            py_data["InstLifePhase"] = rsp_data.InstLifePhase;
            py_data["IsTrading"] = rsp_data.IsTrading;
            py_data["PositionType"] = rsp_data.PositionType;
            py_data["PositionDateType"] = rsp_data.PositionDateType;
            py_data["LongMarginRatio"] = rsp_data.LongMarginRatio;
            py_data["ShortMarginRatio"] = rsp_data.ShortMarginRatio;
            py_data["MaxMarginSideAlgorithm"] = rsp_data.MaxMarginSideAlgorithm;
            py_data["StrikePrice"] = rsp_data.StrikePrice;
            py_data["OptionsType"] = rsp_data.OptionsType;
            py_data["UnderlyingMultiple"] = rsp_data.UnderlyingMultiple;
            py_data["CombinationType"] = rsp_data.CombinationType;
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
            py_data["ExchangeInstID"] = gbk_to_utf8(rsp_data.ExchangeInstID);
            py_data["ProductID"] = gbk_to_utf8(rsp_data.ProductID);
            py_data["UnderlyingInstrID"] = gbk_to_utf8(rsp_data.UnderlyingInstrID);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspQryInstrument(py_data, py_error, request_id, is_last);
    });
}

void TdApi::OnRspError(CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspError(py_error, request_id, is_last);
    });
}

void TdApi::OnRtnOrder(CThostFtdcOrderField *data) {
    CThostFtdcOrderField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["InvestorID"] = gbk_to_utf8(rsp_data.InvestorID);
            py_data["OrderRef"] = gbk_to_utf8(rsp_data.OrderRef);
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
            py_data["OrderPriceType"] = rsp_data.OrderPriceType;
            py_data["Direction"] = rsp_data.Direction;
            py_data["CombOffsetFlag"] = gbk_to_utf8(rsp_data.CombOffsetFlag);
            py_data["CombHedgeFlag"] = gbk_to_utf8(rsp_data.CombHedgeFlag);
            py_data["LimitPrice"] = rsp_data.LimitPrice;
            py_data["VolumeTotalOriginal"] = rsp_data.VolumeTotalOriginal;
            py_data["TimeCondition"] = rsp_data.TimeCondition;
            py_data["GTDDate"] = gbk_to_utf8(rsp_data.GTDDate);
            py_data["VolumeCondition"] = rsp_data.VolumeCondition;
            py_data["MinVolume"] = rsp_data.MinVolume;
            py_data["ContingentCondition"] = rsp_data.ContingentCondition;
            py_data["StopPrice"] = rsp_data.StopPrice;
            py_data["ForceCloseReason"] = rsp_data.ForceCloseReason;
            py_data["IsAutoSuspend"] = rsp_data.IsAutoSuspend;
            py_data["BusinessUnit"] = gbk_to_utf8(rsp_data.BusinessUnit);
            py_data["RequestID"] = rsp_data.RequestID;
            py_data["OrderLocalID"] = gbk_to_utf8(rsp_data.OrderLocalID);
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["ParticipantID"] = gbk_to_utf8(rsp_data.ParticipantID);
            py_data["ClientID"] = gbk_to_utf8(rsp_data.ClientID);
            py_data["TraderID"] = gbk_to_utf8(rsp_data.TraderID);
            py_data["InstallID"] = rsp_data.InstallID;
            py_data["OrderSubmitStatus"] = rsp_data.OrderSubmitStatus;
            py_data["NotifySequence"] = rsp_data.NotifySequence;
            py_data["TradingDay"] = gbk_to_utf8(rsp_data.TradingDay);
            py_data["SettlementID"] = rsp_data.SettlementID;
            py_data["OrderSysID"] = gbk_to_utf8(rsp_data.OrderSysID);
            py_data["OrderSource"] = rsp_data.OrderSource;
            py_data["OrderStatus"] = rsp_data.OrderStatus;
            py_data["OrderType"] = rsp_data.OrderType;
            py_data["VolumeTraded"] = rsp_data.VolumeTraded;
            py_data["VolumeTotal"] = rsp_data.VolumeTotal;
            py_data["InsertDate"] = gbk_to_utf8(rsp_data.InsertDate);
            py_data["InsertTime"] = gbk_to_utf8(rsp_data.InsertTime);
            py_data["ActiveTime"] = gbk_to_utf8(rsp_data.ActiveTime);
            py_data["SuspendTime"] = gbk_to_utf8(rsp_data.SuspendTime);
            py_data["UpdateTime"] = gbk_to_utf8(rsp_data.UpdateTime);
            py_data["CancelTime"] = gbk_to_utf8(rsp_data.CancelTime);
            py_data["ActiveTraderID"] = gbk_to_utf8(rsp_data.ActiveTraderID);
            py_data["ClearingPartID"] = gbk_to_utf8(rsp_data.ClearingPartID);
            py_data["SequenceNo"] = rsp_data.SequenceNo;
            py_data["FrontID"] = rsp_data.FrontID;
            py_data["SessionID"] = rsp_data.SessionID;
            py_data["UserProductInfo"] = gbk_to_utf8(rsp_data.UserProductInfo);
            py_data["StatusMsg"] = gbk_to_utf8(rsp_data.StatusMsg);
            py_data["UserForceClose"] = rsp_data.UserForceClose;
            py_data["ActiveUserID"] = gbk_to_utf8(rsp_data.ActiveUserID);
            py_data["BrokerOrderSeq"] = rsp_data.BrokerOrderSeq;
            py_data["RelativeOrderSysID"] = gbk_to_utf8(rsp_data.RelativeOrderSysID);
            py_data["ZCETotalTradedVolume"] = rsp_data.ZCETotalTradedVolume;
            py_data["IsSwapOrder"] = rsp_data.IsSwapOrder;
            py_data["BranchID"] = gbk_to_utf8(rsp_data.BranchID);
            py_data["InvestUnitID"] = gbk_to_utf8(rsp_data.InvestUnitID);
            py_data["AccountID"] = gbk_to_utf8(rsp_data.AccountID);
            py_data["CurrencyID"] = gbk_to_utf8(rsp_data.CurrencyID);
            py_data["MacAddress"] = gbk_to_utf8(rsp_data.MacAddress);
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
            py_data["ExchangeInstID"] = gbk_to_utf8(rsp_data.ExchangeInstID);
            py_data["IPAddress"] = gbk_to_utf8(rsp_data.IPAddress);
        }
        PyOnRtnOrder(py_data);
    });
}

void TdApi::OnRtnTrade(CThostFtdcTradeField *data) {
    CThostFtdcTradeField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["InvestorID"] = gbk_to_utf8(rsp_data.InvestorID);
            py_data["OrderRef"] = gbk_to_utf8(rsp_data.OrderRef);
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["TradeID"] = gbk_to_utf8(rsp_data.TradeID);
            py_data["Direction"] = rsp_data.Direction;
            py_data["OrderSysID"] = gbk_to_utf8(rsp_data.OrderSysID);
            py_data["ParticipantID"] = gbk_to_utf8(rsp_data.ParticipantID);
            py_data["ClientID"] = gbk_to_utf8(rsp_data.ClientID);
            py_data["TradingRole"] = rsp_data.TradingRole;
            py_data["OffsetFlag"] = rsp_data.OffsetFlag;
            py_data["HedgeFlag"] = rsp_data.HedgeFlag;
            py_data["Price"] = rsp_data.Price;
            py_data["Volume"] = rsp_data.Volume;
            py_data["TradeDate"] = gbk_to_utf8(rsp_data.TradeDate);
            py_data["TradeTime"] = gbk_to_utf8(rsp_data.TradeTime);
            py_data["TradeType"] = rsp_data.TradeType;
            py_data["PriceSource"] = rsp_data.PriceSource;
            py_data["TraderID"] = gbk_to_utf8(rsp_data.TraderID);
            py_data["OrderLocalID"] = gbk_to_utf8(rsp_data.OrderLocalID);
            py_data["ClearingPartID"] = gbk_to_utf8(rsp_data.ClearingPartID);
            py_data["BusinessUnit"] = gbk_to_utf8(rsp_data.BusinessUnit);
            py_data["SequenceNo"] = rsp_data.SequenceNo;
            py_data["TradingDay"] = gbk_to_utf8(rsp_data.TradingDay);
            py_data["SettlementID"] = rsp_data.SettlementID;
            py_data["BrokerOrderSeq"] = rsp_data.BrokerOrderSeq;
            py_data["TradeSource"] = rsp_data.TradeSource;
            py_data["InvestUnitID"] = gbk_to_utf8(rsp_data.InvestUnitID);
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
            py_data["ExchangeInstID"] = gbk_to_utf8(rsp_data.ExchangeInstID);
        }
        PyOnRtnTrade(py_data);
    });
}

void TdApi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *data, CThostFtdcRspInfoField *error) {
    CThostFtdcInputOrderField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["InvestorID"] = gbk_to_utf8(rsp_data.InvestorID);
            py_data["OrderRef"] = gbk_to_utf8(rsp_data.OrderRef);
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
            py_data["OrderPriceType"] = rsp_data.OrderPriceType;
            py_data["Direction"] = rsp_data.Direction;
            py_data["CombOffsetFlag"] = gbk_to_utf8(rsp_data.CombOffsetFlag);
            py_data["CombHedgeFlag"] = gbk_to_utf8(rsp_data.CombHedgeFlag);
            py_data["LimitPrice"] = rsp_data.LimitPrice;
            py_data["VolumeTotalOriginal"] = rsp_data.VolumeTotalOriginal;
            py_data["TimeCondition"] = rsp_data.TimeCondition;
            py_data["GTDDate"] = gbk_to_utf8(rsp_data.GTDDate);
            py_data["VolumeCondition"] = rsp_data.VolumeCondition;
            py_data["MinVolume"] = rsp_data.MinVolume;
            py_data["ContingentCondition"] = rsp_data.ContingentCondition;
            py_data["StopPrice"] = rsp_data.StopPrice;
            py_data["ForceCloseReason"] = rsp_data.ForceCloseReason;
            py_data["IsAutoSuspend"] = rsp_data.IsAutoSuspend;
            py_data["BusinessUnit"] = gbk_to_utf8(rsp_data.BusinessUnit);
            py_data["RequestID"] = rsp_data.RequestID;
            py_data["UserForceClose"] = rsp_data.UserForceClose;
            py_data["IsSwapOrder"] = rsp_data.IsSwapOrder;
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["InvestUnitID"] = gbk_to_utf8(rsp_data.InvestUnitID);
            py_data["AccountID"] = gbk_to_utf8(rsp_data.AccountID);
            py_data["CurrencyID"] = gbk_to_utf8(rsp_data.CurrencyID);
            py_data["ClientID"] = gbk_to_utf8(rsp_data.ClientID);
            py_data["MacAddress"] = gbk_to_utf8(rsp_data.MacAddress);
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
            py_data["IPAddress"] = gbk_to_utf8(rsp_data.IPAddress);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnErrRtnOrderInsert(py_data, py_error);
    });
}

void TdApi::OnErrRtnOrderAction(CThostFtdcOrderActionField *data, CThostFtdcRspInfoField *error) {
    CThostFtdcOrderActionField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    CThostFtdcRspInfoField rsp_error{};
    bool has_error = false;
    if (error) {
        rsp_error = *error;
        has_error = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["BrokerID"] = gbk_to_utf8(rsp_data.BrokerID);
            py_data["InvestorID"] = gbk_to_utf8(rsp_data.InvestorID);
            py_data["OrderActionRef"] = rsp_data.OrderActionRef;
            py_data["OrderRef"] = gbk_to_utf8(rsp_data.OrderRef);
            py_data["RequestID"] = rsp_data.RequestID;
            py_data["FrontID"] = rsp_data.FrontID;
            py_data["SessionID"] = rsp_data.SessionID;
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["OrderSysID"] = gbk_to_utf8(rsp_data.OrderSysID);
            py_data["ActionFlag"] = rsp_data.ActionFlag;
            py_data["LimitPrice"] = rsp_data.LimitPrice;
            py_data["VolumeChange"] = rsp_data.VolumeChange;
            py_data["ActionDate"] = gbk_to_utf8(rsp_data.ActionDate);
            py_data["ActionTime"] = gbk_to_utf8(rsp_data.ActionTime);
            py_data["TraderID"] = gbk_to_utf8(rsp_data.TraderID);
            py_data["InstallID"] = rsp_data.InstallID;
            py_data["OrderLocalID"] = gbk_to_utf8(rsp_data.OrderLocalID);
            py_data["ActionLocalID"] = gbk_to_utf8(rsp_data.ActionLocalID);
            py_data["ParticipantID"] = gbk_to_utf8(rsp_data.ParticipantID);
            py_data["ClientID"] = gbk_to_utf8(rsp_data.ClientID);
            py_data["BusinessUnit"] = gbk_to_utf8(rsp_data.BusinessUnit);
            py_data["OrderActionStatus"] = rsp_data.OrderActionStatus;
            py_data["UserID"] = gbk_to_utf8(rsp_data.UserID);
            py_data["StatusMsg"] = gbk_to_utf8(rsp_data.StatusMsg);
            py_data["BranchID"] = gbk_to_utf8(rsp_data.BranchID);
            py_data["InvestUnitID"] = gbk_to_utf8(rsp_data.InvestUnitID);
            py_data["MacAddress"] = gbk_to_utf8(rsp_data.MacAddress);
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
            py_data["IPAddress"] = gbk_to_utf8(rsp_data.IPAddress);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnErrRtnOrderAction(py_data, py_error);
    });
}
