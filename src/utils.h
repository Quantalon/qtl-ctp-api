#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <locale>
#include <limits>
#include <cstring>
#include <nanobind/nanobind.h>
#include "iconv.hpp"


namespace nb = nanobind;


thread_local static const iconvpp::converter converter("UTF-8", "GBK");

inline std::string gbk_to_utf8(const std::string& in) {
    std::string result;
    converter.convert(in, result);
    return result;
}

template <class NumberType>
NumberType adjust_number(NumberType number) {
    NumberType ret = number;
    if (number >= std::numeric_limits<NumberType>::max() - NumberType(1e-6)) ret = NumberType(0);
    return ret;
}

template<typename T>
void set_field(T &field, const nb::dict &input, const char *field_name) {
    if (input.contains(field_name)) {
        field = nb::cast<T>(input[field_name]);
    }
}

template<typename T>
void set_field(T &field, const nb::dict &input, const char *field_name, size_t size) {
    if (input.contains(field_name)) {
        std::string str = nb::cast<std::string>(input[field_name]);
        strncpy(field, str.c_str(), size);
    }
}


#endif //UTILS_H
