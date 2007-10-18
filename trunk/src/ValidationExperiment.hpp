//@purpose  encapsulate the logic for prediction on the known test data set.
//model-validation

#ifndef VALIDATIONEXPERIMENT_HPP
#define VALIDATIONEXPERIMENT_HPP

#include "Types.hpp"// global typdefs
#include "PreProcWorkUnit.hpp"
#include "ExperimentResult.hpp"
#include "Options.hpp"

class ValidationExperiment
{

public:

	//default constructor
	ValidationExperiment();

	//default destructor
	virtual ~ValidationExperiment();

	//	record percentage of correct results...ignoring true pos, false pos.

	//hits/total = percent correct.

	/**
	@purpose run experiment 
	@params training data set and test data set
	@params cost and gamma parameters from CrossValidation Experiment
	@return Results of ValidationExperiment
	*/
	void run_experiment( 
		const PreProcWUSet& training_data_set, 
		const PreProcWUSet& test_data_set, 
		const Options& svm_param_options,
		ExperimentResult* result
		);

private:

	//copy constructor
	ValidationExperiment( const ValidationExperiment& );

	//assignment operator
	ValidationExperiment& operator= ( const ValidationExperiment& );

};

#endif//VALIDATIONEXPERIMENT_HPP
