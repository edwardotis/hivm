#ifndef PREPROCWORKUNIT_HPP
#define PREPROCWORKUNIT_HPP

#include <string>

class PreProcWorkUnit{

public: 

	//default constructor
	PreProcWorkUnit();	

	//consturctor
	PreProcWorkUnit( std::string id, std::string data, int known_susceptibility );	

	//destructor
	virtual ~PreProcWorkUnit();

	//@returns resistance
	//void isResistant( bool );
        int known_susceptibility() const;

	//@returns amino acid sequence
	std::string get_data() const;

	//@returns unique identifier isolate name
	std::string get_id() const;
	//void setID( std::string isolateName);

	//copy constructor
	PreProcWorkUnit( const PreProcWorkUnit& );

private:

	std::string _id;
	int _known_susceptibility;
	std::string _data;

	//assignment operator
	PreProcWorkUnit& operator= ( const PreProcWorkUnit& );

};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class PreProcWUSet
//	A Collection of PreProcWorkUnits.
//

#include <vector>

class PreProcWUSet : public std::vector< PreProcWorkUnit *>
{

public:
	PreProcWUSet();
	virtual ~PreProcWUSet();

private:

	//copy constructor
	PreProcWUSet( const PreProcWUSet& );

	//assignment operator
	PreProcWUSet& operator= ( const PreProcWUSet& );


};

#endif //PREPROCWORKUNIT_HPP
