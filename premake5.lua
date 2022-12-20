project "AudioToWav"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"
	staticruntime "off"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/**.h",
		"src/**.cpp",
        "src/**.h"
	}

    includedirs
    {
        "include",
        "vendor/AudioFile",
		"vendor/minimp3",
		"vendor/stb"
    }

    filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"