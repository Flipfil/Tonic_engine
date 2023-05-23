workspace "TNK"
    startproject "TNK_editor"
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
externals["SDL2"]   = "external/SDL2"
externals["spdlog"] = "external/spdlog"
externals["GLAD"]   = "external/GLAD"

-- Process GLAD before anything else
include "external/GLAD"

project "TNK_engine"
    location "TNK_engine"
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
        "%{prj.name}/include/TNK_engine",
        "%{externals.SDL2}/include",
        "%{externals.spdlog}/include",
        "%{externals.GLAD}/include"
    }

    flags
    {
        "FatalWarnings"
    }

    defines
    {
        "GLFW_INCLUDE_NONE" -- Ensures glad does not include glfw
    }

    filter {"system:windows", "configurations:*"}
        systemversion "latest"

        defines
        {
            "TNK_PLATFORM_WINDOWS"
        }

    filter {"system:macosx", "configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "TNK_PLATFORM_MAC"
        }

    filter {"system:linux", "configurations:*"}
        defines
        {
            "TNK_PLATFORM_LINUX"
        }

    filter {"configurations:debug"}
        defines
        {
            "TNK_CONFIG_DEBUG"
        }
        runtime "debug"
        symbols "on"
    
    filter {"configurations:release"}
        defines
        {
            "TNK_CONFIG_RELEASE"
        }
        runtime "release"
        symbols "off"
        optimize "on"
    
project "TNK_editor"
    location "TNK_editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    links "TNK_engine"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    externalincludedirs
    {
        "TNK_engine/include"
    }

    flags
    {
        "FatalWarnings"
    }

    filter {"system:windows", "configurations:*"}
        systemversion "latest"

        defines
        {
            "TNK_PLATFORM_WINDOWS"
        }

        libdirs
        {
            "%{externals.SDL2}/lib"
        }

        links
        {
            "SDL2",
            "GLAD"
        }

    filter {"system:macosx", "configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "TNK_PLATFORM_MAC"
        }

        -- TODO: integrate MacOS SDL2.framework
        -- vvvvvvvvv

        -- abspath = path.getabsolute("%{externals.maclibs}")
        -- linkoptions {"-F " .. abspath}

        --links
        --{
        --    "SDL2.framework",
        --    "GLAD"
        --}

        -- ^^^^^^^^^


    filter {"system:linux", "configurations:*"}
        defines
        {
            "TNK_PLATFORM_LINUX"
        }

        links
        {
            "SDL2",
            "GLAD"
        }

    filter {"configurations:debug"}
        defines
        {
            "TNK_CONFIG_DEBUG"
        }
        runtime "debug"
        symbols "on"
    
    filter {"configurations:release"}
        defines
        {
            "TNK_CONFIG_RELEASE"
        }
        runtime "release"
        symbols "off"
        optimize "on"