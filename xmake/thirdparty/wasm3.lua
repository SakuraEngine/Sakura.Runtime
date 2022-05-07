wasm3_includes_dir = "$(projectdir)/thirdparty/wasm3"
table.insert(include_dir_list, wasm3_includes_dir)
table.insert(links_list, "m3")
table.insert(links_list, "uv_a")
table.insert(links_list, "uvwasi_a")

task("unzip-wasm3")
on_run(function ()
    import("find_sdk")
    find_sdk.install_lib("m3")
end)