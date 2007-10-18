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

		{
			std::string msg;
			msg.append( "Elapsed time: " );
			msg.append( Log::elapsed_time() );
			Log::append( msg );
			std::cout << msg << "\n";
		}

	}
	catch( std::exception& e )
	{
		std::cerr << "Exception caught in main(): " << e.what() << std::endl;
	}
	catch(...)
	{
		std::cerr << "Unhandled Exception caught in main()";
	}

	return 0;
}
