workspace "TONIC"
    startproject "Tonic_editor"
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

project "Tonic_engine"
    location "Tonic_engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/include/**.hpp",
        "%{prj.name}/include/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/**.natvis"
    }

    externalincludedirs
    {
        "%{prj.name}/include",
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
            "TONIC_PLATFORM_WINDOWS"
        }

    filter {"system:macosx", "configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "TONIC_PLATFORM_MAC"
        }

    filter {"system:linux", "configurations:*"}
        defines
        {
            "TONIC_PLATFORM_LINUX"
        }

    filter {"configurations:debug"}
        defines
        {
            "TONIC_CONFIG_DEBUG"
        }
        runtime "debug"
        symbols "on"
    
    filter {"configurations:release"}
        defines
        {
            "TONIC_CONFIG_RELEASE"
        }
        runtime "release"
        symbols "off"
        optimize "on"
    
project "Pong"
    location "Pong"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    links "Tonic_engine"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    externalincludedirs
    {
        "Tonic_engine/include",
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
            "TONIC_PLATFORM_WINDOWS"
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
            "TONIC_PLATFORM_MAC"
        }

        -- TODO: integrate MacOS SDL2.framework
        -- >>>>>>>>>>>>>>>>>
        -- abspath = path.getabsolute("%{externals.maclibs}")
        -- linkoptions {"-F " .. abspath}

        --links
        --{
        --    "SDL2.framework",
        --    "GLAD"
        --}
        -- <<<<<<<<<<<<<<<<<<


    filter {"system:linux", "configurations:*"}
        defines
        {
            "TONIC_PLATFORM_LINUX"
        }

        links
        {
            "SDL2",
            "GLAD"
        }

    filter {"configurations:debug"}
        defines
        {
            "TONIC_CONFIG_DEBUG"
        }
        runtime "debug"
        symbols "on"
    
    filter {"configurations:release"}
        defines
        {
            "TONIC_CONFIG_RELEASE"
        }
        runtime "release"
        symbols "off"
        optimize "on"

        project "Tonic_editor"
        location "Tonic_editor"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"
        links "Tonic_engine"
    
        targetdir(tdir)
        objdir(odir)
    
        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }
    
        externalincludedirs
        {
            "Tonic_engine/include",
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
                "TONIC_PLATFORM_WINDOWS"
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
                "TONIC_PLATFORM_MAC"
            }
    
            -- TODO: integrate MacOS SDL2.framework
            -- >>>>>>>>>>>>>>>>>
            -- abspath = path.getabsolute("%{externals.maclibs}")
            -- linkoptions {"-F " .. abspath}
    
            --links
            --{
            --    "SDL2.framework",
            --    "GLAD"
            --}
            -- <<<<<<<<<<<<<<<<<<
    
    
        filter {"system:linux", "configurations:*"}
            defines
            {
                "TONIC_PLATFORM_LINUX"
            }
    
            links
            {
                "SDL2",
                "GLAD"
            }
    
        filter {"configurations:debug"}
            defines
            {
                "TONIC_CONFIG_DEBUG"
            }
            runtime "debug"
            symbols "on"
        
        filter {"configurations:release"}
            defines
            {
                "TONIC_CONFIG_RELEASE"
            }
            runtime "release"
            symbols "off"
            optimize "on"