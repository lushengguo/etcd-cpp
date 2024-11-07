#include "utils.h"
#include "boost/asio/io_context.hpp"
#include <memory>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <vector>

void init_spdlog()
{
    std::vector<spdlog::sink_ptr> sinks;

#ifdef _DEBUG
    // for vscode debug console
    sinks.emplace_back(std::make_shared<spdlog::sinks::msvc_sink_mt>());
    sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#endif

    sinks.emplace_back(std::make_shared<spdlog::sinks::daily_file_format_sink_mt>("RunLog/%Y-%m-%d.log", 0, 0));
    auto logger = std::make_shared<spdlog::logger>("", sinks.begin(), sinks.end());
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] [%!] %v");
    spdlog::set_default_logger(logger);
    spdlog::flush_every(std::chrono::seconds(1));

#ifdef _DEBUG
    spdlog::set_level(spdlog::level::debug);
#endif
}

std::shared_ptr<boost::asio::io_context> get_io_context()
{
    static std::shared_ptr<boost::asio::io_context> io_context = std::make_shared<boost::asio::io_context>();
    static boost::asio::thread_pool thread_pool(std::thread::hardware_concurrency());
    static std::once_flag flag;
    std::call_once(flag, [&]() {
        for (auto i = 0; i < std::thread::hardware_concurrency(); ++i)
        {
            boost::asio::post(thread_pool, [&]() { io_context->run(); });
        }
    });
    return io_context;
}
