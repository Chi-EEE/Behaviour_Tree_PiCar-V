
#ifndef StaticResourceController_cc
#define StaticResourceController_cc

#include "fmt/format.h"
#include "spdlog/spdlog.h"

#include "AbstractController.cc"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * @brief For handling static pages
 * 
 */
class StaticResourceController : public AbstractController
{
private:
    typedef StaticResourceController __ControllerType;

public:
    StaticResourceController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : AbstractController(objectMapper)
    {
    }

public:
    /**
     * @brief Set the index of the page to the index html
     * 
     */
    ENDPOINT_ASYNC("GET", "/", Root){
        ENDPOINT_ASYNC_INIT(Root)

        Action act() override{
            const oatpp::data::stream::Context::Properties connectionProperties 
                = request->getConnection()->getInputStreamContext().getProperties();
            ConnectionProperty connection_property = ConnectionProperty{
                connectionProperties.get("peer_address")->c_str(),
                connectionProperties.get("peer_port")->c_str(),
            };
            return _return(StaticResourceController::return_file_response(controller, connection_property, Status::CODE_200, "frontend/public/index.html"));
        }
    };
    
    /**
     * @brief Try and load resource from path tail
     * 
     */
    ENDPOINT_ASYNC("GET", "*", Path){
        ENDPOINT_ASYNC_INIT(Path)

        Action act() override{
            auto path = request->getPathTail();
            const oatpp::data::stream::Context::Properties connectionProperties 
                = request->getConnection()->getInputStreamContext().getProperties();
            ConnectionProperty connection_property = ConnectionProperty{
                connectionProperties.get("peer_address")->c_str(),
                connectionProperties.get("peer_port")->c_str(),
            };
            std::string html_file_path = fmt::format("frontend/public/{}.html", std::string(path));
            if (StaticResourceController::exists(html_file_path))
            {
                return _return(StaticResourceController::return_file_response(controller,connection_property, Status::CODE_200, html_file_path));
            }
            std::string file_path = fmt::format("frontend/public/{}", std::string(path));
            if (StaticResourceController::exists(file_path))
            {
                return _return(StaticResourceController::return_file_response(controller, connection_property, Status::CODE_200, file_path));
            }
            return _return(controller->createResponse(Status::CODE_404, "404"));
        }
    };
};

#include OATPP_CODEGEN_END(ApiController) //<-- codegen end

#endif /* StaticResourceController_cc */