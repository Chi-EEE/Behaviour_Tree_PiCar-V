
#ifndef PageController_cc
#define PageController_cc

#include <filesystem>
#include <fstream>

#include "fmt/format.h"
#include "spdlog/spdlog.h"

#include "oatpp-websocket/Handshaker.hpp"

#include "oatpp/web/server/api/ApiController.hpp"

#include "oatpp/network/ConnectionHandler.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"


#include OATPP_CODEGEN_BEGIN(ApiController) //<-- codegen begin

/**
 * Controller with WebSocket-connect endpoint.
 */
class PageController : public oatpp::web::server::api::ApiController {
private:
  typedef PageController __ControllerType;
private:
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, websocketConnectionHandler, "websocket");
public:
  PageController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
    template<class ControllerT>
    static std::shared_ptr<OutgoingResponse> return_file_response(ControllerT* const controller, Status status, std::string file_path) {
        std::ifstream file(file_path, std::ios::in | std::ios::binary | std::ios::ate);
        if (file.is_open())
        {
            std::streampos size = file.tellg();
            std::vector<char> memblock(size);
            file.seekg(0, std::ios::beg);
            file.read(memblock.data(), size);
            file.close();
            return controller->createResponse(status, memblock.data());
        } else {
            spdlog::error("{} does not exist!", file_path);
            return controller->createResponse(status, "404");
        }
    }
    ENDPOINT_ASYNC("GET", "/", Root)
    {
        ENDPOINT_ASYNC_INIT(Root)

        Action act() override
        {
            return _return(PageController::return_file_response(controller, Status::CODE_200, "client/public/index.html"));
        }
    };

    ENDPOINT_ASYNC("GET", "/build/{path}", Build){
        ENDPOINT_ASYNC_INIT(Build)

        Action act() override{
            auto path = request->getPathVariable("path");
            std::string file_path = fmt::format("client/public/build/{}", std::string(path));
            spdlog::info("Path: {}, Result: {}", std::string(file_path), std::filesystem::exists(file_path));
            if (std::filesystem::exists(file_path))
            {
                return _return(PageController::return_file_response(controller, Status::CODE_200, file_path));
            }
            else
            {
                return _return(controller->createResponse(Status::CODE_404, "404"));
            }
        }
    };

    ENDPOINT_ASYNC("GET", "/{path}", Pages){
        ENDPOINT_ASYNC_INIT(Pages)
        
        Action act() override{
            auto path = request->getPathVariable("path");
            std::string html_file_path = fmt::format("client/public/{}.html", std::string(path));
            if (std::filesystem::exists(html_file_path))
            {
                return _return(PageController::return_file_response(controller, Status::CODE_200, html_file_path));
            }
            std::string file_path = fmt::format("client/public/{}", std::string(path));
            if (std::filesystem::exists(file_path))
            {
                return _return(PageController::return_file_response(controller, Status::CODE_200, file_path));
            }
            else
            {
                return _return(controller->createResponse(Status::CODE_404, "404"));
            }
        }
    };
};
    

#include OATPP_CODEGEN_END(ApiController) //<-- codegen end

#endif /* PageController_cc */