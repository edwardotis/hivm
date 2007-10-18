#ifndef LOGTEST_CPP
#define LOGTEST_CPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  LOG_TEST// || defined TEST_ALL

#include <boost/test/auto_unit_test.hpp>
//#include <boost/date_time/gregorian/gregorian.hpp> //date
//#include <boost/date_time/posix_time/posix_time.hpp>//times
//#include <boost/date_time/c_local_time_adjustor.hpp>
//#include <boost/date_time/local_time/local_time.hpp>
#include "../Log.hpp"
#include "../FileIO.hpp"
#include <iostream>
#include <time.h>
#include <boost/filesystem/path.hpp>

//namespace gr = boost::gregorian;
//namespace pt = boost::posix_time;
//namespace lt = boost::local_time;

/***************'
Due to the random order of auto boost tests.
and static functions of Log, Log should only be 
tested in isolation.

Other tests will call Log, and this cannot be avoided.

*/
 
BOOST_AUTO_TEST_SUITE( LogTests );

BOOST_AUTO_TEST_CASE( append ) 
{
	{
		
		std::string _empty("");
		//BOOST_CHECK_EQUAL( _empty, Log::get_log_path() );
		BOOST_CHECK( _empty != Log::get_log_path() );
	}
	
	{
		Log _log;
		std::string _empty("");
		BOOST_CHECK( _empty != Log::get_log_path() );
	}

	//static variables, so Log _log above changes it for
	//all scopes
	{
		std::string _empty("");
		BOOST_CHECK( _empty != Log::get_log_path() );
	}

	{
		//add two entries
		Log::append( "1st entry" );
		Log::append( "2nd entry in log file" );

		//read in logfile
		std::string _log_file;
		try
		{
			FileIO::read( Log::get_log_path(), _log_file );
		}
		catch( boost::filesystem::filesystem_error& e )
		{
			std::cerr << e.what();
		}

		BOOST_CHECK_EQUAL( _log_file, "1st entry\n2nd entry in log file");

	}

}

BOOST_AUTO_TEST_CASE( elapsed_time )
{

	Log l;

	std::string time = Log::elapsed_time();
}

BOOST_AUTO_TEST_CASE( date_time )
{
	//DON'T USE BOOST FOR TIME - my needs are too simple
	//lt::time_zone_ptr time_zone(new lt::posix_time_zone("PDT-07"));//pacific daylight

	//boost::local_time::local_date_time ldt = 
	//	boost::local_time::local_sec_clock::local_time( time_zone );

	//std::cerr << std::endl;
	//std::cerr << ldt.to_string() << std::endl;//date and time	

	//USE THIS STD LIB time. It actually pulls it off my computer correctly.
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	//std::cerr << asctime(timeinfo) << std::endl;



	//std::cerr << ldt.utc_time()<< std::endl;//
	//std::cerr << ldt.time_of_day() << std::endl; //time
	//std::cerr << ldt.date() << std::endl;
	////std::cerr << ldt. //time
	//std::cerr << ldt << std::endl;//date and time
	//typedef boost::date_time::c_local_adjustor<pt::ptime> local_adj;

	//gr::date today = gr::day_clock::local_day();
	//boost::date_time::h

	//std::cerr << gr::day_clock::local_day();
	////std::cerr << 
	//greg::date d( gr::day_clock::local_day());
	//well, this is only good for today

}

BOOST_AUTO_TEST_CASE( LogTest_append )
{
	//Log::append(); //write to file on disk each time (good for crashes,
	//could be a perfomance issue. let the profiler decide.

	//preprocessor can disable, enable logging by ifdef inside 
	//Log::append() so that it doesn't do anything.

	//Log << "First log note";

	//Log.create();//create name as date/time
	//BOOST_CHECK_EQUAL( logname, "date+time.log" );

	//have to test without using date+time format for name.
	//maybe just use date a time or two for today only.

	//AND YOU KNOW WHAT? i probably want the damn <ctime>



}

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  LOG_TEST// || defined TEST_ALL
#endif //LOGTEST_CPP
