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

#include "common.pb.h"

#include "schema/message/impl/rabbitmq/AbstractRabbitQueue.h"
#include "schema/message/impl/rabbitmq/raw/RabbitRawBatchSender.h"

namespace th2::common_cpp {

class RabbitRawBatchQueue : public AbstractRabbitQueue<RawMessageBatch> {
protected:
    message_sender_ptr<RawMessageBatch> create_sender(connection_manager_ptr connection_manager,
                                                   queue_configuration_ptr queue_configuration) override {
        auto sender = std::make_shared<RabbitRawBatchSender>();
        sender->init(connection_manager, queue_configuration->get_exchange(), queue_configuration->get_routing_key());
        return sender;
    }
};

}

