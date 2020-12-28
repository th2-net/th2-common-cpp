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

#include <chrono>
#include <memory>
#include <thread>

#include <amqp.h>
#include <amqp_tcp_socket.h>

#include "schema/message/impl/rabbitmq/configuration/RabbitMQConfiguration.h"
#include "Util.h"

namespace th2::common_cpp {

#define AMQP_AUTODELETE		1
#define AMQP_DURABLE		2
#define AMQP_PASSIVE		4
#define AMQP_MANDATORY		8
#define AMQP_IMMIDIATE		16
#define AMQP_IFUNUSED		32
#define AMQP_EXCLUSIVE		64
#define AMQP_NOWAIT			128
#define AMQP_NOACK			256
#define AMQP_NOLOCAL		512
#define AMQP_MULTIPLE		1024

class ConnectionManager {
public:

    ConnectionManager(rmq_configuration_ptr cfg) {
        rmq_configuration = cfg;
        _debug = false;

        if (std::getenv("TH2_COMMON_CPP_DEBUG")) {
        	_debug = true;
        }
    }

    ~ConnectionManager() {
    }

    void init() {
        if (rmq_configuration->get_subscriberName().empty()) {
            subscriber_name = rmq_configuration->get_subscriberName();
        } else {
            subscriber_name = "rabbit_mq_subscriber."; //TODO implement + System.currentTimeMillis();
        }

    	_conn = amqp_new_connection();

    	_socket = amqp_tcp_socket_new(_conn);
    	_channel = 0;

    	_type = "direct";

     	std::cout << "Connecting to amqp://" << rmq_configuration->get_username() << ":" << "***" << "@" << rmq_configuration->get_host() << ":" << rmq_configuration->get_port() << "/" << rmq_configuration->get_vHost() << std::endl;


    	_status = amqp_socket_open(_socket, rmq_configuration->get_host().c_str(), std::stoi(rmq_configuration->get_port()));

    	if (_status) {
    		throw std::runtime_error("RabbitMQ connection failed");
    	}

    	auto login_status = amqp_login(_conn, rmq_configuration->get_vHost().c_str(), 0,
    			AMQP_DEFAULT_FRAME_SIZE, 0, AMQP_SASL_METHOD_PLAIN,
				rmq_configuration->get_username().c_str(),
				rmq_configuration->get_password().c_str());

    	if (login_status.reply_type != AMQP_RESPONSE_NORMAL) {
    		throw std::runtime_error("RabbitMQ login failed");
    	}

    	//channel
   		++_channel;

   		amqp_channel_open(_conn, _channel);

   		auto rpc_reply_status = amqp_get_rpc_reply(_conn);

   		if (rpc_reply_status.reply_type != AMQP_RESPONSE_NORMAL) {
   		  throw std::runtime_error("RabbitMQ channel open failed");
   		}

    	//channel
    }

    void send_publish_command(amqp_connection_state_t conn, int channel, const char * exchange_name, amqp_bytes_t messageByte, const char * key) {
    	amqp_bytes_t exchangeByte = amqp_cstring_bytes(exchange_name);
    	amqp_bytes_t keyrouteByte = amqp_cstring_bytes(key);
    	int parms = 0;
    	amqp_basic_properties_t props;

    	props.content_type = amqp_cstring_bytes("text/plain");

    	props.delivery_mode = 2; // persistent delivery mode

    	props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;

    	props.headers.num_entries = 0;

    	amqp_table_entry_t_ *entries = (amqp_table_entry_t_*) malloc(sizeof(amqp_table_entry_t_) * props.headers.num_entries);

    	int i = 0;

    	props._flags += AMQP_BASIC_HEADERS_FLAG;

    	short mandatory = (parms & AMQP_MANDATORY) ? 1:0;
    	short immediate = (parms & AMQP_IMMIDIATE) ? 1:0;

    	int res = amqp_basic_publish(
    		conn,
    		channel,
    		exchangeByte,
    		keyrouteByte,
    		mandatory,
    		immediate,
    		&props,
    		messageByte
    	);

    	free(entries);

        if ( 0 > res ) {
    		throw std::runtime_error("AMQP Publish Fail. Error: " + std::to_string(res));
    	}
    }

    void basic_publish(const std::string& exchange, const std::string& routing_key, const ByteVector& message) {

		amqp_bytes_t message_bytes;

		message_bytes.len = message.size();
		message_bytes.bytes = const_cast<uint8_t*>(message.data());

		send_publish_command(_conn, _channel, exchange.c_str(), message_bytes, routing_key.c_str());


		if (_debug) {
			std::cout << "Exchange: " << exchange << ", routing key: " << routing_key << ", send: " << message.size() << " byte(s)" << std::endl;
		}
    }

private:
    rmq_configuration_ptr rmq_configuration;


    std::string subscriber_name;

	amqp_socket_t* _socket;
	amqp_connection_state_t _conn;
	int _status;
	int _channel;
	std::string _type;
	bool _debug;
};

using connection_manager_ptr = std::shared_ptr<ConnectionManager>;

}
