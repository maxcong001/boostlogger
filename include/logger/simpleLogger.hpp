/*
 * Copyright (c) 2016-20017 Max Cong <savagecm@qq.com>
 * this code can be found at https://github.com/maxcong001/logger
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
#include "logger.hpp"
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

thread_local static boost::log::sources::severity_logger<log_level> lg;

#define BOOST_LOG_Q_SIZE 1000

typedef sinks::asynchronous_sink<sinks::text_ostream_backend, sinks::bounded_fifo_queue<BOOST_LOG_Q_SIZE, sinks::drop_on_overflow>> sink_t;
static std::ostream &operator<<(std::ostream &strm, log_level level)
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
class boost_logger : public logger_iface
{
public:
	boost_logger() : m_level(log_level::error_level)
	{
	}
	~boost_logger()
	{
		}
	void init() override
	{
		logging::add_common_attributes();
		core = logging::core::get();
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
			<< expr::attr<std::string>("Process") << ":" << expr::attr<attrs::current_thread_id::value_type>("ThreadID") << ":"
			<< expr::attr<unsigned int>("LineID") << "]["
			<< expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "]["
			<< expr::attr<log_level>("Severity") << "] "
			<< expr::smessage);
		{
			sink_t::locked_backend_ptr p = _sink->locked_backend();
			p->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
		}
	}
	void stop() override
	{
		warn_log("boost logger stopping");
		_sink->flush();
		_sink->stop();
		core->remove_sink(_sink);
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
	boost::shared_ptr<logging::core> core;
	boost::shared_ptr<sink_t> _sink;
};