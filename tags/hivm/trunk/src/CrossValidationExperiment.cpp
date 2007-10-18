#include "CrossValidationExperiment.hpp"
#include "BioProcessor.hpp"
#include "Log.hpp"
#include "LibSvmAdapter.hpp"

	//default constructor
CrossValidationExperiment::CrossValidationExperiment()
{}

	//default destructor
CrossValidationExperiment::~CrossValidationExperiment()
{}

//TODO test
//TODO add seed for shuffling
void CrossValidationExperiment::run_experiment(
	const PreProcWUSet& input,
	ExperimentResult* result, 
	const Options& options,
	double log_cost, double log_gamma )
{
	for( size_t fold_it=0; fold_it<10; fold_it++ )
	{
		SvmWUMatrix svm_trainset;
		SvmWUMatrix svm_predictees;
		
		createFold( input, fold_it, svm_trainset, svm_predictees );
		
		//make  predictions
		LibSvmAdapter l;
		std::vector<bool> susceptibility_predicted_results = l.predict_susceptibility( svm_trainset, svm_predictees, options, log_cost, log_gamma );

		//Store datapoints. Each datapoint has predicted and actual susceptibility
		for( size_t i=0; i < susceptibility_predicted_results.size(); i++ )
		{
			struct ExperimentResult::data_point* dp = new ExperimentResult::data_point(
				susceptibility_predicted_results.at(i), 
				svm_predictees.at(i)->known_susceptibility(),
				svm_predictees.at(i)->get_id()
				);
			
			result->datum.data_points.push_back( dp );
		}	
	}
}

//TODO clean out comments
//TODO rename to create_fold
void CrossValidationExperiment::createFold( 
	const PreProcWUSet& input, 
	size_t fold, 
	SvmWUMatrix& trainset_output,
	SvmWUMatrix& predictees_output
	)
{
	if( fold > 10 )
	{
		std::string msg = "CrossValidationExperiment::createFold error. Fold must be 0-9. Exiting.";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	//figure fold size

//	determine which wu's go into predictee.
//	that gives you indices that let you easily put the
//	rest into training set.
//
//	so hard to focus....
//
//example:
//	4,5,6 go into predictee.
//	
//	0-4
//	7-size() go into training.
//
//	ok, so back to determining which go into predictee.
//	first without worrying about the extras. (ex. cases where size % 10 = 0)
//
//	size/10 = foldsize;
//
//	index_first_predictee = foldsize * fold; //for 20: 0, 2, 4, 18
//	index_one_past_last_predictee  = index_first_predictee + fold_size; 2, 4... 20
//
//		ok, that looks good. now add in the code to handle spares:

	size_t fold_size;
	size_t number_of_extras = input.size() % 10;
	size_t index_first_predictee;
	size_t index_one_past_last_predictee;

	if( fold < number_of_extras )
	{
		fold_size = ( input.size()/10 ) + 1;
		index_first_predictee = fold_size * fold; //for 20: 0, 2, 4, 18
		index_one_past_last_predictee  = index_first_predictee + fold_size; //2, 4... 20
	}

	if( fold >= number_of_extras )
	{
		fold_size = ( input.size()/10 );
		index_first_predictee = fold_size * fold + number_of_extras;
		index_one_past_last_predictee = index_first_predictee + fold_size;
	}

	//copy proper WU's for predictee set
	PreProcWUSet predictee_input;
	for( size_t i = index_first_predictee; i < index_one_past_last_predictee; i++ )
	{
		PreProcWorkUnit* wu = new PreProcWorkUnit( *(input.at(i)) );//create new preprocwu pointer using copy constructor
		predictee_input.push_back( wu );
	}

	//copy proper WU's for training set
	PreProcWUSet trainset_input;
	for( size_t i = 0; i < index_first_predictee; i++ )
	{
		PreProcWorkUnit* wu = new PreProcWorkUnit( *(input.at(i)) );//create new preprocwu pointer using copy constructor
		trainset_input.push_back( wu );
	}

	for( size_t i = index_one_past_last_predictee; i < input.size(); i++ )
	{
		PreProcWorkUnit* wu = new PreProcWorkUnit( *(input.at(i)) );//create new preprocwu pointer using copy constructor
		trainset_input.push_back( wu );
	}
	
	BioProcessor b;
	b.process_with_cache( trainset_input, predictee_input, trainset_output, predictees_output);

	return;
	
}
