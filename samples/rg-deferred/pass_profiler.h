#include "render_graph/backend/graph_backend.hpp"

class PassProfiler : public sakura::render_graph::RenderGraphProfiler
{
public:
    void initialize(CGpuDeviceId device)
    {
        CGpuQueryPoolDescriptor desc = {};
        desc.query_count = 512;
        desc.type = QUERY_TYPE_TIMESTAMP;
        query_pool = cgpu_create_query_pool(device, &desc);
        CGpuBufferDescriptor buf_desc = {};
        buf_desc.name = "RenderGraphQueryBuffer";
        buf_desc.flags = BCF_PERSISTENT_MAP_BIT;
        buf_desc.memory_usage = MEM_USAGE_GPU_TO_CPU;
        buf_desc.size = sizeof(uint64_t) * 512;
        query_buffer = cgpu_create_buffer(device, &buf_desc);
    }
    void finalize()
    {
        if (query_pool) cgpu_free_query_pool(query_pool);
        if (query_buffer) cgpu_free_buffer(query_buffer);
        query_pool = nullptr;
        query_buffer = nullptr;
    }
    gsl::span<uint64_t> readback_query_data()
    {
        return gsl::span<uint64_t>((uint64_t*)query_buffer->cpu_mapped_address, query_cursor);
    }
    virtual void on_acquire_executor(class sakura::render_graph::RenderGraph& g, class sakura::render_graph::RenderGraphFrameExecutor&)
    {
        auto timestamps = readback_query_data();
        times_ms.resize(timestamps.size());
        auto detail = cgpu_query_adapter_detail(query_pool->device->adapter);
        auto ns_period = detail->timestamp_period;
        for (uint32_t i = 1; i < times_ms.size(); i++)
        {
            times_ms[i] = ((timestamps[i] - timestamps[i - 1]) * ns_period) * 1e-6;
        }
        frame_index = g.get_frame_index() - RG_MAX_FRAME_IN_FLIGHT;
    }
    virtual void on_cmd_begin(class sakura::render_graph::RenderGraph& g, class sakura::render_graph::RenderGraphFrameExecutor& executor)
    {
        query_cursor = 0;
        query_names.clear();
        cgpu_cmd_reset_query_pool(executor.gfx_cmd_buf, query_pool, 0, 512);
        CGpuQueryDescriptor query_desc = {};
        query_desc.index = query_cursor++;
        query_desc.stage = SHADER_STAGE_NONE;
        query_names.emplace_back("cmd_begin");
        cgpu_cmd_begin_query(executor.gfx_cmd_buf, query_pool, &query_desc);
    }
    virtual void on_cmd_end(class sakura::render_graph::RenderGraph&, class sakura::render_graph::RenderGraphFrameExecutor& executor)
    {
        cgpu_cmd_resolve_query(executor.gfx_cmd_buf, query_pool,
            query_buffer, 0, query_cursor);
    }
    virtual void on_pass_begin(class sakura::render_graph::RenderGraph&, class sakura::render_graph::RenderGraphFrameExecutor&, class sakura::render_graph::PassNode& pass)
    {
    }
    virtual void on_pass_end(class sakura::render_graph::RenderGraph&, class sakura::render_graph::RenderGraphFrameExecutor& executor, class sakura::render_graph::PassNode& pass)
    {
        CGpuQueryDescriptor query_desc = {};
        query_desc.index = query_cursor++;
        query_desc.stage = SHADER_STAGE_ALL_GRAPHICS;
        query_names.emplace_back(pass.get_name());
        cgpu_cmd_begin_query(executor.gfx_cmd_buf, query_pool, &query_desc);
    }
    virtual void before_commit(class sakura::render_graph::RenderGraph&, class sakura::render_graph::RenderGraphFrameExecutor&) {}
    virtual void after_commit(class sakura::render_graph::RenderGraph&, class sakura::render_graph::RenderGraphFrameExecutor&) {}

    CGpuQueryPoolId query_pool = nullptr;
    CGpuBufferId query_buffer = nullptr;
    uint32_t query_cursor = 0;
    eastl::vector<float> times_ms;
    eastl::vector<eastl::string> query_names;
    uint32_t frame_index;
};