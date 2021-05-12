return function()
	warnings 'Off'

	includedirs {
		'third_party/vulkan-headers/include'
	}

    links {
        'volk'
    }

	files {
		'third_party/vma/include/**.h',
		'third_party/vma/src/VmaReplay/**.h',
		'third_party/vma/src/VmaReplay/**.cpp'
	}
end
