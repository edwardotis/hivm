/**
 * @class FileIO
 * Class to simplify hivm File Input/Output
 */

#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp> //portable file system exceptions

namespace fs = boost::filesystem;

class FileIO{

//friend void FileIOTest_Private(  );//unit tests

public: 

	//default constructor
	FileIO();	

	//destructor
	virtual ~FileIO();

	/**
	@purpose read in a file from the file system
	@throws boost::filesystem::filesystem_error
	*/
	static void read( const std::string path, std::string& input);

	/**
	@purpose write a file to the file system, 
	overwriting current file if it exists
	@throws boost::filesystem::filesystem_error
	*/
	static void write( const std::string path,  const std::string& output);

	/**
	@purpose write a file to the file system, appending to
	current file if it exists
	@throws boost::filesystem::filesystem_error
	*/
	static void append( const std::string path,  const std::string& output);

private:

	//TODO remove this
	////error checking code
	//fs::error_code _errorCheck( fs::path );

	//private class to handle most of write and append
	static void _write( const boost::filesystem::path _path, const std::string& output, bool isAppend );
	
	static fs::path _makePath( const std::string );

	//copy constructor
	FileIO( const FileIO& );

	//assignment operator
	FileIO& operator= ( const FileIO& );

	
};

#endif //FILEIO_HPP
