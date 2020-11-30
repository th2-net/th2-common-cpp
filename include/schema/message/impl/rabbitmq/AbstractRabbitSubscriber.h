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

#include "schema/message/MessageSubscriber.h"
#include "schema/message/impl/rabbitmq/configuration/SubscribeTarget.h"

namespace th2::common_cpp {

template <class MessageBatchType>
class AbstractRabbitSubscriber : public MessageSubscriber<MessageBatchType> {
private:

    std::vector<MessageListener<MessageBatchType>> listeners;
    std::string exchange_name;
    subscribe_target_ptr subscribe_target;
    connection_manager_ptr connection_manager;
    std::string consumer_tag;

public:
    void init(connection_manager_ptr connection_manager,
              const std::string_view exchange_name,
              subscribe_target_ptr subscribe_target) override {

        if (this->connection_manager || !this->exchange_name.empty() || this->subscribe_target) {
            throw std::runtime_error("Subscriber is already initialize");
        }

        this->connection_manager = connection_manager;
        this->exchange_name = exchange_name;
        this->subscribe_target = subscribe_target;
    }

    void start() override {
        connection_manager_ptr _connection_manager = this->connection_manager;
        subscribe_target_ptr target = subscribe_target;
        std::string exchange = exchange_name;

        if (!_connection_manager || !target || exchange.empty()) {
            throw std::runtime_error("Subscriber is not initialized");
        }

        try {
            auto target->get_queue();
            auto routing_key = target->get_routing_key();
        }  catch () {

        } {
    }

    /*void send(const MessageBatchType& message) override {
        connection_manager->basic_publish(exchange_name, send_queue, value_to_bytes(message));
    }*/

protected:
    virtual ByteVector value_from_bytes(const MessageBatchType& message) = 0;
};

}

