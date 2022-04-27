rule("utils.dxc")
    set_extensions(".hlsl")
    before_buildcmd_file(function (target, batchcmds, sourcefile_hlsl, opt)
        import("lib.detect.find_tool")
        local dxc = find_tool("dxc")
        assert(dxc, "dxc not found!")

        -- get target profile
        target_profile = sourcefile_hlsl:match("^.+%.(.+)%.")

        -- hlsl to spv
        local targetenv = target:extraconf("rules", "utils.dxc", "targetenv") or "vulkan1.1"
        local spv_outputdir =  path.join(target:autogendir(), "rules", "utils", "dxc-spv")
        local spvfilepath = path.join(spv_outputdir, path.filename(sourcefile_hlsl) .. ".spv")
        batchcmds:show_progress(opt.progress, "${color.build.object}generating.spirv %s", sourcefile_hlsl)
        batchcmds:mkdir(spv_outputdir)
        batchcmds:vrunv(dxc.program, 
            {"-Wno-ignored-attributes -spirv -fspv-target-env=", targetenv, 
            "-Fo ", spvfilepath, 
            "-T ", target_profile,
            sourcefile_hlsl})

        -- hlsl to dxil
        local dxil_outputdir = path.join(target:autogendir(), "rules", "utils", "dxc-dxil")
        local dxilfilepath = path.join(dxil_outputdir, path.filename(sourcefile_hlsl) .. ".dxil")
        batchcmds:show_progress(opt.progress, "${color.build.object}generating.dxil %s", sourcefile_hlsl)
        batchcmds:mkdir(dxil_outputdir)
        batchcmds:vrunv(dxc.program, 
            {"-Wno-ignored-attributes", 
            "-Fo ", dxilfilepath, 
            "-T ", target_profile,
            sourcefile_hlsl})

        -- add deps
        batchcmds:add_depfiles(sourcefile_hlsl)
        batchcmds:set_depmtime(os.mtime(spv_outputdir))
        batchcmds:set_depcache(target:dependfile(spv_outputdir))
        batchcmds:set_depmtime(os.mtime(dxil_outputdir))
        batchcmds:set_depcache(target:dependfile(dxil_outputdir))
    end)
    
    after_build(function(target)
        local spv_path = path.join(target:autogendir(), "rules/utils/dxc-spv")
        local spv_outdir = target:extraconf("rules", "utils.dxc", "spv_outdir")
        if(spv_outdir ~= nil) then
            os.cp(spv_path.."/*.spv", path.join(target:targetdir(), spv_outdir).."/")
        end
        local dxil_path = path.join(target:autogendir(), "rules/utils/dxc-dxil")
        local dxil_outdir = target:extraconf("rules", "utils.dxc", "dxil_outdir")
        if(dxil_outdir ~= nil) then
            os.cp(dxil_path.."/*.dxil", path.join(target:targetdir(), dxil_outdir).."/")
        end
    end)