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
#include <unordered_map>

#include "QueueConfiguration.h"

namespace th2::common_cpp {

class MessageRouterConfiguration {
public:
    static MessageRouterConfiguration read(const std::filesystem::path cfg_file) {
        std::ifstream stream(cfg_file.string().c_str());

        std::cout << cfg_file.string() << ": \n" << stream.rdbuf() << std::endl;

        stream.seekg (0, stream.beg);

        nlohmann::json js;
        stream >> js;

        MessageRouterConfiguration cfg;
        nlohmann::from_json(js, cfg);

        return cfg;
    }

    friend inline void from_json(const nlohmann::json& j, MessageRouterConfiguration& cfg);

    [[nodiscard]]
    const auto& get_queue_by_alias(const std::string& queue_alias) const {
        if (auto it = queues.find(queue_alias); it != std::cend(queues)) {
            return it->second;
        }

        throw std::runtime_error("Queue [" + queue_alias + "] is not exist");
    }

    [[nodiscard]]
    queues_map_t find_queues_by_attr(const queue_attributes_t& attr) const {
        queues_map_t queues_map;

        for (auto&& [name, queue] : queues) {
            if (queue->contains_all_attributes(attr)) {
                queues_map.emplace(name, queue);
            }
        }

        return queues_map;
    }

    [[nodiscard]]
    const auto& get_queues() const noexcept {
        return queues;
    }

private:
    std::unordered_map<std::string, queue_configuration_ptr> queues;
};

void from_json(const nlohmann::json& j, MessageRouterConfiguration& cfg) {
    j.at("queues").get_to(cfg.queues);
}

using message_router_configuration_ptr = std::shared_ptr<MessageRouterConfiguration>;

}
