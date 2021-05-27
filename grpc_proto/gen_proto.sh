#!/bin/bash

protoc -I . --grpc_out=../src/proto --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./nacos_grpc_service.proto
protoc -I . --cpp_out=../src/proto ./nacos_grpc_service.proto

