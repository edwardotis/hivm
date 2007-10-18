#ifndef HASH_H	
#define HASH_H

#include "SvmWorkUnit.hpp"
#include "PreProcessor.hpp"
#include <string>

class Hash
{

public:

	/*
	@purpose Hash a string quantity 
	@credit C.B. Falconer
	@url http://cbfalconer.home.att.net/
	*/
	 static unsigned long hshstrhash( const char *string );

	 //@purpose createa a unique id based on a PreProcWUSet's ids and their order
	 //id == IsolateName. which is unique.
	 static std::string hash( const SvmWUMatrix& );

	 static std::string hash( const PreProcWUSet& );

private:
	
	//constructor
	Hash();

	//destructor
	~Hash();

	//copy constructor
	Hash( const Hash& );

	//assignment operator
	Hash& operator= ( const Hash& );
};

#endif

