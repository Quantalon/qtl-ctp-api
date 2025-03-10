#ifndef TD_API_H
#define TD_API_H

#include <iostream>
#include <string>
#include <optional>
#include <nanobind/nanobind.h>
#include <nanobind/trampoline.h>
#include <nanobind/stl/string.h>

#include "utils.h"
#include "dispatch_queue.h"
#include "ThostFtdcTraderApi.h"


namespace nb = nanobind;


class TdApi : public CThostFtdcTraderSpi {
private:
    std::unique_ptr<DispatchQueue> queue_;
    CThostFtdcTraderApi* api_;

public:
    void CreateApi(const std::string &flow_path);
    void Release();
    void Init();
    int Join();
    static std::string GetApiVersion();
    std::string GetTradingDay();
    void RegisterFront(const std::string &front_address);
    void SubscribePrivateTopic(int resume_type);
    void SubscribePublicTopic(int resume_type);

    int ReqAuthenticate(const nb::dict &data, int request_id);
    int ReqUserLogin(const nb::dict &data, int request_id);
    int ReqUserLogout(const nb::dict &data, int request_id);
    int ReqUserPasswordUpdate(const nb::dict &data, int request_id);
    int ReqOrderInsert(const nb::dict &data, int request_id);
    int ReqOrderAction(const nb::dict &data, int request_id);
    int ReqSettlementInfoConfirm(const nb::dict &data, int request_id);
    int ReqQryOrder(const nb::dict &data, int request_id);
    int ReqQryTrade(const nb::dict &data, int request_id);
    int ReqQryInvestorPosition(const nb::dict &data, int request_id);
    int ReqQryTradingAccount(const nb::dict &data, int request_id);
    int ReqQryInstrument(const nb::dict &data, int request_id);
    int ReqQrySettlementInfo(const nb::dict &data, int request_id);

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
    void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
    void OnRtnOrder(CThostFtdcOrderField *pOrder) override;
    void OnRtnTrade(CThostFtdcTradeField *pTrade) override;
    void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) override;
    void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) override;
    void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) override;

    virtual void PyOnFrontConnected() = 0;
    virtual void PyOnFrontDisconnected(int reason) = 0;
    virtual void PyOnHeartBeatWarning(int time_lapse) = 0;
    virtual void PyOnRspAuthenticate(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspUserLogin(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspUserLogout(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspOrderInsert(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspOrderAction(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspSettlementInfoConfirm(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQryOrder(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQryTrade(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQryInvestorPosition(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQryTradingAccount(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQryInstrument(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspQrySettlementInfo(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRspError(const nb::dict &error, int request_id, bool is_last) = 0;
    virtual void PyOnRtnOrder(const nb::dict &data) = 0;
    virtual void PyOnRtnTrade(const nb::dict &data) = 0;
    virtual void PyOnErrRtnOrderInsert(const nb::dict &data, const nb::dict &error) = 0;
    virtual void PyOnErrRtnOrderAction(const nb::dict &data, const nb::dict &error) = 0;
    virtual void PyOnRtnInstrumentStatus(const nb::dict &data) = 0;
};


class PyTdApi final : public TdApi {
public:
    NB_TRAMPOLINE(TdApi, 21);

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

    void PyOnRspAuthenticate(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspAuthenticate",
            PyOnRspAuthenticate,
            data,
            error,
            request_id,
            is_last
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

    void PyOnRspOrderInsert(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspOrderInsert",
            PyOnRspOrderInsert,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspOrderAction(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspOrderAction",
            PyOnRspOrderAction,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspSettlementInfoConfirm(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspSettlementInfoConfirm",
            PyOnRspSettlementInfoConfirm,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspQryOrder(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspQryOrder",
            PyOnRspQryOrder,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspQryTrade(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspQryTrade",
            PyOnRspQryTrade,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspQryInvestorPosition(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspQryInvestorPosition",
            PyOnRspQryInvestorPosition,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspQryTradingAccount(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspQryTradingAccount",
            PyOnRspQryTradingAccount,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspQryInstrument(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspQryInstrument",
            PyOnRspQryInstrument,
            data,
            error,
            request_id,
            is_last
        );
    }

    void PyOnRspQrySettlementInfo(const nb::dict &data, const nb::dict &error, int request_id, bool is_last) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRspQrySettlementInfo",
            PyOnRspQrySettlementInfo,
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

    void PyOnRtnOrder(const nb::dict &data) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRtnOrder",
            PyOnRtnOrder,
            data
        );
    }

    void PyOnRtnTrade(const nb::dict &data) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRtnTrade",
            PyOnRtnTrade,
            data
        );
    }

    void PyOnErrRtnOrderInsert(const nb::dict &data, const nb::dict &error) override {
        NB_OVERRIDE_PURE_NAME(
            "OnErrRtnOrderInsert",
            PyOnErrRtnOrderInsert,
            data,
            error
        );
    }

    void PyOnErrRtnOrderAction(const nb::dict &data, const nb::dict &error) override {
        NB_OVERRIDE_PURE_NAME(
            "OnErrRtnOrderAction",
            PyOnErrRtnOrderAction,
            data,
            error
        );
    }

    void PyOnRtnInstrumentStatus(const nb::dict &data) override {
        NB_OVERRIDE_PURE_NAME(
            "OnRtnInstrumentStatus",
            PyOnRtnInstrumentStatus,
            data
        );
    }

};

#endif //TD_API_H