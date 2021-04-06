VULKAN_SDK_PATH = /home/ntsako/VulkanSDK/1.2.162.1/x86_64

CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -I$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan
nightLight:	main.cpp
	g++ $(FLAGS) -o nightLight main.cpp $(LDFLAGS)

.PONY:	test	clean

test:	nightLight
	VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/implicit_layer.d
	./nightLight

clean:
	rm -rf nightLight
