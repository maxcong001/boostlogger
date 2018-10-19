
#include "logger/boost_logger.hpp"

std::ostream &operator<<(std::ostream &strm, log_level level)
{
  static const char *strings[] =
      {
          "debug",
          "info",
          "warn",
          "error",
          "critical"};

  if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings))
    strm << strings[level];
  else
    strm << static_cast<int>(level);

  return strm;
}

static const char black[] = {0x1b, '[', '1', ';', '3', '0', 'm', 0};
static const char red[] = {0x1b, '[', '1', ';', '3', '1', 'm', 0};
static const char yellow[] = {0x1b, '[', '1', ';', '3', '3', 'm', 0};
static const char blue[] = {0x1b, '[', '1', ';', '3', '4', 'm', 0};
static const char normal[] = {0x1b, '[', '0', ';', '3', '9', 'm', 0};

boost_logger::boost_logger(log_level level)
    : m_level(level)
{
}
boost_logger::~boost_logger()
{
    boost::shared_ptr<logging::core> core = logging::core::get();
    // Remove the sink from the core, so that no records are passed to it
    core->remove_sink(_sink);
    // Break the feeding loop
    _sink->stop();
    // Flush all log records that may have left buffered
    _sink->flush();
    _sink.reset();
}
void boost_logger::set_log_level(log_level level)
{
    m_level = level;

    if (_sink)
    {
        _sink->set_filter(expr::attr<log_level>("Severity") >= m_level);
    }
}
void boost_logger::init()
{
    logging::add_common_attributes();
    boost::shared_ptr<logging::core> core = logging::core::get();
    boost::shared_ptr<sinks::text_ostream_backend> backend = boost::make_shared<sinks::text_ostream_backend>();
    if (!_sink)
    {
        _sink.reset(new sink_t(backend));
        core->add_sink(_sink);
    }

    core->add_global_attribute("ThreadID", attrs::current_thread_id());
    core->add_global_attribute("Process", attrs::current_process_name());
    _sink->set_filter(expr::attr<log_level>("Severity") >= m_level);
    _sink->set_formatter(
        expr::stream
        << "["
        << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "]["
        << expr::attr<attrs::current_thread_id::value_type>("ThreadID") << ":"
        << expr::attr<unsigned int>("LineID") << "]["
        << expr::attr<std::string>("Process")
        << "][" << expr::attr<log_level>("Severity")
        << "] "
        << ":" << expr::smessage);
    {
        sink_t::locked_backend_ptr p = _sink->locked_backend();
        p->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
    }
}
void boost_logger::debug_log(const std::string &msg, const std::string &file, std::size_t line)
{
    BOOST_LOG_SEV(lg, debug_level) << " [" << file << ":" << line << "] " << msg << std::endl;
}

void boost_logger::info_log(const std::string &msg, const std::string &file, std::size_t line)
{
    BOOST_LOG_SEV(lg, info_level) << blue << "[" << file << ":" << line << "] " << msg << normal << std::endl;
}

void boost_logger::warn_log(const std::string &msg, const std::string &file, std::size_t line)
{
    BOOST_LOG_SEV(lg, warn_level) << yellow << "[" << file << ":" << line << "] " << msg << normal << std::endl;
}

void boost_logger::error_log(const std::string &msg, const std::string &file, std::size_t line)
{
    BOOST_LOG_SEV(lg, error_level) << red << "[" << file << ":" << line << "] " << msg << normal << std::endl;
}

void boost_logger::critical_log(const std::string &msg, const std::string &file, std::size_t line)
{
    BOOST_LOG_SEV(lg, critical_level) << red << "[" << file << ":" << line << "] " << msg << normal << std::endl;
}