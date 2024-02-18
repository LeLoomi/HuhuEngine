include .env

CFLAGS = -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include -I$(BREW_INCLUDES_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib -lvulkan -L$(BREW_CELLAR)/glfw/3.3.9/lib -lglfw -rpath /Users/eliahlohr/VulkanSDK/1.3.275.0/macOS/lib

a.out: *.cpp *.hpp
	g++ $(CFLAGS) -o a.out *.cpp $(LDFLAGS)

.PHONY: test clean

test: a.out
	./a.out

clean:
	rm -f a.out
