VULKAN_SDK_PATH = /home/ntsako/VulkanSDK/1.2.162.1/x86_64

CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -I$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan
NAME	=	nightLight
all:	$(NAME)

$(NAME):	main.cpp
	g++ $(CFLAGS) -o nightLight main.cpp $(LDFLAGS)

.PONY:	test	clean

test:	$(NAME)
	VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/implicit_layer.d
	./nightLight

clean:
	@/bin/rm -rf $(NAME)

re : clean $(NAME)