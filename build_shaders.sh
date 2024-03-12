cd shaders
for file in *.frag; do echo "> ${file}"; /Users/eliahlohr/VulkanSDK/1.3.275.0/macOS/bin/glslc ${file} -o ${file}.spv; done
for file in *.vert; do echo "> ${file}"; /Users/eliahlohr/VulkanSDK/1.3.275.0/macOS/bin/glslc ${file} -o ${file}.spv; done
