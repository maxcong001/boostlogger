

#include "logger/logger.hpp"
#include "logger/boost_logger.hpp"
std::unique_ptr<logger_iface> active_logger(new boost_logger(log_level::error_level)); //nullptr;

void debug(const std::string &msg, const std::string &file, std::size_t line)
{
    if (active_logger)
        active_logger->debug_log(msg, file, line);
}

void info(const std::string &msg, const std::string &file, std::size_t line)
{
    if (active_logger)
        active_logger->info_log(msg, file, line);
}

void warn(const std::string &msg, const std::string &file, std::size_t line)
{
    if (active_logger)
        active_logger->warn_log(msg, file, line);
}

void error(const std::string &msg, const std::string &file, std::size_t line)
{
    if (active_logger)
        active_logger->error_log(msg, file, line);
}

void critical(const std::string &msg, const std::string &file, std::size_t line)
{
    if (active_logger)
        active_logger->critical_log(msg, file, line);
}
void set_log_level(log_level level)
{
    if (active_logger)
        active_logger->set_log_level(level);
}

void init_logger()
{
    if (active_logger)
        active_logger->init();
}