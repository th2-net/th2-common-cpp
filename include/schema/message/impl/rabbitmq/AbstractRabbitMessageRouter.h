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

#include "schema/message/MessageRouter.h"
#include "schema/message/MessageQueue.h"

namespace th2::common_cpp {

template <class MessageBatchType>
class AbstractRabbitMessageRouter : public MessageRouter<MessageBatchType> {
public:
    void init(connection_manager_ptr connection_manager,
              message_router_configuration_ptr message_router_configuration) override {
        this->connection_manager = connection_manager;
        this->message_router_configuration = message_router_configuration;
    }

    subscriber_monitor_ptr subscribe(const std::string& queue_alias, message_listener_ptr<MessageBatchType> callback) override {

        throw std::logic_error("Method subscribe() not yet implemented!");

        auto queue = get_message_queue(queue_alias);
        message_subscriber_ptr<MessageBatchType> subscriber = queue->get_subscriber();
        subscriber->add_listener(callback);

        return nullptr;
    };

    subscriber_monitor_ptr subscribe(message_listener_ptr<MessageBatchType> callback, const queue_attributes_t& queue_attributes) override {
        auto req_subscribe_attributes = required_subscribe_attributes();
        auto queues = get_configuration()->find_queues_by_attr(req_subscribe_attributes);
        if (queues.size() != 1) {
            throw std::runtime_error("Wrong amount of queues for subscribe. Found " + std::to_string(queues.size()) + " queues, but must not be more than 1. Search was done by " + /*std::string(req_subscribe_attributes.data()) +*/ " attributes");
        }
        return subscribe(req_subscribe_attributes[1], callback);
    };

    subscriber_monitor_ptr subscribeAll(message_listener_ptr<MessageBatchType>) {

        throw std::logic_error("Method subscribeAll() not yet implemented!");
        return nullptr;
    }

    subscriber_monitor_ptr subscribeAll(message_listener_ptr<MessageBatchType>, const queue_attributes_t& queue_attributes) {

        throw std::logic_error("Method subscribeAll() not yet implemented!");
        return nullptr;
    }

    using alias_and_message_to_send_t = std::unordered_map<std::string, MessageBatchType>;

    void send(const MessageBatchType& batch) override {
        auto req_send_attributes = required_send_attributes();

        auto filteredByAttrAndFilter = find_by_filter(!req_send_attributes.empty() ?
                                                          get_configuration()->find_queues_by_attr(req_send_attributes) :
                                                          get_configuration()->get_queues(), batch);

        if (filteredByAttrAndFilter.size() != 1) {
            throw std::runtime_error("Wrong count of queues for send. Should be equal to 1. Find queues = " + std::to_string(filteredByAttrAndFilter.size()));
        }

        send(filteredByAttrAndFilter);
    }

    void send(const MessageBatchType& batch, const queue_attributes_t& queue_attributes) override {

        auto filteredByAttr = get_configuration()->find_queues_by_attr(queue_attributes);

        auto filteredByAttrAndFilter = find_by_filter(filteredByAttr, batch);

        if (filteredByAttrAndFilter.size() != 1) {
            throw std::runtime_error("Wrong size of queues for send. Should be equal to 1");
        }

        send(filteredByAttrAndFilter);

    }

    void sendAll(const MessageBatchType& batch, const queue_attributes_t& queue_attributes) override {

        auto filteredByAttr = get_configuration()->find_queues_by_attr(queue_attributes);

        auto filteredByAttrAndFilter = find_by_filter(filteredByAttr, batch);

        if (filteredByAttrAndFilter.empty()) {
            throw std::runtime_error("Wrong size of queues for send. Can't be equal to 0");
        }

        send(filteredByAttrAndFilter);

    }

protected:
    virtual message_queue_ptr<MessageBatchType> create_queue(connection_manager_ptr connection_manager, queue_configuration_ptr queue_configuration) = 0;

    virtual alias_and_message_to_send_t find_by_filter(const queues_map_t& queues_map, const MessageBatchType& batch) = 0;

    virtual queue_attributes_t required_send_attributes() = 0;

    virtual queue_attributes_t required_subscribe_attributes() = 0;

    void send(const alias_and_message_to_send_t& alias_and_message_to_send) {
        for (const auto& [alias, message] : alias_and_message_to_send) {
            auto sender = get_message_queue(alias)->get_sender();
            sender->send(message);
        }
    }

    [[nodiscard]]
    message_queue_ptr<MessageBatchType> get_message_queue(const std::string& queue_alias) {
        if (!connection_manager) {
            throw std::runtime_error("Router is not initialized");
        }

        auto queue_by_alias = get_configuration()->get_queue_by_alias(queue_alias);

        return create_queue(connection_manager, queue_by_alias);
    }

    [[nodiscard]]
    message_router_configuration_ptr get_configuration() {
        if (!message_router_configuration) {
            throw std::runtime_error("Router is not initialized");
        }

        return message_router_configuration;
    }

private:
    connection_manager_ptr connection_manager;
    message_router_configuration_ptr message_router_configuration;
};

}
