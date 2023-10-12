#pragma once
#include <openapi/base/property_base.hpp>
#include <openapi/from_tuple/default.hpp>
#include <type_traits>

namespace openapi::raw
{
template <IsProperty U>
class FromTuple<U>
{
    using T = typename U::value_type;

   public:
    using Src = to_tuple_t<T>;
    using Dst = U;
    static Dst Do(const Src& src)
    {
        return {FromTuple<T>::Do(src)};
    }
};
}  // namespace openapi::raw
