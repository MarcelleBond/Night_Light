//
// Created by ntsako on 2021/04/04.
//

#include    "main.h"
#define     GLFW_INCLUDE_VULKAN
#include    <GLFW/glfw3.h>
#define     GLM_FORCE_RADIANS
#define     GLM_FORCE_DEPTH_ZERO_TO_ONE
#include    <glm/vec4.hpp>
#include    <glm/mat4x4.hpp>
#include    <iostream>
#include    "BaseTriangle.hpp"

int     main ()
{
    BaseTriangle app;

    try {
        app.run();
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return (EXIT_FAILURE);
    }

   return (EXIT_SUCCESS);
}