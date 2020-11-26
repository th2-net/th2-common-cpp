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

#include <string_view>

#include "infra.pb.h"

namespace th2::common_cpp {

class FieldFilterConfiguration {
public:
    [[nodiscard]] std::string_view get_value() const noexcept {
        return value;
    }

    [[nodiscard]] auto get_filter_operation() const noexcept {
        return filter_operation;
    }

private:
    std::string value;
    FilterOperation filter_operation;
};

}
