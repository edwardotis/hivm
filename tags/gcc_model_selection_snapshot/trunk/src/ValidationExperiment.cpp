#include "ValidationExperiment.hpp"
#include "BioProcessor.hpp"
#include "Log.hpp"
#include "LibSvmAdapter.hpp"

	//default constructor
ValidationExperiment::ValidationExperiment()
{}

	//default destructor
ValidationExperiment::~ValidationExperiment()
{}

//TODO test, similar to CrossVal unit tests
void ValidationExperiment::run_experiment( 
		const PreProcWUSet& training_data_set, 
		const PreProcWUSet& test_data_set, 
		const Options& svm_param_options,
		ExperimentResult* result
		)
{
	SvmWUMatrix svm_trainset;
	SvmWUMatrix svm_predictees;
	
	BioProcessor b;
	b.process_with_cache( training_data_set, test_data_set, svm_trainset, svm_predictees);

	//make  predictions
	LibSvmAdapter l;
	l.predict_susceptibility( svm_trainset, svm_predictees, svm_param_options, result );

	//LibSvmAdapter l;
	//std::vector<bool> susceptibility_predicted_results = l.predict_susceptibility( svm_trainset, svm_predictees, svm_param_options );

	//Store datapoints. Each datapoint has predicted and actual susceptibility
	//for( int i=0; i < susceptibility_predicted_results.size(); i++ )
	//{
	//	result->add_data( 
	//		susceptibility_predicted_results.at(i),
	//		svm_predictees.at(i)->known_susceptibility(),
	//		svm_predictees.at(i)->get_id()
	//		);

		//struct ExperimentResult::data_point* dp = new ExperimentResult::data_point(
		//	susceptibility_predicted_results.at(i), 
		//	svm_predictees.at(i)->known_susceptibility(),
		//	svm_predictees.at(i)->get_id()
		//	);
		//
		//result->data.data_points.push_back( dp );
	//}	
}
