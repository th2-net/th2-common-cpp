#   Copyright 2020-2020 Exactpro (Exactpro Systems Limited)
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

#https://github.com/th2-net/th2-grpc-common/blob/master/src/main/proto/th2_grpc_common/common.proto

protoc --proto_path=../submodules/th2-grpc-common/src/main/proto/th2_grpc_common common.proto --cpp_out=../src
protoc --proto_path=../submodules/th2-grpc-common/src/main/proto/th2_grpc_common common.proto --grpc_out=../src --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin

mv -f ../src/*.pb.h ../include