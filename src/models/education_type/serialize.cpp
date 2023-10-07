#include "serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace models
{
std::string Serialize(const EducationType& value,
                      userver::formats::serialize::To<std::string>)
{
    switch (value)
    {
        case EducationType::kMagistracy:
            return "magistracy";
        case EducationType::kPostgraduate:
            return "postgraduate";
        case EducationType::kUndergraduate:
            return "undergraduate";
        case EducationType::kSpecialty:
            return "specialty";
    }
}
userver::formats::json::Value Serialize(
    const EducationType& value,
    userver::formats::serialize::To<userver::formats::json::Value>)
{
    std::string str =
        Serialize(value, userver::formats::serialize::To<std::string>{});
    return userver::formats::json::ValueBuilder(str).ExtractValue();
}
}  // namespace models
