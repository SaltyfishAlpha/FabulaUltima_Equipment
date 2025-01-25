#include <iostream>
#include "editor/FUwindow.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {

    std::unique_ptr<fue::FUwindow> window = fue::FUwindow::Create(SCR_WIDTH, SCR_HEIGHT, "SandBox");

    while (!window->ibWindowShouldClose()) {
        window->ibPollEvents();
        window->ibSwapBuffer();
    }

    return EXIT_SUCCESS;
}