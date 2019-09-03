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

class simpleLogger : public logger_iface
{
public:
	simpleLogger() : m_level(log_level::error_level)
	{
	}
	~simpleLogger()
	{
	}
	void init() override
	{
	}
	void stop() override
	{
	}
	void set_log_level(log_level level) override
	{
		m_level = level;
	}

	log_level get_log_level() override
	{
		return m_level;
	}

	void debug_log(const std::string &msg) override
	{
		std::cout << msg << std::endl;
	}
	void info_log(const std::string &msg) override
	{
		std::cout << blue << msg << normal << std::endl;
	}
	void warn_log(const std::string &msg) override
	{
		std::cout << yellow << msg << normal << std::endl;
	}
	void error_log(const std::string &msg) override
	{
		std::cout << red << msg << normal << std::endl;
	}
	void critical_log(const std::string &msg) override
	{
		std::cout << red << msg << normal << std::endl;
	}

private:
	log_level m_level;
};