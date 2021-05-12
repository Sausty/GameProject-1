return function()
	warnings 'Off'

    defines
    {
        "ENABLE_HLSL"
    }

	includedirs {
		'third_party/vulkan-headers/include',
        'third_party/shaderc/libshaderc_util/include',
        'third_party/shaderc/libshaderc/include',
        'third_party/glslang',
        'third_party/spirv-tools/include',
        'third_party/spirv-tools/spirv',
        'third_party/spirv-headers/include'
	}

    links {
        'volk'
    }

	files {
		'third_party/shaderc/libshaderc/**.h',
        'third_party/shaderc/libshaderc/**.hpp',
        'third_party/shaderc/libshaderc/src/shaderc.cc',

		'third_party/shaderc/libshaderc_util/include/**.h',
		'third_party/shaderc/libshaderc_util/src/file_finder.cc',
        'third_party/shaderc/libshaderc_util/src/io_shaderc.cc',
        'third_party/shaderc/libshaderc_util/src/resources.cc',
        'third_party/shaderc/libshaderc_util/src/shader_stage.cc',
        'third_party/shaderc/libshaderc_util/src/version_profile.cc',
        'third_party/shaderc/libshaderc_util/src/message.cc',
        'third_party/shaderc/libshaderc_util/src/compiler.cc',
        'third_party/shaderc/libshaderc_util/src/args.cc',
        'third_party/shaderc/libshaderc_util/src/spirv_tools_wrapper.cc'
	}
end
