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
{% for req_method in td_req_methods %}
    int {{ req_method['name'] }}(
        {%- for p in req_method['parameters'] -%}
        {{ p['py_type'] }}{% if not p['py_type'].endswith('&') %} {% endif %}{{ p['py_name'] }}{% if not loop.last %}, {% endif %}
        {%- endfor -%}
    );
{%- endfor %}
{% for on_method in td_on_methods %}
    void {{ on_method['name'] }}(
        {%- for p in on_method['parameters'] -%}
        {{ p['type'] }}{% if not p['type'].endswith('*') %} {% endif %}{{ p['name'] }}{% if not loop.last %}, {% endif %}
        {%- endfor -%}
    ) override;
{%- endfor %}
{% for on_method in td_on_methods %}
    virtual void Py{{ on_method['name'] }}(
        {%- for p in on_method['parameters'] -%}
        {{ p['py_type'] }}{% if not p['py_type'].endswith('&') %} {% endif %}{{ p['py_name'] }}{% if not loop.last %}, {% endif %}
        {%- endfor -%}
    ) = 0;
{%- endfor %}
};


class PyTdApi final : public TdApi {
public:
    NB_TRAMPOLINE(TdApi, {{td_on_methods|length}});
{% for on_method in td_on_methods %}
    void Py{{ on_method['name'] }}(
        {%- for p in on_method['parameters'] -%}
        {{ p['py_type'] }}{% if not p['py_type'].endswith('&') %} {% endif %}{{ p['py_name'] }}{% if not loop.last %}, {% endif %}
        {%- endfor -%}
    ) override {
        NB_OVERRIDE_PURE_NAME(
            "{{ on_method['name'] }}",
            Py{{ on_method['name'] }},
            {%- for p in on_method['parameters'] %}
            {{ p['py_name'] }}{% if not loop.last %},{% endif %}
            {%- endfor %}
        );
    }
{% endfor %}
};

#endif //TD_API_H
