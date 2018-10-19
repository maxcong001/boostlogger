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

#include <memory>
#include <mutex>
#include <string>   // std::string
#include <iostream> // std::cout
#include <fstream>
#include <sstream>  // std::ostringstream
#include <array>
#include <atomic>
#include <memory>

typedef std::basic_ostream<char> tostream;
typedef std::basic_istream<char> tistream;
typedef std::basic_ostringstream<char> tostringstream;
typedef std::basic_istringstream<char> tistringstream;

class logger_iface
{
public:
  enum log_level
  {
    debug = 0,
    info,
    warn,
    error,
    critical
  };
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

public:
  logger_iface(void)
  {
  }
  virtual ~logger_iface(void) = default;

  logger_iface(const logger_iface &) = default;
  logger_iface &operator=(const logger_iface &) = default;

public:
  virtual void init() = 0;
  virtual void set_log_level(logger_iface::log_level level) = 0;
  virtual void debug_log(const std::string &msg, const std::string &file, std::size_t line) = 0;
  virtual void info_log(const std::string &msg, const std::string &file, std::size_t line) = 0;
  virtual void warn_log(const std::string &msg, const std::string &file, std::size_t line) = 0;
  virtual void error_log(const std::string &msg, const std::string &file, std::size_t line) = 0;
  virtual void critical_log(const std::string &msg, const std::string &file, std::size_t line) = 0;
  
};

void debug(const std::string &msg, const std::string &file, std::size_t line);
void info(const std::string &msg, const std::string &file, std::size_t line);
void warn(const std::string &msg, const std::string &file, std::size_t line);
void error(const std::string &msg, const std::string &file, std::size_t line);
void critical(const std::string &msg, const std::string &file, std::size_t line);


void set_log_level(logger_iface::log_level level);
void init_logger();

#define __LOGGING_ENABLED

#ifdef __LOGGING_ENABLED
#define __LOG(level, msg)                          \
                                                   \
  {                                                \
    tostringstream var;                            \
    var << "[fuction:" << __func__ << "] " << msg; \
    level(var.str(), __FILE__, __LINE__);          \
  }
#else
#define __LOG(level, msg)
#endif /* __LOGGING_ENABLED */
