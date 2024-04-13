# HuhuEngine

Currently just me following the Vulkan tutorial :P

> Building currently only set up for my M1 Macbook and not tested anywhere else.
> Running build_engine.sh will have CMake build for the Ninja Build system.

## Run-reqs

- CMake <sup>v3.28.0+ req.</sup>
- Ninja
- VulkanSDK <sup>v1.3.275.0+ rec.</sup>
- TinyObjLoader <sup>(included, [this version](https://github.com/tinyobjloader/tinyobjloader/blob/release/tiny_obj_loader.h) req.)</sup>

<br />

Currently you will have to export environment-variables in a way mirroring this one:
````shell
# VULKAN API
export VULKAN_SDK_PATH=/Users/youruser/VulkanSDK/1.3.275.0/macOS
export PATH=${VULKAN_SDK_PATH}/bin:$PATH
export DYLD_LIBRARY_PATH=${VULKAN_SDK_PATH}/lib:$DYLD_LIBRARY_PATH
export VK_ICD_FILENAMES=${VULKAN_SDK_PATH}/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=${VULKAN_SDK_PATH}/share/vulkan/explicit_layer.d
````
> on mac, sadly until bundling them into the binary you will have to export them every time you launch your terminal aka put it in your .zshrc
