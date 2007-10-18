#ifndef SVMWORKUNIT_HPP
#define SVMWORKUNIT_HPP

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Class SvmWUSet
//A Collection of local alignment scores for a given isolate


#include <vector>
#include <string>

class SvmWUSet : public std::vector< double >//each double is LA score
{

public:
	SvmWUSet( std::string id, bool known_susceptibility );
	virtual ~SvmWUSet();

	//@returns resistance
	//void isResistant( bool );
	bool known_susceptibility();

	//@returns isolate name
	std::string get_id();

private:

	SvmWUSet();

	std::string _id;
	bool _known_susceptibility;	

	//copy constructor
	SvmWUSet( const SvmWUSet& );

	//assignment operator
	SvmWUSet& operator= ( const SvmWUSet& );


};

 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 //Class SvmWUMatrix
	//A Collection of SvmWorkUnitSets.
class SvmWUMatrix : public std::vector< SvmWUSet *>
{

public:
	SvmWUMatrix();
	virtual ~SvmWUMatrix();

private:
	
	//copy constructor
	SvmWUMatrix( const SvmWUMatrix& );

	//assignment operator
	SvmWUMatrix& operator= ( const SvmWUMatrix& );


};

#endif //SVMWORKUNIT_HPP
