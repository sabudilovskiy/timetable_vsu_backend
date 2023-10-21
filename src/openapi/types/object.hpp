#pragma once
#include <openapi/base/holder.hpp>
#include <openapi/base/named_traits.hpp>
#include <openapi/base/preferences.hpp>
#include <openapi/base/properties/object.hpp>
#include <string_view>
#include <utils/constexpr_optional.hpp>
#include <utils/constexpr_string.hpp>

namespace openapi
{
namespace detail
{
struct ObjectHolder
{
    traits::HolderField<utils::FixedString> name;
};

template <utils::ConstexprString value>
void consteval Apply(ObjectHolder& traits, preferences::Name<value>)
{
    traits.name = value;
}

template <typename... Option>
void consteval ApplyAll(ObjectHolder& traits, Option... option)
{
    (Apply(traits, option), ...);
}

template <typename T, typename... Option>
struct ObjectMagicHelper
{
    consteval static auto resolve_holder()
    {
        ObjectHolder helper{};
        ApplyAll(helper, Option{}...);
        return helper;
    }
    consteval static auto resolve_type()
    {
        constexpr ObjectHolder traits = resolve_holder();
        static_assert(traits.name.counter_changes <= 1,
                      "Don't use more 1 Name in template args");
        constexpr auto name = utils::MakeConstexprString<traits.name()>();
        using Traits = NamedTraits<name>;
        return ObjectProperty<T, Traits>{};
    }
};

}  // namespace detail

namespace types
{
template <typename T, typename... Option>
using Object = decltype(
    ::openapi::detail::ObjectMagicHelper<T, Option...>::resolve_type());

}
}  // namespace openapi
