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
/*
public abstract class AbstractRabbitSubscriber<T> implements MessageSubscriber<T> {
    protected final Logger logger = LoggerFactory.getLogger(this.getClass() + "@" + this.hashCode());

    private final List<MessageListener<T>> listeners = new CopyOnWriteArrayList<>();
    private final AtomicReference<String> exchangeName = new AtomicReference<>();
    private final AtomicReference<SubscribeTarget> subscribeTarget = new AtomicReference<>();
    private final AtomicReference<ConnectionManager> connectionManager = new AtomicReference<>();
    private final AtomicReference<String> consumerTag = new AtomicReference<>();

    @Override
    public void start() throws Exception {
        ConnectionManager connectionManager = this.connectionManager.get();
        SubscribeTarget target = subscribeTarget.get();
        String exchange = exchangeName.get();

        if (connectionManager == null || target == null || exchange == null) {
            throw new IllegalStateException("Subscriber is not initialized");
        }

        try {
            var queue = target.getQueue();
            var routingKey = target.getRoutingKey();

            consumerTag.updateAndGet(tag -> {
                if (tag != null) {
                    throw new IllegalStateException("Subscriber already started");
                }

                try {
                    tag = connectionManager.basicConsume(queue, this::handle, this::canceled);
                    logger.info("Start listening consumerTag='{}',exchangeName='{}', routing key='{}', queue name='{}'", tag, exchangeName, routingKey, queue);
                } catch (IOException e) {
                    throw new IllegalStateException("Can not start subscribe to queue = " + queue, e);
                }

                return tag;
            });
        } catch (Exception e) {
            throw new IOException("Can not start listening", e);
        }
    }

    @Override
    public void addListener(MessageListener<T> messageListener) {
        listeners.add(messageListener);
    }

    @Override
    public boolean isOpen() {
        ConnectionManager connectionManager = this.connectionManager.get();
        return consumerTag.get() != null && connectionManager != null && connectionManager.isOpen();
    }

    @Override
    public void close() throws Exception {
        ConnectionManager connectionManager = this.connectionManager.get();
        if (connectionManager == null) {
            throw new IllegalStateException("Subscriber is not initialized");
        }

        String tag = consumerTag.getAndSet(null);
        if (tag == null) {
            return;
        }

        connectionManager.basicCancel(tag);

        listeners.forEach(MessageListener::onClose);
        listeners.clear();
    }

    protected abstract T valueFromBytes(byte[] body) throws Exception;

    protected abstract String toShortDebugString(T value);

    @Nullable
    protected abstract T filter(T value) throws Exception;


    private void handle(String consumeTag, Delivery delivery) {
        try {
            T value = valueFromBytes(delivery.getBody());

            if (logger.isDebugEnabled()) {
                logger.debug("The received message {}", toShortDebugString(value));
            }

            var filteredValue = filter(value);

            if (Objects.isNull(filteredValue)) {
                logger.debug("Message is filtred");
                return;
            }

            for (MessageListener<T> listener : listeners) {
                try {
                    listener.handler(consumeTag, filteredValue);
                } catch (Exception listenerExc) {
                    logger.warn("Message listener from class '{}' threw exception", listener.getClass(), listenerExc);
                }
            }
        } catch (Exception e) {
            logger.error("Can not parse value from delivery for: {}", consumeTag, e);
        }
    }

    private void canceled(String consumerTag) {
        logger.warn("Consuming cancelled for: '{}'", consumerTag);
        try {
            close();
        } catch (Exception e) {
            logger.error("Can not close subscriber with exchange name '{}' and queues '{}'", exchangeName.get(), subscribeTarget.get(), e);
        }
    }

}


*/
