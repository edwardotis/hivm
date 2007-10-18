//@purpose  Responsible for running statistics on Experiments and
// for saving Experiment results

#ifndef EXPERIMENTANALYZER_HPP
#define EXPERIMENTANALYZER_HPP

#include "ExperimentResult.hpp"
#include "Options.hpp"
#include "PreProcWorkUnit.hpp"
#include <string>


class ExperimentAnalyzer
{

friend class ExperimentAnalyzerTest;

public:

	//default constructor
	ExperimentAnalyzer();

	////constructor
	//ExperimentAnalyzer( ExperimentResultSet& result_set );

	//default destructor
	virtual ~ExperimentAnalyzer();

	/**
	//@purpose public interface to analyze Experiment
	//@post saves 5 files:
		cmd line options
		config file options
		full experiment stats
		roc curve data points
		roc curve script

		Additionally:
		For model-validation:
		a. save file of isolate names used, prediction, probability, c, g

		@note: analyze should check Options to determine:
		a. type of experiment.
		b. whether probability was used or not.

	*/
	void analyze( const ExperimentResultSet& result_set, const Options& options_ );


//stats
//ALL STATS RETURN -1 FOR DIVIDE BY ZERO
//-1 == Undefined in output (add this to header row)

	/**
	 > > pmax is error rate of the `majority classifier'. That is the
    classifier which assigns all specimens to the majority class (the
    class with most samples). 

	A very simple majority classifier predicts that every new case belongs
    to the most common class in the training data.

	 > If Susceptible equals a positive result.
	> TP 5
	> FN 2
	> FP 2
	> TN 3
    // n1 = Positive class, susceptible = TP + FN = 7
	// n2 = Negative class, resistant  = TN + FP = 5
	// pmax = min(n1, n2)/(n1+n2) = (TN + FP) / ( TP + FN + TN + FP )= 5/12
	*/
	double pmax( const ExperimentResult* result );

	//p-error = error rate / pmax
	double p_error( const ExperimentResult* result );

	//(TP'/FN')/(FP'/TN'), TP'=TP+0.5, FN'=FN+0.5, etc
	//The reason we are adding 0.5 is to
	// avoid the situation where DOR is undefined.
	double diagnostic_odds_ratio( const ExperimentResult* result );

	// (TP+TN)/N
	double accuracy( const ExperimentResult* result );

	// (FP+FN)/N
	double error_rate( const ExperimentResult* result );

	//true positive rate = sensitivity
	// TP/( TP + FN )
	double tpr( const ExperimentResult* result );

	//false positive rate
	// 1.0 - [TN/( FP + TN )]
	// FP/(TN + FP)
	double fpr( const ExperimentResult* result );

	//true negative rate
	// TN/( FP + TN ) == specificity
	double tnr( const ExperimentResult* result );

	//false negative rate
	// 1.0 - [TP/( TP + FN )]
	// FN/(TP + FN)
	double fnr( const ExperimentResult* result );

	// TP/( TP + FN ) = TPR
	double sensitivity( const ExperimentResult* result );//aka true positive rate

	// TN/( FP + TN ) = TNR
	double specificity( const ExperimentResult* result );

	//  TP/(TP+FP)
	double positive_predictive_value( const ExperimentResult* result );

	//  TN/(TN+FN)
	double negative_predictive_value( const ExperimentResult* result );

	//TPR-FPR
	//@post if TPR or FPR is undefined, returns -1
	double tpr_fpr_difference( const ExperimentResult* result );

	//returns a list of isolate names used in an experiment
	std::vector<std::string> get_ids( const ExperimentResult* result );

	//save isolate names of predictees used in cross validation i.e. model selection
	void save_model_selection_ids( const ExperimentResult* result,
				const std::string output_prefix );

	//save isolate names 
	void save_dataset_ids( const PreProcWUSet&, const std::string descriptor,
				const Options& options );

	/**
	@purpose Compute Area under the curve for a ROC Curve
	Uses Trapezoidal Method to compute Area.
	Based on Best TPR for each FPR in the result set.

	@param Choose to extend the curve to points 0,0 and 1,1 if there are 
	not already datapoints at fpr 0 and fpr 1
	*/
	double compute_roc_auc( const ExperimentResultSet& results, bool add_end_points );


private:
	
	//@purpose calculate area of trapezoid formed by fpr and tpr of two ExperimentResults
	double calc_trapezoid_area_( ExperimentResult* first_point, ExperimentResult* second_point );

	double calc_first_trapezoid_( ExperimentResult* first_point );
	double calc_last_trapezoid_(  ExperimentResult* last_point );

	/**
	@purpose a. save file of isolate names used, prediction, probability*, c, g
	(N/A if probability not used.)
	*/
	void save_prediction_detailed_results( const ExperimentResult* result,
		const std::string output_prefix );
	/*
	@purpose To return string with probability of the predicted susceptibility
	@pre Requires iterator to current data point inside ExperimentResult
	Returns "n/a" if there is no probability information in the result.
	//probabilities: class 1 = predicted suceptible, class 0 = predicted resistant
	*/
	std::string determine_probability( const ExperimentResult* result, const int it );


	//@purpose save c,g pairs and accuracy, tpr, fpr, etc in 
	//spreadsheet friendly file for model-validation or model-selection
	void save_experiment_results_data( const ExperimentResultSet& result_set,
		std::string output_prefix );

	//@purpose Save cg pair, TPR and FPR. But only save best TPR for any
	//given FPR (keeps the ROC Curve Graph easier to read for lots of points)
	//@post sorts the ExperimentResultSet by FPR ascending
	void save_roc_script_data_points( const ExperimentResultSet& pruned_result_set,
		std::string output_prefix );

	//@purpose Save script that can create ROC cruve in gnuplot
	//@pre param resultset must be pruned
	void save_gnuplot_script( const ExperimentResultSet& pruned_result_set,
		const std::string output_prefix, const std::string drug );


	//@purpose calls all functions needed for ROC Curve to be analyzed
	//outside of hivm
	void save_all_roc_info( const ExperimentResultSet& result_set,
		const std::string output_prefix, const std::string drug );

	//@post returns result set with only the best TPR per FPR, and sorted in ascending order 
	//by FPR.
	//Also, removes undefined results from output. i.e. if tpr or fpr are undefined.
	void prune( const ExperimentResultSet& input, ExperimentResultSet& output );

	//@purpose Set basic stats from an ExperimentResult for use in this class's stat's functions
	//@post all TN, FN, TP, FP, N class var's are initialized correctly
	void set_tp_tn_fn_fp_n( const ExperimentResult* result );

	//POSITIVE = susceptible
	//NEGATIVE = resistant
	double TN;
	double FN;
	double TP;
	double FP;
	double N;

	//copy constructor
	ExperimentAnalyzer( const ExperimentAnalyzer& );

	//assignment operator
	ExperimentAnalyzer& operator= ( const ExperimentAnalyzer& );


};

#endif//EXPERIMENTANALYZER_HPP

