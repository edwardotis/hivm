#include "Log.hpp"
#include <sstream>
#include <boost/algorithm/string.hpp>//trimming of strings
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp> //safe numerical to string casts

#include "FileIO.hpp"
#include "Types.hpp"

namespace fs = boost::filesystem;

//static variable declaration
std::string Log::_log_path = "";
std::string Log::start_time = "";
time_t		Log::start_timer = 0;

Log::Log()
{
	variable_check();
}

Log::~Log() 
{}

void Log::initialize( std::string output_prefix )
{
	Log::_log_path = ( output_prefix + ".log" );
	variable_check();
}

void Log::variable_check()
{
	if( Log::start_timer == 0 )
	{
		time ( &start_timer );
	}

	if( Log::start_time == "" )
	{
		Log::start_time = get_local_date_time();
	}

	if( Log::_log_path == "" )
	{
		Log::_log_path = ( Log::start_time + ".log" );
	}
}

//DEPRECATED
void Log::check_log_path()
{
	if( Log::start_time == "" )
	{
		Log::start_time = get_local_date_time();
	}

	if( Log::_log_path == "" )
	{
		Log::_log_path = ( "logs/" + Log::start_time + ".log" );
	}
}

std::string Log::get_start_time()
{
	variable_check();

	return start_time;
}

std::string Log::elapsed_time()
{
	variable_check();

	static time_t end_timer;
	time (&end_timer);//get current time
	size_t seconds_elapsed = (size_t)difftime (end_timer, start_timer);
	
	size_t seconds_in_hour = 3600;
	size_t seconds_in_minute = 60;

	size_t hours =     seconds_elapsed / seconds_in_hour;
	size_t minutes = ( seconds_elapsed % seconds_in_hour ) / seconds_in_minute;
	size_t seconds = ( seconds_elapsed % seconds_in_hour ) % seconds_in_minute;

	std::string output;//00hr 00m 00s elapsed
	output.append( "\n" );
	output.append( boost::lexical_cast<std::string>( hours ) + "hr " );
	output.append( boost::lexical_cast<std::string>( minutes ) + "m " );
	output.append( boost::lexical_cast<std::string>( seconds ) + "s " );
	output.append( "elapsed" );
	output.append( "\n" );

	return output;
 
}


std::string Log::get_local_date_time()
{
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	//format date-time into a portable file name
	std::string _str( asctime(timeinfo) );
	boost::algorithm::trim( _str );//trims beginning and trailing white space
	boost::algorithm::replace_all( _str, " ", "_" );
	boost::algorithm::replace_all( _str, ":", "-" );

	return _str;
}

//TODO DRY  with write
//if output file hasn't been made, create new one and open it in append.
 void Log::append( const std::string log_entry )
{
	variable_check();
	
	std::string _sstream( log_entry + "\n" );
	
	try
	{
		FileIO::append( Log::_log_path, _sstream );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
	    std::cerr << "Exception Type " << typeid(e).name() << std::endl;
		std::cerr <<  e.what() << std::endl;
		std::cerr << "Could not open log file for writing." << std::endl;
		std::cerr << "Log file name: " << Log::_log_path << std::endl;
		std::cerr << "Continuing to run hivm..." << std::endl;
	}
}

//TODO DRY  with append
 void Log::write( const std::string log_entry )
{
	variable_check();
	
	std::string _sstream( log_entry + "\n" );
	
	try
	{
		FileIO::write( Log::_log_path, _sstream );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
	    std::cerr << "Exception Type " << typeid(e).name() << std::endl;
		std::cerr <<  e.what() << std::endl;
		std::cerr << "Could not open log file for writing." << std::endl;
		std::cerr << "Log file name: " << Log::_log_path << std::endl;
		std::cerr << "Continuing to run hivm..." << std::endl;
	}
}

//
std::string Log::get_log_path()
{
	variable_check();

	return Log::_log_path;
}
