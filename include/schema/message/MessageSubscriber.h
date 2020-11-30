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
#include "schema/message/impl/rabbitmq/configuration/SubscribeTarget.h"
#include "MessageListener.h"

namespace th2::common_cpp {

template <class MessageBatchType>
class MessageSubscriber {
public:
    virtual ~MessageSubscriber() = default;

    virtual void init(connection_manager_ptr connection_manager,
                      const std::string_view exchange_name,
                      const SubscribeTarget subscribe_targets) = 0;

    virtual void start() = 0;

    //@Deprecated(forRemoval = true, since = "1.4.2")
    [[deprecated]] virtual bool isOpen() = 0;

    virtual void add_listener(message_listener_ptr<MessageBatchType> message_listener) = 0;

};

template <class MessageBatchType>
using message_subscriber_ptr = std::shared_ptr<MessageSubscriber<MessageBatchType>>;

}
