#pragma once
#include <utils/component_list_fwd.hpp>
namespace legacy::components::controllers::postgres
{
namespace user
{
class Controller;
void Append(userver::components::ComponentList& component_list);
}  // namespace user

}  // namespace legacy::components::controllers::postgres