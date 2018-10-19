# logger function


## just include the header file. with 


```
#define __LOGGING_ENABLED
```

## how to set loglevel
```
std::unique_ptr<logger_iface> active_logger(new logger(logger::log_level::debug));
```


## dependancy
C++11

## new update.
add buffer function. with this function, you can add this function to your singal handler. when Segment falt happen. you can dump MAX_LOG_BUFFER of log with all the log level.

This is useful. when a product deliver. we must set the loglevel to warn or error. but when something bad happen(segmetn falt). then is no log context. this will help you !!!


enjoy

Max
