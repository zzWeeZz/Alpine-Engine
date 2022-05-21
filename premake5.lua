workspace "Alpine"
    architecture "x64"
    startproject "Launcher"

    configurations
    {
        "Debug",
        "Release",
        "Export"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Launcher"
    location "Source/Launcher"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    debugdir "Export/"

    targetdir ("Export/")
    objdir ("Intermediates/%{cfg.architecture}")

    files
    {
        "Source/Launcher/src/**.h",
        "Source/Launcher/src/**.cpp",
    }

    includedirs
    {
        "Dependencies/include/",
        "Dependencies/include/imgui/",

        "Source/ToolBox/src/",
        "Source/Alpine/src/",
        "Source/Glacier/src/",
    }

    libdirs
    {
        "Dependencies/lib/",
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib",
        "d3d11.lib",
        "d3dcompiler.lib",
        "DirectXTK.lib",
        "assimp-vc142-mt.lib",

        "ToolBox",
        "ImGui",
        "Glacier",
    }

    defines {"_CONSOLE"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"


	filter "configurations:Export"
		defines "EXPORT"
		runtime "Release"
		optimize "on"

	systemversion "latest"
	
	filter "system:windows"
		symbols "On"		
		systemversion "latest"

		flags 
        { 
			"MultiProcessorCompile"
		}
		defines 
        {
			"WIN32",
			"_LIB",
            "NOMINMAX",
		}






project "Alpine"
    location "Source/Alpine"
    kind "Staticlib"
    language "C++"
    cppdialect "C++20"

    targetdir ("Dependencies/lib/")
    objdir ("Intermediates/%{cfg.architecture}")

    files
    {
        "Source/Alpine/src/**.h",
        "Source/Alpine/src/**.cpp",
        "Source/Alpine/src/**.hpp",
        "Source/Alpine/src/**.hlsl",
        "Source/Alpine/src/**.hlsli",
    }

    includedirs
    {
        "Dependencies/include/",
        "Source/ToolBox/src/",
        "Dependencies/include/imgui/",
        "Source/Alpine/src/"
    }

    links
    {
        "ToolBox",
        "ImGui",
    }

    defines {"_CONSOLE"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"


	filter "configurations:Export"
		defines "EXPORT"
		runtime "Release"
		optimize "on"

	systemversion "latest"
	
	filter "system:windows"
		symbols "On"		
		systemversion "latest"

		flags 
        { 
			"MultiProcessorCompile"
		}
		defines 
        {
			"WIN32",
			"_LIB",
            "NOMINMAX",
		}

    local shader_dir = "../../Export/Shaders/"
    os.mkdir(shader_dir)

    filter("files:Source/Alpine/src/**.hlsl")
        flags("ExcludeFromBuild")
        shaderobjectfileoutput(shader_dir.."%{file.basename}"..".cso")
        shadermodel "5.0"

    filter("files:Source/Alpine/src/**_ps.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Pixel")
        shadermodel "5.0"

    filter("files:Source/Alpine/src/**_vs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Vertex")
        shadermodel "5.0"

    filter("files:Source/Alpine/src/**_gs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Geometry")
        shadermodel "5.0"

    filter("files:Source/Alpine/src/**_cs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Compute")
        shadermodel "5.0"




project "Glacier"
    location "Source/Glacier"
    kind "Staticlib"
    language "C++"
    cppdialect "C++20"

    targetdir ("Dependencies/lib/")
    objdir ("Intermediates/%{cfg.architecture}")

    files
    {
        "Source/Glacier/src/**.h",
        "Source/Glacier/src/**.cpp",
    }

    includedirs
    {
        "Dependencies/include/",
        "Dependencies/include/imgui/",
        "Source/Alpine/src/"
    }

    links
    {
        "ToolBox",
        "Alpine",
        "imGui",
    }

    defines {"_CONSOLE"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"


	filter "configurations:Export"
		defines "EXPORT"
		runtime "Release"
		optimize "on"

	systemversion "latest"
	
	filter "system:windows"
		symbols "On"		
		systemversion "latest"

		flags 
        { 
			"MultiProcessorCompile"
		}
		defines 
        {
			"WIN32",
			"_LIB",
            "NOMINMAX",
		}






project "ToolBox"
    location "Source/ToolBox"
    kind "Staticlib"
    language "C++"
    cppdialect "C++20"

    targetdir ("Dependencies/lib/")
    objdir ("Intermediates/%{cfg.architecture}")

    files
    {
        "Source/ToolBox/src/**.h",
        "Source/ToolBox/src/**.cpp",
        "Source/ToolBox/src/**.hpp",
    }

    includedirs
    {
        "Dependencies/include/",
    }

    defines {"_CONSOLE"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"


	filter "configurations:Export"
		defines "EXPORT"
		runtime "Release"
		optimize "on"

	systemversion "latest"
	
	filter "system:windows"
		symbols "On"		
		systemversion "latest"

		flags 
        { 
			"MultiProcessorCompile"
		}
		defines 
        {
			"WIN32",
			"_LIB",
            "NOMINMAX",
		}






project "ImGui"
    location "Source/Imgui"
    kind "Staticlib"
    language "C++"
    cppdialect "C++20"

    targetdir ("Dependencies/lib/")
    objdir ("Intermediates/%{cfg.architecture}")

    files
    {
        "Source/Imgui/src/**.cpp",
        "Source/Imgui/src/**.hpp",
    }

    includedirs
    {
        "Dependencies/include/",
        "Dependencies/include/imgui/",
        "Dependencies/include/imgui/misc/cpp/",
        "Dependencies/include/imgui/misc/freetype",
        "Dependencies/include/imgui/misc/fonts",
    }

    defines {"_CONSOLE"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"


	filter "configurations:Export"
		defines "EXPORT"
		runtime "Release"
		optimize "on"

	systemversion "latest"
	
	filter "system:windows"
		symbols "On"		
		systemversion "latest"

		flags 
        { 
			"MultiProcessorCompile"
		}
		defines 
        {
			"WIN32",
			"_LIB",
            "NOMINMAX",
		}