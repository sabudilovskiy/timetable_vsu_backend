#include "serialize.hpp"

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/datetime.hpp>

namespace timetable_vsu_backend::models
{
std::string Serialize(const TimeString& value,
                      userver::formats::serialize::To<std::string>)
{
    return userver::utils::datetime::Timestring(value.GetUnderlying());
}
userver::formats::json::Value Serialize(
    const TimeString& value,
    userver::formats::serialize::To<userver::formats::json::Value>)
{
    std::string str =
        Serialize(value, userver::formats::serialize::To<std::string>{});
    return userver::formats::json::ValueBuilder(str).ExtractValue();
}
}  // namespace timetable_vsu_backend::models
