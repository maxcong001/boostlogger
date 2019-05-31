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
	std::unique_ptr<boost_logger> boostloggerUptr(new boost_logger());
	INIT_LOGGER(boostloggerUptr);
	SET_LOG_LEVEL(debug);
	if (CHECK_LOG_LEVEL(critical))
	{
		__LOG(critical, "hello logger!"
								<< "this is critical log");
	}
}
```


## dependancy

boost C++11


enjoy

Max
