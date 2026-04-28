# 工具函数

`src/utils.h` 提供了编码转换、字段填充、数值处理等核心工具函数，被 `MdApi` 和 `TdApi` 共享使用。

## 字符编码转换

### gbk_to_utf8

```cpp
thread_local static const iconvpp::converter converter("UTF-8", "GBK");

inline std::string gbk_to_utf8(const std::string& in) {
    std::string result;
    converter.convert(in, result);
    return result;
}
```

将 GBK 编码字符串转换为 UTF-8。

**实现细节**:
- 使用 `thread_local` 保证每个线程拥有独立的 `iconvpp::converter` 实例
- `iconvpp::converter` 是对系统 `iconv` 库的 C++ 封装（定义在 `iconv.hpp`）
- CTP 库使用 GBK 编码传输中文文本，必须转换为 UTF-8 才能在 Python 中正确显示

## 数值处理

### adjust_number

```cpp
template <class NumberType>
NumberType adjust_number(NumberType number) {
    NumberType ret = number;
    if (number >= std::numeric_limits<NumberType>::max() - NumberType(1e-6))
        ret = NumberType(0);
    return ret;
}
```

处理 CTP 中的哨兵值。

CTP 对某些未设置或无效的浮点数字段，会使用该类型的最大值作为哨兵（如 `DBL_MAX`）。此函数检测这种哨兵值并将其转换为 0，避免 Python 端收到异常大的数值。

**使用场景**: 行情数据中的 `LastPrice`, `PreSettlementPrice`, `OpenPrice` 等价格字段，以及合约信息中的 `LongMarginRatio`, `ShortMarginRatio` 等。

## 字段填充

### set_field（普通类型）

```cpp
template<typename T>
void set_field(T &field, const nb::dict &input, const char *field_name) {
    if (input.contains(field_name)) {
        field = nb::cast<T>(input[field_name]);
    }
}
```

从 Python `dict` 中提取指定字段并赋值给 C struct 成员。如果字典中不存在该字段，则不做任何操作（保持默认值）。

支持类型：`int`, `double`, `char` 等。

### set_field（字符数组）

```cpp
template<typename T>
void set_field(T &field, const nb::dict &input, const char *field_name, size_t size) {
    if (input.contains(field_name)) {
        std::string str = nb::cast<std::string>(input[field_name]);
        strncpy(field, str.c_str(), size);
    }
}
```

专用于 CTP 中的定长字符数组（如 `char BrokerID[11]`）。使用 `strncpy` 安全复制，防止缓冲区溢出。

**使用示例**（`td_api.cpp` 中）：

```cpp
int TdApi::ReqUserLogin(const nb::dict &data, int request_id) {
    CThostFtdcReqUserLoginField request{};
    set_field(request.TradingDay, data, "TradingDay", sizeof(request.TradingDay));
    set_field(request.BrokerID, data, "BrokerID", sizeof(request.BrokerID));
    set_field(request.UserID, data, "UserID", sizeof(request.UserID));
    set_field(request.Password, data, "Password", sizeof(request.Password));
    set_field(request.ClientIPPort, data, "ClientIPPort");  // int 类型，无 size 参数
    // ...
    return api_->ReqUserLogin(&request, request_id);
}
```

## iconv.hpp

`iconv.hpp` 是 `iconv` 库的 C++ RAII 封装，来自第三方（Yuya Unno，BSD 许可证）。

### converter 类

```cpp
namespace iconvpp {

class converter {
public:
    converter(const std::string& out_encode,
              const std::string& in_encode,
              bool ignore_error = true,
              size_t buf_size = 1024);

    void convert(const std::string& input, std::string& output) const;

private:
    iconv_t iconv_;
    bool ignore_error_;
    const size_t buf_size_;
};

}
```

- **构造函数**: 打开 `iconv` 转换描述符，失败时抛出 `std::runtime_error`
- **析构函数**: 自动调用 `iconv_close()` 关闭描述符
- **convert()**: 执行编码转换，使用内部缓冲区循环处理大字符串
- `ignore_error`: 遇到无法转换的字符时是跳过还是抛异常（默认跳过）

## 设计要点

1. **模板函数内联**: `gbk_to_utf8`, `adjust_number`, `set_field` 都是 `inline` 函数，避免跨编译单元链接问题
2. **线程局部存储**: `thread_local converter` 避免多线程竞争和频繁创建/销毁转换器
3. **安全复制**: 字符数组版本使用 `strncpy` 而非 `strcpy`，防止越界
4. **可选字段**: `set_field` 只在字典包含对应键时才赋值，允许 Python 侧省略不必要的字段
