/**
 * @class Log
 * Class to manage log files
 @usage Initialize in main, then use static append()
 function to add entrie to it.
 This makes the time stamp match the start of execution.
 */

#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <time.h>//for local date and time
#include <string>

class Log{

//friend void LogTest_private_tests();//unit tests

public:

	//default ctor
	//starts timer
	Log();

	//default dtor
	virtual ~Log();

	static void initialize( std::string output_prefix );

	static void append( const std::string );

	static void write( const std::string );

	static std::string get_log_path();

	//@purpose Provides time stamp for any files that
	//hivm needs to write.
	static std::string get_start_time();

	//@purpose Provide time elapsed since Log construtor called
	static std::string elapsed_time();//call in save cmd line and config

private:

	//check for all static variables  to be initialized
	static void variable_check();

	//DEPRECATED
	//check for log path initialization
	static void check_log_path();

	//returns computer system date and time
	static std::string get_local_date_time();

	static std::string start_time;
	static std::string  _log_path;

	//for timer in constructor
	static time_t start_timer;

	//copy constructor
	Log( const Log& );

	//assignment operator
	Log& operator= ( const Log& );

};

#endif //LOG_HPP
