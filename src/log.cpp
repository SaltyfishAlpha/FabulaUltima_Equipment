//
// Created by 19144 on 2025/1/21.
//

#include "log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>

namespace fue {
    log log::sLoggerInstance{};
    static std::shared_ptr<spdlog::logger> sSpdLogger{};

    void log::Init() {
        sSpdLogger = spdlog::stdout_color_mt<spdlog::async_factory>("async_logger");
        sSpdLogger->set_level(spdlog::level::trace);
        sSpdLogger->set_pattern("%^%H:%M:%S:%e [%P-%t] [%1!L] [%20s:%-4#] - %v%$");
    }

    void log::Log(spdlog::source_loc loc, spdlog::level::level_enum lvl, const spdlog::memory_buf_t &buffer) {
        sSpdLogger->log(loc, lvl, spdlog::string_view_t(buffer.data(), buffer.size()));
    }
} // fue