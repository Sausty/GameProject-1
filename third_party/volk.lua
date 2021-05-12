return function()
	warnings 'Off'

	includedirs {
		'third_party/vulkan-headers/include',
		'third_party/volk'
	}

	files {
		'third_party/volk/volk.h',
		'third_party/volk/volk.c'
	}

    defines
    {
        "VK_NO_PROTOTYPES"
    }

    filter "system:windows"
		defines
		{
			"VK_USE_PLATFORM_WIN32_KHR"
		}

	filter "system:linux"
		defines
		{
			"VK_USE_PLATFORM_XCB_KHR"
		}
		buildoptions
    	{
    	  "-fPIC"
		}

	filter "system:macosx"
		defines
		{
			"VK_USE_PLATFORM_METAL_EXT",
			"VK_EXT_metal_surface"
		}

	filter "system:ios"
		defines
		{
			"VK_USE_PLATFORM_IOS_MVK"
		}

	filter "configurations:Debug"
	    symbols "On"

	filter "configurations:Release"
	    optimize "On"

	filter "configurations:Production"
	    optimize "On"
end
