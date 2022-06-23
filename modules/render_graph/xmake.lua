target("SkrRenderGraph")
    set_kind("shared")
    add_deps("SkrRT")
    add_includedirs("include", {public=true})
    add_defines("SKR_RENDER_GRAPH_SHARED", {public=true})
    add_defines("SKR_RENDER_GRAPH_IMPL")
    add_files("src/*.cpp")