#ifndef TD_API_H
#define TD_API_H

#include <iostream>
#include <string>
#include <pybind11/pybind11.h>

#include "utils.h"
#include "dispatch_queue.h"
#include "ThostFtdcTraderApi.h"


namespace py = pybind11;


class TdApi : public CThostFtdcTraderSpi {
private:
    CtpUniquePtr<CThostFtdcTraderApi> api_;
    std::unique_ptr<DispatchQueue> queue_;

public:
    void CreateApi(const std::string &flow_path);
    static std::string GetApiVersion();
    void Init();
    int Join();
    std::string GetTradingDay();
    void RegisterFront(const std::string &front_address);
    void SubscribePrivateTopic(int resume_type);
    void SubscribePublicTopic(int resume_type);

    int ReqAuthenticate(const py::dict &data, int request_id);
    int ReqUserLogin(const py::dict &data, int request_id);
    int ReqUserLogout(const py::dict &data, int request_id);
    int ReqOrderInsert(const py::dict &data, int request_id);
    int ReqOrderAction(const py::dict &data, int request_id);
    int ReqSettlementInfoConfirm(const py::dict &data, int request_id);
    int ReqQryOrder(const py::dict &data, int request_id);
    int ReqQryTrade(const py::dict &data, int request_id);
    int ReqQryInvestorPosition(const py::dict &data, int request_id);
    int ReqQryTradingAccount(const py::dict &data, int request_id);
    int ReqQryInstrument(const py::dict &data, int request_id);

    void OnFrontConnected() override;
    void OnFrontDisconnected(int nReason) override;
    void OnHeartBeatWarning(int nTimeLapse) override;
    void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRtnOrder(CThostFtdcOrderField *pOrder) override;
    void OnRtnTrade(CThostFtdcTradeField *pTrade) override;

    virtual void PyOnFrontConnected() = 0;
    virtual void PyOnFrontDisconnected(int reason) = 0;
    virtual void PyOnHeartBeatWarning(int time_lapse) = 0;
    virtual void PyOnRspAuthenticate(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspUserLogin(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspUserLogout(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspOrderInsert(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspOrderAction(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspSettlementInfoConfirm(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQryOrder(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQryTrade(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQryInvestorPosition(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQryTradingAccount(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQryInstrument(const py::dict &data, const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspError(const py::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRtnOrder(const py::dict &data) = 0;
    virtual void PyOnRtnTrade(const py::dict &data) = 0;
};


class PyTdApi final : public TdApi {
public:
    using TdApi::TdApi;

    void PyOnFrontConnected() override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnFrontConnected",
            PyOnFrontConnected,
        )
    }

    void PyOnFrontDisconnected(int reason) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnFrontDisconnected",
            PyOnFrontDisconnected,
            reason
        )
    }

    void PyOnHeartBeatWarning(int time_lapse) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnHeartBeatWarning",
            PyOnHeartBeatWarning,
            time_lapse
        )
    }

    void PyOnRspAuthenticate(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRspAuthenticate",
            PyOnRspAuthenticate,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspUserLogin(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
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
            TdApi,
            "OnRspUserLogout",
            PyOnRspUserLogout,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspOrderInsert(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRspOrderInsert",
            PyOnRspOrderInsert,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspOrderAction(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRspOrderAction",
            PyOnRspOrderAction,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspSettlementInfoConfirm(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRspSettlementInfoConfirm",
            PyOnRspSettlementInfoConfirm,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspQryOrder(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRspQryOrder",
            PyOnRspQryOrder,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspQryTrade(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRspQryTrade",
            PyOnRspQryTrade,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspQryInvestorPosition(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRspQryInvestorPosition",
            PyOnRspQryInvestorPosition,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspQryTradingAccount(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRspQryTradingAccount",
            PyOnRspQryTradingAccount,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspQryInstrument(const py::dict &data, const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRspQryInstrument",
            PyOnRspQryInstrument,
            data,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRspError(const py::dict &error, int request_id, bool is_last) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRspError",
            PyOnRspError,
            error,
            request_id,
            is_last
        )
    }

    void PyOnRtnOrder(const py::dict &data) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRtnOrder",
            PyOnRtnOrder,
            data
        )
    }

    void PyOnRtnTrade(const py::dict &data) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            TdApi,
            "OnRtnTrade",
            PyOnRtnTrade,
            data
        )
    }

};

#endif //TD_API_H