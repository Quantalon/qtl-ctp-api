from qtl_ctp_api import MdApi


class PyMdApi(MdApi):

    def __init__(self, settings):
        super().__init__()
        self.settings = settings
        self.request_id = 0
        self.CreateApi(self.settings['flow_path'])

    def connect(self):
        print('PyMdApi.connect')
        self.RegisterFront(self.settings['address'])
        self.Init()

    def login(self):
        print('PyMdApi.login')
        request = {}
        request['BrokerID'] = self.settings['broker_id']
        request['UserID'] = self.settings['user_id']
        request['Password'] = self.settings['password']

        self.request_id += 1
        self.ReqUserLogin(request, self.request_id)

    def OnFrontConnected(self):
        print("PyMdApi.OnFrontConnected")
        self.login()

    def OnFrontDisconnected(self, reason):
        print(f"PyMdApi.OnFrontDisconnected, reason: {reason}")
        pass

    def OnRspUserLogin(self, data, error, request_id, is_last):
        print("PyMdApi.OnRspUserLogin")

        if data is None:
            print("data is None")
        else:
            print(f"data FrontID: {data['FrontID']}")
            print(f"data SessionID: {data['SessionID']}")

        if error is None:
            print("error is None")
        else:
            print(f"error ErrorID: {error['ErrorID']}")
            print(f"error ErrorMsg: {error['ErrorMsg']}")

        print(f"request_id: {request_id}")
        print(f"is_last: {is_last}")

        print(f"Trading Day: {self.GetTradingDay()}")

        self.SubscribeMarketData(['eb2509'])

        pass

    def OnRspSubMarketData(self, data, error, request_id, is_last):
        print("PyMdApi.OnRspSubMarketData")
        print(f"sub data: {data}")


    def OnRtnDepthMarketData(self, data):
        print('PyMdApi.OnRtnDepthMarketData')
        print(f'tick:\n{data}')


def test():
    print(f'version: {MdApi.GetApiVersion()}')

    settings = {
        'address': 'tcp://180.166.103.21:55213',
        'broker_id': '4040',
        'user_id': '',
        'password': '',
        'flow_path': 'flow',
    }

    md_api = PyMdApi(settings)
    md_api.connect()

    input('Waiting...\n')



if __name__ == '__main__':
    test()
