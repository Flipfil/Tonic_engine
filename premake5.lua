workspace "2NK"
    startproject "2NK_editor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

project "2NK_editor"
    location "2NK_editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    flags
    {
        "FatalWarnings"
    }