//
// Created by 19144 on 2025/1/20.
//

#ifndef FUEQUIP_FUWINDOW_GLFW_H
#define FUEQUIP_FUWINDOW_GLFW_H

#include "FUwindow.h"
#include <GLFW/glfw3.h>

class FUwindow_glfw {

};
namespace fue {
    class FUwindow_glfw : public FUwindow {
    public:
        FUwindow_glfw() = delete;
        FUwindow_glfw(uint32_t width, uint32_t height, const char *title);

        ~FUwindow_glfw() override;

        bool ibWindowShouldClose() override;
        void ibPollEvents() override;
        void ibSwapBuffer() override;

        [[nodiscard]] GLFWwindow *getWindowHandler() const { return glfwWindow; }
    private:
        GLFWwindow *glfwWindow;
    };
}


#endif //FUEQUIP_FUWINDOW_GLFW_H
