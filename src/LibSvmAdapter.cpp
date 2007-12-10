#include "LibSvmAdapter.hpp"
#include "Types.hpp"
#include "Log.hpp"

#include <cmath> //power(base, exponent)
#include <cassert> //

	//default constructor
LibSvmAdapter::LibSvmAdapter(){}

	//default destructor
LibSvmAdapter::~LibSvmAdapter(){}

int LibSvmAdapter::get_number_of_classes( const SvmWUMatrix& trainset, const Options& svm_param_options )
{
	//train
	//ALLOCATE MEMORY 
	struct svm_problem* problem = create_svm_problem( trainset );//NEW SVM_PROBLEM
	struct svm_parameter* param = create_svm_parameter( problem, svm_param_options  );//NEW SVM_PARAMETER
	struct svm_model* train_model = train( trainset, param, problem );//NEW SVM_MODEL
	
	int classes = svm_get_nr_class( train_model );

	//FREE MEMORY
	svm_destroy_model( train_model );//DELETE SVM_MODEL
	delete_svm_parameter( param );//DELETE SVM_PARAMETER
	delete_svm_problem( problem );//DELETESVM_PROBLEM

	return classes;
}

std::string LibSvmAdapter::one_class_trainset_details_( const SvmWUMatrix& trainset )
{
	std::string msg;
	std::string id;
	bool suscep;
	
	msg.append( "\nIsolate, Known Susceptibility of Training Set\n" );
	
	for( int d=0; d < trainset.size(); d++ )
	{
		msg.append( trainset.at(d)->get_id() );
		suscep = trainset.at(d)->known_susceptibility();
		msg.append( ", " );
		msg.append(  boost::lexical_cast<std::string>( suscep ) );
		msg.append( "\n" );
	}
	
	return msg;
}


void LibSvmAdapter::predict_susceptibility( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees,
	const Options& svm_param_options, ExperimentResult* result )
{
  //train
  //ALLOCATE MEMORY 
  struct svm_problem* problem = create_svm_problem( trainset );//NEW SVM_PROBLEM
  struct svm_parameter* param = create_svm_parameter( problem, svm_param_options  );//NEW SVM_PARAMETER
  struct svm_model* train_model = train( trainset, param, problem );//NEW SVM_MODEL

  //check for 1 class problem. i.e. All training data resistant or all was suscepitble.
  //This means the svm will predict all one class. (I should test assumption.)
  int num_of_classes = svm_get_nr_class( train_model );
  
  if( num_of_classes != 2 )
    {
      if( num_of_classes == 1 )
	{	
	  std::string msg;
	  msg.append( "\n\nWarning! Training data is all in same class. Predictions were automatically all for same class." );
	  msg.append( "Please view Readme.txt to see ways to avoid this problem. View log to see details of training data." );
	  //add details of training set here
	  msg.append( "\nContinuing hivm...\n\n" );
	  Log::append( msg );
#if !defined UNIT_TESTING//don't fill up regression tests w/ this warning
	  std::cerr << msg;
#endif
	  
	  Log::append( one_class_trainset_details_( trainset ) );
	}
      else
	{	
	  std::string msg;
	  msg.append( "Error! Number of classes can only be 1 or 2 in this version of hivm.\n" );
	  msg.append( "Number of classes is: " );
	  msg.append( boost::lexical_cast<std::string>( num_of_classes ) );
	  msg.append( "\nDon't know how to handle this. Exiting..." );
	  Log::append( msg );
	  std::cerr << msg;
	  exit(1);
	}
      
    }
  
  //std::cout << "Number of classes: " << classes << '\n';
  //svm_save_model( "mymodel.txt", train_model );
  
  //declare output container
  std::vector<bool> susceptibility_predicted_results;
  std::vector< std::vector <double> > probabilities_collection;
  
  //predict loop
  for( int i=0; i < predictees.size(); i++ )
    {
      bool is_susceptible;
      if( svm_param_options.probability== 1 )//use probabiltiy
	{
	  std::vector<double> probability_output;
	  is_susceptible =  predict_with_probabilities_( predictees.at(i), train_model, param, probability_output );
	  
	  //record probability information
	  probabilities_collection.push_back( probability_output ); 
	  
	}
      else//straight model-validation with no probability information
	{
	  is_susceptible =  predict_( predictees.at(i), train_model, param );
	}
      
      susceptibility_predicted_results.push_back( is_susceptible );
    }
  
  //FREE MEMORY
  svm_destroy_model( train_model );//DELETE SVM_MODEL
  delete_svm_parameter( param );//DELETE SVM_PARAMETER
  delete_svm_problem( problem );//DELETESVM_PROBLEM
  
  //Prepare Output - ExperimentResultSet manages destruction of these results
  //ExperimentResult* result = new ExperimentResult( svm_param_options.lg_cost,
  //	svm_param_options.lg_gamma );
  result->lg_cost = svm_param_options.lg_cost;
  result->lg_gamma = svm_param_options.lg_gamma;
  std::vector<double> empty;
  
  //Store datapoints. Each datapoint has predicted and actual susceptibility
  for( int i=0; i < susceptibility_predicted_results.size(); i++ )
    {
      if( probabilities_collection.size() > 0 )//we have probability information to store
	{
	  result->add( 
		      susceptibility_predicted_results.at(i),
		      predictees.at(i)->known_susceptibility(),
		      predictees.at(i)->get_id(),
		      probabilities_collection.at( i )
		      );
	}
      else
	{
	  result->add( 
		      susceptibility_predicted_results.at(i),
		      predictees.at(i)->known_susceptibility(),
		      predictees.at(i)->get_id(),
		      empty
		      );
	}
    }		
}


void LibSvmAdapter::delete_svm_parameter( struct svm_parameter* param )
{
	svm_destroy_param( param );
	delete( param );
}

//@pre svm_param loaded properly
bool LibSvmAdapter::predict_(  const SvmWUSet* predictee, const svm_model* train_model, const struct svm_parameter* param )
{

	if(  train_model == NULL )//todo DRY
	{
		std::string msg = "Sorry, Trying to predict without a valid model. Exiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	double result;

	struct svm_node* predictee_ = create_svm_node_ptr( predictee );//CALLOC SVM_NODE*
	
	try
	{
		result = svm_predict( train_model, predictee_ );
	}
	catch( std::exception& e )
	{
		std::string msg = "\nException caught in LibSvmAdapter::predict_with_probabilities_ call to svm.cpp, svm_predict(): \n";
		msg +=   e.what();
		msg += "\n\nElapsed Time: ";
		msg += Log::elapsed_time();
		msg += "\nExiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}



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
	else// result == -1
	{
		return not_susceptible;
	}
}

bool LibSvmAdapter::predict_with_probabilities_(  const SvmWUSet* predictee, const svm_model* train_model, const struct svm_parameter* param,
	std::vector<double>& probabilities_output )
{
	if(  train_model == NULL )//todo DRY
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
//	double prob_estimates[ 2 ];//todo. better to leave this as array?
	
	int num_classes =  svm_get_nr_class( train_model );
	double *prob_estimates;
	prob_estimates = (double *) calloc( num_classes, sizeof( double ) );//svm_problem_categories CALLOC

	//check for calloc error, return of null pointer
	if( prob_estimates == 0 )
	{
		std::string msg = "Memory Allocation Error!";
		msg += " calloc returned null pointer ";
		msg += " in LibSvmAdapter::predict_with_probabilities_ ";
		msg += " cannot continue. Exiting...";
		msg.append( "\nElapsed time: " );
		msg.append( Log::elapsed_time() );
		std::cerr << msg;
		Log::append( msg );
		exit(1);
	}

	//set -1 default for unit test purposes. don't remove this
	double* walker = prob_estimates;
	for( int k = 0; k < num_classes; k++ )
	{
		*walker = -1;
		walker++;
	}

	//test for 2 classes. That's all the classes we use right now.
	//assert( svm_get_nr_class( train_model ) == 2 );

	struct svm_node* predictee_ = create_svm_node_ptr( predictee );//CALLOC SVM_NODE*
	try
	{
		result = svm_predict_probability( train_model, predictee_, prob_estimates );
	}
	catch( std::exception& e )
	{
		std::string msg = "\nException caught in LibSvmAdapter::predict_with_probabilities_ call to svm.cpp, svm_predict_probability(): \n";
		msg +=   e.what();
		msg += "\n\nElapsed Time: ";
		msg += Log::elapsed_time();
		msg += "\nExiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}
	free( predictee_ );//FREE SVM_NODE*

	//double class_not_susceptible = prob_estimates[0];
	//double class_susceptible = prob_estimates[1];
	
	if( num_classes == 2 )
	{
		//Manage the order of probabilities to match semantics above.
		//"The lable which appears in the training file first gets [0th probability estimate position]" - Dr. Chih-Jen Lin

		int first_label = train_model->label[0];
		if( first_label == 1 )
		{
			probabilities_output.push_back( *(prob_estimates+1) );// probability of resistance
			probabilities_output.push_back( *(prob_estimates) );// probability of susceptible
		}
		else //first_label == -1
		{
			probabilities_output.push_back( *(prob_estimates) );// probability of resistance
			probabilities_output.push_back( *(prob_estimates+1) );// probability of susceptible
		}
	}
	else if( num_classes == 1 ) //special case
	{	
		if( result == 1 )//susceptible
		{
			probabilities_output.push_back( 0 );//0% probability of resistance
			probabilities_output.push_back( 1 );//100% probability of susceptible
		}
		else//resistant
		{
			probabilities_output.push_back( 1 );//100% probability of resistance
			probabilities_output.push_back( 0 );//0% probability of susceptible
		}
	}
	else//problem, hivm only handles max of 2 classes
	{
		std::string msg = "Error: Number of classes was not 1 or 2. Don't know what to do. Exiting...";
		msg.append( "\n Error in LibSvmAdapter::predict_with_probabilities_" );
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	free( prob_estimates );

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

	try
	{
		result_model = svm_train( problem, param );//svm.h
	}
		catch( std::exception& e )
		{
			std::string msg = "\nException caught in LibSvmAdapter::train, call to svm.cpp svm_train() \n";
			msg +=   e.what();
			msg += "\n\nElapsed Time: ";
			msg += Log::elapsed_time();
			msg += "\nExiting...";
			Log::append( msg );
			std::cerr << msg;
			exit(1);
		}

	return result_model;
}

//
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


/**
	pow(2,-3) == pow(1/2, 3 )
*/
//
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

//
//TODO DRY
struct svm_parameter* LibSvmAdapter::create_svm_parameter( const struct svm_problem* problem, const Options& options  )
{
	struct svm_parameter* param;
	
	try
	{
		param = new svm_parameter();
	}
	catch( std::exception& e )
	{
		std::string msg = "\nException caught in LibSvmAdapter::create_svm_parameter: \n";
		msg +=   e.what();
		msg += "\n\nElapsed Time: ";
		msg += Log::elapsed_time();
		msg += "\nExiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	if( problem->l == 0 )
	{
		std::string msg = "Number of vectors cannot be zero.";
		msg.append( "struct svm_parameter* LibSvmAdapter::create_svm_parameter( int num_vectors )" );
		msg.append( "Exiting" );
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	//k means the number of attributes in the input data
//	double problem_length = problem->l;

	param->C			= powSigned_( 2, options.lg_cost );//cost
	param->gamma		= powSigned_( 2, options.lg_gamma );//gamma, //(default 1/k, k is the number of support vectors, but I don't know how to get # svm's)

	//param->C	 = options.lg_cost;//cost
	//param->gamma = options.lg_gamma;//(default 1/k, k is the number of support vectors, but I don't know how to get # svm's)

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

double* LibSvmAdapter::create_svm_problem_categories( const SvmWUMatrix& trainset )
{
	int length = trainset.size();

	double *head;
	head = (double *) calloc( length, sizeof( double ) );//svm_problem_categories CALLOC
	
	//check for calloc error, return of null pointer
	if( head == 0 )
	{
		std::string msg = "Memory Allocation Error!";
		msg += " calloc returned null pointer ";
		msg += " in LibSvmAdapter::create_svm_problem_categories ";
		msg += " cannot continue. Exiting...";
		msg.append( "\nElapsed time: " );
		msg.append( Log::elapsed_time() );
		std::cerr << msg;
		Log::append( msg );
		exit(1);
	}

	double *walker = head;

	std::vector< SvmWUSet*>::const_iterator it;

	//fill up double array with Category/Label values of WorkUnits
	for( it = trainset.begin(); it != trainset.end(); it++ )
	{
		double susceptible;
		std::string _id = (*it)->get_id();//TODO this is debug code

		if( (*it)->known_susceptibility() )//true
		{
			susceptible = 1;
		}
		else //if( (*it)->get_susceptibility() == 0 )//false
		{
			susceptible = -1;//resistant
		}

		//std::cerr << _id << " " << susceptible << "\n";

		(*walker) = susceptible;
		walker++;
	}

	return head;
}

struct svm_problem* LibSvmAdapter::create_svm_problem( const SvmWUMatrix& trainset )
{
	struct svm_problem* problem;
	
	try
	{
		problem = new svm_problem();//PROBLEM NEW
	}
	catch( std::exception& e )
	{
		std::string msg = "\nException caught in LibSvmAdapter::create_svm_parameter: \n";
		msg +=   e.what();
		msg += "\n\nElapsed Time: ";
		msg += Log::elapsed_time();
		msg += "\nExiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	problem->l = (int)trainset.size();

	//2. make array of doubles containing category value for each WorkUnit
	//(problem->y) = makeSvmProblemCategories( wus );
	
	//make array of doubles
	(problem->y) = create_svm_problem_categories( trainset );

	//3. make a 2-dimensional array of svm_node*'s containing contents of
	//wu's numseqSacled's
	//aka, setup svm_problem->x
	(problem->x) = create_svm_node_ptr_to_ptr( trainset );

	return problem;
}

struct svm_node* LibSvmAdapter::create_svm_node_ptr( const SvmWUSet* wu_set )
{
	//allocate memory
	int length = wu_set->size();
	struct svm_node* head;
	head = (svm_node *) calloc( ( length+1 ), sizeof( struct svm_node ));//svm_node_ptr CALL0C
	
	//check for calloc error, return of null pointer
	if( head == 0 )
	{
		std::string msg = "Memory Allocation Error!";
		msg += " calloc returned null pointer ";
		msg += " in LibSvmAdapter::create_svm_node_ptr ";
		msg += " cannot continue. Exiting...";
		msg.append( "\nElapsed time: " );
		msg.append( Log::elapsed_time() );
		std::cerr << msg;
		Log::append( msg );
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


struct svm_node ** LibSvmAdapter::create_svm_node_ptr_to_ptr( const SvmWUMatrix& train_set )
{
	int length = train_set.size();
	struct svm_node** result = (svm_node **)calloc( length, sizeof( struct svm_node* ));//svm_node_ptr_to_ptr CALLOC
	//check for calloc error, return of null pointer
	if( result == 0 )
	{
		std::string msg = "Memory Allocation Error!";
		msg += " calloc returned null pointer ";
		msg += " in LibSvmAdapter::create_svm_node_ptr_to_ptr ";
		msg += " cannot continue. Exiting...";
		msg.append( "\nElapsed time: " );
		msg.append( Log::elapsed_time() );
		std::cerr << msg;
		Log::append( msg );
		exit(1);
	}


	struct svm_node** walker = result;

	std::vector< SvmWUSet* >::const_iterator it;
	int i;
	for( it = train_set.begin(), i=0; it != train_set.end(); it++, i++ )
	{
		walker[i] = create_svm_node_ptr( *it );
	}

	return result;
}
