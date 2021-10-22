#ifndef MD_API_H
#define MD_API_H

#include <iostream>
#include <string>
#include <pybind11/pybind11.h>

#include "utils.h"
#include "dispatch_queue.h"
#include "ThostFtdcMdApi.h"


namespace py = pybind11;


class MdApi : public CThostFtdcMdSpi {
private:
    CtpUniquePtr<CThostFtdcMdApi> api_;
    std::unique_ptr<DispatchQueue> queue_;

public:
    void CreateApi(const std::string &flow_path);
    static std::string GetApiVersion();
    void Init();
    int Join();
    std::string GetTradingDay();
    void RegisterFront(const std::string &front_address);
    int SubscribeMarketData(const std::string &instrument_id);
    int UnSubscribeMarketData(const std::string &instrument_id);
{% for req_method in md_req_methods %}
    int {{ req_method['name'] }}(
        {%- for p in req_method['parameters'] -%}
        {{ p['py_type'] }}{% if not p['py_type'].endswith('&') %} {% endif %}{{ p['py_name'] }}{% if not loop.last %}, {% endif %}
        {%- endfor -%}
    );
{%- endfor %}
{% for on_method in md_on_methods %}
    void {{ on_method['name'] }}(
        {%- for p in on_method['parameters'] -%}
        {{ p['type'] }}{% if not p['type'].endswith('*') %} {% endif %}{{ p['name'] }}{% if not loop.last %}, {% endif %}
        {%- endfor -%}
    ) override;
{%- endfor %}
{% for on_method in md_on_methods %}
    virtual void Py{{ on_method['name'] }}(
        {%- for p in on_method['parameters'] -%}
        {{ p['py_type'] }}{% if not p['py_type'].endswith('&') %} {% endif %}{{ p['py_name'] }}{% if not loop.last %}, {% endif %}
        {%- endfor -%}
    ) = 0;
{%- endfor %}
};


class PyMdApi final : public MdApi {
public:
    using MdApi::MdApi;
{% for on_method in md_on_methods %}
    void Py{{ on_method['name'] }}(
        {%- for p in on_method['parameters'] -%}
        {{ p['py_type'] }}{% if not p['py_type'].endswith('&') %} {% endif %}{{ p['py_name'] }}{% if not loop.last %}, {% endif %}
        {%- endfor -%}
    ) override {
        PYBIND11_OVERLOAD_PURE_NAME(
            void,
            MdApi,
            "{{ on_method['name'] }}",
            Py{{ on_method['name'] }},
            {%- for p in on_method['parameters'] %}
            {{ p['py_name'] }}{% if not loop.last %},{% endif %}
            {%- endfor %}
        )
    }
{% endfor %}
};

#endif //MD_API_H
