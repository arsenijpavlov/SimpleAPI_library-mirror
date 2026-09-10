#pragma once

#include <string>


namespace simpleapi {

// ошибки loadConfig заполнят эту строку
static std::string static_config_error_str;

namespace tools {

// базовое объявление шаблона (сформирует ошибку компиляции для типов, которые не объявлены)
template<typename T, typename Enable = void>
struct ConfigTypeTraits;

} // namespace simpleapi
} // namespace tools
