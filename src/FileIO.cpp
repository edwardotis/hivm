#include "FileIO.hpp"
#include "Log.hpp"//logging

#include <sstream> //stringstreams
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp> //path based file streams
#include <boost/filesystem/operations.hpp> //provides operations on files and directories
#include <boost/filesystem/convenience.hpp>//provides convenience functions that combine lower-level functions in useful ways
#include <boost/algorithm/string.hpp> //trimming of strings
#include <iostream>


FileIO::FileIO()
{}

FileIO::~FileIO()
{}

void FileIO::_write( const boost::filesystem::path _path, const std::string& output, bool isAppend )
{
	if( !boost::filesystem::portable_name( _path.leaf() ) )
	{
		fs::error_code ec = fs::path_error;
		throw( fs::filesystem_error( "FileIO", _path, "Non OS Portable File Name", ec ) );		
	}

	//create directories
	try
	{
		boost::filesystem::create_directories( _path.branch_path() );
	}
	catch( const boost::filesystem::filesystem_error& e )
	{
		throw( e );
	}

	fs::ofstream outFile;
	
	if( isAppend == true )
	{
		outFile.open( _path, std::ios::app);
	}
	else
	{
		outFile.open( _path, std::ios::out);
	}
	outFile << output;
	outFile.close();
	
	//error check file exists after writing it
	if ( !fs::exists( _path ) )
	{
		fs::error_code ec = fs::not_found_error;
		throw( fs::filesystem_error( "FileIO", _path, "File Not Written", ec ) );
	}

	if ( outFile.fail() )
	{
		fs::error_code ec = fs::system_error;
		throw( fs::filesystem_error( "FileIO", _path, "File Not Written", ec ) );
	}
}

void FileIO::write( const std::string path, const std::string& output)
{
	try
	{
		bool is_append = false;
		_write( _makePath( path ), output, is_append);
	}
	catch( const boost::filesystem::filesystem_error& e )
	{
		//Log::append( "Error in: " );
		//Log::append( "void FileIO::write( const std::string path, const std::string& output)");
		//Log::append( e.what() );
		throw( e );
	}

}

fs::path FileIO::_makePath( const std::string _str_path )
{
	try //fs::path throws error on malformed paths
	{
		return fs::path( _str_path );
	}
	catch( const boost::filesystem::filesystem_error& e )
	{
		//returns error code 2, "other_error"
		//I think it should return code 6, "path_error"
		throw( e );
	}
}

void FileIO::append( const std::string path,  const std::string& output)
{
	try
	{
		bool is_append = true;
		_write( _makePath( path ), output, is_append);
	}
	catch( const boost::filesystem::filesystem_error& e )
	{
		//Log::append( "Error in: " );
		//Log::append( "void FileIO::append( const std::string path,  const std::string& output)");
		//Log::append( e.what() );
		throw( e );
	}
}

void FileIO::read( const std::string path, std::string& input)
{
	fs::path _path;
	try
	{
		_path = _makePath( path );
	}
	catch( const boost::filesystem::filesystem_error& e )
	{
		//Log::append( "Error in: " );
		//Log::append( "void FileIO::read( const std::string path, std::string& input)");
		//Log::append( e.what() );

		throw( e );
	}

	if ( !fs::exists( _path ) )
	{
		//Log::append( "Error in: " );
		//Log::append( "void FileIO::read( const std::string path, std::string& input)");
		//Log::append( "fs::filesystem_error( \"FileIO\", _path, \"File Not Found\", fs::not_found_error )" );

		fs::error_code ec = fs::not_found_error;
		throw( fs::filesystem_error( "FileIO", _path, "File Not Found", ec ) );
	}

	if ( fs::is_directory( _path ) )
	{
		//Log::append( "Error in: " );
		//Log::append( "void FileIO::read( const std::string path, std::string& input)");
		//Log::append( "fs::filesystem_error( \"FileIO\", _path, \"File Not Found\", fs::not_found_error )" );

		fs::error_code ec = fs::is_directory_error;
		throw( fs::filesystem_error( "FileIO", _path, "Is Directory, Not File", ec ) );
	}

	fs::ifstream inFile;

	inFile.open( _path, std::ios::in );

	std::string line;
	std::string temp_str;
	while ( !inFile.eof() )
	{
		std::getline( inFile, line );
		temp_str.append( line + "\n" );
		//std::cout << line;
	}

	//remove all the extra line breaks. (sometimes getline() appends an extra
	//line break on the last line of a text file)
	input.append( boost::algorithm::trim_right_copy_if( 
										temp_str,  
										std::bind2nd( std::equal_to<char>(), '\n')
										) 
	);
	
	inFile.close();
}
