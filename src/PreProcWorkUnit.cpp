#include "PreProcWorkUnit.hpp"

PreProcWorkUnit::PreProcWorkUnit() :
_id( "" ),
_data( "" ),
_known_susceptibility( 0 )
{}

PreProcWorkUnit::PreProcWorkUnit(std::string id, std::string data, int known_susceptibility) :
_id( id ),
_data( data ),
_known_susceptibility( known_susceptibility )
{}

PreProcWorkUnit::~PreProcWorkUnit(){}

//copy constructor
PreProcWorkUnit::PreProcWorkUnit( const PreProcWorkUnit& rhs) :
_id( rhs.get_id() ),
_data( rhs.get_data() ),
_known_susceptibility( rhs.known_susceptibility() )
{}

int PreProcWorkUnit::known_susceptibility() const
{
  return _known_susceptibility;
}

std::string PreProcWorkUnit::get_data() const
{
	return _data;
}

std::string PreProcWorkUnit::get_id() const
{
	return _id;
}

//--------PreProcWUSet-----------
PreProcWUSet::PreProcWUSet(){}

PreProcWUSet::~PreProcWUSet()
{
	//free memory
	std::vector< PreProcWorkUnit *>::const_iterator it;
	for( it = this->begin(); it != this->end(); it++ )
	{
		PreProcWorkUnit* wu = (*it);
		delete wu;
		wu = NULL;
	}

	//removes all elements from the vector
	this->clear();
}
