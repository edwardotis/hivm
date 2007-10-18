#include "LibSvmAdapter.hpp"
#include "Types.hpp"
#include "Log.hpp"

#include <cmath> //power(base, exponent)
#include <cassert> //

	//default constructor
LibSvmAdapter::LibSvmAdapter(){}

	//default destructor
LibSvmAdapter::~LibSvmAdapter(){}

bool LibSvmAdapter::predict_susceptibility( const SvmWUMatrix& trainset, const SvmWUSet* predictee, 
										   const Options& svm_param_options )
{
	//defaults of cost and gamma
	const double log_cost_default = svm_param_options.log_cost;// 
	const double log_gamma_default = svm_param_options.log_gamma;//

	return predict_susceptibility_( trainset, predictee, svm_param_options, log_cost_default, log_gamma_default  );
}

bool LibSvmAdapter::predict_susceptibility_with_probabilities( const SvmWUMatrix& trainset, const SvmWUSet* predictee, const Options& svm_param_options,
	std::vector<double>& probabilities_output)
{
	//defaults of cost and gamma
	const double log_cost_default = svm_param_options.log_cost;//
	const double log_gamma_default = svm_param_options.log_gamma;//

	return predict_susceptibility_with_probabilities_(
		trainset, predictee, svm_param_options, log_cost_default, log_gamma_default, probabilities_output  );
}

bool LibSvmAdapter::predict_susceptibility( const SvmWUMatrix& trainset, const SvmWUSet* predictee,
										   const Options& svm_param_options, double log_cost, double log_gamma )
{
	return predict_susceptibility_( trainset, predictee, svm_param_options, log_cost, log_gamma  );
}

bool LibSvmAdapter::predict_susceptibility_with_probabilities( const SvmWUMatrix& trainset, const SvmWUSet* predictee, 
										  const Options& svm_param_options,	double log_cost, double log_gamma, 
										  std::vector<double>& probabilities_output)
{
	return predict_susceptibility_with_probabilities_( trainset, predictee, svm_param_options, log_cost, log_gamma,
		probabilities_output );
}

bool LibSvmAdapter::predict_susceptibility_( const SvmWUMatrix& trainset, const SvmWUSet* predictee, 
											const Options& svm_param_options, const double log_cost, const double log_gamma )
{
	//train
	//ALLOCATE MEMORY - DRY WARNING
	struct svm_problem* problem = create_svm_problem( trainset );//NEW SVM_PROBLEM
	struct svm_parameter* param = create_svm_parameter( problem, svm_param_options, log_cost, log_gamma );//NEW SVM_PARAMETER
	struct svm_model* train_model = train( trainset, param, problem );//NEW SVM_MODEL

	//predict
	bool is_susceptible = predict_( predictee, train_model, param );
	
	//FREE MEMORY DRY WARNING
	svm_destroy_model( train_model );//DELETE SVM_MODEL
	delete_svm_parameter( param );//DELETE SVM_PARAMETER
	delete_svm_problem( problem );//DELETESVM_PROBLEM

	return is_susceptible;
}

bool LibSvmAdapter::predict_susceptibility_with_probabilities_( const SvmWUMatrix& trainset, const SvmWUSet* predictee, 
											const Options& svm_param_options, const double log_cost, const double log_gamma,
											std::vector<double>& probabilities_output)
{
	//train
	//ALLOCATE MEMORY - DRY WARNING
	struct svm_problem* problem = create_svm_problem( trainset );//NEW SVM_PROBLEM
	struct svm_parameter* param = create_svm_parameter( problem, svm_param_options, log_cost, log_gamma );//NEW SVM_PARAMETER
	struct svm_model* train_model = train( trainset, param, problem );//NEW SVM_MODEL

	//predict
	bool is_susceptible = predict_with_probabilities_( predictee, train_model, param, probabilities_output );
	
	//FREE MEMORY DRY WARNING
	svm_destroy_model( train_model );//DELETE SVM_MODEL
	delete_svm_parameter( param );//DELETE SVM_PARAMETER
	delete_svm_problem( problem );//DELETESVM_PROBLEM

	return is_susceptible;
}

//TODO DRY cost and gamma
std::vector<bool> LibSvmAdapter::predict_susceptibility( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees, 
														const Options& svm_param_options )
{
	//defaults of cost and gamma
	const double log_cost_default = svm_param_options.log_cost;
	const double log_gamma_default = svm_param_options.log_gamma;//

	return predict_susceptibility_many_( trainset, predictees, svm_param_options, log_cost_default, log_gamma_default  );
}

//std::vector<bool>  LibSvmAdapter::predict_susceptibility_with_probabilities( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees,
//	const Options& svm_param_options, std::vector<double> probabilities_output )
//{
//	//defaults of cost and gamma
//	const double log_cost_default = svm_param_options.log_cost;
//	const double log_gamma_default = svm_param_options.log_gamma;
//
//	return predict_susceptibility_many_( trainset, predictees, svm_param_options, log_cost_default, log_gamma_default  );
//	
//}

std::vector<bool> LibSvmAdapter::predict_susceptibility( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees, 
														const Options& svm_param_options, double log_cost, double log_gamma )
{
	return predict_susceptibility_many_( trainset, predictees, svm_param_options, log_cost, log_gamma  );
}

std::vector<bool> LibSvmAdapter::predict_susceptibility_many_( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees, 
															  const Options& svm_param_options, const double log_cost, const double log_gamma )
{
	//train
	//ALLOCATE MEMORY - DRY WARNING
	struct svm_problem* problem = create_svm_problem( trainset );//NEW SVM_PROBLEM
	struct svm_parameter* param = create_svm_parameter( problem, svm_param_options, log_cost, log_gamma );//NEW SVM_PARAMETER
	struct svm_model* train_model = train( trainset, param, problem );//NEW SVM_MODEL

	//declare output container
	std::vector<bool> susceptibility_results;
	
	//predict loop
	for( size_t i=0; i < predictees.size(); i++ )
	{
		bool is_susceptible =  predict_( predictees.at(i), train_model, param );
		susceptibility_results.push_back( is_susceptible );
	}

	//FREE MEMORY DRY WARNING
	svm_destroy_model( train_model );//DELETE SVM_MODEL
	delete_svm_parameter( param );//DELETE SVM_PARAMETER
	delete_svm_problem( problem );//DELETESVM_PROBLEM

	return susceptibility_results;
}

//std::vector<bool> LibSvmAdapter::predict_susceptibility_many_with_probabilities_( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees,
//				const Options&, const double log_cost, const double log_gamma, std::vector<double> probabilities_output )
//{
//	//train
//	//ALLOCATE MEMORY - DRY WARNING
//	struct svm_problem* problem = create_svm_problem( trainset );//NEW SVM_PROBLEM
//	struct svm_parameter* param = create_svm_parameter( problem, svm_param_options, log_cost, log_gamma );//NEW SVM_PARAMETER
//	struct svm_model* train_model = train( trainset, param, problem );//NEW SVM_MODEL
//
//	//declare output container
//	std::vector<bool> susceptibility_results;
//	
//	//predict loop
//	for( size_t i=0; i < predictees.size(); i++ )
//	{
//		bool is_susceptible =  predict_with_probabilities_( predictees.at(i), train_model, param );
//		susceptibility_results.push_back( is_susceptible );
//	}
//
//	//FREE MEMORY DRY WARNING
//	svm_destroy_model( train_model );//DELETE SVM_MODEL
//	delete_svm_parameter( param );//DELETE SVM_PARAMETER
//	delete_svm_problem( problem );//DELETESVM_PROBLEM
//
//	return susceptibility_results;	
//}



void LibSvmAdapter::delete_svm_parameter( struct svm_parameter* param )
{
	svm_destroy_param( param );
	free( param );
}

//TODO test
//@pre svm_param loaded properly
bool LibSvmAdapter::predict_(  const SvmWUSet* predictee, const svm_model* train_model, const struct svm_parameter* param )
{

	if(  train_model == NULL )
	{
		std::string msg = "Sorry, Trying to predict without a valid model. Exiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	double result;

	struct svm_node* predictee_ = create_svm_node_ptr( predictee );//CALLOC SVM_NODE*
	result = svm_predict( train_model, predictee_ );
	free( predictee_ );//FREE SVM_NODE*

	//check that result is always either 1 or -1
	if( result != 1 )
	{
		if( result != -1 )
		{
			assert(false);
		}
	}

	bool susceptible = true;
	bool not_susceptible = false;

	if( result == 1 )//labels set in create_svm_problem_categories
	{
        return susceptible;
	}
	else
	{
		return not_susceptible;
	}
}

bool LibSvmAdapter::predict_with_probabilities_(  const SvmWUSet* predictee, const svm_model* train_model, const struct svm_parameter* param,
	std::vector<double>& probabilities_output )
{
	if(  train_model == NULL )
	{
		std::string msg = "Sorry, Trying to predict without a valid model in LibSvmAdaper::predict_with_probabilities_(). Exiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	if(  svm_check_probability_model( train_model ) != 1 )
	{
		std::string msg = "Sorry, Trying to predict probabilities without a valid model in LibSvmAdaper::predict_with_probabilities_(). Exiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}


	//svm_get_nr_class( train_model );

	double result = -24;
	double prob_estimates[ 2 ];//prob_estimates MEM
	
	//set to -1 default for unit test purposes. don't remove this
	prob_estimates[0] = -1;
	prob_estimates[1] = -1;

	//@todo debug code. remove me.
	for(int i=0; i< 2; i++ )
	{
		double p;
		p = prob_estimates[ i ];
		double x = p;
	}

	//test for 2 classes. That's all the classes we use right now.
	assert( svm_get_nr_class( train_model ) == 2 );

	struct svm_node* predictee_ = create_svm_node_ptr( predictee );//CALLOC SVM_NODE*
	result = svm_predict_probability( train_model, predictee_, prob_estimates );
	free( predictee_ );//FREE SVM_NODE*

	//check that result is always either 1 or -1
	if( result != 1 )
	{
		if( result != -1 )
		{
			assert(false);
		}
	}

	//@todo debug code. remove me.
	double class_not_susceptible = prob_estimates[0];
	double class_susceptible = prob_estimates[1];
	
	//Reverse order of probabilities to match semantics above.
	probabilities_output.push_back( prob_estimates[0] );
	probabilities_output.push_back( prob_estimates[1] );

	//@todo test code - remove me
	for(int i=0; i< probabilities_output.size(); i++ )
	{
		double p;
		p = probabilities_output.at( i );
		double x = p;
	}

	bool susceptible = true;
	bool not_susceptible = false;

	if( result == 1 )//labels set in create_svm_problem_categories
	{
        return susceptible;
	}
	else
	{
		return not_susceptible;
	}
}

struct svm_model* LibSvmAdapter::train( const SvmWUMatrix& trainset, struct svm_parameter* param,
	struct svm_problem* problem )
{
	//CHECK PARAM
	if( svm_check_parameter( problem, param ) != NULL )
	{
		std::string msg = svm_check_parameter( problem, param );
		msg.append( "\n Exiting program from struct svm_model* LibSvmAdapter::train( const SvmWUMatrix& trainset, struct svm_parameter* param )" ); 
		msg.append( "Failed svm_check_parameter" );
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	struct svm_model* result_model;

	result_model = svm_train( problem, param );//svm.h

	return result_model;
}

//TODO test
void LibSvmAdapter::delete_svm_problem( struct svm_problem* problem )
{

	if( problem == NULL )
	{
		return;
	}

	//free the SvmNodePtrs
	struct svm_node** walker = problem->x;//points a walker to head
	for( int i = 0; i < problem->l; i++ )
	{
		if( walker != NULL )
		{
			free( walker[i] );//svm_node_ptr FREE
		}
	}

	free( problem->x );//svm_node_ptr_to_ptr FREE

	//free the double array y of svm_problem_categories
	free( problem->y );//svm_problem_categories FREE

	delete problem;//PROBLEM NEW
	problem = NULL;
}

//TODO test
//TODO DRY
struct svm_parameter* LibSvmAdapter::create_svm_parameter( const struct svm_problem* problem, const Options& options, double log_base2_cost, double log_base2_gamma)
{
	struct svm_parameter* param = new svm_parameter();

	param->C			= powSigned_( 2, log_base2_cost );//cost
	param->gamma		= powSigned_( 2, log_base2_gamma );//gamma

	//defaults
	param->svm_type		= options.svm_type;
	param->kernel_type	= options.kernel_type;
	param->degree		= options.degree;
	param->coef0		= options.coef0;
	param->nu			= options.nu;//MATCHES LIBSVM PYTHON EXAMPLE 
	param->cache_size	= options.cache_size;
	param->eps			= options.eps;
	param->p			= options.p;//MATCHES LIBSVM PYTHON EXAMPLE
	param->shrinking	= options.shrinking;
	param->nr_weight	= options.nr_weight;
	param->weight_label	= options.weight_label;
	param->weight		= options.weight;
	param->probability  = options.probability;

	return param;
}

/**
	pow(2,-3) == pow(1/2, 3 )
*/
//TODO test
double LibSvmAdapter::powSigned_( const double base, const double exponent ) const
{
	double tmpBase = base;
	double tmpExp = exponent;

	if( exponent < 0.0 )
	{
		tmpBase = 1.0/base;
		tmpExp = exponent * -1.0;
	}
	
	return pow( tmpBase, tmpExp );

}

//TODO test
//TODO DRY
struct svm_parameter* LibSvmAdapter::create_svm_parameter( const struct svm_problem* problem, const Options& options  )
{
	struct svm_parameter* param = new svm_parameter();

	if( problem->l == 0 )
	{
		std::string msg = "Number of vectors cannot be zero.";
		msg.append( "struct svm_parameter* LibSvmAdapter::create_svm_parameter( size_t num_vectors )" );
		msg.append( "Exiting" );
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	//k means the number of attributes in the input data
//	double problem_length = problem->l;

	param->C			= powSigned_( 2, options.log_cost );//cost
	param->gamma		= powSigned_( 2, options.log_gamma );//gamma, //(default 1/k, k is the number of support vectors, but I don't know how to get # svm's)

	//param->C	 = options.log_cost;//cost
	//param->gamma = options.log_gamma;//(default 1/k, k is the number of support vectors, but I don't know how to get # svm's)

	//defaults
	param->svm_type		= options.svm_type;
	param->kernel_type	= options.kernel_type;
	param->degree		= options.degree;
	param->coef0		= options.coef0;
	param->nu			= options.nu;//MATCHES LIBSVM PYTHON EXAMPLE 
	param->cache_size	= options.cache_size;
	param->eps			= options.eps;
	param->p			= options.p;//MATCHES LIBSVM PYTHON EXAMPLE
	param->shrinking	= options.shrinking;
	param->nr_weight	= options.nr_weight;
	param->weight_label	= options.weight_label;
	param->weight		= options.weight;
	param->probability  = options.probability;

	return param;
}

//TODO test
double* LibSvmAdapter::create_svm_problem_categories( const SvmWUMatrix& trainset )
{
	size_t length = trainset.size();

	double *head;
	head = (double *) calloc( length, sizeof( double ) );//svm_problem_categories CALLOC
	double *walker = head;

	std::vector< SvmWUSet*>::const_iterator it;

	//fill up double array with Category/Label values of WorkUnits
	for( it = trainset.begin(); it != trainset.end(); it++ )
	{
		double susceptible;

		if( (*it)->known_susceptibility() )//true
		{
			susceptible = 1;
		}
		else //if( (*it)->get_susceptibility() == 0 )//false
		{
			susceptible = -1;//resistant
		}
		//else //susceptibility not set. shouldn't be here.
		//{
		//	std::string msg;
		//	msg.append( "Error! Susceptibility not set yet in training data.\n" );
		//	msg.append( "double* LibSvmAdapter::create_svm_problem_categories( const SvmWUMatrix& trainset )\n" );
		//	msg.append( "Exiting...");
		//	Log::append( msg );
		//	std::cerr << msg ;
		//	exit(1);
		//}

		(*walker) = susceptible;
		walker++;
	}

	return head;
}

//TODO test
struct svm_problem* LibSvmAdapter::create_svm_problem( const SvmWUMatrix& trainset )
{
	struct svm_problem* problem = new svm_problem();//PROBLEM NEW
	problem->l = (int)trainset.size();

	//2. make array of doubles containing category value for each WorkUnit
	//(problem->y) = makeSvmProblemCategories( wus );
	
	//make array of doubles
	//TODO test
	(problem->y) = create_svm_problem_categories( trainset );

	//3. make a 2-dimensional array of svm_node*'s containing contents of
	//wu's numseqSacled's
	//aka, setup svm_problem->x
	(problem->x) = create_svm_node_ptr_to_ptr( trainset );

	return problem;
}

//TODO test
struct svm_node* LibSvmAdapter::create_svm_node_ptr( const SvmWUSet* wu_set )
{
	//allocate memory
	size_t length = wu_set->size();
	struct svm_node* head;
	head = (svm_node *) calloc( ( length+1 ), sizeof( struct svm_node ));//svm_node_ptr CALL0C

	if( head == NULL )
	{
		std::string msg = "Sorry, the system appears to have run out of memory. Exiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	struct svm_node* walker = head;//now point a walker to that head
	
	std::vector< double >::const_iterator it;
	int i;
	for( it = wu_set->begin(), i=0; it != wu_set->end(); it++, i++, walker++)
	{
		walker->index = i;
		walker->value = (*it);
	}

	//now add special libsvm termination symbol: index = -1
	walker->index = -1;
	walker->value = -1;

	return head;
}

//TODO test
struct svm_node ** LibSvmAdapter::create_svm_node_ptr_to_ptr( const SvmWUMatrix& train_set )
{
	size_t length = train_set.size();
	struct svm_node** result = (svm_node **)calloc( length, sizeof( struct svm_node* ));//svm_node_ptr_to_ptr CALLOC
	struct svm_node** walker = result;

	std::vector< SvmWUSet* >::const_iterator it;
	int i;
	for( it = train_set.begin(), i=0; it != train_set.end(); it++, i++ )
	{
		walker[i] = create_svm_node_ptr( *it );
	}

	return result;
}
