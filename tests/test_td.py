from time import sleep

from qtl_ctp_api import TdApi, consts


class PyTdApi(TdApi):

    def __init__(self, settings):
        super().__init__()
        self.settings = settings
        self.request_id = 0
        self.settlement_info_content = b''
        self.CreateApi(self.settings['flow_path'])

    def next_request_id(self):
        self.request_id += 1
        return self.request_id

    def connect(self):
        print('PyTdApi.connect')
        self.RegisterFront(self.settings['address'])
        self.SubscribePrivateTopic(consts.THOST_TERT_QUICK)
        self.SubscribePublicTopic(consts.THOST_TERT_RESTART)
        self.Init()

    def auth(self):
        print('PyTdApi.auth')
        request = {}
        request['BrokerID'] = self.settings['broker_id']
        request['UserID'] = self.settings['user_id']
        request['AuthCode'] = self.settings['auth_code']
        request['AppID'] = self.settings['app_id']
        self.ReqAuthenticate(request, self.next_request_id())

    def login(self):
        print('PyTdApi.login')
        request = {}
        request['BrokerID'] = self.settings['broker_id']
        request['UserID'] = self.settings['user_id']
        request['Password'] = self.settings['password']
        self.request_id += 1
        self.ReqUserLogin(request, self.request_id)

    def OnHeartBeatWarning(self, time_lapse):
        print('PyTdApi.OnHeartBeatWarning')
        print(f'Timelapse: {time_lapse}')

    def OnFrontConnected(self):
        print("PyTdApi.OnFrontConnected")
        self.auth()

    def OnFrontDisconnected(self, reason):
        print(f"PyTdApi.OnFrontDisconnected, reason: {reason}")
        pass

    def OnRspAuthenticate(self, data, error, request_id, is_last):
        print(f"PyTdApi.OnRspAuthenticate")
        print(f"error ErrorID: {error['ErrorID']}")
        print(f"error ErrorMsg: {error['ErrorMsg']}")
        if is_last:
            self.login()

    def OnRspUserLogin(self, data, error, request_id, is_last):
        print("PyTdApi.OnRspUserLogin")

        print(f"data FrontID: {data['FrontID']}")
        print(f"data SessionID: {data['SessionID']}")

        print(f"error ErrorID: {error['ErrorID']}")
        print(f"error ErrorMsg: {error['ErrorMsg']}")

        print(f"request_id: {request_id}")
        print(f"is_last: {is_last}")

        print(f"Trading Day: {self.GetTradingDay()}")

        if is_last:
            sleep(1)
            print('PyTdApi.ReqSettlementInfoConfirm')
            request = {}
            request['BrokerID'] = self.settings['broker_id']
            request['InvestorID'] = self.settings['user_id']
            self.ReqSettlementInfoConfirm(request, self.next_request_id())

    def OnRspQryTradingAccount(self, data, error, request_id, is_last):
        print('PyTdApi.OnRspQryTradingAccount')
        print(f'data: {data}')

    def OnRspQryInvestorPosition(self, data, error, request_id, is_last):
        print('PyTdApi.OnRspQryInvestorPosition')
        print(f'data: {data}')

    def OnRspError(self, error, request_id, is_last):
        print('PyTdApi.OnRspError')
        print(f"error ErrorID: {error['ErrorID']}")
        print(f"error ErrorMsg: {error['ErrorMsg']}")

    def OnRtnInstrumentStatus(self, data):
        print('PyTdApi.OnRtnInstrumentStatus')
        print(f'data: {data}')

    def OnRspSettlementInfoConfirm(self, data, error, request_id, is_last):
        print('PyTdApi.OnRspSettlementInfoConfirm')
        print(f'data: {data}')
        print(f"error ErrorID: {error['ErrorID']}")
        print(f"error ErrorMsg: {error['ErrorMsg']}")

        if is_last:
            sleep(2)

            print('PyTdApi.ReqQrySettlementInfo')
            request = {}
            request['BrokerID'] = self.settings['broker_id']
            request['InvestorID'] = self.settings['user_id']
            self.settlement_info_content = b''
            self.ReqQrySettlementInfo(request, self.next_request_id())

    def OnRspQrySettlementInfo(self, data, error, request_id, is_last):
        print('PyTdApi.OnRspQrySettlementInfo')
        # print(f'data: {data}')
        if error:
            print(f"error ErrorID: {error['ErrorID']}")
            print(f"error ErrorMsg: {error['ErrorMsg']}")

        print('Get Part Content...')
        self.settlement_info_content += data['Content']

        if is_last:
            print('Settlement Info:')
            print(self.settlement_info_content.decode(encoding='gbk'))

            sleep(2)

            print('PyTdApi.ReqQryTradingAccount')
            request = {}
            request['BrokerID'] = self.settings['broker_id']
            request['InvestorID'] = self.settings['user_id']
            self.ReqQryTradingAccount(request, self.next_request_id())

            sleep(2)

            print('PyTdApi.ReqQryInvestorPosition')
            request = {}
            request['BrokerID'] = self.settings['broker_id']
            request['InvestorID'] = self.settings['user_id']
            self.ReqQryInvestorPosition(request, self.next_request_id())

    def OnRtnOrder(self, data):
        print('PyTdApi.OnRtnOrder')
        print(f'data: {data}')

    def OnRtnTrade(self, data):
        print('PyTdApi.OnRtnTrade')
        print(f'data:\n{data}')


def test():
    print('test TdApi...')
    print(f'version: {TdApi.GetApiVersion()}')
    settings = {
        'address': 'tcp://180.168.146.187:10201',
        'broker_id': '9999',
        'user_id': '',
        'password': '',
        'flow_path': 'flow',
        'app_id': 'simnow_client_test',
        'auth_code': '0000000000000000',
    }

    td_api = PyTdApi(settings)
    td_api.connect()

    input('Waiting...\n')


if __name__ == '__main__':
    test()
