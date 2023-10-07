#include <userver/storages/postgres/io/composite_types.hpp>
#include <utils/convert/drop_properties_ref.hpp>

#include "models/education_type/postgre.hpp"
#include "models/faculty_filter/type.hpp"
namespace models
{
using TupleFacultyFilter =
    ::utils::convert::drop_properties_to_ref_const_t<
        FacultyFilter>;
}  // namespace models

namespace userver::storages::postgres::io
{
template <>
struct CppToUserPg<::models::TupleFacultyFilter>
{
    static constexpr DBTypeName postgres_name = "timetable_vsu.faculty_filter";
};
}  // namespace userver::storages::postgres::io
