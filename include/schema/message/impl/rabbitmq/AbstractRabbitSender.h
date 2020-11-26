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

#include "schema/message/MessageSender.h"

namespace th2::common_cpp {

template <class MessageBatchType>
class AbstractRabbitSender : public MessageSender<MessageBatchType> {
private:
    std::string send_queue;
    std::string exchange_name;
    connection_manager_ptr connection_manager;

public:
    void init(connection_manager_ptr connection_manager,
              const std::string_view exchange_name,
              const std::string_view send_queue) override {
        this->connection_manager = connection_manager;
        this->exchange_name = exchange_name;
        this->send_queue = send_queue;
    }

    void send(const MessageBatchType& message) override {
        connection_manager->basic_publish(exchange_name, send_queue, value_to_bytes(message));
    }

protected:
    virtual ByteVector value_to_bytes(const MessageBatchType& message) = 0;
};

}
