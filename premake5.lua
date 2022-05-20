workspace "Alpine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Export"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Alpine"
    location "Source/Alpine"
    kind "Staticlib"
    language "C++"
    cppdialect "c++20"

    targetdir ("Dependencies/lib/")
    objdir ("Intermediates/%{cfg.architecture}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hlsl",
        "%{prj.name}/src/**.hlsli",
    }

    includedirs
    {
        "Dependencies/include/"
        "Source/ToolBox/src/",
        "Dependencies/include/imgui/",
        "Source/Alpine/src/"
    }

    links
    {
        "Dependencies/lib/"
    }
