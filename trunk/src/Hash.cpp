#include "Hash.hpp"
#include <boost/lexical_cast.hpp> //safe numerical to string casts

//TODO DRY
std::string Hash::hash( const SvmWUMatrix& set )
{
	std::string str;

	for( int i = 0; i < set.size(); i++ )
	{
		str.append( set.at(i)->get_id() );
	}

	unsigned long id = Hash::hshstrhash( str.c_str() );

	std::string result = boost::lexical_cast<std::string>( id );
	return result;
}

//TODO DRY 
std::string Hash::hash( const PreProcWUSet&  set )
 {
	std::string str;

	for( int i = 0; i < set.size(); i++ )
	{
		str.append( set.at(i)->get_id() );
	}

	unsigned long id = Hash::hshstrhash( str.c_str() );

	std::string result = boost::lexical_cast<std::string>( id );
	return result;
 }

//STATIC
unsigned long Hash::hshstrhash( const char *string )
{
   unsigned long h;
   h = 0;

   while (*string)
   {
      h = h *31UL + (unsigned char) *string++;
   }

   return h; 	
}

Hash::Hash(){}

Hash::~Hash(){}
