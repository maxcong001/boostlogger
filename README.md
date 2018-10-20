# boost logger
this is a logger function baseed on boost::log. to use it,just include logger.hpp.

the gole of this project is to provide a fast, easy to use logger function.

## build
just cmake and make
## example

just init log and set log level
```
#include "logger/logger.hpp"

int main()
{
	init_logger();
	set_log_level(log_level::error_level);
	__LOG(error, "hello logger!"
					 << "this is error log");
}
```


## dependancy

boost C++11


enjoy

Max
