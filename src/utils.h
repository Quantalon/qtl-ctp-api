#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <codecvt>
#include <locale>
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

inline std::string gb2312_to_utf8(const std::string& gb2312) {
    static const std::locale loc("zh_CN.GB18030");

    std::vector<wchar_t> wstr(gb2312.size());
    wchar_t* wstr_end = nullptr;
    const char* gb_end = nullptr;
    mbstate_t state{};
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t>>(loc).in(
            state, gb2312.data(), gb2312.data() + gb2312.size(), gb_end, wstr.data(),
            wstr.data() + wstr.size(), wstr_end);

    if (res == std::codecvt_base::ok) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
        return cutf8.to_bytes(std::wstring(wstr.data(), wstr_end));
    }

    return "";
}

template <class PriceType>
inline PriceType adjust_price(PriceType price) {
    PriceType ret = price;
    if (price >= std::numeric_limits<PriceType>::max() - PriceType(1e-6)) ret = PriceType(0);
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
