//
// Created by 19144 on 2025/1/20.
//

#ifndef FUEQUIP_FUWINDOW_H
#define FUEQUIP_FUWINDOW_H

#include "global.h"

namespace fue {
    class FUwindow {
    public:
        FUwindow(const FUwindow&) = delete;
        FUwindow &operator=(const FUwindow&) = delete;
        virtual ~FUwindow() = default;

        static std::unique_ptr<FUwindow> Create(uint32_t width, uint32_t height, const char *title);

        virtual bool ibWindowShouldClose() = 0;
        virtual void ibPollEvents() = 0;
        virtual void ibSwapBuffer() = 0;
    protected:
        FUwindow() = default;
    };
}


#endif //FUEQUIP_FUWINDOW_H
