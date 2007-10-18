#ifndef FILEIOTEST_HPP
#define FILEIOTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  FILEIO_TEST || defined TEST_ALL

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration
#include <boost/filesystem/path.hpp> // Portable paths
#include <boost/filesystem/operations.hpp> // Portable paths
#include <boost/filesystem/exception.hpp> //portable file system exceptions
#include <boost/lexical_cast.hpp>		//safe lexical casts
#include <iostream> //cout, cerr

#include "../FunctionObjects.cpp" //for random num generator
#include "../FileIO.hpp"

namespace fs = boost::filesystem;


struct FileIOTest_Fixture
{
	//ctor
    FileIOTest_Fixture()  
    {}

	//dtor
    ~FileIOTest_Fixture() 
    {}

	FileIO myFileIO;
	std::string str_;
	std::string path_;

};

BOOST_AUTO_TEST_SUITE( FileIOTests );

BOOST_AUTO_TEST_CASE( FileIOTest_Read )
{
	FileIOTest_Fixture f;

	//1
	BOOST_MESSAGE("purpose: test opening file using FileIO");
	f.path_ = "../data/TestFileIO/file1.txt" ;
	std::string s;

	try
	{
		f.myFileIO.read( f.path_, s );
	}
	//catch( const std::exception& e )
	catch( const fs::filesystem_error& e )
	{
		std::cerr <<  e.what() << std::endl;
		std::cerr <<  e.error() << std::endl;
	    std::cerr << "Exception Type " << typeid(e).name() << std::endl;

		BOOST_ERROR("Should not have caught an error.");
	}

	BOOST_CHECK_EQUAL( "hi there", s ); 

	//2
	BOOST_MESSAGE("purpose: test opening file with same stringstream");

	f.path_ =  "../data/TestFileIO/file2.txt";

	try
	{
		f.myFileIO.read( f.path_, s );
	}
	catch( const fs::filesystem_error& e )
	{
		std::cerr <<  e.what() << std::endl;
		std::cerr <<  e.error() << std::endl;
	    std::cerr << "Exception Type " << typeid(e).name() << std::endl;

		BOOST_ERROR("Should not have caught an error.");
	}

	BOOST_CHECK_EQUAL( "hi therebrown bear", s );
}

// helper function for testing fs::not_found_error exceptions with BOOST_CHECK_EXCEPTION
bool isNotFoundError( const fs::filesystem_error& ex )
{
	return ex.error() == fs::not_found_error;
}

// helper function for testing fs::not_found_error exceptions with BOOST_CHECK_EXCEPTION
bool isDirError( const fs::filesystem_error& ex )
{
	return ex.error() == fs::is_directory_error;
}

// helper function for testing fs::not_found_error exceptions with BOOST_CHECK_EXCEPTION
bool is_portable_name_error( const fs::filesystem_error& ex )
{
	//std::cout << "is_portable_name_error()" << std::endl;
	//std::cout << ex.error() << std::endl;
	//std::cout << ex.what() << std::endl;
	//std::cout << ex.who() << std::endl;
	//std::cout << "" << std::endl;

	return ex.error() == boost::filesystem::path_error;
}

bool is_other_error( const fs::filesystem_error& ex )
{

	return ex.error() == boost::filesystem::other_error;
}

// helper function for testing fs::not_found_error exceptions with BOOST_CHECK_EXCEPTION
bool is_system_error( const fs::filesystem_error& ex )
{
	return ex.error() == boost::filesystem::system_error;
}

BOOST_AUTO_TEST_CASE( FileIOTest_read_exceptions )
{

	FileIOTest_Fixture f;

	f.path_ = "nofile";
	std::string s;

	//3
	BOOST_MESSAGE("purpose: test missing file");


	try
	{
		f.myFileIO.read( f.path_, s );
	}
	catch( const fs::filesystem_error& e )
	{
	 //   std::cerr << "Exception Type " << typeid(e).name() << std::endl;
		//std::cerr <<  e.what() << std::endl;
 
		BOOST_CHECK_EQUAL( e.error(), fs::not_found_error );
	}

	BOOST_CHECK_THROW( f.myFileIO.read( f.path_, s ), fs::filesystem_error);
	BOOST_CHECK_EXCEPTION( f.myFileIO.read( f.path_, s ), fs::filesystem_error, isNotFoundError  );

	//4
	f.path_ = "../data/TestFileIO";
	BOOST_MESSAGE("purpose: opening directory instead of file");
	BOOST_CHECK_EXCEPTION( f.myFileIO.read( f.path_, s ), fs::filesystem_error, isDirError  );
} 

BOOST_AUTO_TEST_CASE( FileIOTest_write_exceptions )
{//TODO path throw exceptions for bad leafs
	FileIOTest_Fixture f;

	f.path_ = "../data/TestFileIO/testwrite1::.txt";//non portable name

	f.str_.append( "exception test" );
//	BOOST_CHECK_EXCEPTION( f.myFileIO.write( f.path_ , f.str_ ), fs::filesystem_error, is_other_error  );

	//TODO exception for system error and not found
}

BOOST_AUTO_TEST_CASE( FileIOTest_write )
{
	FileIOTest_Fixture f;
	f.path_ = "../data/TestFileIO/testwrite1.txt";

	//clear previous file
	if( fs::exists( f.path_ ) )
	{
		fs::remove( f.path_ );
	}

	BOOST_CHECK_EQUAL( fs::exists( f.path_ ), false );


	//write
	std::string _test_text( "jackie brown" );
	f.myFileIO.write( f.path_, _test_text );
	
	//read
	try
	{
		f.myFileIO.read( f.path_, f.str_ ); 
	}
	catch( const fs::filesystem_error& e )
	{
		e.what();
		BOOST_ERROR( "couldn't read file for written as a test" );
	}

		//write a file, then read it back in and compare to verify
	
	//test
	BOOST_CHECK_EQUAL( f.str_ ,"jackie brown");
}

BOOST_AUTO_TEST_CASE( FileIOTest_append )
{
	FileIOTest_Fixture f;
	f.path_ = "../data/TestFileIO/testappend.txt";

	//clear previous file
	if( fs::exists( f.path_ ) )
	{
		fs::remove( f.path_ );
	}

	BOOST_CHECK_EQUAL( fs::exists( f.path_ ), false );

	//append 1st time
	std::string _test_text("jackie brown");
	f.myFileIO.append( f.path_, _test_text );

	//append 2nd time
	std::string _test_text2(" is pam grier");
	f.myFileIO.append( f.path_, _test_text2 );

	//read
	try
	{
		f.myFileIO.read( f.path_, f.str_ ); 
	}
	catch( const fs::filesystem_error& e )
	{
		e.what();
		BOOST_ERROR( "couldn't read file for written as a test" );
	}

	//test
	BOOST_CHECK_EQUAL( f.str_ ,"jackie brown is pam grier");

	//write, deletes old changes
	std::string _test_text3("hey now ");
	f.myFileIO.write( f.path_, _test_text3 );

	//append
	std::string _test_text4("quentin");
	f.myFileIO.append( f.path_, _test_text4 );

	//clear input stream
	f.str_.assign("");//clears out stringstream

	//read
	try
	{
		f.myFileIO.read( f.path_, f.str_ ); 
	}
	catch( const fs::filesystem_error& e )
	{
		e.what();
		BOOST_ERROR( "couldn't read file for written as a test" );
	}

	BOOST_CHECK_EQUAL( f.str_ ,"hey now quentin");
}

BOOST_AUTO_TEST_CASE( FileIOTest_write_create_new_directories_if_needed )
{
	//create seeded random file name
	RandInt r;
	unsigned int rand1 = r(100000);
	unsigned int rand2 = r(100000);

	std::string rand_path1 = boost::lexical_cast<std::string>( rand1 );
	std::string rand_path2 = boost::lexical_cast<std::string>( rand2 );

	//TODO user random to create new folder
	FileIOTest_Fixture f;
	std::string str_path = "../data/TestFileIO/" + rand_path1 + "/" + rand_path2 + "/test1.txt";
	f.path_ = str_path;

	BOOST_CHECK_EQUAL( fs::exists( f.path_ ), false );

	//write
	std::string _test_text( "jackie brown" );
	f.myFileIO.write( f.path_, _test_text );
	
	//read
	try
	{
		f.myFileIO.read( f.path_, f.str_ ); 
	}
	catch( const fs::filesystem_error& e )
	{
		e.what();
		BOOST_ERROR( "couldn't read file for written as a test" );
	}

		//write a file, then read it back in and compare to verify
	
	//test
	BOOST_CHECK_EQUAL( f.str_ ,"jackie brown");

	//CLEAN UP. test files and directories from this unit test
	std::string str_path1 = "../data/TestFileIO/" + rand_path1;
	fs::remove_all( str_path1 );

}

BOOST_AUTO_TEST_CASE( FileIOTest_write_same_file_twice )
{

	//TODO user random to create new folder
	FileIOTest_Fixture f;
	std::string str_path = "../data/TestFileIO/test_twice.txt";
	f.path_ = str_path;

	BOOST_CHECK_EQUAL( fs::exists( f.path_ ), false );

	//write
	std::string _test_text( "jackie brown" );
	f.myFileIO.write( f.path_, _test_text );
	
	//read
	try
	{
		f.myFileIO.read( f.path_, f.str_ ); 
	}
	catch( const fs::filesystem_error& e )
	{
		e.what();
		BOOST_ERROR( "couldn't read file for test_twice as a test" );
	}

	//test
	BOOST_CHECK_EQUAL( f.str_ ,"jackie brown");

	//write 2nd time
	_test_text =  "angelina jolie";
	f.myFileIO.write( f.path_, _test_text );
	
	//read 2nd time
	std::string input;
	try
	{
		f.myFileIO.read( f.path_, input); 
	}
	catch( const fs::filesystem_error& e )
	{
		e.what();
		BOOST_ERROR( "couldn't read file for test_twice as a test" );
	}

	//test
	BOOST_CHECK_EQUAL( input ,"angelina jolie");

	//CLEAN UP. test files and directories from this unit test
	//std::string str_path1 = "../data/TestFileIO/;
	//fs::remove_all( str_path1 );
	fs::remove( f.path_ );
}

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  FILEIO_TEST || defined TEST_ALL
#endif //FILEIOTEST_HPP
