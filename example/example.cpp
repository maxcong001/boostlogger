
#include "logger/boost_logger.hpp"
#include "test.hpp"
#include <chrono>
#include <thread>
int main()
{

	//std::unique_ptr<boost_logger> boostloggerUptr(new boost_logger());
	INIT_LOGGER(new boost_logger());
	SET_LOG_LEVEL(debug);
	__LOG(critical, "hello logger!"
						<< "this is critical log" << (void *)ACTIVE_LOGGER_INSTANCE);
	__LOG(error, "hello logger!"
					 << "this is error log" << (void *)ACTIVE_LOGGER_INSTANCE);
	__LOG(warn, "hello logger!"
					<< "this is warn log" << (void *)ACTIVE_LOGGER_INSTANCE);
	__LOG(info, "hello logger!"
					<< "this is info log" << (void *)ACTIVE_LOGGER_INSTANCE);
	__LOG(debug, "hello logger!"
					 << "this is debug log" << (void *)ACTIVE_LOGGER_INSTANCE);

	test();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
