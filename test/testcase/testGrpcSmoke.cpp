#include <iostream>
#include <grpcpp/grpcpp.h>
#include "src/proto/nacos_grpc_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
//using namespace nacos;

bool testGrpcSmoke() {
    std::cout << "in function testGrpcSmoke" << std::endl;

    std::unique_ptr<Request::Stub> stub_ = Request::NewStub(grpc::CreateChannel("127.0.0.1:9848", grpc::InsecureChannelCredentials()));

    Payload request;
    //request.set_body("");

    // Container for the data we expect from the server.
    Payload reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->request(&context, request, &reply);

    std::cout << "error msg got from RPC:" << status.error_message() << std::endl;
    std::cout << "error code got from RPC:" << status.error_code() << std::endl;
    std::cout << "ok() got from RPC:" << status.ok() << std::endl;
    std::cout << "error_details() got from RPC:" << status.error_details() << std::endl;

    return true;
}

bool testGrpcSmoke() {
    std::cout << "in function testGrpcSmoke" << std::endl;

    std::unique_ptr<Request::Stub> stub_ = Request::NewStub(grpc::CreateChannel("127.0.0.1:9848", grpc::InsecureChannelCredentials()));

    Payload request;
    //request.set_body("");

    // Container for the data we expect from the server.
    Payload reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->request(&context, request, &reply);

    std::cout << "error msg got from RPC:" << status.error_message() << std::endl;
    std::cout << "error code got from RPC:" << status.error_code() << std::endl;
    std::cout << "ok() got from RPC:" << status.ok() << std::endl;
    std::cout << "error_details() got from RPC:" << status.error_details() << std::endl;

    return true;
}