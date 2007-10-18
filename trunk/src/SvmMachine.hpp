//@purpose  encapsulate the logic for running parameter searches on Experiments

#ifndef SVMMACHINE_HPP
#define SVMMACHINE_HPP

#include "PreProcWorkUnit.hpp"
#include "ExperimentResult.hpp"
#include "CrossValidationExperiment.hpp"
#include "Options.hpp"	

class SvmMachine
{

friend class SvmMachineTest;//unit tests
friend class ExperimentAnalyzerTest;//unit tests

public:

	//default constructor
	SvmMachine();

	//default destructor
	virtual ~SvmMachine();

	/**
	@purpose Run appropriate function of SvmMachine based on options
	*/
	void run( const std::string purpose, Options& options );//choose purpose

private:

	void model_select( Options& options );//param search and crossval on train data
	void model_validate( const Options& options );//run single c,g pair on test data
	
	/**
	@purpose Place holder for future function. 
	Predict should be created based on the use case for a doctor providing actual
	unknown HIV sequences to hivm, and wanting appropriate prediction information back.
	*/
	void predict( const Options& options );

	/**
	@purpose find best cost and gamma parameters for an Experiment
	Best pair is determined by best accuracy
	//thoughts: test to see if more than one pair generates best accuracy
	//TODO add seed for shuffling?
    */
	//add ExperimentResult param

	void parameter_search( 
		CrossValidationExperiment&,
		const PreProcWUSet& input_data,
		Options& svm_param_options,
		const double lg_cost_low,
		const double lg_cost_high,
		const double lg_cost_inc,
		const double lg_gamma_low,
		const double lg_gamma_high,
		const double lg_gamma_inc,
		ExperimentResultSet& result_set
		);


	/**
	@purpose Keep alignment straight for output grid search dots.
	*/
	void format_grid_search_output_( double lg_cost );

//	Options& options;

	//copy constructor
	SvmMachine( const SvmMachine& );

	//assignment operator
	SvmMachine& operator= ( const SvmMachine& );

};

#endif//SVMMACHINE_HPP
