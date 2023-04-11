from pathlib import Path

import CppHeaderParser
from jinja2 import Template


class Generator:
    def __init__(
            self,
            include_dir_path,
            templates_dir_path,
            output_dir_path
    ):
        self.data_type_h_file_path = include_dir_path / 'ThostFtdcUserApiDataType.h'
        self.struct_h_file_path = include_dir_path / 'ThostFtdcUserApiStruct.h'
        self.md_api_h_file_path = include_dir_path / 'ThostFtdcMdApi.h'
        self.td_api_h_file_path = include_dir_path / 'ThostFtdcTraderApi.h'

        self.bind_consts_cpp_tpl_file_path = templates_dir_path / 'bind_consts.cpp.tpl'
        self.bind_md_api_cpp_tpl_file_path = templates_dir_path / 'bind_md_api.cpp.tpl'
        self.bind_td_api_cpp_tpl_file_path = templates_dir_path / 'bind_td_api.cpp.tpl'

        self.md_api_h_tpl_file_path = templates_dir_path / 'md_api.h.tpl'
        self.md_api_cpp_tpl_file_path = templates_dir_path / 'md_api.cpp.tpl'
        self.td_api_h_tpl_file_path = templates_dir_path / 'td_api.h.tpl'
        self.td_api_cpp_tpl_file_path = templates_dir_path / 'td_api.cpp.tpl'

        self.output_dir_path = output_dir_path

        self.exported_md_on_methods = [
            'OnFrontConnected',
            'OnFrontDisconnected',
            'OnHeartBeatWarning',
            'OnRspUserLogin',
            'OnRspUserLogout',
            'OnRspError',
            'OnRspSubMarketData',
            'OnRspUnSubMarketData',
            'OnRtnDepthMarketData',
        ]
        self.exported_md_req_methods = [
            'ReqUserLogin',
            'ReqUserLogout',
        ]
        self.exported_td_on_methods = [
            'OnFrontConnected',
            'OnFrontDisconnected',
            'OnHeartBeatWarning',
            'OnRspAuthenticate',
            'OnRspUserLogin',
            'OnRspUserLogout',
            'OnRspOrderInsert',
            'OnRspOrderAction',
            'OnRspSettlementInfoConfirm',
            'OnRspQryOrder',
            'OnRspQryTrade',
            'OnRspQryInvestorPosition',
            'OnRspQryTradingAccount',
            'OnRspQryInstrument',
            'OnRspError',
            'OnRtnOrder',
            'OnRtnTrade',
            'OnErrRtnOrderInsert',
            'OnErrRtnOrderAction',
            'OnRtnInstrumentStatus',
            'OnRspQrySettlementInfo',
        ]
        self.exported_td_req_methods = [
            'ReqAuthenticate',
            'ReqUserLogin',
            'ReqUserLogout',
            'ReqUserPasswordUpdate',
            'ReqOrderInsert',
            'ReqOrderAction',
            'ReqSettlementInfoConfirm',
            'ReqQryOrder',
            'ReqQryTrade',
            'ReqQryInvestorPosition',
            'ReqQryTradingAccount',
            'ReqQryInstrument',
            'ReqQrySettlementInfo',
        ]

        self.data_consts = []
        self.data_typedefs = {}
        self.data_structs = {}
        self.md_req_methods = []
        self.md_on_methods = []

        self.td_req_methods = []
        self.td_on_methods = []

    def parse_data_type(self):
        self.parse_data_type_enum()
        with self.data_type_h_file_path.open(encoding='utf-8') as f:
            for line in f.readlines():
                line = line.strip()
                if line.startswith('#define'):
                    tokens = line.split()
                    if len(tokens) != 3:
                        continue
                    name = tokens[1]
                    value = tokens[2]
                    self.data_consts.append({
                        'name': name,
                        'value': value.replace("'", '"'),
                    })
                if line.startswith('typedef'):
                    tokens = line.split()
                    t1 = tokens[1]
                    t2 = tokens[2].rstrip(';')
                    if 'TThostFtdcContentType' in t2:
                        value = 'bytes'
                        name = t2[:t2.index('[')]
                    elif '[' in t2:
                        value = 'str'
                        name = t2[:t2.index('[')]
                    else:
                        value = t1
                        name = t2
                    self.data_typedefs[name] = value

    def parse_data_type_enum(self):
        h = CppHeaderParser.CppHeader(self.data_type_h_file_path, encoding='utf-8')
        for enum in h.enums:
            for value in enum['values']:
                self.data_consts.append({
                    'name': value['name'],
                    'value': str(value['value']),
                })

    def parse_struct(self):
        h = CppHeaderParser.CppHeader(self.struct_h_file_path, encoding='utf-8')
        for name, c in h.classes.items():
            if c['declaration_method'] == 'struct':
                properties = c['properties']['public']
                parsed_properties = []
                for p in properties:
                    if p['name'].startswith('reserve'):
                        continue
                    parsed_properties.append({
                        'name': p['name'],
                        'type': self.data_typedefs[p['type']],
                    })
                self.data_structs[name] = parsed_properties

    def generate_py_parameter(self, name, type_):
        if type_ == 'int' and name == 'nReason':
            return 'reason', 'int'
        if type_ == 'int' and name == 'nTimeLapse':
            return 'time_lapse', 'int'
        if type_ == 'int' and name == 'nRequestID':
            return 'request_id', 'int'
        if type_ == 'bool' and name == 'bIsLast':
            return 'is_last', 'bool'
        if name == 'pRspInfo':
            return 'error', 'const nb::dict &'
        if type_.endswith(' *'):
            return 'data', 'const nb::dict &'
        return '', ''

    def parse_methods(self, cpp_header):
        on_methods = []
        req_methods = []

        for name, c in cpp_header.classes.items():
            if name.endswith('Spi'):
                for method in c['methods']['public']:
                    if method['name'].startswith('On'):
                        parameters = []
                        for p in method['parameters']:
                            py_name, py_type = self.generate_py_parameter(p['name'], p['type'])
                            parameters.append({
                                'name': p['name'],
                                'type': p['type'],
                                'py_name': py_name,
                                'py_type': py_type,
                            })
                        on_methods.append({
                            'name': method['name'],
                            'parameters': parameters,
                        })

            if name.endswith('Api'):
                for method in c['methods']['public']:
                    if method['name'].startswith('Req'):
                        parameters = []
                        for p in method['parameters']:
                            py_name, py_type = self.generate_py_parameter(p['name'], p['type'])
                            parameters.append({
                                'name': p['name'],
                                'type': p['type'],
                                'py_name': py_name,
                                'py_type': py_type,
                            })
                        req_methods.append({
                            'name': method['name'],
                            'parameters': parameters,
                        })

        return on_methods, req_methods

    def parse_md_api(self):
        h = CppHeaderParser.CppHeader(self.md_api_h_file_path, encoding='utf-8')
        md_on_methods, md_req_methods = self.parse_methods(h)
        self.md_on_methods = [method for method in md_on_methods if method['name'] in self.exported_md_on_methods]
        self.md_req_methods = [method for method in md_req_methods if method['name'] in self.exported_md_req_methods]

    def parse_td_api(self):
        h = CppHeaderParser.CppHeader(self.td_api_h_file_path, encoding='utf-8')
        td_on_methods, td_req_methods = self.parse_methods(h)
        self.td_on_methods = [method for method in td_on_methods if method['name'] in self.exported_td_on_methods]
        self.td_req_methods = [method for method in td_req_methods if method['name'] in self.exported_td_req_methods]

    def render_template(self, tpl_file, output_file, data):
        tpl_content = tpl_file.read_text(encoding='utf-8')
        tpl = Template(tpl_content)
        r = tpl.render(**data)
        output_file.write_text(r, encoding='utf-8')

    def generate_consts(self):
        self.render_template(
            self.bind_consts_cpp_tpl_file_path,
            self.output_dir_path / 'bind_consts.cpp',
            {'data_consts': self.data_consts}
        )

    def generate_md_api(self):
        self.render_template(
            self.bind_md_api_cpp_tpl_file_path,
            self.output_dir_path / 'bind_md_api.cpp',
            {
                'md_req_methods': self.md_req_methods,
                'md_on_methods': self.md_on_methods,
            }
        )

        self.render_template(
            self.md_api_h_tpl_file_path,
            self.output_dir_path / 'md_api.h',
            {
                'md_req_methods': self.md_req_methods,
                'md_on_methods': self.md_on_methods,
            }
        )

        self.render_template(
            self.md_api_cpp_tpl_file_path,
            self.output_dir_path / 'md_api.cpp',
            {
                'md_req_methods': self.md_req_methods,
                'md_on_methods': self.md_on_methods,
                'structs': self.data_structs,
            }
        )

    def generate_td_api(self):
        self.render_template(
            self.bind_td_api_cpp_tpl_file_path,
            self.output_dir_path / 'bind_td_api.cpp',
            {
                'td_req_methods': self.td_req_methods,
                'td_on_methods': self.td_on_methods,
            }
        )

        self.render_template(
            self.td_api_h_tpl_file_path,
            self.output_dir_path / 'td_api.h',
            {
                'td_req_methods': self.td_req_methods,
                'td_on_methods': self.td_on_methods,
            }
        )

        self.render_template(
            self.td_api_cpp_tpl_file_path,
            self.output_dir_path / 'td_api.cpp',
            {
                'td_req_methods': self.td_req_methods,
                'td_on_methods': self.td_on_methods,
                'structs': self.data_structs,
            }
        )

    def pprint(self, data):
        import json
        r = json.dumps(data, indent=2)
        print(r)

    def generate(self):
        self.parse_data_type()
        self.parse_struct()
        self.parse_md_api()
        self.parse_td_api()
        self.generate_consts()
        self.generate_md_api()
        self.generate_td_api()
        print('Generated...')


def main():
    current_dir = Path(__file__).parent
    include_dir_path = current_dir / Path('../../libs/ctp/include')
    templates_dir_path = current_dir / Path('templates')
    output_dir_path = current_dir / Path('../../src')

    generator = Generator(
        include_dir_path,
        templates_dir_path,
        output_dir_path
    )
    generator.generate()


if __name__ == '__main__':
    main()
