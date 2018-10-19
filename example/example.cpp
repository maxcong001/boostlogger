
#include "logger/logger.hpp"

int main()
{
    init_logger();
    set_log_level(logger_iface::log_level::warn);

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
