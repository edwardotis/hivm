#ifndef LIBSVMFACADE_H
#define LIBSVMFACADE_H

#include "svm.h"//LibSvm

#ifdef __cplusplus
	extern "C" {
#endif

/**
@purpose Define a clean, simpler interface to LibSvm functions needed by hivm
REMEMBER: This is C not a C++ class. All functions and structures are globally accessible.
*/

	/**************************************************************
	// TRAIN
	// @purpose This function constructs and returns an SVM model according to
    //  the given training data and parameters.
	// @pre svm_check_parameter returns NULL
	   @post returns pointer to a training model
	//
	*/
	struct svm_model *svm_train( const struct svm_problem *prob,
		const struct svm_parameter *param );

	/****************************************************************
	PREDICT aka Classify
    @purpose This function does classification or regression on a test vector x
    given a model.
	@pre already ran svm_train to generate an svm_model
	@post -1 false, 1 true

    For a classification model, the predicted class for x is returned.
    For an one-class model, +1 or -1 is  returned.
	*/
	double svm_predict(const struct svm_model *model, const struct svm_node *x);


	/****************************************************************
	  
    This function does classification or regression on a test vector x
    given a model with probability information.

    For a classification model with probability information, this
    function gives nr_class probability estimates in the array
    prob_estimates. i.e. svm_get_nr_class() returns size of prob_estimates array
	nr_class can be obtained from the function svm_get_nr_class. 
	The class with the highest probability is
    returned. For all other situations, the array prob_estimates is
    unchanged and the returned value is the same as that of
    svm_predict.

	Ed Notes:
double* prob_estimates is the output structure. It is of size svm_get_nr_class()
So, if there are 3 classes, then I should have 3 doubles in prob_estimates.

If the third class has the highest probablity, then 2 is returned by svm_predict_probability().

position of max value in prob_estimates is what is returned by svm_predict_probability.

I have 2 classes defined LibSvmAdapter. -1 and 1. not-susceptible and susceptible.
*/
	double svm_predict_probability(const struct svm_model *model, 
	    const struct svm_node *x, double* prob_estimates);


	/****************************************************************
    For a classification model, this function gives the number of
    classes. For a regression or an one-class model, 2 is returned.
	*/
	int svm_get_nr_class(const svm_model *model);

	/****************************************************************
    Function: int svm_check_probability_model(const struct svm_model *model);

    This function checks whether the model contains required
    information to do probability estimates. If so, it returns
    +1. Otherwise, 0 is returned. This function should be called
    before calling svm_get_svr_probability and
    svm_predict_probability.
	*/
	int svm_check_probability_model(const svm_model *model);

	/******************************************************************
	//Check for valid parameter values
	//@post Returns NULL no problem, else returns error message

    //This function checks whether the parameters are within the feasible
    //range of the problem. This function should be called before calling
    //svm_train(). 
	*/
	const char *svm_check_parameter(const struct svm_problem *prob, 
		const struct svm_parameter *param);

	/**
	This function frees the memory used by a model.
	*/
	
	void svm_destroy_model(struct svm_model *model);
	
	/**
	This function frees the memory used by a parameter set.
	*/	
	void svm_destroy_param(struct svm_parameter *param);

	/**
	This function saves a model to a file; returns 0 on success, or -1
    if an error occurs.
	*/
	int svm_save_model(const char *model_file_name, const struct svm_model *model);




#ifdef __cplusplus
}
#endif


#endif

