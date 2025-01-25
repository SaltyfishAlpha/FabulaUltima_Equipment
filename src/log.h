//
// Created by 19144 on 2025/1/21.
//

#ifndef FUEQUIP_LOG_H
#define FUEQUIP_LOG_H

#include <spdlog/common.h>

namespace fue {

    class log {
    public:
        log() = delete;
        log(const log&) = delete;
        log &operator=(const log&) = delete;

        static void Init();

        static log* GetLoggerInstance() {
//            assert(sLoggerInstance && "Logger instance is null");
            return &sLoggerInstance;
        }

        template<typename... Args>
        void Log(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::format_string_t<Args...> fmt, Args &&...args){
            spdlog::memory_buf_t buf;
            fmt::vformat_to(fmt::appender(buf), fmt, fmt::make_format_args(args...));
            Log(loc, lvl, buf);
        }

    private:
        void Log(spdlog::source_loc loc, spdlog::level::level_enum lvl, const spdlog::memory_buf_t &buffer);

        static log sLoggerInstance;
    };

#define IB_LOG_LOGGER_CALL(ibLog, level, ...) (ibLog)->Log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

#define LOG_T(...) IB_LOG_LOGGER_CALL(fue::log::GetLoggerInstance(), spdlog::level::trace, __VA_ARGS__)
#define LOG_D(...) IB_LOG_LOGGER_CALL(fue::log::GetLoggerInstance(), spdlog::level::debug, __VA_ARGS__)
#define LOG_I(...) IB_LOG_LOGGER_CALL(fue::log::GetLoggerInstance(), spdlog::level::info, __VA_ARGS__)
#define LOG_W(...) IB_LOG_LOGGER_CALL(fue::log::GetLoggerInstance(), spdlog::level::warn, __VA_ARGS__)
#define LOG_E(...) IB_LOG_LOGGER_CALL(fue::log::GetLoggerInstance(), spdlog::level::err, __VA_ARGS__)

} // fue

#endif //FUEQUIP_LOG_H
