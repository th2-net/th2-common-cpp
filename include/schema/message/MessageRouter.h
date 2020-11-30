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

#include "schema/message/configuration/MessageRouterConfiguration.h"
#include "schema/message/impl/rabbitmq/connection/ConnectionManager.h"
#include "schema/message/SubscriberMonitor.h"
#include "schema/message/MessageListener.h"

namespace th2::common_cpp {

template <class MessageBatchType>
class MessageRouter {
public:
    ~MessageRouter() = default;

    virtual void init(connection_manager_ptr connection_manager, message_router_configuration_ptr message_router_configuration) = 0;

    //Deprecated(since = "1.4.2", forRemoval = true)
    [[deprecated]] virtual subscriber_monitor_ptr subscribe(const std::string& queue_alias, message_listener_ptr<MessageBatchType> callback) = 0;
    virtual subscriber_monitor_ptr subscribe(message_listener_ptr<MessageBatchType> callback, const queue_attributes_t& queue_attributes) = 0;
    virtual subscriber_monitor_ptr subscribeAll(message_listener_ptr<MessageBatchType> callback) = 0;
    virtual subscriber_monitor_ptr subscribeAll(message_listener_ptr<MessageBatchType> callback, const queue_attributes_t& queue_attributes) = 0;

    virtual void send(const MessageBatchType& batch) = 0;
    virtual void send(const MessageBatchType& batch, const queue_attributes_t& queue_attributes) = 0;
    virtual void sendAll(const MessageBatchType& batch, const queue_attributes_t& queue_attributes) = 0;
};

template <class MessageBatchType>
using message_router_ptr = std::shared_ptr<MessageRouter<MessageBatchType>>;

}
