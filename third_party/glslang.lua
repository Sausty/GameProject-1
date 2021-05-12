return function()
	warnings 'Off'

    filter 'system:windows'
        defines
        {
            "ENABLE_HLSL",
            "GLSLANG_OSINCLUDE_WIN32"
        }

    filter 'system:linux'
        defines
        {
            "ENABLE_HLSL",
            "GLSLANG_OSINCLUDE_UNIX"
        }

	includedirs {
		'third_party/vulkan-headers/include',
        'third_party/glslang/spirv',
        'third_party/glslang/hlsl',
        'third_party/glslang',
        'third_party/spirv-tools/include',
        'third_party/spirv-tools/spirv',
        'third_party/spirv-headers/include'
	}

    links {
        'volk'
    }

	files {
		'third_party/glslang/glslang/**.cpp',
        'third_party/glslang/glslang/**.h',
        'third_party/glslang/hlsl/**.cpp',
        'third_party/glslang/spirv/**.h',
        'third_party/glslang/spirv/**.cpp',
	}
end
