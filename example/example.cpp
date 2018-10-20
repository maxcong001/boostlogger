
#include "logger/logger.hpp"

int main()
{
	init_logger();
	set_log_level(log_level::error_level);
	__LOG(error, "hello logger!"
					 << "this is error log");
	set_log_level(log_level::debug_level);
	for (int i = 0; i < 10; i++)
	{
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
	}
}
