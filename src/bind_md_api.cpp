#include "bind_md_api.h"

void bind_md_api(nb::module_ &m) {
    nb::class_<MdApi, PyMdApi> md_api(m, "MdApi");
    md_api
            .def(nb::init<>())
            .def("CreateApi", &MdApi::CreateApi)
            .def("Release", &MdApi::Release)
            .def("Init", &MdApi::Init)
            .def("Join", &MdApi::Join)
            .def_static("GetApiVersion", &MdApi::GetApiVersion)
            .def("GetTradingDay", &MdApi::GetTradingDay)
            .def("RegisterFront", &MdApi::RegisterFront)
            .def("SubscribeMarketData", &MdApi::SubscribeMarketData)
            .def("UnSubscribeMarketData", &MdApi::UnSubscribeMarketData)
            // Req Methods
            .def("ReqUserLogin", &MdApi::ReqUserLogin)
            .def("ReqUserLogout", &MdApi::ReqUserLogout)
            // On Methods
            .def("OnFrontConnected", &MdApi::PyOnFrontConnected)
            .def("OnFrontDisconnected", &MdApi::PyOnFrontDisconnected)
            .def("OnHeartBeatWarning", &MdApi::PyOnHeartBeatWarning)
            .def("OnRspUserLogin", &MdApi::PyOnRspUserLogin)
            .def("OnRspUserLogout", &MdApi::PyOnRspUserLogout)
            .def("OnRspError", &MdApi::PyOnRspError)
            .def("OnRspSubMarketData", &MdApi::PyOnRspSubMarketData)
            .def("OnRspUnSubMarketData", &MdApi::PyOnRspUnSubMarketData)
            .def("OnRtnDepthMarketData", &MdApi::PyOnRtnDepthMarketData)
            ;
}