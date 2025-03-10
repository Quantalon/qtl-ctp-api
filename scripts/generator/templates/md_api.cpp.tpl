#include "md_api.h"


void MdApi::CreateApi(const std::string &flow_path) {
    queue_ = std::make_unique<DispatchQueue>();
    api_ = CThostFtdcMdApi::CreateFtdcMdApi(flow_path.c_str());
    api_->RegisterSpi(this);
}

void MdApi::Release() {
    api_->RegisterSpi(nullptr);
    api_->Release();
    api_ = nullptr;
    queue_ = nullptr;
}

void MdApi::Init() {
    api_->Init();
}

int MdApi::Join() {
    return api_->Join();
}

std::string MdApi::GetApiVersion() {
    return CThostFtdcMdApi::GetApiVersion();
}

std::string MdApi::GetTradingDay() {
    return api_->GetTradingDay();
}

void MdApi::RegisterFront(const std::string &front_address) {
    api_->RegisterFront(const_cast<char *>(front_address.c_str()));
}

int MdApi::SubscribeMarketData(const std::vector<std::string> &instrument_ids) {
    std::vector<char *> instrument_ids_{};
    for (const auto &i : instrument_ids) instrument_ids_.emplace_back(const_cast<char *>(i.c_str()));
    return api_->SubscribeMarketData(instrument_ids_.data(), instrument_ids_.size());
}

int MdApi::UnSubscribeMarketData(const std::vector<std::string> &instrument_ids) {
    std::vector<char *> instrument_ids_{};
    for (const auto &i : instrument_ids) instrument_ids_.emplace_back(const_cast<char *>(i.c_str()));
    return api_->UnSubscribeMarketData(instrument_ids_.data(), instrument_ids_.size());
}
{% for req_method in md_req_methods %}
int MdApi::{{ req_method['name'] }}(
    {%- for p in req_method['parameters'] -%}
    {{ p['py_type'] }}{% if not p['py_type'].endswith('&') %} {% endif %}{{ p['py_name'] }}{% if not loop.last %}, {% endif %}
    {%- endfor -%}
) {
    {{req_method['parameters'][0]['type'].strip(' *')}} request{};
    {%- for p in structs[req_method['parameters'][0]['type'].strip(' *')] %}
    set_field(request.{{ p['name'] }}, data, "{{ p['name'] }}"{% if p['type']=='str' %}, sizeof(request.{{ p['name'] }}){% endif %});
    {%- endfor %}
    return api_->{{ req_method['name'] }}(&request, request_id);
}
{% endfor -%}
{% for on_method in md_on_methods %}
void MdApi::{{ on_method['name'] }}(
    {%- for p in on_method['parameters'] -%}
    {{ p['type'] }}{% if not p['type'].endswith('*') %} {% endif %}{{ p['py_name'] }}{% if not loop.last %}, {% endif %}
    {%- endfor -%}
) {
    {%- for p in on_method['parameters'] %}{% if p['type'].endswith('*') and p['py_name'] == 'data' %}
    std::optional<{{ p['type'].strip(' *') }}> rsp_data;
    if (data) {
        rsp_data = *data;
    }
    {%- elif p['type'].endswith('*') and p['py_name'] == 'error' %}
    std::optional<{{ p['type'].strip(' *') }}> rsp_error;
    if (error) {
        rsp_error = *error;
    }
    {%- endif %}{% endfor %}
    queue_->dispatch([=]() {
        nb::gil_scoped_acquire acquire;
        {%- for p in on_method['parameters'] %}{% if p['type'].endswith('*') and p['py_name'] == 'data' %}
        nb::dict py_data;
        if (rsp_data) {
            {%- for sp in structs[p['type'].strip(' *')] %}
            py_data["{{ sp['name'] }}"] = {% if sp['type']=='str' %}gbk_to_utf8({% elif sp['type']=='double' and ('Price' in sp['name'] or 'Delta' in sp['name']) %}adjust_number({% endif %}rsp_data->{{ sp['name'] }}{% if sp['type']=='str' or (sp['type']=='double' and ('Price' in sp['name'] or 'Delta' in sp['name'])) %}){% endif %};
            {%- endfor %}
        }
        {%- elif p['type'].endswith('*') and p['py_name'] == 'error' %}
        nb::dict py_error;
        if (rsp_error) {
            {%- for sp in structs[p['type'].strip(' *')] %}
            py_error["{{ sp['name'] }}"] = {% if sp['type']=='str' %}gbk_to_utf8({% endif %}rsp_error->{{ sp['name'] }}{% if sp['type']=='str' %}){% endif %};
            {%- endfor %}
        }
        {%- endif %}{% endfor %}
        Py{{ on_method['name'] }}(
        {%- for p in on_method['parameters'] -%}
        {% if p['type'].endswith('*') %}py_{% endif %}{{ p['py_name'] }}{% if not loop.last %}, {% endif %}
        {%- endfor -%}
        );
    });
}
{% endfor %}
