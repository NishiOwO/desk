function common()
	filter("platforms:Native")
		includedirs("/usr/X11R7/include")
		includedirs("/usr/X11R6/include")
		includedirs("/usr/pkg/include")

		libdirs("/usr/X11R7/lib")
		libdirs("/usr/X11R6/lib")
		libdirs("/usr/pkg/lib")
	filter({})

	filter("configurations:Debug")
		defines({
			"DEBUG"
		})
		symbols("On")
	filter("configurations:Release")
		defines({
			"NDEBUG"
		})
		optimize("On")
	filter({})

	defines("_DEFAULT_SOURCE")
end

workspace("bb")
        configurations({
                "Debug",
                "Release"
        })
        platforms({
                "Native",
                "Win32",
                "Win64"
        })
        defaultplatform("Native")

filter("platforms:Win32")
        system("windows")
        architecture("x86")
        gccprefix("i686-w64-mingw32-")

filter("platforms:Win64")
        system("windows")
        architecture("x86_64")
        gccprefix("x86_64-w64-mingw32-")

filter({})

project("desk")
	kind("StaticLib")
	targetdir("lib/%{cfg.buildcfg}/%{cfg.platform}")
	objdir("obj")
	defines("DESK_INTERNAL")
	files("src/*.c")
	files("external/*.c")
	includedirs("include")
	includedirs("external")
	common()
	filter({})
