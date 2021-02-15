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

#include <filesystem>
#include <string_view>

#include "schema/message/impl/rabbitmq/parsed/RabbitParsedBatchRouter.h"
#include "schema/message/impl/rabbitmq/raw/RabbitRawBatchRouter.h"
#include "schema/message/impl/rabbitmq/group/RabbitMessageGroupBatchRouter.h"
#include "schema/event/EventBatchRouter.h"

#include "schema/message/configuration/MessageRouterConfiguration.h"
#include "schema/message/impl/rabbitmq/configuration/RabbitMQConfiguration.h"

namespace th2::common_cpp {

class AbstractCommonFactory {
public:
    AbstractCommonFactory() = default;

    virtual ~AbstractCommonFactory() = default;

    void init() {
        if (!rmq_cfg) {
            rmq_cfg = std::make_shared<RabbitMQConfiguration>(load_rabbit_mq_configuration());
        }

        if (!message_router_cfg) {
            message_router_cfg = std::make_shared<MessageRouterConfiguration>(load_message_router_configuration());
        }

        init(get_message_router_parsed_batch(), get_message_router_raw_batch(), get_event_batch_router(), get_message_router_message_group_batch());
    }

    void init(message_router_ptr<MessageBatch> message_router_parsed_batch,
              message_router_ptr<RawMessageBatch> message_router_raw_batch,
              message_router_ptr<EventBatch> event_batch_router,
              message_router_ptr<MessageGroupBatch> message_router_group_batch)
    {
        if (!rmq_cfg) {
            rmq_cfg = std::make_shared<RabbitMQConfiguration>(load_rabbit_mq_configuration());
        }

        if (!message_router_cfg) {
            message_router_cfg = std::make_shared<MessageRouterConfiguration>(load_message_router_configuration());
        }

        this->message_router_parsed_batch = message_router_parsed_batch;
        this->message_router_raw_batch = message_router_raw_batch;
        this->event_batch_router = event_batch_router;
        this->message_router_group_batch = message_router_group_batch;
    }

    [[nodiscard]]
    message_router_ptr<MessageBatch> get_message_router_parsed_batch() {
        if (message_router_parsed_batch == nullptr) {
            message_router_parsed_batch = std::make_shared<RabbitParsedBatchRouter>();
            message_router_parsed_batch->init(get_rmq_connection_manager(), message_router_cfg);
        }
        return message_router_parsed_batch;
    }

    [[nodiscard]]
    message_router_ptr<RawMessageBatch> get_message_router_raw_batch() {
        if (message_router_raw_batch == nullptr) {
            message_router_raw_batch = std::make_shared<RabbitRawBatchRouter>();
            message_router_raw_batch->init(get_rmq_connection_manager(), message_router_cfg);
        }
        return message_router_raw_batch;
    }

    [[nodiscard]]
    message_router_ptr<EventBatch> get_event_batch_router() {
        if (event_batch_router == nullptr) {
            event_batch_router = std::make_shared<EventBatchRouter>();
            event_batch_router->init(get_rmq_connection_manager(), message_router_cfg);
        }
        return event_batch_router;
    }

    [[nodiscard]]
    message_router_ptr<MessageGroupBatch> get_message_router_message_group_batch() {
        if (message_router_group_batch == nullptr) {
            message_router_group_batch = std::make_shared<RabbitMessageGroupBatchRouter>();
            message_router_group_batch->init(get_rmq_connection_manager(), message_router_cfg);
        }
        return message_router_group_batch;
    }

protected:
    RabbitMQConfiguration load_rabbit_mq_configuration() {
        auto cfg_path = get_path_to_rmq_configuration();

        if (!std::filesystem::exists(cfg_path)) {
            throw std::runtime_error("Cannot read RabbitMQ configuration");
        }

        return RabbitMQConfiguration::read(cfg_path);
    }

    MessageRouterConfiguration load_message_router_configuration() {
        auto cfg_path = get_path_to_message_router_configuration();

        if (!std::filesystem::exists(cfg_path)) {
            throw std::runtime_error("Cannot read MessageRouter configuration");
        }

        return MessageRouterConfiguration::read(cfg_path);
    }

    virtual std::filesystem::path get_path_to_rmq_configuration() const = 0;

    virtual std::filesystem::path get_path_to_message_router_configuration() const = 0;

    [[nodiscard]]
    connection_manager_ptr create_rmq_connection_manager() const {
        auto cm = std::make_shared<ConnectionManager>(rmq_cfg);
        cm->init();
        return cm;
    }

    [[nodiscard]]
    connection_manager_ptr get_rmq_connection_manager() const noexcept {
        if (!rmq_connection_manager) {
            rmq_connection_manager = create_rmq_connection_manager();
        }
        return rmq_connection_manager;
    }

private:
    rmq_configuration_ptr rmq_cfg;
    message_router_configuration_ptr message_router_cfg;

    message_router_ptr<MessageBatch> message_router_parsed_batch;
    message_router_ptr<RawMessageBatch> message_router_raw_batch;
    message_router_ptr<EventBatch> event_batch_router;
    message_router_ptr<MessageGroupBatch> message_router_group_batch;

    mutable connection_manager_ptr rmq_connection_manager;
};

}
