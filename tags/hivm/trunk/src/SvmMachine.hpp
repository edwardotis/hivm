//@purpose  encapsulate the logic for running parameter searches on Experiments

#ifndef SVMMACHINE_HPP
#define SVMMACHINE_HPP

//#include "Types.hpp"// global typdefs
//#include "SvmWorkUnit.hpp"
#include "PreProcWorkUnit.hpp"
#include "SelfTestExperiment.hpp"
#include "ExperimentResult.hpp"
#include "CrossValidationExperiment.hpp"
#include "Options.hpp"	

class SvmMachine
{

public:

	//default constructor
	SvmMachine();

	//default constructor
	//SvmMachine( Options& );

	//default destructor
	virtual ~SvmMachine();

	void run( const std::string purpose, const Options& options );//choose purpose
	void cross_validate( const Options& options );//param search and crossval on train data
	void self_validate( const Options& options );//param search and full selftest on train data
	void test( const Options& options );//run single c,g pair on test data

	//future:
	//SvmMachine::train();
	//SvmMachine::classify();
	
	/**
	@purpose find best cost and gamma parameters for an Experiment
	Best pair is determined by best accuracy
	//thoughts: test to see if more than one pair generates best accuracy
	//TODO add seed for shuffling?
    */
	//add ExperimentResult param
	void parameter_search( 
		SelfTestExperiment&,
		const PreProcWUSet& input_data,
		const Options& svm_param_options,
		const double log_cost_low,
		const double log_cost_high,
		const double log_cost_increment,
		const double log_gamma_low,
		const double log_gamma_high,
		const double log_gamma_increment,
		ExperimentResultSet& result_set
		);

	void parameter_search( 
		CrossValidationExperiment&,
		const PreProcWUSet& input_data,
		const Options& svm_param_options,
		const double log_cost_low,
		const double log_cost_high,
		const double log_cost_increment,
		const double log_gamma_low,
		const double log_gamma_high,
		const double log_gamma_increment,
		ExperimentResultSet& result_set
		);

//private:


private:

//	Options& options;

	//copy constructor
	SvmMachine( const SvmMachine& );

	//assignment operator
	SvmMachine& operator= ( const SvmMachine& );

};

#endif//SVMMACHINE_HPP
