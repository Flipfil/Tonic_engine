workspace "2NK"
    startproject "2NK_editor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"

project "2NK_editor"
    location "2NK_editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    flags
    {
        "FatalWarnings"
    }