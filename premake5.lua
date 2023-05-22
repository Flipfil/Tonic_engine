workspace "2NK"
    startproject "2NK_editor"
    architecture "x64"

    configurations
    {
        "debug",
        "release"
    }

tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"

-- External dependencies
externals = {}
externals["SDL2"] = "external/SDL2"
externals["spdlog"] = "external/spdlog"

project "2NK_engine"
    location "2NK_enigne"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    externalincludedirs
    {
        "%{prj.name}/include/2NK_engine",
        "%{externals.SDL2}/include",
        "%{externals.spdlog}/include"
    }

    flags
    {
        "FatalWarnings"
    }

    filter {"system:windows", "configurations:*"}
        systemversion "latest"

        defines
        {
            "_2NK_PLATFORM_WINDOWS"
        }

    filter {"system:macosx", "configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "_2NK_PLATFORM_MAC"
        }

    filter {"system:linux", "configurations:*"}
        defines
        {
            "_2NK_PLATFORM_LINUX"
        }

    filter {"configurations:debug"}
        defines
        {
            "_2NK_CONFIG_DEBUG"
        }
        runtime "debug"
        symbols "on"
    
    filter {"configurations:release"}
        defines
        {
            "_2NK_CONFIG_RELEASE"
        }
        runtime "release"
        symbols "off"
        optimize "on"
    
project "2NK_editor"
    location "2NK_editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    links "2NK_engine"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    externalincludedirs
    {
        "2NK_engine/include"
    }

    flags
    {
        "FatalWarnings"
    }

    filter {"system:windows", "configurations:*"}
        systemversion "latest"

        defines
        {
            "_2NK_PLATFORM_WINDOWS"
        }

    filter {"system:macosx", "configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "_2NK_PLATFORM_MAC"
        }

        -- TODO: integrate MacOS SDL2.framework 

    filter {"system:linux", "configurations:*"}
        defines
        {
            "_2NK_PLATFORM_LINUX"
        }

    filter {"configurations:debug"}
        defines
        {
            "_2NK_CONFIG_DEBUG"
        }
        runtime "debug"
        symbols "on"
    
    filter {"configurations:release"}
        defines
        {
            "_2NK_CONFIG_RELEASE"
        }
        runtime "release"
        symbols "off"
        optimize "on"