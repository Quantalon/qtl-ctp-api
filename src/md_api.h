#ifndef MD_API_H
#define MD_API_H

#include <iostream>
#include <string>
#include <pybind11/pybind11.h>

#include "utils.h"
#include "dispatch_queue.h"
#include "ThostFtdcMdApi.h"


namespace py = pybind11;


class MdApi : public CThostFtdcMdSpi {
private:
    CtpUniquePtr<CThostFtdcMdApi> api_;
    std::unique_ptr<DispatchQueue> queue_;

public:
    void CreateApi(const std::string &flow_path);
    static std::string GetApiVersion();
    void Init();
    int Join();
    std::string GetTradingDay();
    void RegisterFront(const std::string &front_address);
    int SubscribeMarketData(const std::string &instrument_id);
    int UnSubscribeMarketData(const std::string &instrument_id);

    int ReqUserLogin(const py::dict &data, int request_id);
    int ReqUserLogout(const py::dict &data, int request_id);

    void OnFrontConnected() override;
    void OnFrontDisconnected(int nReason) override;
    void OnHeartBeatWarning(int nTimeLapse) override;
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) override;

    virtual void PyOnFrontConnected() = 0;
    virtual void PyOnFrontDisconnected(int reason) = 0;
    virtual void PyOnHeartBeatWarning(int time_lapse) = 0;
    virtual void PyOnRspUserLogin(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspUserLogout(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspError(const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspSubMarketData(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspUnSubMarketData(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRtnDepthMarketData(const py::dict &data) = 0;
};


class PyMdApi final : public MdApi {
public:
    using MdApi::MdApi;

    void PyOnFrontConnected() override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            MdApi,
            "OnFrontConnected",
            PyOnFrontConnected,
        )
    }

    void PyOnFrontDisconnected(int reason) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            MdApi,
            "OnFrontDisconnected",
            PyOnFrontDisconnected,
            reason
        )
    }

    void PyOnHeartBeatWarning(int time_lapse) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            MdApi,
            "OnHeartBeatWarning",
            PyOnHeartBeatWarning,
            time_lapse
        )
    }

    void PyOnRspUserLogin(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            MdApi,
            "OnRspUserLogin",
            PyOnRspUserLogin,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspUserLogout(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            MdApi,
            "OnRspUserLogout",
            PyOnRspUserLogout,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspError(const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            MdApi,
            "OnRspError",
            PyOnRspError,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspSubMarketData(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            MdApi,
            "OnRspSubMarketData",
            PyOnRspSubMarketData,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspUnSubMarketData(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            MdApi,
            "OnRspUnSubMarketData",
            PyOnRspUnSubMarketData,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRtnDepthMarketData(const py::dict &data) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            MdApi,
            "OnRtnDepthMarketData",
            PyOnRtnDepthMarketData,
            data
        )
    }

};

#endif //MD_API_H