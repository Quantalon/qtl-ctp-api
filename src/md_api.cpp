#include "md_api.h"


void MdApi::CreateApi(const std::string &flow_path) {
    queue_ = std::make_unique<DispatchQueue>();
    api_ = CThostFtdcMdApi::CreateFtdcMdApi(flow_path.c_str());
    api_->RegisterSpi(this);
}

void MdApi::Release() {
    api_->RegisterSpi(nullptr);
    api_->Release();
    api_ = nullptr;
    queue_ = nullptr;
}

void MdApi::Init() {
    api_->Init();
}

int MdApi::Join() {
    return api_->Join();
}

std::string MdApi::GetApiVersion() {
    return CThostFtdcMdApi::GetApiVersion();
}

std::string MdApi::GetTradingDay() {
    return api_->GetTradingDay();
}

void MdApi::RegisterFront(const std::string &front_address) {
    api_->RegisterFront(const_cast<char *>(front_address.c_str()));
}

int MdApi::SubscribeMarketData(const std::string &instrument_id) {
    char *buffer = (char*)instrument_id.c_str();
    char *req[1] = { buffer };
    return api_->SubscribeMarketData(req, 1);
}

int MdApi::UnSubscribeMarketData(const std::string &instrument_id) {
    char *buffer = (char*)instrument_id.c_str();
    char *req[1] = { buffer };
    return api_->UnSubscribeMarketData(req, 1);
}

int MdApi::ReqUserLogin(const py::dict &data, int request_id) {
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

int MdApi::ReqUserLogout(const py::dict &data, int request_id) {
    CThostFtdcUserLogoutField request{};
    set_str_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_str_field(request.UserID, data, "UserID", sizeof(request.UserID));
    return api_->ReqUserLogout(&request, request_id);
}

void MdApi::OnFrontConnected() {
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        PyOnFrontConnected();
    });
}

void MdApi::OnFrontDisconnected(int reason) {
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        PyOnFrontDisconnected(reason);
    });
}

void MdApi::OnHeartBeatWarning(int time_lapse) {
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        PyOnHeartBeatWarning(time_lapse);
    });
}

void MdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
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
            py_data["SysVersion"] = gbk_to_utf8(rsp_data.SysVersion);
            py_data["GFEXTime"] = gbk_to_utf8(rsp_data.GFEXTime);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspUserLogin(py_data, py_error, request_id, is_last);
    });
}

void MdApi::OnRspUserLogout(CThostFtdcUserLogoutField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
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

void MdApi::OnRspError(CThostFtdcRspInfoField *error, int request_id, bool is_last) {
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

void MdApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcSpecificInstrumentField rsp_data{};
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
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspSubMarketData(py_data, py_error, request_id, is_last);
    });
}

void MdApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *data, CThostFtdcRspInfoField *error, int request_id, bool is_last) {
    CThostFtdcSpecificInstrumentField rsp_data{};
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
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
        }
        py::dict py_error;
        if (has_error) {
            py_error["ErrorID"] = rsp_error.ErrorID;
            py_error["ErrorMsg"] = gbk_to_utf8(rsp_error.ErrorMsg);
        }
        PyOnRspUnSubMarketData(py_data, py_error, request_id, is_last);
    });
}

void MdApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *data) {
    CThostFtdcDepthMarketDataField rsp_data{};
    bool has_data = false;
    if (data) {
        rsp_data = *data;
        has_data = true;
    }
    queue_->dispatch([=]() {
        py::gil_scoped_acquire acquire;
        py::dict py_data;
        if (has_data) {
            py_data["TradingDay"] = gbk_to_utf8(rsp_data.TradingDay);
            py_data["ExchangeID"] = gbk_to_utf8(rsp_data.ExchangeID);
            py_data["LastPrice"] = adjust_number(rsp_data.LastPrice);
            py_data["PreSettlementPrice"] = adjust_number(rsp_data.PreSettlementPrice);
            py_data["PreClosePrice"] = adjust_number(rsp_data.PreClosePrice);
            py_data["PreOpenInterest"] = rsp_data.PreOpenInterest;
            py_data["OpenPrice"] = adjust_number(rsp_data.OpenPrice);
            py_data["HighestPrice"] = adjust_number(rsp_data.HighestPrice);
            py_data["LowestPrice"] = adjust_number(rsp_data.LowestPrice);
            py_data["Volume"] = rsp_data.Volume;
            py_data["Turnover"] = rsp_data.Turnover;
            py_data["OpenInterest"] = rsp_data.OpenInterest;
            py_data["ClosePrice"] = adjust_number(rsp_data.ClosePrice);
            py_data["SettlementPrice"] = adjust_number(rsp_data.SettlementPrice);
            py_data["UpperLimitPrice"] = adjust_number(rsp_data.UpperLimitPrice);
            py_data["LowerLimitPrice"] = adjust_number(rsp_data.LowerLimitPrice);
            py_data["PreDelta"] = adjust_number(rsp_data.PreDelta);
            py_data["CurrDelta"] = adjust_number(rsp_data.CurrDelta);
            py_data["UpdateTime"] = gbk_to_utf8(rsp_data.UpdateTime);
            py_data["UpdateMillisec"] = rsp_data.UpdateMillisec;
            py_data["BidPrice1"] = adjust_number(rsp_data.BidPrice1);
            py_data["BidVolume1"] = rsp_data.BidVolume1;
            py_data["AskPrice1"] = adjust_number(rsp_data.AskPrice1);
            py_data["AskVolume1"] = rsp_data.AskVolume1;
            py_data["BidPrice2"] = adjust_number(rsp_data.BidPrice2);
            py_data["BidVolume2"] = rsp_data.BidVolume2;
            py_data["AskPrice2"] = adjust_number(rsp_data.AskPrice2);
            py_data["AskVolume2"] = rsp_data.AskVolume2;
            py_data["BidPrice3"] = adjust_number(rsp_data.BidPrice3);
            py_data["BidVolume3"] = rsp_data.BidVolume3;
            py_data["AskPrice3"] = adjust_number(rsp_data.AskPrice3);
            py_data["AskVolume3"] = rsp_data.AskVolume3;
            py_data["BidPrice4"] = adjust_number(rsp_data.BidPrice4);
            py_data["BidVolume4"] = rsp_data.BidVolume4;
            py_data["AskPrice4"] = adjust_number(rsp_data.AskPrice4);
            py_data["AskVolume4"] = rsp_data.AskVolume4;
            py_data["BidPrice5"] = adjust_number(rsp_data.BidPrice5);
            py_data["BidVolume5"] = rsp_data.BidVolume5;
            py_data["AskPrice5"] = adjust_number(rsp_data.AskPrice5);
            py_data["AskVolume5"] = rsp_data.AskVolume5;
            py_data["AveragePrice"] = adjust_number(rsp_data.AveragePrice);
            py_data["ActionDay"] = gbk_to_utf8(rsp_data.ActionDay);
            py_data["InstrumentID"] = gbk_to_utf8(rsp_data.InstrumentID);
            py_data["ExchangeInstID"] = gbk_to_utf8(rsp_data.ExchangeInstID);
            py_data["BandingUpperPrice"] = adjust_number(rsp_data.BandingUpperPrice);
            py_data["BandingLowerPrice"] = adjust_number(rsp_data.BandingLowerPrice);
        }
        PyOnRtnDepthMarketData(py_data);
    });
}
