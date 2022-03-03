#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <codecvt>
#include <locale>
#include <limits>
#include <pybind11/pybind11.h>


namespace py = pybind11;


struct CtpApiDeleter {
    template <class T>
    void operator()(T* p) {
        if (p) {
            p->RegisterSpi(nullptr);
            p->Release();
        }
    }
};

template <class T>
using CtpUniquePtr = std::unique_ptr<T, CtpApiDeleter>;

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

inline void set_str_field(char *field, const py::dict &input, const char *field_name, size_t size) {
    if (input.contains(field_name)) {
        strncpy(field, input[field_name].cast<std::string>().c_str(), size);
    }
}

inline void set_char_field(char &field, const py::dict &input, const char *field_name) {
    if (input.contains(field_name)) {
        field = input[field_name].cast<char>();
    }
}

inline void set_int_field(int &field, const py::dict &input, const char *field_name) {
    if (input.contains(field_name)) {
        field = input[field_name].cast<int>();
    }
}

inline void set_double_field(double &field, const py::dict &input, const char *field_name) {
    if (input.contains(field_name)) {
        field = input[field_name].cast<double>();
    }
}

#endif //UTILS_H
