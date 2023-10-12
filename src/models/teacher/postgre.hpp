#pragma once
#include <userver/storages/postgres/io/composite_types.hpp>
#include <utility>
#include <utils/convert/drop_properties_ref.hpp>

#include "models/teacher/type.hpp"
namespace models
{
using ConstTupleTeacher =
    ::utils::convert::drop_properties_to_ref_const_t<Teacher>;

}  // namespace models

namespace userver::storages::postgres::io
{
template <>
struct CppToUserPg<::models::ConstTupleTeacher>
{
    static constexpr DBTypeName postgres_name = "timetable_vsu.teacher_filter";
};
}  // namespace userver::storages::postgres::io
