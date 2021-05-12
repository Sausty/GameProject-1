
libraries = { }

function library( name )
	group 'Libraries'
	project( name )

	kind 'StaticLib'
	links( libraries )
	location 'build/%{_ACTION}'

	sysincludedirs {
		'third_party/glfw/include',
		'third_party/glad/include',
		'third_party/glm/glm',
		'third_party/openal-soft/include',
		'third_party/openal-soft/src',
		'third_party/openal-soft/src/common',
		'third_party/dr_libs',
		'third_party/minimp3_s',
		'third_party/volk',
		'third_party/vulkan-headers/include',
		'third_party/spirv-headers/include',
		'third_party/spirv-tools/include',
		'third_party/glslang/glslang',
		'third_party/shaderc/libshaderc/include',
	}

	files {
		'include/%{prj.name}/**.h',
		'src/%{prj.name}/**.cpp',
		'src/%{prj.name}/**.h',
	}

	defines
	{
		"AL_LIBTYPE_STATIC"
	}

	filter 'system:macosx or ios'
		files { 'src/%{prj.name}/**.mm' }

	filter { 'system:macosx or ios', 'files:**.cpp' }
		compileas 'Objective-C++'

	filter { }

	table.insert( libraries, name )
end
