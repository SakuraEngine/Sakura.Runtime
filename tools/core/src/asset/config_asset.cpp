#include "asset/config_asset.hpp"
#include "asset/asset_registry.hpp"
#include "asset/importer.hpp"
#include "platform/configure.h"
#include "platform/memory.h"
#include "resource/config_resource.h"
#include "json/reader.h"
#include "platform/debug.h"
#include "simdjson.h"
#include "type/type_registry.h"
#include "utils/log.h"
#include "utils/defer.hpp"
#include "SkrRT/typeid.generated.hpp"

namespace skd::asset
{
TOOL_API SConfigRegistry* GetConfigRegistry()
{
    static SConfigRegistry registry;
    return &registry;
}
} // namespace skd::asset

namespace skd
{
namespace asset
{
void* SJsonConfigImporter::Import(const SAssetRecord* record)
{
    auto registry = GetConfigRegistry();
    auto iter = registry->typeInfos.find(configType);
    if (iter == registry->typeInfos.end())
    {
        SKR_LOG_ERROR("import resource %s failed, type is not registered as config", record->path.u8string().c_str());
        return nullptr;
    }
    // TODO: replace file load with skr api
    auto jsonString = simdjson::padded_string::load(record->path.u8string());
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(jsonString);
    skr_config_resource_t* resource = skr::resource::SConfigFactory::NewConfig(configType);
    iter->second.Import(doc.value(), resource->configData);
    return resource; //导入具体数据
}
uint32_t SConfigCooker::Version()
{
    return 0;
}
bool SConfigCooker::Cook(SCookContext* ctx)
{
    //-----load config
    // no cook config for config, skipping
    //-----import resource object
    auto resource = ctx->Import<skr_config_resource_t>();
    SKR_DEFER({ SkrDelete(resource); });
    //-----emit dependencies
    // no static dependencies
    //-----cook resource
    // no cook needed for config, just binarize it
    //-----fetch runtime dependencies
    auto type = (skr::type::RecordType*)skr_get_type(&resource->configType);
    for (auto& field : type->fields)
    {
        if (field.type->Same(skr::type::type_of<skr_resource_handle_t>::get()))
        {
            auto handle = (skr_resource_handle_t*)((char*)resource->configData + field.offset);
            if (handle->is_null())
                continue;
            ctx->AddRuntimeDependency(handle->get_guid());
        }
    }
    //-----write resource header
    eastl::vector<uint8_t> buffer;
    skr::resource::SBinarySerializer archive(buffer);
    ctx->WriteHeader(archive, this);
    //------write resource object
    skr::resource::SConfigFactory::Serialize(*resource, archive);
    //------save resource to disk
    auto file = fopen(ctx->output.u8string().c_str(), "wb");
    SKR_DEFER({ fclose(file); });
    fwrite(buffer.data(), 1, buffer.size(), file);
    return true;
}

bool SJsonConfigImporterFactory::CanImport(const SAssetRecord* record)
{
    if (record->path.extension() == ".json")
        return true;
    return false;
}
skr_guid_t SJsonConfigImporterFactory::GetResourceType()
{
    return get_type_id_skr_config_resource_t();
}
SImporter* SJsonConfigImporterFactory::CreateImporter(const SAssetRecord* record)
{
    // TODO: invoke user interface?
    SKR_UNIMPLEMENTED_FUNCTION();
    return nullptr;
}
} // namespace asset
} // namespace skd