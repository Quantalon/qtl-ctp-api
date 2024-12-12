#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <locale>
#include <limits>
#include <cstring>
#include <nanobind/nanobind.h>
#include "iconv.hpp"


namespace nb = nanobind;


static const iconvpp::converter converter("UTF-8", "GBK");

inline std::string gbk_to_utf8(const std::string& in) {
    // iconvpp::converter converter("UTF-8", "GBK");
    std::string result;
    converter.convert(in, result);
    return result;
}

template <class NumberType>
inline NumberType adjust_number(NumberType number) {
    NumberType ret = number;
    if (number >= std::numeric_limits<NumberType>::max() - NumberType(1e-6)) ret = NumberType(0);
    return ret;
}

inline bool contains(const nb::dict &d, const std::string &k) {
    for (auto i: d) {
        if (nb::cast<std::string>(i.first) == k) {
            return true;
        }
    }
    return false;
}

// 通用模板
template<typename T>
inline void set_field(T &field, const nb::dict &input, const char *field_name) {
    if (contains(input, field_name)) {
        field = nb::cast<T>(input[field_name]);
    }
}

// 字符串的特化版本
template<>
inline void set_field<char*>(char* field, const nb::dict &input, const char *field_name, size_t size) {
    if (contains(input, field_name)) {
        strncpy(field, nb::cast<std::string>(input[field_name]).c_str(), size);
    }
}

#endif //UTILS_H
