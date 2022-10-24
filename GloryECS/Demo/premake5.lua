project "Demo"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("%{exe_outdir}")
	objdir ("%{cfg.buildcfg}/%{cfg.platform}")

	pchheader "pch.h"
	pchsource "pch.cpp"

	files
	{
		"**.h",
		"**.cpp"
	}

	vpaths
	{
		
	}

	includedirs
	{
		"../GloryECSStatic",
		"../GloryReflectStatic",
	}

	libdirs
	{
		"%{lib_outdir}",
	}

	links
	{
		"GloryECSStatic",
		"GloryReflectStatic",
	}

	filter "system:windows"
		systemversion "10.0.19041.0"
		toolset "v142"

		defines
		{
			"_CONSOLE"
		}

	filter "platforms:Win32"
		architecture "x86"
		defines "WIN32"

	filter "platforms:x64"
		architecture "x64"

	filter "configurations:Debug"
		kind "ConsoleApp"
		runtime "Debug"
		defines "_DEBUG"
		symbols "On"

	filter "configurations:Release"
		kind "ConsoleApp"
		runtime "Release"
		defines "NDEBUG"
		optimize "On"
