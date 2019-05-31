
#include "logger/boost_logger.hpp"

int main()
{
	
	std::unique_ptr<boost_logger> boostloggerUptr(new boost_logger());
	INIT_LOGGER(boostloggerUptr);
	SET_LOG_LEVEL(debug);
	__LOG(critical, "hello logger!"
						<< "this is critical log");
	__LOG(error, "hello logger!"
					 << "this is error log");
	__LOG(warn, "hello logger!"
					<< "this is warn log");
	__LOG(info, "hello logger!"
					<< "this is info log");
	__LOG(debug, "hello logger!"
					 << "this is debug log");

	for (int i = 0; i < 10; i++)
	{
		if (CHECK_LOG_LEVEL(critical))
		{
			__LOG(critical, "hello logger!"
								<< "this is critical log");
		}
		if (CHECK_LOG_LEVEL(error))
		{
			__LOG(error, "hello logger!"
							 << "this is error log");
		}
		if (CHECK_LOG_LEVEL(warn))
		{
			__LOG(warn, "hello logger!"
							<< "this is warn log");
		}
		if (CHECK_LOG_LEVEL(info))
		{
			__LOG(info, "hello logger!"
							<< "this is info log");
		}
		if (CHECK_LOG_LEVEL(debug))
		{
			__LOG(debug, "hello logger!"
							 << "this is debug log");
		}
	}
}
