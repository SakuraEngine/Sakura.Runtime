#include "asset/asset_registry.hpp"
#include "asset/cooker.hpp"
#include "asset/importer.hpp"
#include "bitsery/adapter/buffer.h"
#include "ftl/task_counter.h"
#include "ghc/filesystem.hpp"
#include "google/protobuf/empty.pb.h"
#include "platform/guid.h"
#include "platform/thread.h"
#include "resource/config_resource.h"
#include "resource/resource_header.h"
#include "skrcompiler.grpc.pb.h"
#include "skrcompiler.pb.h"
#include <grpcpp/completion_queue.h>
#include <grpcpp/support/status.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/create_channel.h>
#include <mutex>
#include "asset/config_asset.hpp"
#include "bitsery/serializer.h"
#include "type/type_registry.h"
#include "utils/defer.hpp"
#include "resource/resource_header.h"
#include "SkrTool/serialize.generated.h"
#include "utils/format.hpp"
#include "SkrRT/typeid.generated.hpp"
#include "module/module_manager.hpp"
#include "platform/vfs.h"

class CompileResourceImpl final : public skrcompiler::CompileResource::Service
{
    ::grpc::Status Compile(::grpc::ServerContext* context, const ::skrcompiler::CompileInfo* request, ::skrcompiler::CompileResult* response) override
    {
        using namespace grpc;
        response->set_errorcode(0);
        response->set_errormessage("fuck");
        (void)stub;

        return Status::OK;
    }
    skrcompiler::HostResource::Stub* stub;
};

class HostResourceImpl final : public skrcompiler::HostResource::Service
{
    ::grpc::Status GetPath(::grpc::ServerContext* context, const ::skrcompiler::ResourceId* request, ::skrcompiler::ResourcePath* response) override
    {
        using namespace grpc;
        response->set_path(request->guid() + "asdsad");
        return Status::OK;
    }

    ::grpc::Status Register(::grpc::ServerContext* context, const ::skrcompiler::Port* request, ::google::protobuf::Empty* response) override
    {
        using namespace grpc;
        std::cout << "Worker registering on localhost:" << request->number() << std::endl;
        stub.push_back(std::make_unique<skrcompiler::CompileResource::Stub>(CreateChannel("localhost:" + request->number(), InsecureChannelCredentials())));

        std::cout << "Worker connected on localhost:" << request->number() << std::endl;
        return Status::OK;
    }
    std::vector<std::unique_ptr<skrcompiler::CompileResource::Stub>> stub;
};

int main(int argc, char** argv)
{
    auto moduleManager = skr_get_module_manager();
    auto root = ghc::filesystem::current_path();
    moduleManager->mount(root.u8string().c_str());
    moduleManager->make_module_graph("GameTool", true);
    moduleManager->init_module_graph();

    auto& registry = *skd::asset::GetAssetRegistry();
    //----- register project
    // TODO: project discover?
    auto project = SkrNew<skd::asset::SProject>();
    auto parentPath = root.parent_path().u8string();
    skr_vfs_desc_t vfs_desc = {};
    vfs_desc.app_name = "Projejct";
    vfs_desc.mount_type = SKR_MOUNT_TYPE_ABSOLUTE;
    vfs_desc.override_mount_dir = parentPath.c_str();
    project->vfs = skr_create_vfs(&vfs_desc);
    project->assetPath = (root.parent_path() / "../../../samples/game/assets").lexically_normal();
    project->outputPath = (root.parent_path() / "resources/game").lexically_normal();
    project->dependencyPath = (root.parent_path() / "deps/game").lexically_normal();
    registry.AddProject(project);
    //----- run cook tasks
    auto& system = *skd::asset::GetCookSystem();
    system.Initialize();
    {
        ftl::TaskCounter counter(&system.scheduler);
        for (auto& pair : registry.assets)
        {
            if (!(pair.second->type == skr_guid_t()))
            {
                auto Task = [](ftl::TaskScheduler* scheduler, void* data) {
                    auto& system = *skd::asset::GetCookSystem();
                    system.EnsureCooked(((skd::asset::SAssetRecord*)data)->guid);
                };
                system.scheduler.AddTask({ Task, pair.second }, ftl::TaskPriority::High, &counter);
            }
        }
        system.scheduler.WaitForCounter(&counter);
    }
    //----- wait
    while (1)
    {
        eastl::shared_ptr<ftl::TaskCounter> counter;
        {
            SMutexLock lock(system.taskMutex);
            if (system.cooking.empty())
                break;
            counter = system.cooking.begin()->second->counter;
        }
        system.scheduler.WaitForCounter(counter.get());
    }
    moduleManager->destroy_module_graph();
    return 0;
    using namespace grpc;
    if (argc != 1)
    {
        std::string server_address1("localhost:0");
        CompileResourceImpl service;
        ServerBuilder builder;
        int selectedPort = 0;
        std::stringstream ss;
        builder.AddListeningPort(server_address1, grpc::InsecureServerCredentials(), &selectedPort);
        builder.RegisterService(&service);
        std::unique_ptr<Server> server(builder.BuildAndStart());
        ss << selectedPort;
        std::cout << "Server listening on localhost:" << ss.str() << std::endl;
        skrcompiler::HostResource::Stub stub(CreateChannel("localhost:50052", InsecureChannelCredentials()));
        std::cout << "connected to localhost:50052" << std::endl;
        ClientContext context;
        grpc::CompletionQueue cq;
        skrcompiler::Port id;
        id.set_number(ss.str());
        google::protobuf::Empty empty;
        auto reg = stub.Register(&context, id, &empty);
        std::cout << "registed to localhost:50052" << std::endl;
        server->Wait();
    }
    else
    {
        std::string server_address("localhost:50052");
        HostResourceImpl service;
        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;
        server->Wait();
    }
}