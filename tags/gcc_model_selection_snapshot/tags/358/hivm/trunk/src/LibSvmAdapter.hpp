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

public:

	//default constructor
	LibSvmAdapter();

	//default destructor
	virtual ~LibSvmAdapter();

	//TODO this group of functions is growing way too fast. refactor it.

	//@purpose detect predictee sequence susceptibility to drug
	//@post returns true if predictee if susceptible (aka not-resistant)
	//bool predict_susceptibility( const SvmWUMatrix& trainset, const SvmWUSet* predictee, const Options& svm_param_options);

	//bool predict_susceptibility_with_probabilities( const SvmWUMatrix& trainset, const SvmWUSet* predictee, const Options& svm_param_options,
	//	std::vector<double>& probabilities_output);

	//bool predict_susceptibility( const SvmWUMatrix& trainset, const SvmWUSet* predictee,
	//	const Options& svm_param_options, double log_cost, double log_gamma );

	//bool predict_susceptibility_with_probabilities( const SvmWUMatrix& trainset, const SvmWUSet* predictee, 
	//	const Options& svm_param_options,	double log_cost, double log_gamma, 
	//	std::vector<double>& probabilities_output);

	//@purpose detect multiple predictee sequences susceptibility to drug
	//@post: true = susceptible, false = resistant
	//@ExperimentResultSet will have a new ExperimentResult instance
	void  predict_susceptibility( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees,
		const Options& svm_param_options, ExperimentResultSet& output );

	//std::vector<bool>  predict_susceptibility_with_probabilities( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees,
	//	const Options& svm_param_options, std::vector<double> probabilities_output );

	//std::vector<bool>  predict_susceptibility( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees,
	//	const Options& svm_param_options, double log_cost, double log_gamma );

	//std::vector<bool>  predict_susceptibility_with_probabilities( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees,
	//	const Options& svm_param_options, double log_cost, double log_gamma, std::vector<double> probabilities_output );

//private:

	//std::vector<bool> predict_susceptibility_many_( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees,
	//										const Options& );

	//std::vector<bool> predict_susceptibility_many_with_probabilities_( const SvmWUMatrix& trainset, const SvmWUMatrix& predictees,
	//										const Options&, const double log_cost, const double log_gamma,
	//										std::vector<double> probabilities_output );

	//bool predict_susceptibility_( const SvmWUMatrix& trainset, const SvmWUSet* predictee,
	//										const Options& svm_param_options  );

	//bool predict_susceptibility_with_probabilities_( const SvmWUMatrix& trainset, const SvmWUSet* predictee,
	//										const Options& svm_param_options, const double log_cost, const double log_gamma,
	//										std::vector<double>& probabilities_output );

	bool predict_(  const SvmWUSet* predictee, const svm_model* train_model, const struct svm_parameter* param );

	//bool predict_with_probabilities_(  const SvmWUSet* predictee, const svm_model* train_model, const struct svm_parameter* param,
	//	std::vector<double>& probabilities_output );

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
