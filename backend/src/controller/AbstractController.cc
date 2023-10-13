
#ifndef AbstractController_cc
#define AbstractController_cc

#include <filesystem>
#include <fstream>

#include <boost/dll/runtime_symbol_info.hpp>

#include "fmt/format.h"
#include "spdlog/spdlog.h"

#include "oatpp/web/server/api/ApiController.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class AbstractController : public oatpp::web::server::api::ApiController
{
private:
    typedef AbstractController __ControllerType;

public:
    AbstractController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper)
    {
    }

public:
    template <class ControllerT>
    static std::shared_ptr<OutgoingResponse> return_file_response(ControllerT *const controller, Status status, std::string relative_file_path)
    {
        std::string program_path = boost::dll::program_location().parent_path().string();
        std::string file_path = fmt::format("{program_path}/{file_path}",
                                            fmt::arg("program_path", program_path),
                                            fmt::arg("file_path", relative_file_path));
        std::ifstream file(file_path,
            std::ios::in | std::ios::binary | std::ios::ate);
        if (file.is_open())
        {
            std::streampos size = file.tellg();
            std::vector<char> memblock(size);
            file.seekg(0, std::ios::beg);
            file.read(memblock.data(), size);
            file.close();
            return controller->createResponse(status, std::string(memblock.begin(), memblock.end()));
        }
        else
        {
            spdlog::error("{} does not exist!", file_path);
            return controller->createResponse(Status::CODE_404, "404");
        }
    }

    static bool exists(std::string relative_file_path) {
        std::string program_path = boost::dll::program_location().parent_path().string();
        std::string file_path = fmt::format("{program_path}/{file_path}",
                                            fmt::arg("program_path", program_path),
                                            fmt::arg("file_path", relative_file_path));
        return std::filesystem::exists(file_path);
    }
};

#include OATPP_CODEGEN_END(ApiController) //<-- codegen end

#endif /* AbstractController_cc */