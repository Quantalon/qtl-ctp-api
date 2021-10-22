#include "bind_td_api.h"

void bind_td_api(py::module &m) {
    py::class_<TdApi, PyTdApi> td_api(m, "TdApi", py::module_local());
    td_api
            .def(py::init<>())
            .def("CreateApi", &TdApi::CreateApi)
            .def_static("GetApiVersion", &TdApi::GetApiVersion)
            .def("Init", &TdApi::Init)
            .def("Join", &TdApi::Join)
            .def("GetTradingDay", &TdApi::GetTradingDay)
            .def("RegisterFront", &TdApi::RegisterFront)
            .def("SubscribePrivateTopic", &TdApi::SubscribePrivateTopic)
            .def("SubscribePublicTopic", &TdApi::SubscribePublicTopic)
            // Req Methods
            {%- for req_method in td_req_methods %}
            .def("{{ req_method['name'] }}", &TdApi::{{ req_method['name'] }})
            {%- endfor %}
            // On Methods
            {%- for on_method in td_on_methods %}
            .def("{{ on_method['name'] }}", &TdApi::Py{{ on_method['name'] }})
            {%- endfor %}
            ;
}