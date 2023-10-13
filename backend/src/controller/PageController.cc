
#ifndef PageController_cc
#define PageController_cc

#include "fmt/format.h"
#include "spdlog/spdlog.h"

#include "AbstractController.cc"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class PageController : public AbstractController
{
private:
    typedef PageController __ControllerType;

public:
    PageController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : AbstractController(objectMapper)
    {
    }

public:
    ENDPOINT_ASYNC("GET", "/", Root){
        ENDPOINT_ASYNC_INIT(Root)

        Action act() override{
            return _return(PageController::return_file_response(controller, Status::CODE_200, "frontend/public/index.html"));
        }
    };

    ENDPOINT_ASYNC("GET", "/{path}", Path){
        ENDPOINT_ASYNC_INIT(Path)

        Action act() override{
            auto path = request->getPathVariable("path");
            spdlog::info("The page ({}) has been visited", std::string(path));
            std::string html_file_path = fmt::format("frontend/public/{}.html", std::string(path));
            if (PageController::exists(html_file_path))
            {
                return _return(PageController::return_file_response(controller, Status::CODE_200, html_file_path));
            }
            std::string file_path = fmt::format("frontend/public/{}", std::string(path));
            if (PageController::exists(file_path))
            {
                return _return(PageController::return_file_response(controller, Status::CODE_200, file_path));
            }
            return _return(controller->createResponse(Status::CODE_404, "404"));
        }
    };

    ENDPOINT_ASYNC("GET", "/build/{path}", BuildPath){
        ENDPOINT_ASYNC_INIT(BuildPath)

        Action act() override{
            auto path = request->getPathVariable("path");
            std::string file_path = fmt::format("frontend/public/build/{}", std::string(path));
            return _return(PageController::return_file_response(controller, Status::CODE_200, file_path));
        }
    };
};

#include OATPP_CODEGEN_END(ApiController) //<-- codegen end

#endif /* PageController_cc */