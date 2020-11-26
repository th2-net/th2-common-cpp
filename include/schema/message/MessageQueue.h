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

#include "schema/message/impl/rabbitmq/connection/ConnectionManager.h"
#include "schema/message/MessageSubscriber.h"
#include "schema/message/MessageSender.h"
#include "schema/message/configuration/QueueConfiguration.h"

namespace th2::common_cpp {

template <class MessageBatchType>
class MessageQueue {
public:
    virtual ~MessageQueue() = default;

    virtual void init(connection_manager_ptr connection_manager, queue_configuration_ptr queue_configuration) = 0;

    virtual message_subscriber_ptr<MessageBatchType> get_subscriber() = 0;

    virtual message_sender_ptr<MessageBatchType> get_sender() = 0;
};

template <class MessageBatchType>
using message_queue_ptr = std::shared_ptr<MessageQueue<MessageBatchType>>;

}
