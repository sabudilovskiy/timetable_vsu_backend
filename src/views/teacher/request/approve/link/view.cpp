#include "view.hpp"

#include <chrono>
#include <exception>
#include <userver/components/component_list.hpp>
#include <userver/formats/parse/boost_uuid.hpp>
#include <userver/formats/parse/to.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/datetime.hpp>

#include "Request.hpp"
#include "Responses.hpp"
#include "components/controllers/postgres/teacher/controller.hpp"
#include "components/controllers/postgres/user/controller.hpp"
#include "http/handler_parsed.hpp"
#include "models/auth_token/serialize.hpp"
#include "models/user/serialize.hpp"
#include "models/user_type/serialize.hpp"
#include "models/user_type/type.hpp"
#include "userver/logging/log.hpp"
#include "utils/parse/uuid/string.hpp"
#include "utils/perform_common_errors.hpp"
namespace timetable_vsu_backend::views::teacher::requests::approve::link
{
namespace
{
namespace pg = components::controllers::postgres;
class Handler final
    : public http::HandlerParsed<Request, Response200, Response400, Response401,
                                 Response403>
{
   public:
    [[maybe_unused]] static constexpr std::string_view kName =
        "handler-teacher-requests-approve-link";
    Handler(const userver::components::ComponentConfig& config,
            const userver::components::ComponentContext& context)
        : HandlerParsed(config, context),
          teacher_controller(context.FindComponent<pg::teacher::Controller>()),
          user_controller(context.FindComponent<pg::user::Controller>())
    {
    }

    Response Handle(Request&& request) const override
    {
        auto user = user_controller.GetByToken(request.token());
        if (!user)
        {
            return utils::common_errors::PerformInvalidToken();
        }
        if (user->type() != models::UserType::kAdmin &&
            user->type() != models::UserType::kRoot)
        {
            return utils::common_errors::PerformForbidden();
        }
        auto link_id = teacher_controller.ApproveAndLink(request.request_id(),
                                                         request.teacher_id());
        if (!link_id)
        {
            return utils::common_errors::PerformInvalidData(
                "Request_id or teacher_id are incorrect");
        }

        return Response200{*link_id};
    }

   private:
    const pg::teacher::Controller& teacher_controller;
    const pg::user::Controller& user_controller;
};
}  // namespace

void Append(userver::components::ComponentList& component_list)
{
    component_list.Append<Handler>();
}

}  // namespace timetable_vsu_backend::views::teacher::requests::approve::link
