#include "ExperimentAnalyzer.hpp" //.
#include "Log.hpp"
#include "FileIO.hpp"
#include "FunctionObjects.cpp"

#include <boost/lexical_cast.hpp> //safe numerical to string casts
//#include <boost/filesystem/path.hpp> // Portable paths
#include <boost/filesystem.hpp>

ExperimentAnalyzer::ExperimentAnalyzer()
{
	TN = 0;
	FN = 0;
	TP = 0;
	FP = 0;
	N = 0;
}

ExperimentAnalyzer::~ExperimentAnalyzer(){}

//keep an eye out for following:
// TPR 1, FPR -1 (undefined). This is a perfect result-> except all
//drugs would have fallen on one side of the threshold.
void ExperimentAnalyzer::save_roc_script_data_points( const ExperimentResultSet& pruned_result_set,
													  std::string output_prefix)
{
	double auc = compute_roc_auc( pruned_result_set, true );

	//create output
	std::string output;
	output.append( "lg_cost, lg_gamma, TPR, FPR," );
	output.append( "\n" );

	for(int i=0; i < pruned_result_set.size(); i++ )
	{
		output.append( boost::lexical_cast<std::string>( pruned_result_set.at(i)->lg_cost ) + ", " );//cost
		output.append( boost::lexical_cast<std::string>( pruned_result_set.at(i)->lg_gamma ) + ", " );//gamma
		output.append( boost::lexical_cast<std::string>( tpr( pruned_result_set.at(i) ) ) + ", " );
		output.append( boost::lexical_cast<std::string>( fpr( pruned_result_set.at(i) ) ) + ", " );
		output.append( "\n" );
	}

	//filename
	std::string file_name = output_prefix + "_roc_data_points.csv";

	try
	{
		FileIO::write( file_name, output );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::cerr << "Exception Type " << typeid(e).name() << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << "Could not open results file for writing." << std::endl;
		std::cerr << "Data Results file name: " << file_name << std::endl;
		std::cerr << "Sending results to std err instead:" << std::endl;
		std::cerr << output << std::endl;
		std::cerr << "Continuing to run hivm..." << std::endl;
	}
}

void ExperimentAnalyzer::prune( const ExperimentResultSet& input, ExperimentResultSet& output )
{
	//output input native
	//save_experiment_results_data( input, "input" );

	//best to make a copy of ExperimentResultSet here, so the original is never
	//messed with, i.e. sorted
	ExperimentResultSet input_copy( input );//copy constructor

	//Remove undefined results
	std::vector< ExperimentResult* >::iterator it;
	for( it = input_copy.begin(); it != input_copy.end(); it++ )
	{
		if( fpr( *it ) == -1 || tpr( *it ) == -1 )
		{
			delete *it;
			input_copy.erase( it );
		}
	}

	//output input_copy
	LessThanFpr ltf;//function object
	std::sort( input_copy.begin(), input_copy.end(), ltf );

	//output input_copy after sort

	double group_fpr = fpr( input_copy.at(0) );
	int best_group_tpr_index = 0;

	for( int i=1; i < input_copy.size(); i++ )//skip front
	{
		if( fpr( input_copy.at(i) ) == group_fpr )//test same group
		{
			if( tpr( input_copy.at(i) ) > tpr( input_copy.at(best_group_tpr_index) ) )//test better tpr in group
			{
				best_group_tpr_index = i;
			}
		}
		else//next fpr group
		{
			//save previous group's best
			ExperimentResult *er;
			try
			{
				er = new ExperimentResult( *input_copy.at(best_group_tpr_index) );
			}
			catch( std::exception& e )
			{
				std::string msg = "\nException caught in ExperimentAnalyzer::prune() new ExperimentResult: \n";
				msg +=   e.what();
				msg += "\n\nElapsed Time: ";
				msg += Log::elapsed_time();
				msg += "\nExiting...";
				Log::append( msg );
				std::cerr << msg;
				exit(1);
			}

			output.push_back( er );

			//setup next group
			group_fpr = fpr( input_copy.at(i) );
			best_group_tpr_index = i;
		}
	}

	//save last fpr group
	ExperimentResult *er;

	try
	{
		er = new ExperimentResult( *input_copy.at(best_group_tpr_index) );
	}
	catch( std::exception& e )
	{
		std::string msg = "\nException caught in ExperimentAnalyzer::prune() new ExperimentResult: \n";
		msg +=   e.what();
		msg += "\n\nElapsed Time: ";
		msg += Log::elapsed_time();
		msg += "\nExiting...";
		Log::append( msg );
		std::cerr << msg;
		exit(1);
	}

	output.push_back( er );
}


std::vector<std::string> ExperimentAnalyzer::get_ids( const ExperimentResult* result )
{
	std::vector<std::string> ids;

	for( int i=0; i < result->size(); i++ )
	{
		ids.push_back( result->at(i)->id );
	}
	return ids;
}

//probabilities: class 1 = predicted suceptible, class 0 = predicted resistant
std::string ExperimentAnalyzer::determine_probability( const ExperimentResult* result, const int it )
{
	std::vector<double> probabilities = result->at(it)->probabilities;
	if( probabilities.size() == 0 )//probability data was not collected
	{
		return "n/a";
	}

	if (probabilities.size() == 2) {
		if(result->at(it)->predicted_susceptibility == Options::hivm_susceptible)
			return boost::lexical_cast<std::string>(probabilities.at(1));
		else//resistant
			return boost::lexical_cast<std::string>(probabilities.at(0));
	} else if (probabilities.size() == 3) {
		if(result->at(it)->predicted_susceptibility == Options::hivm_susceptible)
			return boost::lexical_cast<std::string>(probabilities.at(0));
		if(result->at(it)->predicted_susceptibility == Options::hivm_intermediate)
			return boost::lexical_cast<std::string>(probabilities.at(1));
		if(result->at(it)->predicted_susceptibility == Options::hivm_resistant)
			return boost::lexical_cast<std::string>(probabilities.at(2));
	}
}

/**
@purpose a. save file of isolate names used, prediction, probability, c, g
(put n/a in probability column if probability not used.)
*/
void ExperimentAnalyzer::save_prediction_detailed_results( const ExperimentResult* result,
														   const std::string output_prefix )
{
	std::string output;

	//create header row
	output.append("Isolate,PredictedSusceptibility,ActualSusceptibility,PredictionProbability,lg_cost,lg_gamma\n");

	//enter data
	for(int i = 0; i < result->size(); i++ )
	{
		output.append( result->at(i)->id + "," );
		output.append( boost::lexical_cast<std::string>( result->at(i)->predicted_susceptibility ) + "," );
		output.append( boost::lexical_cast<std::string>( result->at(i)->actual_susceptibility ) + "," );
		output.append( determine_probability( result, i ) + "," );
		output.append( boost::lexical_cast<std::string>( result->lg_cost ) + "," );//cost
		output.append( boost::lexical_cast<std::string>( result->lg_gamma ) + "\n" );//gamma
	}

	//filename
	std::string file_name = output_prefix + "_prediction_detailed_results.csv";

	try
	{
		FileIO::write( file_name, output );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::cerr << "Exception Type " << typeid(e).name() << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << "Could not open prediction_detailed_results.csv file for writing." << std::endl;
		std::cerr << "Data Results file name: " << file_name << std::endl;
		std::cerr << "Sending results to std err instead:" << std::endl;
		std::cerr << output << std::endl;
		std::cerr << "Continuing to run hivm..." << std::endl;
	}
}

void ExperimentAnalyzer::save_model_selection_ids( const ExperimentResult* result,
												   const std::string output_prefix )
{
	std::string output;
	//output.append( "Isolate, Known Susceptibility" );

	for(int i=0; i < result->size(); i++ )
	{
		output.append( result->at(i)->id + '\n' );
		//output.append( result->at(i)->actual_susceptibility + '\n' );
	}

	//filename
	std::string file_name = output_prefix + "_model_selection_ids.csv";

	try
	{
		FileIO::write( file_name, output );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::cerr << "Exception Type " << typeid(e).name() << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << "Could not open model_selection_ids.csv file for writing." << std::endl;
		std::cerr << "Data Results file name: " << file_name << std::endl;
		std::cerr << "Sending results to std err instead:" << std::endl;
		std::cerr << output << std::endl;
		std::cerr << "Continuing to run hivm..." << std::endl;
	}
}

void ExperimentAnalyzer::save_dataset_ids( const PreProcWUSet& data_set, const std::string descriptor,
										   const Options& options )
{
	std::string output;

	for(int i=0; i < data_set.size(); i++ )
	{
		output.append( data_set[i]->get_id() + '\n' );
	}

	//filename
	std::string file_name = options.output_prefix + "_" +descriptor + "_dataset_ids.csv";

	try
	{
		FileIO::write( file_name, output );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::cerr << "Exception Type " << typeid(e).name() << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << "Could not open dataset file for writing." << std::endl;
		std::cerr << "Data set file name: " << file_name << std::endl;
		std::cerr << "Sending results to std err instead:" << std::endl;
		std::cerr << output << std::endl;
		std::cerr << "Continuing to run hivm..." << std::endl;
	}
}

//
// size is the number of thresholds (i.e., number of classes minus
// one).
//
void ExperimentAnalyzer::save_experiment_results_data(const ExperimentResultSet& result_set,
													  std::string output_prefix, int size)
{
	//TODO will the string run out of room in big programs?
	//create output
	std::string output;
	output.append("lg_cost,lg_gamma,M_rate,DOR,Sensitivity,Specificity,PPV,NPV,Accuracy,Error_Rate,TPR_less_FPR,TP,FP,TN,FN\n");

	for(int i=0; i < result_set.size(); i++ )
	{
		output.append( boost::lexical_cast<std::string>( result_set.at(i)->lg_cost ) + "," );//cost
		output.append( boost::lexical_cast<std::string>( result_set.at(i)->lg_gamma ) + "," );//gamma
		output.append( boost::lexical_cast<std::string>(M_rate(result_set.at(i), size))+",");
		if (size > 1) {
			//
			// For two or more thresholds, only accuracy, error rate and
			// M_rate are defined.
			//
			for (int j = 0; j < 5; j++)
				output.append("-1,");
			output.append( boost::lexical_cast<std::string>(m_acc(result_set.at(i)))+",");
			output.append( boost::lexical_cast<std::string>(m_err(result_set.at(i)))+",");
		} else {
			output.append( boost::lexical_cast<std::string>( diagnostic_odds_ratio( result_set.at(i) ) ) + "," );
			output.append( boost::lexical_cast<std::string>( sensitivity( result_set.at(i) ) ) + "," );
			output.append( boost::lexical_cast<std::string>( specificity( result_set.at(i) ) ) + "," );
			output.append( boost::lexical_cast<std::string>( positive_predictive_value( result_set.at(i) ) ) + "," );
			output.append( boost::lexical_cast<std::string>( negative_predictive_value( result_set.at(i) ) ) + "," );
			output.append( boost::lexical_cast<std::string>( accuracy( result_set.at(i) ) ) + "," );
			output.append( boost::lexical_cast<std::string>( error_rate( result_set.at(i) ) ) + "," );
		}
		if (size > 1) {
			for (int j = 0; j < 4; j++)
				output.append("-1,");
			output.append("-1");
		} else {
			output.append( boost::lexical_cast<std::string>( tpr_fpr_difference( result_set.at(i) ) ) + "," );

			set_tp_tn_fn_fp_n( result_set.at(i) );//prepare class var's for each result in the result set: TP, TN, FP, FN
			output.append( boost::lexical_cast<std::string>( TP ) + "," );
			output.append( boost::lexical_cast<std::string>( FP ) + "," );
			output.append( boost::lexical_cast<std::string>( TN ) + "," );
			output.append( boost::lexical_cast<std::string>( FN ));
		}
		output.append( "\n" );
	}

	//filename
	//std::string file_name = get_file_path( result_set ) + "_results.csv";
	std::string file_name = output_prefix + "_results.csv";

	try
	{
		FileIO::write( file_name, output );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::cerr << "Exception Type " << typeid(e).name() << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << "Could not open results file for writing." << std::endl;
		std::cerr << "Data Results file name: " << file_name << std::endl;
		std::cerr << "Sending results to std err instead:" << std::endl;
		std::cerr << output << std::endl;
		std::cerr << "Continuing to run hivm..." << std::endl;
	}
}


void ExperimentAnalyzer::analyze( const ExperimentResultSet& result_set, const Options& options )
{
	Log::append( "Number of Experiment Results collected: " +
				 boost::lexical_cast<std::string>( result_set.size() ) );
	Log::append( "Number of data points collected per Experiment: " +
				 boost::lexical_cast<std::string>( ( result_set.front()->size() ) ) );
	Log::append( "Total data points collected: " +
				 boost::lexical_cast<std::string>( ( result_set.size() * result_set.front()->size() ) ) );
	if( options.kernel_type == 2 ) {
		Log::append( "AUC without endpoints added: " +
					 boost::lexical_cast<std::string>( compute_roc_auc( result_set, false ) ) );
		Log::append( "AUC with endpoints added: " +
					 boost::lexical_cast<std::string>( compute_roc_auc( result_set, true ) ) );
	}
	//options.save_config_file_options();
	std::cerr << "\n\nSaving Results to local results directory...";

	// save all data points and statistics
	save_experiment_results_data(result_set, options.output_prefix, options.thresholds.size());

	if( options.purpose == "model-selection" )
	{
		save_all_roc_info( result_set, options.output_prefix, options.drug );//for ROC Curve. data points and script
		save_model_selection_ids( result_set.front(), options.output_prefix );
	}

	if( options.purpose == "model-validation" )
		save_prediction_detailed_results( result_set.front(), options.output_prefix );
}

void ExperimentAnalyzer::save_all_roc_info( const ExperimentResultSet& result_set,
											const std::string output_prefix, const std::string drug )
{
	ExperimentResultSet pruned_result_set;//only contains best TPR's per FPR
	prune( result_set, pruned_result_set );

	save_roc_script_data_points( pruned_result_set, output_prefix );
	save_gnuplot_script( pruned_result_set, output_prefix, drug );
}

void ExperimentAnalyzer::set_tp_tn_fn_fp_n(const ExperimentResult* result)
{
	//zero out all the variables
	TN = 0;
	FN = 0;
	TP = 0;
	FP = 0;
	N = 0;

	//N
	this->N = result->size();

	for (int i = 0; i < result->size(); i++)
	{
		if (result->at(i)->predicted_susceptibility == Options::hivm_susceptible)
		{
			if (result->at(i)->actual_susceptibility == Options::hivm_susceptible)
				TP++;
			if (result->at(i)->actual_susceptibility == Options::hivm_resistant)
				FP++;
		}

		if (result->at(i)->predicted_susceptibility == Options::hivm_resistant)
		{
			if (result->at(i)->actual_susceptibility == Options::hivm_susceptible)
				FN++;
			if (result->at(i)->actual_susceptibility == Options::hivm_resistant)
				TN++;
		}
	}

}

//ALL STATS RETURN -1 FOR DIVIDE BY ZERO
//-1 == Undefined in output (add this to header row)


//   (TP+TN)/N

double ExperimentAnalyzer::tpr_fpr_difference( const ExperimentResult* result )
{
	if( tpr( result ) == -1 )
	{
		return -1;
	}

	if( fpr( result ) == -1 )
	{
		return -1;
	}

	return ( tpr( result ) - fpr( result ) );
}

//(TP'/FN')/(FP'/TN'), TP'=TP+0.5, FN'=FN+0.5, etc
double ExperimentAnalyzer::diagnostic_odds_ratio( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	double num = ( (TP+0.5) / (FN+0.5) );
	double denom = ( (FP+0.5) / (TN+0.5) );
	return num / denom;
}

//
// pmax is error rate using majority rule, whereas all samples are
// assigned to the dominant class. Therefore accuracy is equal to the
// dominant class probability. Error rate is one minus that.
//
// pmax = 1-max(ni)/N
//
// NOTE: works only for two or three classes.
//
double ExperimentAnalyzer::pmax( const ExperimentResult* result )
{
	double retval;

	this->N = result->size();
	int n1 = 0;
	int n2 = 0;
	int n3 = 0;
	for (int i = 0; i < N; i++)
	{
		if (result->at(i)->actual_susceptibility == Options::hivm_susceptible)
			n1++;
		if (result->at(i)->actual_susceptibility == Options::hivm_intermediate)
			n2++;
		if (result->at(i)->actual_susceptibility == Options::hivm_resistant)
			n3++;
	}
	if (N == 0)
		retval = -1;
	else
	{
		int max = std::max(n1, n2);
		max = std::max(max, n3);
		retval = 1-((double) max)/N;
	}
	return retval;
}

//M_rate = error rate / pmax
double ExperimentAnalyzer::M_rate(const ExperimentResult* result, int nthd)
{
	double retval;
	double erate;

	if (pmax(result) == -1 || pmax(result) == 0)
		retval = -1;
	else
	{
		if (nthd > 1)
			erate = m_err(result);
		else
			erate = error_rate(result);
		if (erate == -1)
			retval = -1;
		else
			retval = erate/pmax(result);
	}
	return retval;
}

double ExperimentAnalyzer::accuracy( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( N == 0 )
	{
		return -1;
	}

	return (TP+TN)/N;
}

// (FP+FN)/N
double ExperimentAnalyzer::error_rate( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( N == 0 )
	{
		return -1;
	}

	return(FP+FN)/N;
}

//
// Error rate for multiple thresholds.
//
double ExperimentAnalyzer::m_err(const ExperimentResult* result)
{
	int count;
	int nx;
	double retval;

	this->N = result->size();
	nx = 0;
	for (int i = 0; i < N; i++)
		if (result->at(i)->actual_susceptibility != result->at(i)->predicted_susceptibility)
			nx++;
	if (N == 0)
		retval = -1;
	else
		retval = nx/N;
	return retval;
}

//
// Accuracy rate for multiple thresholds.
//
double ExperimentAnalyzer::m_acc(const ExperimentResult* result)
{
	int count;
	int nx;
	double retval;

	this->N = result->size();
	nx = 0;
	for (int i = 0; i < N; i++)
		if (result->at(i)->actual_susceptibility == result->at(i)->predicted_susceptibility)
			nx++;
	if (N == 0)
		retval = -1;
	else
		retval = nx/N;
	return retval;
}

//true positive rate
// TP/( TP + FN )
double ExperimentAnalyzer::tpr( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( ( TP + FN ) == 0 )
	{
		return -1;
	}

	return TP/( TP + FN );
}

//false positive rate
// 1.0 - [TN/( FP + TN )] or
// FP/(TN + FP)
double ExperimentAnalyzer::fpr( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( ( TN + FP ) == 0 )
	{
		return -1;
	}

	//return 1.0 - ( TN/( FP + TN ) );
	return FP/(TN + FP);
}

//true negative rate
// TN/( FP + TN ) == specificity
double ExperimentAnalyzer::tnr( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( ( FP + TN ) == 0 )
	{
		return -1;
	}

	return TN/( FP + TN );
}

//false negative rate
// 1.0 - [TP/( TP + FN )] or
//FN/(TP + FP)
double ExperimentAnalyzer::fnr( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( ( TP + FN ) == 0 )
	{
		return -1;
	}

	//return 1.0 - ( TP/( TP + FN ) );

	return FN/(TP + FN);
}

// TP/( TP + FN )
double ExperimentAnalyzer::sensitivity( const ExperimentResult* result )//aka true positive rate
{
	set_tp_tn_fn_fp_n( result );

	if( ( TP + FN ) == 0 )
	{
		return -1;
	}

	return TP/( TP + FN );
}

// TN/( FP + TN ) = TNR
double ExperimentAnalyzer::specificity( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( ( FP + TN ) == 0 )
	{
		return -1;
	}

	return TN/( FP + TN );
}

//  TP/(TP+FP)
double ExperimentAnalyzer::positive_predictive_value( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( (TP+FP) == 0 )
	{
		return -1;
	}

	return TP/(TP+FP);
}

//  TN/(TN+FN)
double ExperimentAnalyzer::negative_predictive_value( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( (TN+FN) == 0 )
	{
		return -1;
	}

	return TN/(TN+FN);
}



void ExperimentAnalyzer::save_gnuplot_script( const ExperimentResultSet& pruned_result_set,
											  const std::string output_prefix, const std::string drug )
{
	std::string output;

	boost::filesystem::path output_prefix_path( output_prefix );//for getting leaf of output prefix

	//create labels of c,g params
	//set label "(-4,7)" at .5,.5
	for( int i=0; i < pruned_result_set.size(); i++ )
	{
		output.append( "set label ''" );
		output.append( " at " );
		output.append( boost::lexical_cast<std::string>( fpr( pruned_result_set.at(i) ) ) );//x coordinate
		output.append( "," );
		output.append( boost::lexical_cast<std::string>( tpr( pruned_result_set.at(i) ) ) );//y coordinate
		output.append( " # (" );//gnuplot comment marker
		output.append( boost::lexical_cast<std::string>( pruned_result_set.at(i)->lg_cost ) );//cost
		output.append( "," );
		output.append( boost::lexical_cast<std::string>( pruned_result_set.at(i)->lg_gamma ) );//gamma
		output.append( ")\n" );
	}

	output.append( "set xrange [ 0 : 1 ] noreverse nowriteback\n" );
	output.append( "set yrange [ 0 : 1.1 ] noreverse nowriteback\n" );
	output.append( "set xlabel 'False Positive Rate'\n" );
	output.append( "set ylabel 'True Positive Rate'\n" );
	output.append( "set title '" + drug + " ROC Curve. Each point is TPR and FPR of a lg cost, lg gamma pair. \n" );
	//output.append( "set title '" + drug + " ROC Curve. (values are lg2). AUC = " + boost::lexical_cast<std::string>( compute_roc_auc( pruned_result_set, true )) + "'\n" );
	output.append( "set terminal png\n" );
	output.append( "set output '" + output_prefix_path.leaf().string() + "_roc_curve.png'\n" );
	output.append( "plot'" + output_prefix_path.leaf().string() + "_roc_data_points.csv' using 4:3 with linespoints\n" );
	output.append( "# EOF\n" );

	//using 4:3 with linespoints = column 4 for y-axis, col 3 for x-axis. connected by red line with points

	//filename
	//std::string file_name = get_file_path( pruned_result_set ) + "_gnuplot_script.gpl";
	std::string file_name = output_prefix + "_gnuplot_script.plt";

	try
	{
		FileIO::write( file_name, output );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::cerr << "Exception Type " << typeid(e).name() << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << "Could not open file for writing." << std::endl;
		std::cerr << "Results file name: " << file_name << std::endl;
		std::cerr << "Sending results to std err instead:" << std::endl;
		std::cerr << output << std::endl;
		std::cerr << "Continuing to run hivm..." << std::endl;
	}
}

/**
2 data points that fill entire x-axis
(0,.5) and (1,1) == (x1,y1) and (x2,y2)
Area of Trapezoid = ((a+b)h )/2, where h = x2-x1 and
a = y1-0, b = y2-0
basically rotate the roc curve 90 degrees right.
A = ((.5+1)1)/2 = .75
*/
double ExperimentAnalyzer::compute_roc_auc( const ExperimentResultSet& results, bool add_end_points )
{
	ExperimentResultSet local_results;
	prune( results, local_results );

	LessThanFpr ltf;//function object
	std::sort( local_results.begin(), local_results.end(), ltf );

	double total_area = 0;
	std::vector<ExperimentResult*>::iterator prevIt = local_results.begin();
	std::vector<ExperimentResult*>::iterator curIt = (local_results.begin()+1);

	//if first fpr is not 0.0
	//sum up the fist trapezoid from point (0,0).
	if( add_end_points == true )
	{
		total_area += calc_first_trapezoid_( *prevIt );
	}

	while(	curIt!=local_results.end() 	)
	{
		total_area += calc_trapezoid_area_( *prevIt, *curIt );
		++curIt; ++prevIt;
	}

	//if last fpr is not 1.0
	//sum up the final trapezoid to point (1,1).
	if( add_end_points == true )
	{
		total_area += calc_last_trapezoid_( *prevIt );
	}

	return total_area;
}

double ExperimentAnalyzer::calc_trapezoid_area_( ExperimentResult* first_point, ExperimentResult* second_point )
{
	double x1 = fpr(first_point);
	double x2 = fpr(second_point);
	double h = x2 - x1;//height

	double a = tpr(first_point) - 0;//parallel side a
	double b = tpr(second_point) - 0;//parallel side b

	double area = (a+b)*h/2;
	return area;
}

double ExperimentAnalyzer::calc_first_trapezoid_( ExperimentResult* first_point )
{
	double area = 0;

	//if first fpr is not 0
	//sum up the first trapezoid beginning with point (0,1)
	if(  fpr(first_point) != 0 )
	{
		std::vector<double> empty;
		// make artifical point (0,0). tpr=1, fpr=1
		struct data_point* dp0 = new data_point(
				false, true, "",  empty );//FN
		struct data_point* dp1 = new data_point(
				false, false, "", empty );//TN

		ExperimentResult *start_point;
		try
		{
			start_point = new ExperimentResult(0,0);
		}
		catch( std::exception& e )
		{
			std::string msg = "\nException caught in ExperimentAnalyzer::calc_first_trapezoid_() new ExperimentResult: \n";
			msg +=   e.what();
			msg += "\n\nElapsed Time: ";
			msg += Log::elapsed_time();
			msg += "\nExiting...";
			Log::append( msg );
			std::cerr << msg;
			exit(1);
		}

		start_point->push_back( dp0 );
		start_point->push_back( dp1 );

		area = calc_trapezoid_area_( start_point, first_point  );

		delete start_point;
	}

	return area;
}

double ExperimentAnalyzer::calc_last_trapezoid_( ExperimentResult* last_point )
{
	double area = 0;
	//if first fpr is not 0
	//sum up the first trapezoid beginning with point (0,1)
	if( fpr(last_point) != 1 )
	{
		std::vector<double> empty;

		// make artifical point (1,1). tpr=1, fpr=1
		struct data_point* dp0 = new data_point(
				true, true, "",  empty );//TP
		struct data_point* dp1 = new data_point(
				true, false, "", empty );//FP

		ExperimentResult *end_point = new ExperimentResult(1,1);
		end_point->push_back( dp0 );
		end_point->push_back( dp1 );

		area = calc_trapezoid_area_( last_point, end_point );

		delete end_point;
	}

	return area;
}

