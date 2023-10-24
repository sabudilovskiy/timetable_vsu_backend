#pragma once
#include <userver/storages/postgres/io/composite_types.hpp>
#include <utility>
#include <utils/convert/drop_properties_ref.hpp>

#include "legacy/models/user/type.hpp"
#include "legacy/models/user_type/postgre.hpp"
namespace legacy::models
{
using TupleUserRaw = ::utils::convert::drop_properties_to_ref_const_t<User>;

// using TupleUser =
//     userver::utils::StrongTypedef<struct TupleLessonFilterTag,
//                                   TupleUserRaw>;
using TupleUser = TupleUserRaw;
}  // namespace legacy::models

namespace userver::storages::postgres::io
{
template <>
struct CppToUserPg<::legacy::models::TupleUser>
{
    static constexpr DBTypeName postgres_name = "timetable_vsu.userV1";
};
}  // namespace userver::storages::postgres::io