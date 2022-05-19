#include "render-scene.h"
#include "gamert.h"
#include "utils/hash.h"
#include "ecs/dualX.h"

dual_type_index_t gfx_shader_set_type = UINT32_MAX;  
dual_type_index_t processor_shader_set_type = UINT32_MAX;  
dual_type_index_t gfx_material_type = UINT32_MAX;  
dual_type_index_t gfx_root_sig_type = UINT32_MAX;  
dual_type_index_t processor_material_type = UINT32_MAX;  
// index/vertex buffers
dual_type_index_t index_buffer_type = UINT32_MAX;  
dual_type_index_t vertex_buffer_type = UINT32_MAX; 
dual_type_index_t transform_type = UINT32_MAX; 

static struct 
{
    dual_storage_t* storage;
} ecsRenderer;

void __gfx_shader_set_construct_callback(void* u, dual_chunk_view_t* view)
{
    const gfx_shader_set_t* set = u;
    *(gfx_shader_set_t*)dualV_get_owned_rw(view, gfx_shader_set_type) = *set;
}
gfx_shader_set_id_t ecsr_register_gfx_shader_set(const gfx_shader_set_t* set) SKR_NOEXCEPT
{
    dual_storage_t* storage = gamert_get_ecs_world();
    dual_entity_type_t alloc_type = {
        .type.data = &gfx_shader_set_type,
        .type.length = 1,
    };
    dual_type_set_t key_set = {
        .data = &gfx_shader_set_type,
        .length = 1
    };
    return dualX_hashset_insert(storage, &key_set, &alloc_type, &__gfx_shader_set_construct_callback, (void*)set);
}

gfx_shader_set_t* ecsr_query_gfx_shader_set(gfx_shader_set_id_t id) SKR_NOEXCEPT
{
    dual_storage_t* storage = gamert_get_ecs_world();
    dual_chunk_view_t view = {0};
    dualS_access(storage, id, &view);
    return (gfx_shader_set_t*)dualV_get_owned_rw(&view, gfx_shader_set_type);
}

bool ecsr_unregister_gfx_shader_set(gfx_shader_set_id_t ent) SKR_NOEXCEPT
{
    dual_storage_t* storage = gamert_get_ecs_world();
    dual_chunk_view_t cv = {0};
    dualS_access(storage, ent, &cv);
    dualS_destroy(storage, &cv);
    return true;
}

const char8_t* entry = "material";
void __gfx_material_construct_callback(void* u, dual_chunk_view_t* view)
{
    const gfx_material_t* mat = u;
    gfx_material_t* new_mat = (gfx_material_t*)dualV_get_owned_rw(view, gfx_material_type);
    *new_mat = *mat;
    CGPURootSignatureId* pRS = dualV_get_owned_rw(view, gfx_root_sig_type);
    gfx_shader_set_t* set = ecsr_query_gfx_shader_set(mat->m_gfx);
    uint32_t sindex = 0;
    CGPUPipelineShaderDescriptor gshaders[5];
    if(set->vs)
    {
        gshaders[sindex].entry = entry;
        gshaders[sindex].stage = CGPU_SHADER_STAGE_VERT;
        gshaders[sindex++].library = set->vs;
    } 
    if(set->hs)
    {
        gshaders[sindex].entry = entry;
        gshaders[sindex].stage = CGPU_SHADER_STAGE_HULL;
        gshaders[sindex++].library = set->hs;
    } 
    if(set->ds)
    {
        gshaders[sindex].entry = entry;
        gshaders[sindex].stage = CGPU_SHADER_STAGE_DOMAIN;
        gshaders[sindex++].library = set->ds;
    }
    if(set->gs)
    {
        gshaders[sindex].entry = entry;
        gshaders[sindex].stage = CGPU_SHADER_STAGE_GEOM;
        gshaders[sindex++].library = set->gs;
    }
    if(set->ps)
    {
        gshaders[sindex].entry = entry;
        gshaders[sindex].stage = CGPU_SHADER_STAGE_FRAG;
        gshaders[sindex++].library = set->ps;
    }
    CGPURootSignatureDescriptor rs_desc = {
        .shader_count = sindex,
        .shaders = &gshaders[0],
        .pool = NULL,
        .push_constant_names = mat->push_constant_names,
        .push_constant_count = mat->push_constant_count,
        .static_sampler_count = mat->static_sampler_count,
        .static_samplers = mat->static_samplers,
        .static_sampler_names = mat->static_sampler_names
    };
    *pRS = cgpu_create_root_signature(mat->device, &rs_desc);
}
gfx_material_id_t ecsr_register_gfx_material(const gfx_material_t* mat) SKR_NOEXCEPT
{
    dual_storage_t* storage = gamert_get_ecs_world();
    dual_type_index_t types[] = { gfx_material_type, gfx_root_sig_type };
    dual_entity_type_t alloc_type = {
        .type.data = types,
        .type.length = 2,
        //.meta.data = &mat->m_gfx,
        //.meta.length = 1
    };
    dual_type_set_t key_set = {
        .data = &gfx_material_type,
        .length = 1
    };
    gfx_material_id_t material = dualX_hashset_insert(storage, &key_set, &alloc_type, &__gfx_material_construct_callback, (void*)mat);    
    // initialize all parameter types
    ecsr_query_material_parameter_type(material, NULL);
    return material;
}

gfx_material_t* ecsr_query_gfx_material(gfx_material_id_t id) SKR_NOEXCEPT
{
    dual_storage_t* storage = gamert_get_ecs_world();
    dual_chunk_view_t view = {0};
    dualS_access(storage, id, &view);
    return (gfx_material_t*)dualV_get_owned_rw(&view, gfx_material_type);
}

bool ecsr_unregister_gfx_material(gfx_material_id_t ent) SKR_NOEXCEPT
{
    dual_storage_t* storage = gamert_get_ecs_world();
    dual_chunk_view_t cv = {0};
    dualS_access(storage, ent, &cv);
    CGPURootSignatureId* pRS = dualV_get_owned_rw(&cv, gfx_root_sig_type);
    if(pRS) cgpu_free_root_signature(*pRS);
    dualS_destroy(storage, &cv);
    return true;
}

// TODO: REFACTOR THIS
const ECGPUFormat gbuffer_formats[] = {
    CGPU_FORMAT_R8G8B8A8_UNORM, CGPU_FORMAT_R16G16B16A16_SNORM
};
const ECGPUFormat gbuffer_depth_format = CGPU_FORMAT_D32_SFLOAT;
CGPURenderPipelineId create_gbuffer_render_pipeline(
    CGPUDeviceId device, CGPURootSignatureId root_sig,
    const CGPUPipelineShaderDescriptor* vs,
    const CGPUPipelineShaderDescriptor* ps)
{
    CGPUVertexLayout vertex_layout = {
        .attributes = {
            { "POSITION", 1, CGPU_FORMAT_R32G32B32_SFLOAT, 0, 0, sizeof(skr_float3_t), CGPU_INPUT_RATE_VERTEX },
            { "TEXCOORD", 1, CGPU_FORMAT_R32G32_SFLOAT, 1, 0, sizeof(skr_float2_t), CGPU_INPUT_RATE_VERTEX },
            { "NORMAL", 1, CGPU_FORMAT_R8G8B8A8_SNORM, 2, 0, sizeof(uint32_t), CGPU_INPUT_RATE_VERTEX },
            { "TANGENT", 1, CGPU_FORMAT_R8G8B8A8_SNORM, 3, 0, sizeof(uint32_t), CGPU_INPUT_RATE_VERTEX },
            { "MODEL", 4, CGPU_FORMAT_R32G32B32A32_SFLOAT, 4, 0, sizeof(skr_float4x4_t), CGPU_INPUT_RATE_INSTANCE }
        },
        .attribute_count = 5
    };
    CGPURenderPipelineDescriptor rp_desc = {
        .root_signature = root_sig,
        .prim_topology = CGPU_PRIM_TOPO_TRI_LIST,
        .vertex_layout = &vertex_layout,
        .vertex_shader = vs,
        .fragment_shader = ps,
        .render_target_count = sizeof(gbuffer_formats) / sizeof(ECGPUFormat),
        .color_formats = gbuffer_formats,
        .depth_stencil_format = gbuffer_depth_format,
    };
    CGPURasterizerStateDescriptor raster_desc = {0};
    raster_desc.cull_mode = CGPU_CULL_MODE_BACK;
    raster_desc.depth_bias = 0;
    raster_desc.fill_mode = CGPU_FILL_MODE_SOLID;
    raster_desc.front_face = CGPU_FRONT_FACE_CCW;
    rp_desc.rasterizer_state = &raster_desc;
    CGPUDepthStateDescriptor ds_desc = {0};
    ds_desc.depth_func = CGPU_CMP_LEQUAL;
    ds_desc.depth_write = true;
    ds_desc.depth_test = true;
    rp_desc.depth_state = &ds_desc;
    return cgpu_create_render_pipeline(device, &rp_desc);
}
// END TODO: REFACTOR THIS

/*
void ecsr_draw_with_gfx_mat_inst(dual_entity_type_t type, gfx_material_inst_id_t mat_id) SKR_NOEXCEPT
{
    dual_storage_t* storage = gamert_get_ecs_world();
    dual_chunk_view_t view = {0};
    dualS_access(storage, mat_id, &view);
    CGPURootSignatureId* pRS = (CGPURootSignatureId*)dualV_get_owned_rw(&view, gfx_root_sig_type);
    if(!pRS) // create a new one and attaches it
    {

    }
    // fetch vertex buffer & index buffer
}
*/

void ecsr_register_types()
{
    // program objects
    {
        dual_type_description_t desc = {
            .name = "gfx_shader_set",
            .size = sizeof(gfx_shader_set_t),
            .guid = gfx_shader_set_guid,
            .alignment = _Alignof(gfx_shader_set_t)
        };
        gfx_shader_set_type = dualT_register_type(&desc);
    }
    {
        dual_type_description_t desc = {
            .name = "processor_shader_set",
            .size = sizeof(processor_shader_set_t),
            .guid = processor_shader_set_guid,
            .alignment = _Alignof(processor_shader_set_t)
        };
        processor_shader_set_type = dualT_register_type(&desc);
    }
    {
        dual_type_description_t desc = {
            .name = "gfx_material",
            .size = sizeof(gfx_material_t),
            .guid = gfx_material_guid,
            .alignment = _Alignof(gfx_material_t)
        };
        gfx_material_type = dualT_register_type(&desc);
    }
    {
        dual_type_description_t desc = {
            .name = "gfx_root_signature",
            .size = sizeof(CGPURootSignatureId),
            .guid = {0xc3d2cf26, 0x5a7e, 0x4bcc, {0xb5, 0x1e, 0xae, 0xc5, 0x1c, 0xd0, 0x4c, 0x49}},
            .alignment = _Alignof(CGPURootSignatureId)
        };
        gfx_root_sig_type = dualT_register_type(&desc);
    }
    {
        dual_type_description_t desc = {
            .name = "processor_material",
            .size = sizeof(processor_material_t),
            .guid = cmpt_material_guid,
            .alignment = _Alignof(processor_material_t)
        };
        processor_material_type = dualT_register_type(&desc);
    }
    // objects on scene prims
    {
        dual_type_description_t desc = {
            .name = "transform",
            .size = sizeof(transform_t),
            .guid = transform_guid,
            .alignment = _Alignof(transform_t)
        };
        transform_type = dualT_register_type(&desc);
    }
    {
        dual_type_description_t desc = {
            .name = "index_buffer",
            .size = sizeof(CGPUBufferId),
            .guid = {0x925a3900, 0xed0d, 0x4aa9, {0x8b, 0xbb, 0x05, 0x0a, 0xca, 0x80, 0xfb, 0x4a}},
            .alignment = _Alignof(CGPUBufferId)
        };
        index_buffer_type = dualT_register_type(&desc);
    }
    {
        dual_type_description_t desc = {
            .name = "vertex_buffer",
            .size = 5 * sizeof(CGPUBufferId),
            .guid = {0xcd550555, 0x71ec, 0x4df9, {0x90, 0xc6, 0x60, 0x0a, 0x35, 0x1c, 0xe3, 0x83}},
            .alignment = _Alignof(CGPUBufferId),
            .elementSize = 5
        };
        vertex_buffer_type = dualT_register_type(&desc);
    }
}