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
	~boost_logger()
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

	void init() override
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

	void set_log_level(log_level level) override
	{
		m_level = level;

		if (_sink)
		{
			_sink->set_filter(expr::attr<log_level>("Severity") >= m_level);
		}
	}

	log_level get_log_level() override
	{
		return m_level;
	}

	void debug_log(const std::string &msg) override
	{
		BOOST_LOG_SEV(lg, debug_level) << msg << std::endl;
	}
	void info_log(const std::string &msg) override
	{
		BOOST_LOG_SEV(lg, info_level) << blue << msg << normal << std::endl;
	}
	void warn_log(const std::string &msg) override
	{
		BOOST_LOG_SEV(lg, warn_level) << yellow << msg << normal << std::endl;
	}
	void error_log(const std::string &msg) override
	{
		BOOST_LOG_SEV(lg, error_level) << red << msg << normal << std::endl;
	}
	void critical_log(const std::string &msg) override
	{
		BOOST_LOG_SEV(lg, critical_level) << red << msg << normal << std::endl;
	}

private:
	log_level m_level;
	boost::shared_ptr<sink_t> _sink;
	boost::log::sources::severity_logger_mt<log_level> lg;
};