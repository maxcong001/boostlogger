#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/sinks/async_frontend.hpp>

// Related headers
#include <boost/log/sinks/unbounded_fifo_queue.hpp>
#include <boost/log/sinks/unbounded_ordering_queue.hpp>
#include <boost/log/sinks/bounded_fifo_queue.hpp>
#include <boost/log/sinks/bounded_ordering_queue.hpp>
#include <boost/log/sinks/drop_on_overflow.hpp>
#include <boost/log/sinks/block_on_overflow.hpp>

#include <thread>
#include <chrono>
#include "logger/logger.hpp"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;

typedef sinks::asynchronous_sink<sinks::text_ostream_backend, sinks::bounded_fifo_queue<1000, sinks::drop_on_overflow>> sink_t;

class boost_logger : public logger_iface
{
  public:
	boost_logger(log_level level = log_level::info_level);
	~boost_logger(void);

	boost_logger(const boost_logger &) = default;
	boost_logger &operator=(const boost_logger &) = default;

  public:
	void init() override;
	void set_log_level(log_level level) override;
	void debug_log(const std::string &msg, const std::string &file, std::size_t line) override;
	void info_log(const std::string &msg, const std::string &file, std::size_t line) override;
	void warn_log(const std::string &msg, const std::string &file, std::size_t line) override;
	void error_log(const std::string &msg, const std::string &file, std::size_t line) override;
	void critical_log(const std::string &msg, const std::string &file, std::size_t line) override;

  private:
	log_level m_level;
	boost::shared_ptr<sink_t> _sink;
	boost::log::sources::severity_logger_mt<log_level> lg;
};