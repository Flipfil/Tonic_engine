project "GLAD"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    targetdir(tdir)
    objdir(odir)

    files
    {
        "include/**.h",
        "src/**.h",
        "src/**.c"
    }

    externalincludedirs
    {
        "include"
    }

    filter {"system:windows"}
        systemversion "latest"

    filter {"system:macosx"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

    filter {"configurations:debug"}
        runtime "debug"
        symbols "on"
    
    filter {"configurations:release"}
        runtime "release"
        symbols "off"
        optimize "on"