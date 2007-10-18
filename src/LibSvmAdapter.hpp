/**
@purpose Provides an interface to the libsvm C-style functions. 
Provides memory management and error checking using full features of C++. 

Category labels (aka Class Labels) are set by me in this method:
LibSvmAdapter::create_svm_problem_categories
-1 == not-susceptible 
 1 == susceptible
*/

#ifndef LIBSVMADAPTER_HPP
#define LIBSVMADAPTER_HPP

#include "SvmWorkUnit.hpp"
#include "LibSvmFacade.h"
#include "Types.hpp"
#include "Options.hpp"
#include "ExperimentResult.hpp" //for holding prediction results

class LibSvmAdapter
{

friend class LibSvmAdapterTest;//unit tests

public:

	//default constructor
	LibSvmAdapter();

	//default destructor
	virtual ~LibSvmAdapter();

	/**
	@purpose detect multiple predictee sequences susceptibility to drug
	@post result contains all the predictions from the predictees
		If svm_param_options.probability is 1, 
		probabilities_output: position 0 = resistance probability, position 1 = suceptibility probability
	*/
	void  predict_susceptibility( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees,
		const Options& svm_param_options, ExperimentResult* result );

private:
	
	/**
	@purpose Wrapper for libsvm function to find number of different classes in the svm_model
	*/
	int get_number_of_classes( const SvmWUMatrix& trainset, const Options& svm_param_options );

	/**
	@purpose For use in debugging when get_number_of_classes() returns one class in a situation where
	we expect two classes. Usually this means that training data all fell into a single class.
	This class collects that data for help in displaying the problem to the user.
	*/
	std::string one_class_trainset_details_( const SvmWUMatrix& trainset );

	bool predict_(  const SvmWUSet* predictee, const svm_model* train_model, const struct svm_parameter* param );

	/**
	@post
	probabilities_output: position 0 = resistance probability, position 1 = suceptibility probability
	*/
	bool predict_with_probabilities_(  const SvmWUSet* predictee, const svm_model* train_model, const struct svm_parameter* param,
		std::vector<double>& probabilities_output );

	//@purpose 
	struct svm_model* train( const SvmWUMatrix& trainset, struct svm_parameter*,
		struct svm_problem* problem );

	/**
	 @purpose Converts SvmWUMatrix  into svm_problem*
	 @post delegate memory removal of svm_problem struct
	*/
	//calls new
	struct svm_problem* create_svm_problem( const SvmWUMatrix&  );

	//calls calloc
	double* create_svm_problem_categories( const SvmWUMatrix&  );

	//TODO can this go into destructor?
	void delete_svm_problem( struct svm_problem* );

	//calls calloc
	struct svm_node* create_svm_node_ptr( const SvmWUSet* wu_set );

	//calls calloc
	struct svm_node** create_svm_node_ptr_to_ptr( const SvmWUMatrix& train_set );

	//calls new
	struct svm_parameter* create_svm_parameter(  const struct svm_problem*, const Options& );

	//calls new
	//struct svm_parameter* create_svm_parameter(  const struct svm_problem*, const Options&, double log_base2_cost, double log_base2_gamma);

	/**
	@purpose improved power math function that correctly computes
	negative exponents
	*/
	double powSigned_( const double base, const double exponent ) const;

	//@purpose destroy_svm_parameter doesn't finish the job
	void delete_svm_parameter( struct svm_parameter*  );

private:

	//copy constructor
	LibSvmAdapter( const LibSvmAdapter& );

	//assignment operator
	LibSvmAdapter& operator= ( const LibSvmAdapter& );

};


#endif //LIBSVMADAPTER_HPP
