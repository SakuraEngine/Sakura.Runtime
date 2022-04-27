task("unzip-gfx-sdk")
    on_run(function ()
        import("utils.archive")
        if(os.host() == "windows") then
            zipped_ags = "amd_ags.zip"
            zipped_nvapi = "nvapi.zip"
            zipped_pix3 = "WinPixEventRuntime.zip"
            ags_sdkdir = path.join(os.projectdir(), "SDKs/"..zipped_ags)
            nvapi_sdkdir = path.join(os.projectdir(), "SDKs/"..zipped_nvapi)
            pix3_sdkdir = path.join(os.projectdir(), "SDKs/"..zipped_pix3)
            local outputdir = path.join(os.projectdir(), "build/"..os.host().."/"..os.arch().."/"..vformat("$(mode)"))
            archive.extract(ags_sdkdir, outputdir)
            archive.extract(nvapi_sdkdir, outputdir)
            archive.extract(pix3_sdkdir, outputdir)

            sdl2_sdkdir = path.join(os.projectdir(), "SDKs/SDL2-win.zip")
            archive.extract(sdl2_sdkdir, outputdir)
        end
        -- dxc
        if(os.host() == "windows") then
            zipped_dxc = "dxc-win.zip"
            dxcdir = path.join(os.projectdir(), "SDKs/"..zipped_dxc)
        else 
            zipped_dxc = "dxc-macos-amd64.zip"
            dxcdir = path.join(os.projectdir(), "SDKs/"..zipped_dxc)
        end
        local outputdir = path.join(os.projectdir(), vformat("$(buildir)/$(os)/$(arch)/$(mode)"))
        print("unzip dxc "..zipped_dxc.." -> "..outputdir)
        archive.extract(dxcdir, outputdir)
end)