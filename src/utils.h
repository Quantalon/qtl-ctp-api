#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <codecvt>
#include <locale>
#include <limits>
#include <vector>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>


namespace nb = nanobind;


inline std::string gbk_to_utf8(const std::string& gbk) {
    static const std::locale loc("zh_CN.GBK");

    std::vector<wchar_t> wstr(gbk.size());
    wchar_t* wstr_end = nullptr;
    const char* gb_end = nullptr;
    mbstate_t state{};
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t>>(loc).in(
            state, gbk.data(), gbk.data() + gbk.size(), gb_end, wstr.data(),
            wstr.data() + wstr.size(), wstr_end);

    if (res == std::codecvt_base::ok) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
        return cutf8.to_bytes(std::wstring(wstr.data(), wstr_end));
    }

    return "";
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

inline void set_str_field(char *field, const nb::dict &input, const char *field_name, size_t size) {
    if (contains(input, field_name)) {
        strncpy(field, nb::cast<std::string>(input[field_name]).c_str(), size);
    }
}

inline void set_char_field(char &field, const nb::dict &input, const char *field_name) {
    if (contains(input, field_name)) {
        field = nb::cast<int>(input[field_name]);  // todo
    }
}

inline void set_int_field(int &field, const nb::dict &input, const char *field_name) {
    if (contains(input, field_name)) {
        field = nb::cast<int>(input[field_name]);
    }
}

inline void set_double_field(double &field, const nb::dict &input, const char *field_name) {
    if (contains(input, field_name)) {
        field = nb::cast<double>(input[field_name]);
    }
}

#endif //UTILS_H
