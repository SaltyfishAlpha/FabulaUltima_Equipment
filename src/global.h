//
// Created by 19144 on 2025/1/20.
//

#ifndef FUEQUIP_GLOBAL_H
#define FUEQUIP_GLOBAL_H

#include <memory>
#include <string>
#include <random>

#include "log.h"

namespace fue{
    class RandamEngine {
    public:
        RandamEngine(const RandamEngine&) = delete;
        RandamEngine& operator=(const RandamEngine&) = delete;

        static RandamEngine& getInstance() {
            static RandamEngine instance;
            return instance;
        }

        int random_range(int min, int max) {
            std::uniform_int_distribution<> dist(min, max);
            return dist(gen);
        }

    private:
        RandamEngine(): gen(std::random_device()()) {}
        std::mt19937 gen;
    };
}

#endif //FUEQUIP_GLOBAL_H
