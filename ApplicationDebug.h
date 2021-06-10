//
// Created by ntsako on 2021/06/10.
//

#ifndef NIGHT_LIGHT_APPLICATIONDEBUG_H
#define NIGHT_LIGHT_APPLICATIONDEBUG_H

#include "BaseTriangle.hpp"

#ifdef NDEBUG
const bool  enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

VkResult    CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                         const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto    func = (PFN_vkCreateDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if ( func != nullptr)
        return  (func(instance, pCreateInfo, pAllocator, pDebugMessenger));
    else
        return (VK_ERROR_EXTENSION_NOT_PRESENT);
}

VkResult        DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT pDebugMessenger,
                                              const VkAllocationCallbacks *pAllocator)
{
    auto    func = (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if ( func != nullptr)
        func(instance, pDebugMessenger, pAllocator);
    else
        return (VK_ERROR_EXTENSION_NOT_PRESENT);
}

class ApplicationDebug {
private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
public:
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    void setupDebugMessenger();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }
};


#endif //NIGHT_LIGHT_APPLICATIONDEBUG_H
