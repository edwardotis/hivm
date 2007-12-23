#include "SvmWorkUnit.hpp"

////--------SvmWorkUnit-----------
//
//SvmWorkUnit::SvmWorkUnit() :
//_la_score(0)
//{}
//
//SvmWorkUnit::SvmWorkUnit( double la_score ) :
//_la_score( la_score )
//{}
//
//SvmWorkUnit::~SvmWorkUnit(){}
//
//double SvmWorkUnit::get_la_score()
//{
//	return _la_score;
//}

//--------SvmWUSet-----------

//SvmWUSet::SvmWUSet() :
//_id( "" ),
//_known_susceptibility( false )
//{}

SvmWUSet::SvmWUSet( std::string id, int known_susceptibility ) :
  _id( id ),
  _known_susceptibility( known_susceptibility )
{
}

int SvmWUSet::known_susceptibility()
{
  return _known_susceptibility;
}

std::string SvmWUSet::get_id()
{
  return _id;
}

SvmWUSet::~SvmWUSet()
{
  ////free memory
  //std::vector< SvmWorkUnit *>::const_iterator it;
  //for( it = this->begin(); it != this->end(); it++ )
  //{
  //	SvmWorkUnit* wu = (*it);
  //	delete wu;
  //	wu = NULL;
  //}
  
  ////removes all elements from the vector
  //this->clear();
}

//--------SvmWUMatrix-----------

SvmWUMatrix::SvmWUMatrix(){}

SvmWUMatrix::~SvmWUMatrix()
{
  //free memory
  std::vector< SvmWUSet *>::const_iterator it;
  for( it = this->begin(); it != this->end(); it++ )
    {
      SvmWUSet* wu_set = (*it);
      delete wu_set;
      wu_set = NULL;
    }
  
  //removes all elements from the vector
  this->clear();
}
