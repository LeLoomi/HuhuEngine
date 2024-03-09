include .env

COMPILERFLAGS = -std=c++17
INCLUDEFLAGS = -I. -I$(VULKAN_SDK)/include -I$(VULKAN_SDK)/lib -I$(BREW_INCLUDES_PATH) -I$(TINYOBJ_PATH)
LINKERFLAGS = -L$(VULKAN_SDK)/lib -lvulkan -L$(BREW_CELLAR)/glfw/3.4/lib -lglfw

a.out: *.cpp *.hpp
	rm -f a.out
	g++ $(COMPILERFLAGS) -o a.out *.cpp $(INCLUDEFLAGS) $(LINKERFLAGS)

.PHONY: test clean

test: a.out
	./a.out

clean:
	rm -f a.out

debug: *.cpp *.hpp
	rm -f debug.out
	g++ -fsanitize=address $(COMPILERFLAGS) -o debug.out *.cpp $(INCLUDEFLAGS) $(LINKERFLAGS)
