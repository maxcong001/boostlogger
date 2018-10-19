#pragma once
#include "logger/logger.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>
class boost_logger : public logger_iface
{
  public:
    boost_logger(logger_iface::log_level level = logger_iface::log_level::info);
    ~boost_logger(void) ;

    boost_logger(const boost_logger &) = default;
    boost_logger &operator=(const boost_logger &) = default;

  public:
    void set_log_level(logger_iface::log_level level);
    void debug_log(const std::string &msg, const std::string &file, std::size_t line);
    void info_log(const std::string &msg, const std::string &file, std::size_t line);
    void warn_log(const std::string &msg, const std::string &file, std::size_t line);
    void error_log(const std::string &msg, const std::string &file, std::size_t line);
   void critical_log(const std::string &msg, const std::string &file, std::size_t line);
    
    void init();

  private:
    logger_iface::log_level m_level;
    boost::shared_ptr<sink_t> _sink;
    boost::log::sources::severity_logger_mt<log_level> lg;
};