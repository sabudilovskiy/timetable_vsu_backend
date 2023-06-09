#pragma once
#include <string>
#include <userver/formats/common/meta.hpp>
#include <userver/formats/parse/to.hpp>
#include <userver/server/http/http_request.hpp>

#include "models/lesson_filter/type.hpp"
#include "models/substring/parse.hpp"
#include "utils/convert/additional_properties.hpp"
#include "utils/convert/base.hpp"
#include "utils/convert/http_request_parse.hpp"
#include "utils/convert/json_parse.hpp"

namespace timetable_vsu_backend::views::timetable::get
{
using namespace utils::convert;
struct Request
{
    OptionalProperty<models::LessonFilter, "filter"> filter;
    static constexpr TypeOfBody kTypeOfBody = TypeOfBody::Json;
    static constexpr PolicyFields kPolicyFields = PolicyFields::ConvertAll;
};
}  // namespace timetable_vsu_backend::views::timetable::get
