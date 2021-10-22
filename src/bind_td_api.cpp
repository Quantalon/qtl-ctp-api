#include "bind_td_api.h"

void bind_td_api(py::module &m) {
    py::class_<TdApi, PyTdApi> td_api(m, "TdApi", py::module_local());
    td_api
            .def(py::init<>())
            .def("CreateApi", &TdApi::CreateApi)
            .def_static("GetApiVersion", &TdApi::GetApiVersion)
            .def("Init", &TdApi::Init)
            .def("Join", &TdApi::Join)
            .def("GetTradingDay", &TdApi::GetTradingDay)
            .def("RegisterFront", &TdApi::RegisterFront)
            .def("SubscribePrivateTopic", &TdApi::SubscribePrivateTopic)
            .def("SubscribePublicTopic", &TdApi::SubscribePublicTopic)
            // Req Methods
            .def("ReqAuthenticate", &TdApi::ReqAuthenticate)
            .def("ReqUserLogin", &TdApi::ReqUserLogin)
            .def("ReqUserLogout", &TdApi::ReqUserLogout)
            .def("ReqOrderInsert", &TdApi::ReqOrderInsert)
            .def("ReqOrderAction", &TdApi::ReqOrderAction)
            .def("ReqSettlementInfoConfirm", &TdApi::ReqSettlementInfoConfirm)
            .def("ReqQryOrder", &TdApi::ReqQryOrder)
            .def("ReqQryTrade", &TdApi::ReqQryTrade)
            .def("ReqQryInvestorPosition", &TdApi::ReqQryInvestorPosition)
            .def("ReqQryTradingAccount", &TdApi::ReqQryTradingAccount)
            .def("ReqQryInstrument", &TdApi::ReqQryInstrument)
            // On Methods
            .def("OnFrontConnected", &TdApi::PyOnFrontConnected)
            .def("OnFrontDisconnected", &TdApi::PyOnFrontDisconnected)
            .def("OnHeartBeatWarning", &TdApi::PyOnHeartBeatWarning)
            .def("OnRspAuthenticate", &TdApi::PyOnRspAuthenticate)
            .def("OnRspUserLogin", &TdApi::PyOnRspUserLogin)
            .def("OnRspUserLogout", &TdApi::PyOnRspUserLogout)
            .def("OnRspOrderInsert", &TdApi::PyOnRspOrderInsert)
            .def("OnRspOrderAction", &TdApi::PyOnRspOrderAction)
            .def("OnRspSettlementInfoConfirm", &TdApi::PyOnRspSettlementInfoConfirm)
            .def("OnRspQryOrder", &TdApi::PyOnRspQryOrder)
            .def("OnRspQryTrade", &TdApi::PyOnRspQryTrade)
            .def("OnRspQryInvestorPosition", &TdApi::PyOnRspQryInvestorPosition)
            .def("OnRspQryTradingAccount", &TdApi::PyOnRspQryTradingAccount)
            .def("OnRspQryInstrument", &TdApi::PyOnRspQryInstrument)
            .def("OnRspError", &TdApi::PyOnRspError)
            .def("OnRtnOrder", &TdApi::PyOnRtnOrder)
            .def("OnRtnTrade", &TdApi::PyOnRtnTrade)
            ;
}