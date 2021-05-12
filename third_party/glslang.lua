return function()
	warnings 'Off'

    defines
    {
        "ENABLE_OPT"
    }

	includedirs {
		'third_party/vulkan-headers/include',
        'third_party/glslang/spirv',
        'third_party/glslang/hlsl',
        'third_party/glslang',
        'third_party/spirv-tools/include',
        'third_party/spirv-tools/spirv',
        'third_party/spirv-headers/include',
        'third_party/glslang/OGLCompilersDLL'
	}

    links {
        'volk',
        'spirv-tools'
    }

	files {
        'third_party/glslang/glslang/Include/**.h',
        'third_party/glslang/glslang/Public/**.h',
		'third_party/glslang/glslang/MachineIndependent/**.cpp',
        'third_party/glslang/glslang/MachineIndependent/**.h',
        'third_party/glslang/glslang/GenericCodeGen/**.cpp',
        'third_party/glslang/hlsl/**.cpp',
        'third_party/glslang/spirv/**.h',
        'third_party/glslang/spirv/**.cpp',
        'third_party/glslang/OGLCompilersDLL/**.h',
        'third_party/glslang/OGLCompilersDLL/**.cpp',
        'third_party/glslang/StandAlone/**.h',
        'third_party/glslang/StandAlone/**.cpp'
	}

    filter 'system:windows'
        defines
        {
            "ENABLE_HLSL",
            "GLSLANG_OSINCLUDE_WIN32"
        }

        files
        {
            'third_party/glslang/glslang/OSDependent/**.h',
            'third_party/glslang/glslang/OSDependent/Windows/**.cpp',
        }

    filter 'system:linux'
        defines
        {
            "ENABLE_HLSL",
            "GLSLANG_OSINCLUDE_UNIX"
        }

        files
        {
            'third_party/glslang/glslang/OSDependent/**.h',
            'third_party/glslang/glslang/OSDependent/Unix/**.cpp',
        }
end
