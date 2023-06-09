#pragma once
#include <string>

#include "utils/convert/base.hpp"
namespace timetable_vsu_backend::models
{
using namespace utils::convert;
struct UserCredentials
{
    Property<std::string, "login"> login;
    Property<std::string, "password"> password;
    static constexpr TypeOfBody kTypeOfBody =
        TypeOfBody::Json;  //открываем возможность использовать структуру, как
                           //запрос
    static constexpr PolicyFields kPolicyFields = PolicyFields::ConvertAll;
};
}  // namespace timetable_vsu_backend::models
