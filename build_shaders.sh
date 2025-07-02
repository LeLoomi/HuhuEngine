cd shaders
VULKAN_SDK_PATH="/Users/eliah/VulkanSDK/1.4.313.1/macOS"
for file in *.frag; do echo "> ${file}"; ${VULKAN_SDK_PATH}/bin/glslc ${file} -o ${file}.spv; done
for file in *.vert; do echo "> ${file}"; ${VULKAN_SDK_PATH}/bin/glslc ${file} -o ${file}.spv; done
