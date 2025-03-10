#include "bind_td_api.h"

void bind_td_api(nb::module_ &m) {
    nb::class_<TdApi, PyTdApi> td_api(m, "TdApi");
    td_api
            .def(nb::init<>())
            .def("CreateApi", &TdApi::CreateApi)
            .def("Release", &TdApi::Release)
            .def("Init", &TdApi::Init)
            .def("Join", &TdApi::Join)
            .def_static("GetApiVersion", &TdApi::GetApiVersion)
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
