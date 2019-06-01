#pragma once
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

#include <string>   // std::string
#include <iostream> // std::cout
#include <fstream>
#include <sstream> // std::ostringstream
#include <memory>

typedef std::basic_ostringstream<char> tostringstream;
static const char black[] = {0x1b, '[', '1', ';', '3', '0', 'm', 0};
static const char red[] = {0x1b, '[', '1', ';', '3', '1', 'm', 0};
static const char yellow[] = {0x1b, '[', '1', ';', '3', '3', 'm', 0};
static const char blue[] = {0x1b, '[', '1', ';', '3', '4', 'm', 0};
static const char normal[] = {0x1b, '[', '0', ';', '3', '9', 'm', 0};

#define INIT_LOGGER(loggerImpUptr)                  \
                                                    \
	{                                               \
		active_logger = (std::move(loggerImpUptr)); \
		active_logger->init();                      \
	}
#define CHECK_LOG_LEVEL(logLevel) (active_logger ? ((active_logger->get_log_level() <= log_level::logLevel##_level) ? true : false) : false)
#define SET_LOG_LEVEL(logLevel)                                          \
	{                                                                    \
		if (active_logger)                                               \
			(active_logger->set_log_level(log_level::logLevel##_level)); \
	}

enum log_level
{
	debug_level,
	info_level,
	warn_level,
	error_level,
	critical_level
};

class logger_iface
{
public:
	logger_iface(void) = default;
	virtual ~logger_iface(void) = default;
	logger_iface(const logger_iface &) = default;
	logger_iface &operator=(const logger_iface &) = default;

public:
	virtual void init() = 0;
	virtual void set_log_level(log_level level) = 0;
	virtual log_level get_log_level() = 0;
	virtual void debug_log(const std::string &msg) = 0;
	virtual void info_log(const std::string &msg) = 0;
	virtual void warn_log(const std::string &msg) = 0;
	virtual void error_log(const std::string &msg) = 0;
	virtual void critical_log(const std::string &msg) = 0;
};
static std::unique_ptr<logger_iface> active_logger;

#define __LOGGING_ENABLED

#ifdef __LOGGING_ENABLED
#define __LOG(level, msg)                                                      \
                                                                               \
	{                                                                          \
		tostringstream var;                                                    \
		var << "[" << __FILE__ << ":" << __LINE__ << ":" << __func__ << "] \n" \
			<< msg;                                                            \
		if (active_logger)                                                     \
			active_logger->level##_log(var.str());                             \
	}
#else
#define __LOG(level, msg)
#endif /* __LOGGING_ENABLED */
