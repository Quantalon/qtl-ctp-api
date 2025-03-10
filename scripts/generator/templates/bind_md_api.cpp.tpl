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
            {%- for req_method in md_req_methods %}
            .def("{{ req_method['name'] }}", &MdApi::{{ req_method['name'] }})
            {%- endfor %}
            // On Methods
            {%- for on_method in md_on_methods %}
            .def("{{ on_method['name'] }}", &MdApi::Py{{ on_method['name'] }})
            {%- endfor %}
            ;
}
