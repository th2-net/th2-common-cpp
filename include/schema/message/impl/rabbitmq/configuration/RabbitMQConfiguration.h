/*
 * Copyright 2020-2020 Exactpro (Exactpro Systems Limited)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <fstream>
#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

namespace th2::common_cpp {

class RabbitMQConfiguration {
public:
    static RabbitMQConfiguration read(const std::filesystem::path cfg_file) {
        std::ifstream stream(cfg_file.string().c_str());

        std::cout << cfg_file.string() << ": \n" << stream.rdbuf() << std::endl;

        stream.seekg (0, stream.beg);

        nlohmann::json js;
        stream >> js;

        RabbitMQConfiguration rmq_cfg;
        nlohmann::from_json(js, rmq_cfg);

        return rmq_cfg;
    }

    friend inline void from_json(const nlohmann::json& j, RabbitMQConfiguration& cfg);

    [[nodiscard]] std::string_view get_host() const noexcept {
        return host;
    }

    //not std::string_view because AMQP::Connection handles const std::string&
    [[nodiscard]] const auto& get_vHost() const noexcept {
        return vHost;
    }

    [[nodiscard]] auto get_port() const noexcept {
        return port;
    }

    [[nodiscard]] std::string_view get_username() const noexcept {
        return username;
    }

    [[nodiscard]] std::string_view get_password() const noexcept {
        return password;
    }

    [[nodiscard]] std::string_view get_subscriberName() const noexcept {
        return subscriberName;
    }

    [[nodiscard]] const auto& get_exchangeName() const noexcept {
        return exchangeName;
    }

    [[nodiscard]] auto get_connectionTimeout() const noexcept {
        return connectionTimeout;
    }

    [[nodiscard]] auto get_connectionCloseTimeout() const noexcept {
        return connectionCloseTimeout;
    }

    [[nodiscard]] auto get_maxRecoveryAttempts() const noexcept {
        return maxRecoveryAttempts;
    }

    [[nodiscard]] auto get_minConnectionRecoveryTimeout() const noexcept {
        return minConnectionRecoveryTimeout;
    }

    [[nodiscard]] auto get_maxConnectionRecoveryTimeout() const noexcept {
        return maxConnectionRecoveryTimeout;
    }

    [[nodiscard]] auto get_prefetchCount() const noexcept {
        return prefetchCount;
    }

private:
     std::string host;
     std::string vHost;
     std::string port;
     std::string username;
     std::string password;
     std::string subscriberName;
     std::string exchangeName;

     int32_t connectionTimeout = -1;

     int32_t connectionCloseTimeout = 10'000;

     int32_t maxRecoveryAttempts = 5;

     int32_t minConnectionRecoveryTimeout = 10'000;

     int32_t maxConnectionRecoveryTimeout = 60'000;

     int32_t prefetchCount = 10;
};

using rmq_configuration_ptr = std::shared_ptr<RabbitMQConfiguration>;

//void to_json(nlohmann::json& j, const RabbitMQConfiguration& p) {

//}

void from_json(const nlohmann::json& j, RabbitMQConfiguration& cfg) {
    j.at("host").get_to(cfg.host);
    j.at("vHost").get_to(cfg.vHost);
    j.at("port").get_to(cfg.port);
    j.at("username").get_to(cfg.username);
    j.at("password").get_to(cfg.password);
    j.at("subscriberName").get_to(cfg.subscriberName);

    if (j.contains("exchangeName")) {
        j.at("exchangeName").get_to(cfg.exchangeName);
    }

    if (j.contains("connectionTimeout")) {
        j.at("connectionTimeout").get_to(cfg.connectionTimeout);
    }

    if (j.contains("connectionCloseTimeout")) {
        j.at("connectionCloseTimeout").get_to(cfg.connectionCloseTimeout);
    }

    if (j.contains("maxRecoveryAttempts")) {
        j.at("maxRecoveryAttempts").get_to(cfg.maxRecoveryAttempts);
    }

    if (j.contains("minConnectionRecoveryTimeout")) {
        j.at("minConnectionRecoveryTimeout").get_to(cfg.minConnectionRecoveryTimeout);
    }

    if (j.contains("exchangeName")) {
        j.at("exchangeName").get_to(cfg.exchangeName);
    }

    if (j.contains("maxConnectionRecoveryTimeout")) {
        j.at("maxConnectionRecoveryTimeout").get_to(cfg.maxConnectionRecoveryTimeout);
    }
}

}
