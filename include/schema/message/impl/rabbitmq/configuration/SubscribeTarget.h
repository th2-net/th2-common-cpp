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

namespace th2::common_cpp {

class SubscribeTarget {

public:

    [[nodiscard]] std::string_view get_queue() const noexcept { return queue; }
    [[nodiscard]] std::string_view get_routing_key() const noexcept { return routing_key; }

private:

    std::string queue;
    std::string routing_key;

};

using subscribe_target_ptr = std::shared_ptr<SubscribeTarget>;

}
