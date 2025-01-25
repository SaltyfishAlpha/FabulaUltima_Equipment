//
// Created by 19144 on 2025/1/20.
//

#include "FUwindow_glfw.h"

namespace fue{
    FUwindow_glfw::FUwindow_glfw(uint32_t width, uint32_t height, const char *title) {
        if (!glfwInit()) {
            LOG_E("Failed to init glfw.");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        // Create window
        glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!glfwWindow) {
            LOG_E("Failed to create glfw window.");
        }

        // experimental
        GLFWmonitor *glfWmonitor = glfwGetPrimaryMonitor();
        if (glfWmonitor) {
            int xPos, yPos, workWidth, workHeight;
            glfwGetMonitorWorkarea(glfWmonitor, &xPos, &yPos, &workWidth, &workHeight);
            glfwSetWindowPos(glfwWindow, (workWidth-width)/2, (workHeight-height)/2);
        }

        // if opengl / opengl es
        // glfwSwapInterval(0);

        glfwMakeContextCurrent(glfwWindow);
        glfwShowWindow(glfwWindow);
    }

    FUwindow_glfw::~FUwindow_glfw() {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
        LOG_D("Destory finished.");
    }

    bool FUwindow_glfw::ibWindowShouldClose() {
        return glfwWindowShouldClose(glfwWindow);
    }

    void FUwindow_glfw::ibPollEvents() {
        glfwPollEvents();
    }

    void FUwindow_glfw::ibSwapBuffer() {
        glfwSwapBuffers(glfwWindow);
    }
}