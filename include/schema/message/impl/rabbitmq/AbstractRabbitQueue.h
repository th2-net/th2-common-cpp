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

#include "schema/message/MessageQueue.h"

namespace th2::common_cpp {

template <class MessageBatchType>
class AbstractRabbitQueue : public MessageQueue<MessageBatchType> {
public:
    void init(connection_manager_ptr connection_manager, queue_configuration_ptr queue_configuration) override {
        if (!connection_manager) {
            throw std::runtime_error("Connection can not be null");
        }

        if (this->connection_manager != nullptr) {
            throw std::runtime_error("Queue is already initialized");
        }

        this->connection_manager = connection_manager;
        this->queue_configuration = queue_configuration;
    }

    message_subscriber_ptr<MessageBatchType> get_subscriber() override {
        throw std::logic_error("Method get_subscriber() not yet implemented!");

        if (!connection_manager || !queue_configuration) {
            throw std::runtime_error("Queue is not initialized");
        }

        if (!queue_configuration->is_readable()) {
            throw std::runtime_error("Queue can not read");
        }

        /* Original Java
        return subscriber.updateAndGet( subscriber -> {
            if (subscriber == null) {
                return createSubscriber(connectionManger, queueConfiguration);
            }
            return subscriber;
        });
        */
        return nullptr;
    }

    message_sender_ptr<MessageBatchType> get_sender() override {
        if (!connection_manager || !queue_configuration) {
            throw std::runtime_error("Queue is not initialized");
        }

        if (!queue_configuration->is_writable()) {
            throw std::runtime_error("Queue can not write");
        }

        if (!sender) {
            sender = create_sender(connection_manager, queue_configuration);
        }

        return sender;
    }

    virtual message_sender_ptr<MessageBatchType> create_sender(connection_manager_ptr connection_manager,
                                                               queue_configuration_ptr queue_configuration) = 0;

private:
    connection_manager_ptr connection_manager;
    queue_configuration_ptr queue_configuration;

    message_sender_ptr<MessageBatchType> sender;
    message_subscriber_ptr<MessageBatchType> subscriber;
};

}
