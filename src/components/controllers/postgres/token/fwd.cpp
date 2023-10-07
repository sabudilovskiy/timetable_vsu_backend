#include "fwd.hpp"

#include <userver/components/component_list.hpp>

#include "controller.hpp"

namespace components::controllers::postgres::token
{
void Append(userver::components::ComponentList& component_list)
{
    component_list.Append<Controller>();
}
}  // namespace components::controllers::postgres::token
