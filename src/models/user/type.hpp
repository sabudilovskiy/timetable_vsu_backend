#pragma once
#include <boost/uuid/uuid.hpp>
#include <userver/utils/strong_typedef.hpp>

#include "models/user_type/type.hpp"
#include "utils/convert/base.hpp"

namespace timetable_vsu_backend::models
{
namespace convert = utils::convert;
struct User
{
    convert::Property<boost::uuids::uuid, "id"> id;
    convert::Property<models::UserType, "type"> type;
    static constexpr convert::PolicyFields kPolicyFields =
        convert::PolicyFields::ConvertAll;
};

static_assert(utils::convert::IsConvertAll<User>);
}  // namespace timetable_vsu_backend::models
