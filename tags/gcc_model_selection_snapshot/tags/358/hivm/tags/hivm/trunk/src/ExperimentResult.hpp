//@purpose Responsible for containing data and setup of an experiment
//TODO cost_low, increment etc. can be determined from the datapointset
//I think this is fine, but may be problem later.
//Prediction uses a single ExperimentResult to handle entire test dataset.
//Model Selection uses ExperimentResultSet because of 10fold cross validation.
#ifndef ExperimentResult_HPP
#define ExperimentResult_HPP

#include <string>
#include <vector>
#include "Types.hpp"//typedefs

class ExperimentResult
{

public:

//	struct data_point_set;//forward declaration

	//constructor
	ExperimentResult(
		double _log_cost,
		double _log_gamma
		);

//TODO move all except cost and gamma up
//to level of ExperimentResultSet

	//default destructor
	virtual ~ExperimentResult();

	//TODO make all member data private
	//TODO make unit tests functions into friends to test private

//private:
	struct data_point
	{
		//constructor
		data_point(
			bool _predicted_susceptibility,
			bool _actual_susceptibility,
			std::string _id
			)
		{
			predicted_susceptibility = _predicted_susceptibility;
			actual_susceptibility = _actual_susceptibility;
			id = _id;
		}

		//default destructor
		~data_point(){}
		
		bool predicted_susceptibility;
		bool actual_susceptibility;
		std::string id;//isolate name
	};


	//collection of datapoints
	struct data_point_set
	{
		std::vector< struct data_point* > data_points;

		//default constructor
		data_point_set(){}

		//default destructor
		~data_point_set()
		{
			for( size_t i=0; i < data_points.size(); i++)
			{
				delete data_points.at( i );
			}
			data_points.clear();
		}

	} datum;

	//my instance of data_point_set
	//struct data_point_set *datum;

	double log_cost;
	double log_gamma;
	
	//copy constructor
	ExperimentResult( const ExperimentResult& );

private:

	//assignment operator
	ExperimentResult& operator= ( const ExperimentResult& );

	//default constructor
	ExperimentResult();
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class ExperimentResultSet
//	A Collection of ExperimentResults.
//

class ExperimentResultSet : public std::vector< ExperimentResult *>
{

public:

	ExperimentResultSet(
		std::string _experiment_type,
		std::string _drug,
		double _fold_threshold,
		std::string _wild_seq_path,
		std::string _test_data_path,
		size_t _num_training_seqs
		);

	//default destructor
	virtual ~ExperimentResultSet();

	//copy constructor
	ExperimentResultSet( const ExperimentResultSet& );

	//default constructor
	ExperimentResultSet();

	//DEPRECATE
	void set_params(
		std::string _experiment_type,
		std::string _drug,
		double _fold_threshold,
		std::string _wild_seq_path,
		std::string _test_data_path,
		size_t _num_training_seqs
		);

	void set_seed( size_t );
	size_t get_seed();

//private:
	//information about the experiment variables
	//should be enought to recreate any experiment
	std::string experiment_type;
    std::string drug;
	double fold_threshold;
	std::string wild_seq_path;
	std::string test_data_path;
	size_t num_training_seqs;
	size_t seed;

private:


	//assignment operator
	ExperimentResultSet& operator= ( const ExperimentResultSet& );

};


#endif //ExperimentResult_HPP

