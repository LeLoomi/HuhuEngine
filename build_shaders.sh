cd shaders
for file in *.frag; do echo "> ${file}"; ${VULKAN_SDK_PATH}/bin/glslc ${file} -o ${file}.spv; done
for file in *.vert; do echo "> ${file}"; ${VULKAN_SDK_PATH}/bin/glslc ${file} -o ${file}.spv; done
