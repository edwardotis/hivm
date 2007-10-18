//@purpose  encapsulate the logic for running a 10 fold cross validation experiement

#ifndef SELFTESTEXPERIMENT_HPP
#define SELFTESTEXPERIMENT_HPP
//

#include "PreProcWorkUnit.hpp"
#include "ExperimentResult.hpp"
#include "Options.hpp"

class SelfTestExperiment
{

public:

	//default constructor
	SelfTestExperiment();

	//default destructor
	virtual ~SelfTestExperiment();


//NEXT STEP
//	record percentage of correct results...ignoring true pos, false pos.
//
//	hits/total = percent correct.
//
////ah, shit. I forgot to do self test experiment class
//	train with everything. (use copy constructor?)
//	predict on everything
//	use defaults
//
//	find median value for threshold in Excel.
//	compare results. ignoring true/false. what percentage of the time was it correct at 
//	predicting?

	//ExperimentResult class?
	//ExperimentResult( const SvmWUMatrix& predictees, 
	//	std::vector<bool> susceptibility_results );

	//ExperimentResult::save_results();


	/**
	@purpose 	train with everything. 	predict on everything.	use defaults
	see what the results are 
	@return accuracy as defined by hits/total_predictions
	*/
	//void run_experiment( const PreProcWUSet& input, ExperimentResult* result );

	/**
	@purpose 	train with everything. 	predict on everything.	use defaults
	see what the results are 
	@params custom cost and gamma parameters
	@return accuracy as defined by hits/total_predictions
	*/
	void run_experiment( const PreProcWUSet& input, ExperimentResultSet& results, const Options& svm_param_options );
	
	//@purpose save experiment details and results to file. 
	//should write to stdout too.
	//maybe this is inside run_experiment
	//void save_experiment();


//private:
//void run_experiment_( const PreProcWUSet& input, ExperimentResult* result,  const Options& svm_param_options, double log_cost, double log_gamma );


private:

	//copy constructor
	SelfTestExperiment( const SelfTestExperiment& );

	//assignment operator
	SelfTestExperiment& operator= ( const SelfTestExperiment& );

};

#endif//SELFTESTEXPERIMENT_HPP
