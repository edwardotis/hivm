#include "SelfTestExperiment.hpp"
#include "BioProcessor.hpp"
#include "SvmWorkUnit.hpp"
#include "Types.hpp"// global typdefs
#include "LibSvmAdapter.hpp"

	//default constructor
SelfTestExperiment::SelfTestExperiment()
{}

	//default destructor
SelfTestExperiment::~SelfTestExperiment()
{}

void SelfTestExperiment::run_experiment( const PreProcWUSet& input, ExperimentResultSet& results,
										const Options& svm_param_options )
{
		//make copy of input
	PreProcWUSet predictee_input;
	for( size_t i = 0; i < input.size(); i++ )
	{
		PreProcWorkUnit* wu = new PreProcWorkUnit( *(input.at(i)) );
		predictee_input.push_back( wu );
	}
	
	//prepare for svm
	SvmWUMatrix svm_trainset;
	SvmWUMatrix svm_predictees;

	BioProcessor b;
	b.process_with_cache( input, predictee_input, svm_trainset, svm_predictees );

	LibSvmAdapter l;
	l.predict_susceptibility( svm_trainset, svm_predictees, svm_param_options, results );

	//LibSvmAdapter l;
	//std::vector<bool> susceptibility_predicted_results = l.predict_susceptibility( svm_trainset, svm_predictees, svm_param_options );

	//Collect each Result here:
	//for( size_t i=0; i < susceptibility_predicted_results.size(); i++ )
	//{
	//	result->add_data( 
	//		susceptibility_predicted_results.at(i),
	//		svm_predictees.at(i)->known_susceptibility(),
	//		svm_predictees.at(i)->get_id()
	//		);

	//	//struct ExperimentResult::data_point* dp = new ExperimentResult::data_point(
	//	//	susceptibility_predicted_results.at(i), 
	//	//	svm_predictees.at(i)->known_susceptibility(),
	//	//	svm_predictees.at(i)->get_id()
	//	//	);
	//	//
	//	//result->data.data_points.push_back( dp );
	//}
	//return run_experiment_( input, result, svm_param_options, log_cost, log_gamma );
}

//TODO depracated
//void SelfTestExperiment::run_experiment( const PreProcWUSet& input, const Options& svm_param_options, ExperimentResult* result )
//{
//	//TODO DRY with LibSvmAdapter
//	int default_log_cost = 0;
//	int default_gamma_cost = 0;
//
//	return run_experiment_( input, result, svm_param_options, default_log_cost, default_gamma_cost );
//}

//void SelfTestExperiment::run_experiment_( const PreProcWUSet& input, ExperimentResult* result, 
//										 const Options& svm_param_options, double log_cost, double log_gamma )
//{
//	//make copy of input
//	PreProcWUSet predictee_input;
//	for( size_t i = 0; i < input.size(); i++ )
//	{
//		PreProcWorkUnit* wu = new PreProcWorkUnit( *(input.at(i)) );
//		predictee_input.push_back( wu );
//	}
//	
//	//prepare for svm
//	SvmWUMatrix svm_trainset;
//	SvmWUMatrix svm_predictees;
//
//	BioProcessor b;
//	b.process_with_cache( input, predictee_input, svm_trainset, svm_predictees );
//
//	LibSvmAdapter l;
//	std::vector<bool> susceptibility_predicted_results = l.predict_susceptibility( svm_trainset, svm_predictees, svm_param_options, log_cost, log_gamma );
//
//	//Collect each Result here:
//	for( size_t i=0; i < susceptibility_predicted_results.size(); i++ )
//	{
//		struct ExperimentResult::data_point* dp = new ExperimentResult::data_point(
//			susceptibility_predicted_results.at(i), 
//			svm_predictees.at(i)->known_susceptibility(),
//			svm_predictees.at(i)->get_id()
//			);
//		
//		result->data.data_points.push_back( dp );
//	}

	//

	//calculate results
	//int hits = 0;
	//for( size_t i = 0; i < svm_predictees.size(); i++ )
	//{
	//	if( svm_predictees.at(i)->known_susceptibility() == susceptibility_results.at(i) )
	//	{
	//		hits++;
	//	}
	//}

	//size_t num_total_predictions = svm_predictees.size();

	//double accuracy = (double)hits/(double)num_total_predictions;

	//return accuracy;
//}
