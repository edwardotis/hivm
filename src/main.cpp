#include "Log.hpp"
#include "Options.hpp"
#include "SvmMachine.hpp"

#ifdef VISUAL_LEAK_DETECTOR //http://www.codeproject.com/tools/visualleakdetector.asp
//	#include <vld.h> //check's for Memory Leaks when running in  Microsoft Visual Studio
#endif

#include <exception>
#include <iostream>

// Main program
int main(int argc, char* argv[] )
{
	try
	{
		Options options( argc, argv );

		if( options.help_flag == true )
		{
			//already displayed help inside Option ctor, time to leave main()
			return 0;
		}

		Log::initialize( options.output_prefix );//init the log timers

		{
			std::string msg;
			msg.append( "Start time: " );
			msg.append( Log::get_start_time() );
			Log::write( msg );//overwrite previous log file with same name
			std::cout << msg << "\n";
		}

		SvmMachine machine;
		machine.run( options.purpose, options );

		std::cerr << "\n\nFinished!";
		std::cerr << "\n\nThank you for using hivm.";

		{
			std::string msg;
			msg.append( "Elapsed time: " );
			msg.append( Log::elapsed_time() );
			Log::append( msg );
			std::cout << msg << "\n";
		}
	}
	catch( std::bad_alloc& e )
	{
		std::string msg = "\nBad Allocation exception caught in main(): \n";
		msg += "System could not provide more virtual memory.";
		msg += "Consider running hivm on linux. Linux 2.6.x handled hivm's memory much better than Windows during development, and never caused this error on large datasets.";
		msg +=   e.what();
		msg += "\n\nElapsed Time: ";
		msg += Log::elapsed_time();
		msg += "\nExiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}
	catch( std::exception& e )
	{
		std::string msg = "\nException caught in main(): \n";
		msg +=   e.what();
		msg += "\n\nElapsed Time: ";
		msg += Log::elapsed_time();
		msg += "\nExiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}
	catch(...)
	{
		std::string msg = "\nUnhandled, unknown Exception caught in main(): \n";
		msg += "\n\nElapsed Time: ";
		msg += Log::elapsed_time();
		msg += "\nExiting...";
		Log::append( msg );
		std::cerr << msg;
	}

	return 0;
}
