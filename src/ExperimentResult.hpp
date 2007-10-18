//@purpose Responsible for containing data and setup of an experiment
//Prediction uses a single ExperimentResult to handle entire test dataset.
//Model Selection uses ExperimentResultSet because of 10fold cross validation.
#ifndef ExperimentResult_HPP
#define ExperimentResult_HPP

#include <string>
#include <vector>
#include "Types.hpp"//typedefs

//@purpose struct to hold information about a single prediction
//probabilities are stored in ascending order according to label.
//i.e. -1,1 aka resistant, susceptible
struct data_point
{
	//constructor
	data_point(
		bool _predicted_susceptibility,
		bool _actual_susceptibility,
		std::string _id,
		std::vector<double> probabilities_
		)
	{
		predicted_susceptibility = _predicted_susceptibility;
		actual_susceptibility = _actual_susceptibility;
		id = _id;
		probabilities = probabilities_;
	}

	//default destructor
	~data_point(){}
	
	bool predicted_susceptibility;
	bool actual_susceptibility;
	std::string id;//isolate name
	std::vector<double> probabilities;
	//probabilities[0] = resistance probability
	//probabilities[1] = susceptibility probability
};

class ExperimentResult : public std::vector< struct data_point* >
{

public:

	//default constructor
	ExperimentResult();

	//constructor
	ExperimentResult( double _lg_cost, double _lg_gamma );

	//copy constructor
	ExperimentResult( const ExperimentResult& );

	//default destructor
	virtual ~ExperimentResult();

	/**
	@purpose add a data point by individual datum
	@param probabilities should receive an empty vector of doubles if libsvm
	probabilities option has NOT been set.
	*/
	void add( bool predicted_susceptibility, bool actual_susceptibility,
		const std::string id, const std::vector<double>& probabilities_ );

	double lg_cost;
	double lg_gamma;

private:

	//assignment operator
	ExperimentResult& operator= ( const ExperimentResult& );
};


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class ExperimentResultSet
//	A Collection of ExperimentResults.
//

class ExperimentResultSet : public std::vector< ExperimentResult *>
{

public:

	//default destructor
	virtual ~ExperimentResultSet();

	//copy constructor
	ExperimentResultSet( const ExperimentResultSet& );

	//default constructor
	ExperimentResultSet();

	void add( ExperimentResult* );


private:

	//assignment operator
	ExperimentResultSet& operator= ( const ExperimentResultSet& );

};


#endif //ExperimentResult_HPP

