//
// Created by 19144 on 2025/1/20.
//

#include "FUwindow.h"
#include "FUwindow_glfw.h"

namespace fue{
    std::unique_ptr<FUwindow> FUwindow::Create(uint32_t width, uint32_t height, const char *title) {
        return std::make_unique<FUwindow_glfw>(width, height, title);
    }
}