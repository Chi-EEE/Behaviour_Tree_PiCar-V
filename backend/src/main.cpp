#include <iostream>

#include <drogon/drogon.h>
#include <rplidar.h>

using namespace drogon;
int main()
{
    
    app().setLogPath("./")
         .setLogLevel(trantor::Logger::kWarn)
         .addListener("0.0.0.0", 9999)
         .setThreadNum(16)
         .enableRunAsDaemon()
         .run();
}