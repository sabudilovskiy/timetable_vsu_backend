#pragma once
#include <algorithm>
#include <boost/pfr/core.hpp>
#include <boost/pfr/tuple_size.hpp>
#include <cctype>
#include <concepts>
#include <cstdint>
#include <experimental/source_location>
#include <iostream>
#include <openapi/base/array_property.hpp>
#include <openapi/base/array_traits.hpp>
#include <openapi/base/doc.hpp>
#include <openapi/base/object_property.hpp>
#include <openapi/base/object_traits.hpp>
#include <openapi/base/optional_property.hpp>
#include <openapi/base/reflective_preferences.hpp>
#include <openapi/base/string_property.hpp>
#include <openapi/base/string_traits.hpp>
#include <openapi/http/base/header_property.hpp>
#include <source_location>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <userver/compiler/demangle.hpp>
#include <userver/formats/common/type.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/formats/yaml/serialize.hpp>
#include <userver/formats/yaml/types.hpp>
#include <userver/formats/yaml/value_builder.hpp>
#include <userver/logging/log.hpp>
#include <userver/utils/overloaded.hpp>
#include <utility>
#include <utils/compilers_macros.hpp>

namespace openapi
{
inline void log_yaml_impl(userver::formats::yaml::ValueBuilder yaml,
                          std::string_view file_name, int line,
                          std::string_view function_name)
{
    std::clog << "=======================\n";
    std::clog << file_name << "(" << line << ") " << function_name << "\n";
    std::clog << "====================\n";
    std::clog << ToString(yaml.ExtractValue()) << '\n';
}

#define log_yaml(yaml) \
    ::openapi::log_yaml_impl(yaml, __FILE__, __LINE__, __PRETTY_FUNCTION__)

// some_namespace..some_class
// precondition: string from demangle
inline void RemoveNamespaces(std::string& name_class)
{
    auto read = std::find(name_class.begin(), name_class.end(), ':');
    auto write = read;
    while (read != name_class.end())
    {
        if (*read == ':')
        {
            read++;
            *read = '.';
        }
        *write = *read;
        read++;
        write++;
    }
    name_class.erase(write, name_class.end());
}
/*
это нужно для:

1) явного переопределения названий классов в рамках опенапи

2) написания какой-то логики преобразований
*/
template <typename T>
std::string GetOpenApiTypeName()
{
    std::string raw_name = userver::compiler::GetTypeName<T>();
    if (raw_name.find_first_of('<') != std::string::npos ||
        raw_name.find_first_of('>') != std::string::npos)
    {
        throw std::runtime_error(fmt::format(
            "template in name type: {}, overload this function", raw_name));
    }
    RemoveNamespaces(raw_name);
    return raw_name;
}

/*не перегружайте эту функцию без необходимости.

Нужна для проверки, что это действительно один тип.*/
template <typename T>
std::string GetRawTypeName()
{
    std::string raw_name = userver::compiler::GetTypeName<T>();
    return raw_name;
}

template <checks::IsReflective T>
void PlaceRefToType(userver::formats::yaml::ValueBuilder& place)
{
    if (!place.IsObject())
    {
        place = userver::formats::common::Type::kObject;
    }
    auto ref_str =
        std::string(R"(#/components/schemas/)").append(GetOpenApiTypeName<T>());
    place["$ref"] = std::move(ref_str);
}

template <checks::IsReflective T>
void PlaceRefToResponse(userver::formats::yaml::ValueBuilder& place)
{
    if (!place.IsObject())
    {
        place = userver::formats::common::Type::kObject;
    }
    auto ref_str = std::string(R"(#/components/responses/)")
                       .append(GetOpenApiTypeName<T>());
    place["$ref"] = std::move(ref_str);
}

template <checks::IsReflective T>
void PlaceRefToRequest(userver::formats::yaml::ValueBuilder& place)
{
    if (!place.IsObject())
    {
        place = userver::formats::common::Type::kObject;
    }
    auto ref_str =
        std::string(R"(#/requests/)").append(GetOpenApiTypeName<T>());
    place["$ref"] = std::move(ref_str);
}

template <typename T, typename... Other>
consteval bool IsAnyOf()
{
    return (std::same_as<T, Other> || ...);
}

template <typename T>
concept IsDefaultOpenApiType = requires
{
    requires IsAnyOf<T, std::string>() || std::is_integral_v<T>;
};
using StringMap = std::unordered_map<std::string, std::string>;

struct DocHelper
{
    userver::formats::yaml::ValueBuilder& root;
    userver::formats::yaml::ValueBuilder& cur_place;
};

inline void log_dock_helper_impl(DocHelper doc_helper,
                                 std::string_view file_name, int line,
                                 std::string_view function_name)
{
    std::clog << "=======================\n";
    std::clog << file_name << "(" << line << ") " << function_name << "\n";
    std::clog << "====================\n";
    auto [cur_place, root] = doc_helper;

    std::clog << ToString(root.ExtractValue()) << '\n';
}

template <typename T, typename Traits>
void Append(DocHelper doc_helper,
            std::type_identity<ObjectProperty<T, Traits>> = {})
{
    Append(doc_helper, std::type_identity<T>{});
}

inline void Append(DocHelper doc_helper, std::type_identity<std::int32_t>)
{
    auto& cur = doc_helper.cur_place;
    if (cur.IsObject())
    {
        cur = userver::formats::yaml::Type::kObject;
    }
    cur["type"] = "integer";
    cur["format"] = "int32";
}

inline void Append(DocHelper doc_helper, std::type_identity<std::string>)
{
    auto& cur = doc_helper.cur_place;
    if (cur.IsObject())
    {
        cur = userver::formats::yaml::Type::kObject;
    }
    cur["type"] = "string";
}

template <typename Traits>
void Append(DocHelper doc_helper,
            std::type_identity<StringProperty<Traits>> = {})
{
    constexpr traits::StringHelperTraits<Traits> traits;
    auto& field_node = doc_helper.cur_place;
    if (!field_node.IsObject())
        field_node = userver::formats::common::Type::kObject;
    field_node["type"] = "string";
    if constexpr (!traits.pattern.empty())
    {
        field_node["pattern"] = std::string{traits.pattern.AsStringView()};
    }
}

}  // namespace openapi