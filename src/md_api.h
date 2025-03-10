#ifndef MD_API_H
#define MD_API_H

#include <iostream>
#include <string>
#include <optional>
#include <nanobind/nanobind.h>
#include <nanobind/trampoline.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "utils.h"
#include "dispatch_queue.h"
#include "ThostFtdcMdApi.h"


namespace nb = nanobind;


class MdApi : public CThostFtdcMdSpi {
private:
    std::unique_ptr<DispatchQueue> queue_;
    CThostFtdcMdApi* api_;

public:
    void CreateApi(const std::string &flow_path);
    void Release();
    void Init();
    int Join();
    static std::string GetApiVersion();
    std::string GetTradingDay();
    void RegisterFront(const std::string &front_address);
    int SubscribeMarketData(const std::vector<std::string> &instrument_ids);
    int UnSubscribeMarketData(const std::vector<std::string> &instrument_ids);

    int ReqUserLogin(const nb::dict &data, int request_id);
    int ReqUserLogout(const nb::dict &data, int request_id);

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
    virtual void PyOnRspUserLogin(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspUserLogout(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspError(const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspSubMarketData(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspUnSubMarketData(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRtnDepthMarketData(const nb::dict &data) = 0;
};


class PyMdApi final : public MdApi {
public:
    NB_TRAMPOLINE(MdApi, 9);

    void PyOnFrontConnected() override {
        NB_OVERRIDE_PURE_NAME(
            "OnFrontConnected",
            PyOnFrontConnected,
        );
    }

    void PyOnFrontDisconnected(int reason) override {
        NB_OVERRIDE_PURE_NAME(
            "OnFrontDisconnected",
            PyOnFrontDisconnected,
            reason
        );
    }

    void PyOnHeartBeatWarning(int time_lapse) override {
        NB_OVERRIDE_PURE_NAME(
            "OnHeartBeatWarning",
            PyOnHeartBeatWarning,
            time_lapse
        );
    }

    void PyOnRspUserLogin(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspUserLogin",
            PyOnRspUserLogin,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspUserLogout(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspUserLogout",
            PyOnRspUserLogout,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspError(const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspError",
            PyOnRspError,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspSubMarketData(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspSubMarketData",
            PyOnRspSubMarketData,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspUnSubMarketData(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspUnSubMarketData",
            PyOnRspUnSubMarketData,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRtnDepthMarketData(const nb::dict &data) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRtnDepthMarketData",
            PyOnRtnDepthMarketData,
            data
        );
    }

};

#endif //MD_API_H