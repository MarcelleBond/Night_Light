//
// Created by ntsako on 2021/04/04.
//

#ifndef NIGHT_LIGHT_BASETRIANGLE_HPP
#define NIGHT_LIGHT_BASETRIANGLE_HPP
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <optional>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

struct      QueueFamilyIndices
{
    std::optional<uint32_t>    graphicsFamily;

    bool    isComplete()
    {
        return (graphicsFamily.has_value());
    }
};

class BaseTriangle {
public:
    void run();


private:
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;

    void initWindow();
    void initVulkan();
    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }
    void cleanup();
    void createInstance();
    void pickPhysicalDevice();
    void createLogicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();

};

#endif //NIGHT_LIGHT_BASETRIANGLE_HPP
