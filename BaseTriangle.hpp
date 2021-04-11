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
#include <experimental/optional>
#ifdef NDEBUG
    const bool  enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};
VkPhysicalDevice  physicalDevice = VK_NULL_HANDLE;
struct      QueueFamilyIndices
{
    std::experimental::optional<uint32_t>    graphicsFamily;

    bool    isComplete()
    {
        return (graphicsFamily.value());
    }
};

class BaseTriangle {
public:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    VkInstance  instance;
    VkDevice    device;
    VkQueue     graphicsQueue;
    void initVulkan()
    {
        createInstance();
        pickPhysicalDevice();
        createLogicalDevice();
    }
    void    createInstance()
    {
        if (enableValidationLayers && !checkValidationLayerSupport())
            throw  std::runtime_error("validation layers requested, but not available");

        VkApplicationInfo  appInfo{};

        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "NightLight";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        auto    extensions = getRequiredExtensions();
        createInfo.enabledLayerCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledLayerNames = extensions.data();
        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }
        createInfo.pApplicationInfo = &appInfo;

        uint32_t        glfwExtensionCount = 0;
        const char**    glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;


        VkResult    result = vkCreateInstance(&createInfo, nullptr, &instance);

        if (result != VK_SUCCESS)
            throw std::runtime_error("Failed to create instance!");

//        for (const auto &extension : extensions)
//            std::cout << '\t' << extension.extensionName << '\n';
    }
//    bool    checkExtensionNames()
//    {
//        uint32_t    extensionCount = 0;
//        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//
//        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
//        const char**    instanceExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
//
//        for (const char* extensionName : instanceExtensions)
//        {
//            bool    supportExtenions = false;
//            for(const auto & extensionProperties : availableExtensions.data())
//            {
//                if (strcmp(extensionName, extensionProperties) == 0)
//                {
//                    supportExtenions = true;
//                    break;
//                }
//            }
//            if (!supportExtenions)
//                return (false);
//        }
//        return (true);
//    }

void    pickPhysicalDevice()
{
        uint32_t    deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0)
            throw std::runtime_error("Failed to find GPU");
        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());


        for (const auto &candidate : physicalDevices)
        {
            if (isDeviceSuitable(candidate))
                physicalDevice = candidate;
            break;
        }
        if (physicalDevice == VK_NULL_HANDLE)
            std::runtime_error("Failed to find GPU");

}

void    createLogicalDevice()
{
        QueueFamilyIndices familyIndices = findQueueFamilies(physicalDevice);
        VkDeviceQueueCreateInfo queueCreateInfo{};
        VkPhysicalDeviceFeatures deviceFeatures{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = familyIndices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;

        queueCreateInfo.pQueuePriorities = &queuePriority;
        VkDeviceCreateInfo  createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = 0;
        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t> (validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
            createInfo.enabledLayerCount = 0;

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
            throw std::runtime_error("Failed to create logical device");

        vkGetDeviceQueue(device, familyIndices.graphicsFamily.value(), 0, &graphicsQueue);
}

//int     rateDeviceSuitability(VkPhysicalDevice device)
//{
//        VkPhysicalDeviceProperties deviceProperties;
//        VkPhysicalDeviceFeatures deviceFeatures;
//        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
//        vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
//        int     score = 0;
//        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
//            score += 1000;
//
//        score += deviceProperties.limits.maxImageDimension2D;
//
//        if (!deviceFeatures.geometryShader)
//            return (0);
//
//    return (score);
//}

QueueFamilyIndices    findQueueFamilies(VkPhysicalDevice device)
{
        QueueFamilyIndices      indices;
        uint32_t        familyCount = 0;

        vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamily(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, queueFamily.data());

        int i = 0;
        for (const auto &queueFam : queueFamily)
        {
            if (queueFam.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;
            i++;
        }
    return (indices);
}

bool    isDeviceSuitable(VkPhysicalDevice physicalDevices)
{
        QueueFamilyIndices indices = findQueueFamilies(physicalDevices);
        return (indices.isComplete());
}

bool    checkValidationLayerSupport()
{
        uint32_t    layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers)
        {
            bool  layerFound = false;
            for(const auto & layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound)
                return(false);
        }
        return (true);
}

std::vector<const  char*> getRequiredExtensions()
{
    uint32_t    glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return (extensions);
}
    void    initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "NightLight", nullptr, nullptr);
    }
    void    mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }
    void    cleanup()
    {
        vkDestroyDevice(device, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

#endif //NIGHT_LIGHT_BASETRIANGLE_HPP
