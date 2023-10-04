#include <openapi/all.hpp>
#include <openapi/http/handler.hpp>
#include <type_traits>
#include <userver/utest/utest.hpp>
#include <utils/tests_macros.hpp>

using namespace timetable_vsu_backend::openapi;
using namespace timetable_vsu_backend::openapi::http;
using namespace types;
using namespace preferences;

namespace tests_path
{
struct FindUserBody
{
    String<Name<"user">> user;
    String<Name<"password">> password;
    auto operator<=>(const FindUserBody&) const = default;
};
struct SomeRequest
{
    Body<FindUserBody> body;
    Header<std::string, Name<"some_header_name">> some_header;
    Header<std::string, Name<"some_another_header">> some_another_header;
    auto operator<=>(const SomeRequest&) const = default;
};
struct SomeResponse
{
    Body<FindUserBody> body;
    Header<std::string, Name<"some_header_name">> some_header;
    Header<std::string, Name<"some_another_header">> some_another_header;
    auto operator<=>(const SomeResponse&) const = default;
};
}  // namespace tests_path

UTEST(Openapi_Doc_Serialize, BasicPath)
{
    Doc doc;
    std::string path = "/login";
    std::string method = "post";
    AppendPath(doc, HandlerInfo{path, method},
               std::type_identity<tests_path::SomeRequest>{},
               std::type_identity<Resp<tests_path::SomeResponse, 200>>{});
    auto value = doc().ExtractValue();
    auto result_schema = ToString(value);
    std::clog << result_schema << '\n';
    EXPECT_EQ(result_schema, RAW_STRING(
                                 R"(
paths:
  /login:
    post:
      request:
        $ref: "#/requests/TestsPathSomeRequest"
      responses:
        200:
          $ref: "#/components/responses/TestsPathSomeResponse"
requests:
  TestsPathSomeRequest:
    requestBody:
      required: false
      content:
        application/json:
          schema:
            $ref: "#/components/schemas/TestsPathFindUserBody"
    parameters:
      - in: header
        name: some_header_name
        required: false
        schema:
          type: string
      - in: header
        name: some_another_header
        required: false
        schema:
          type: string
components:
  schemas:
    TestsPathFindUserBody:
      type: object
      additionalProperties: false
      properties:
        user:
          type: string
        password:
          type: string
      required:
        - user
        - password
  responses:
    TestsPathSomeResponse:
      content:
        application/json:
          schema:
            $ref: "#/components/schemas/TestsPathFindUserBody"
      headers:
        some_header_name:
          schema:
            type: string
        some_another_header:
          schema:
            type: string
)"));
}
