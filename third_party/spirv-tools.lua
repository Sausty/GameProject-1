return function()
	warnings 'Off'

    defines
    {
        "ENABLE_HLSL"
    }

	includedirs {
		'third_party/vulkan-headers/include',
        'third_party/glslang',
        'third_party/spirv-tools/include',
        'third_party/spirv-tools/',
        'third_party/spirv-tools/spirv',
        'third_party/spirv-headers/include',
        'third_party/spirv-headers/include/spirv/unified1',
        'third_party/spirv-tools-generated'
	}

    links {
        'volk'
    }

	files {
        'third_party/spirv-tools/source/**.h',
        'third_party/spirv-tools/source/*.cpp',
		'third_party/spirv-tools/source/opt/**.cpp',
        'third_party/spirv-tools/source/util/**.cpp',
        'third_party/spirv-tools/source/val/**.cpp',
	}

    filter 'system:windows'
        defines
        {
            "SPIRV_WINDOWS"
        }

    filter 'system:linux'
        defines 
        {
            "SPIRV_LINUX"
        }
end
