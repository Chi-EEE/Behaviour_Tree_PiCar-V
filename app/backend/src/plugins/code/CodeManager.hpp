#ifndef CODEMANAGER_HPP
#define CODEMANAGER_HPP

#pragma once

#include <memory>
#include <numeric>
#include <chrono>

#include <drogon/plugins/Plugin.h>

#include <effolkronium/random.hpp>

#include "RaspberryPiConnectionCode.h"

using effolkronium_Random = effolkronium::random_static;

constexpr int THIRTY_MINUTES = 30 * 60 * 1000;
constexpr int CODE_LENGTH = 5;
constexpr int PASSCODE_LENGTH = 3;

class CodeManager : public drogon::Plugin<CodeManager>
{
public:
	void initAndStart(const Json::Value& config) override {
		this->raspberry_pi_connection_codes.resize(std::pow(10, CODE_LENGTH));
		this->raspberry_pi_connection_code_list.resize(std::pow(10, CODE_LENGTH));
		for (int i = 0; i < this->raspberry_pi_connection_codes.size(); i++) {
			std::shared_ptr<RaspberryPiConnectionCode> admin_code = std::make_shared<RaspberryPiConnectionCode>(i, nullptr);
			this->raspberry_pi_connection_codes[i] = admin_code;
			this->raspberry_pi_connection_code_list[i] = admin_code;
		}
		effolkronium_Random::shuffle(this->raspberry_pi_connection_codes.begin(), this->raspberry_pi_connection_codes.end());
	}

	void shutdown() override {}

	std::shared_ptr<RaspberryPiConnectionCode> getRaspberryPiConnectionCode(drogon::SessionPtr session) {
		const trantor::Date now = trantor::Date::date();
		std::shared_ptr<RaspberryPiConnectionCode> raspberry_pi_connection_code = nullptr;
		// BEWARE: This can loop forever if there are no valid admin codes
		while (true) {
			raspberry_pi_connection_code = this->raspberry_pi_connection_codes.front();
			if (raspberry_pi_connection_code->session == nullptr) {
				break;
			}
			if (raspberry_pi_connection_code->time < now) {
				break;
			}
			this->raspberry_pi_connection_codes.pop_front();
			this->raspberry_pi_connection_codes.push_back(raspberry_pi_connection_code);
		}

		// A new session is going to be assigned to the admin code
		raspberry_pi_connection_code->passcode = effolkronium_Random::get(0, 9999);
		raspberry_pi_connection_code->session = session;
		raspberry_pi_connection_code->time = now;

		this->raspberry_pi_connection_codes.pop_front();
		this->raspberry_pi_connection_codes.push_back(raspberry_pi_connection_code);

		return raspberry_pi_connection_code;
	}

	std::optional<drogon::SessionPtr> useRaspberryPiConnectionCode(int code, int passcode) {
		std::shared_ptr<RaspberryPiConnectionCode> raspberry_pi_connection_code = this->raspberry_pi_connection_code_list[code];
		if (raspberry_pi_connection_code->passcode != passcode) {
			return std::nullopt;
		}
		drogon::SessionPtr session = raspberry_pi_connection_code->session;
		raspberry_pi_connection_code->session = nullptr;
		return std::make_optional<drogon::SessionPtr>(session);
	}

private:
	std::deque<std::shared_ptr<RaspberryPiConnectionCode>> raspberry_pi_connection_codes;
	std::vector<std::shared_ptr<RaspberryPiConnectionCode>> raspberry_pi_connection_code_list;
};

#endif