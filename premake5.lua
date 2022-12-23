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
        "src/**.h",
		"vendor/Vorbis/lib/**.h",
		"vendor/Vorbis/lib/**.c",
		"vendor/libogg/include/**.h",
		"vendor/libogg/src/**.c"
	}

    includedirs
    {
        "include",
        "vendor/AudioFile",
		"vendor/minimp3",
		"vendor/stb",
		"vendor/Vorbis/include",
		"vendor/libogg/include"
    }

	defines
	{
		"LIBOGG_EXPORTS"
	}

    filter "system:windows"
		systemversion "latest"

		excludes
		{
			"vendor/Vorbis/lib/barkmel.c",
			"vendor/Vorbis/lib/misc.c",
			"vendor/Vorbis/lib/psytune.c",
			"vendor/Vorbis/lib/tone.c"
		}

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