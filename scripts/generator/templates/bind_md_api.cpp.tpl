#include "bind_md_api.h"

void bind_md_api(py::module &m) {
    py::class_<MdApi, PyMdApi> md_api(m, "MdApi", py::module_local());
    md_api
            .def(py::init<>())
            .def("CreateApi", &MdApi::CreateApi)
            .def_static("GetApiVersion", &MdApi::GetApiVersion)
            .def("Init", &MdApi::Init)
            .def("Join", &MdApi::Join)
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