#define BOOST_AUTO_TEST_MAIN

#ifdef VISUAL_LEAK_DETECTOR //http://www.codeproject.com/tools/visualleakdetector.asp
	#include <vld.h> //check's for Memory Leaks when running in  Microsoft Visual Studio
#endif

#include <boost/test/auto_unit_test.hpp> 
