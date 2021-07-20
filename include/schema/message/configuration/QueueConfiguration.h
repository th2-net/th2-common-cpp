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

#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "schema/message/configuration/MqRouterFilterConfiguration.h"

namespace th2::common_cpp {

class QueueConfiguration;
using queue_configuration_ptr = std::shared_ptr<QueueConfiguration>;
using queues_map_t = std::unordered_map<std::string, queue_configuration_ptr>;
using filters_t = std::vector<std::shared_ptr<MqRouterFilterConfiguration>>;

using queue_attributes_t = std::vector<std::string>;

class QueueConfiguration {
public:
    friend inline void from_json(const nlohmann::json& j, QueueConfiguration& cfg);
    friend inline void from_json(const nlohmann::json& j, queue_configuration_ptr& cfg);

//    QueueConfiguration() = default;
//    QueueConfiguration(const QueueConfiguration& queue) = default;

//    QueueConfiguration& operator=(const QueueConfiguration& queue) = default;

    [[nodiscard]] auto get_routing_key() const noexcept {
        return routing_key;
    }

    [[nodiscard]] auto is_writable() const noexcept {
        return isWritable;
    }

    [[nodiscard]] auto is_readable() const noexcept {
        return isReadable;
    }

    [[nodiscard]] const auto& get_queue() const noexcept {
        return queue;
    }

    [[nodiscard]] const auto& get_exchange() const noexcept {
        return exchange;
    }

    [[nodiscard]] const auto& get_attributes() const noexcept {
        return attributes;
    }

    [[nodiscard]] const auto& get_filters() const noexcept {
        return filters;
    }

    [[nodiscard]]
    bool contains_all_attributes(const queue_attributes_t& attrs) const noexcept {
        bool res = true;

        for (const auto attr : attrs) {
            if (std::find(std::cbegin(attributes), std::cend(attributes), attr) == std::cend(attributes)) {
                res = false;
                break;
            }
        }

        return res;
    }

private:

    /**
     * Routing key in RabbitMQ
    */
    std::string routing_key;

    /**
     * Queue name in RabbitMQ
     */
    std::string queue;

    std::string exchange;

    //@JsonAlias({"labels", "tags"})
    queue_attributes_t attributes;

    filters_t filters;

    bool isReadable = true;

    bool isWritable = true;
};

void from_json(const nlohmann::json& j, QueueConfiguration& cfg) {
    //j.at("name").get_to(cfg.name);
    j.at("queue").get_to(cfg.queue);
    //j.at("name").get_to(cfg.routing_key);
    j.at("exchange").get_to(cfg.exchange);
    j.at("attributes").get_to(cfg.attributes);

    if (j.contains("filters")) {

    }
}

void from_json(const nlohmann::json& j, queue_configuration_ptr& cfg) {
    if (cfg == nullptr) {
        cfg = std::make_shared<QueueConfiguration>();
    }

    //j.at("name").get_to(cfg->name);
    j.at("queue").get_to(cfg->queue);
    //j.at("name").get_to(cfg->routing_key);
    j.at("exchange").get_to(cfg->exchange);
    j.at("attributes").get_to(cfg->attributes);

    if (j.contains("filters")) {

    }
}

}
