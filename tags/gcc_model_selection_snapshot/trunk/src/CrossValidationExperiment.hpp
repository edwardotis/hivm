//@purpose  encapsulate the logic for running a 10 fold cross validation experiement

#ifndef CROSSVALIDATIONEXPERIMENT_HPP
#define CROSSVALIDATIONEXPERIMENT_HPP

#include "Types.hpp"// global typdefs
#include "SvmWorkUnit.hpp"
#include "PreProcWorkUnit.hpp"
#include "ExperimentResult.hpp"
#include "Options.hpp"

class CrossValidationExperiment
{

public:

	//default constructor
	CrossValidationExperiment();

	//default destructor
	virtual ~CrossValidationExperiment();


//NEXT STEP
	//for 10 folds
	//	record percentage of correct results...ignoring true pos, false pos.

	//hits/total = percent correct.

	/**
	@purpose run 10-fold cross-validation of training data
	@params custom cost and gamma parameters
	@return Results in ExperimentResult
	*/
	void run_experiment( const PreProcWUSet& input, ExperimentResult* result,
		const Options& svm_param_options );
	

//private:
	/**
	//@purpose setup a fold for cross validation
	//@param fold 0-9 for 10 fold cross validation
	@pre SvmWUMatrices for output should be empty
	*/
	void createFold( const PreProcWUSet& input, int fold, 
		SvmWUMatrix& trainset_output,
		SvmWUMatrix& predictees_output
		);

private:

	//copy constructor
	CrossValidationExperiment( const CrossValidationExperiment& );

	//assignment operator
	CrossValidationExperiment& operator= ( const CrossValidationExperiment& );

};

#endif//CROSSVALIDATIONEXPERIMENT_HPP
