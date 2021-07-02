-- premake5.lua
-- Cndr Project

workspace "ChotuEditor"
	architecture "x64"

	configurations{
		"Debug",
		"Release"
	}

	startproject "ChotuEditor"

	flags{
		"MultiProcessorCompile",
		"FloatFast"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ChotuEditor"
	
	location "ChotuEditor"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
	
	filter "configurations:Debug"
		defines "CR_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "CR_RELEASE"
		optimize "On"