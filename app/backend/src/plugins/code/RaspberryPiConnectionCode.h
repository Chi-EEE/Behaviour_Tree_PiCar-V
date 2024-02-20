#ifndef RASPBERRY_PI_CONNECTION_CODE_H
#define RASPBERRY_PI_CONNECTION_CODE_H

#pragma once

#include <drogon/Session.h>

#include <string>
#include <cstdint>

struct RaspberryPiConnectionCode
{
	int id;
	int passcode = 0;
	trantor::Date time;
	drogon::SessionPtr session;
};

#endif