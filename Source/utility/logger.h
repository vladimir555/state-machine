/*
 * logger.h
 *
 *  Created on: 08.12.2011
 *      Author: volodja
 */


#ifndef LOGGER_H_
#define LOGGER_H_


//#include <log4cplus/logger.h>
//#include <log4cplus/configurator.h>
//#include <iomanip>


#include <iostream>


namespace utility {


/*
#define LOG_DEBUG(message_) LOG4CPLUS_DEBUG(logger,message_);
#define LOG_INFO(message_)  LOG4CPLUS_INFO(logger,message_);
#define LOG_WARN(message_)  LOG4CPLUS_WARN(logger,message_);
#define LOG_ERROR(message_) LOG4CPLUS_ERROR(logger,message_);
#define LOG_FATAL(message_) LOG4CPLUS_FATAL(logger,message_);
#define LOG_DEFINE                                                          \
static log4cplus::Logger logger;                                            \
static log4cplus::Logger loggerInit(const string &class_name) {             \
    log4cplus::Logger logger = log4cplus::Logger::getInstance(class_name);  \
    log4cplus::BasicConfigurator config;                                    \
    config.configure();                                                     \
    return logger;                                                          \
}
#define LOG_IMPLEMENT(class_) log4cplus::Logger class_::logger = class_::loggerInit((#class_))
*/


/*
#define LOG_DEBUG(message_) LOG4CXX_DEBUG(logger,(message_));
#define LOG_INFO(message_)  LOG4CXX_INFO(logger,(message_));
#define LOG_WARN(message_)  LOG4CXX_WARN(logger,(message_));
#define LOG_ERROR(message_) LOG4CXX_ERROR(logger,(message_));
#define LOG_FATAL(message_) LOG4CXX_FATAL(logger,(message_));
#define LOG_DEFINE                                                          \
static log4cxx::Logger logger;                                            \
static log4cxx::Logger loggerInit(const string &class_name) {             \
    log4cxx::Logger logger = log4cxx::Logger::getInstance(class_name);  \
    return logger;                                                          \
}
#define LOG_IMPLEMENT(class_) log4cxx::Logger class_::logger = class_::loggerInit((#class_))
*/



using namespace std;
#define LOG_DEBUG(message_) cout << "DEBUG " << message_ << endl;
#define LOG_INFO(message_)  cout << "INFO  " << message_ << endl;
#define LOG_WARN(message_)  cout << "WARN  " << message_ << endl;
#define LOG_ERROR(message_) cout << "ERROR " << message_ << endl;
#define LOG_FATAL(message_) cout << "FATAL " << message_ << endl;
#define LOG_DEFINE ;
#define LOG_IMPLEMENT(class_);
}


#endif /* LOGGER_H_ */
