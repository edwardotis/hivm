#include "ExperimentAnalyzer.hpp"
#include "Log.hpp"
#include "FileIO.hpp"
#include "FunctionObjects.cpp"

//#include <cassert>

#include <boost/lexical_cast.hpp> //safe numerical to string casts
#include <boost/filesystem/path.hpp> // Portable paths
//#include <boost/filesystem/operations.hpp> // Portable paths operations

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
// TPR 1, FPR -1 (undefined). This is a perfect result. except all
//drugs would have fallen on one side of the threshold. which is sketchy.
void ExperimentAnalyzer::save_roc_script_data_points( const ExperimentResultSet& pruned_result_set,
													std::string output_prefix)
{
	//create output
	std::string output;
	output.append( "Log_Cost, Log_Gamma, TPR, FPR, \n" );		

	for(size_t i=0; i < pruned_result_set.size(); i++ )
	{
		output.append( boost::lexical_cast<std::string>( pruned_result_set.at(i)->log_cost ) + ", " );//cost
		output.append( boost::lexical_cast<std::string>( pruned_result_set.at(i)->log_gamma ) + ", " );//gamma
		output.append( boost::lexical_cast<std::string>( tpr( pruned_result_set.at(i) ) ) + ", " );
		output.append( boost::lexical_cast<std::string>( fpr( pruned_result_set.at(i) ) ) + ", " );
		output.append( "\n" );
	}

	//filename
	//std::string file_name = get_file_path( pruned_result_set ) + "_roc_data_points.csv";
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

	//best to make a copy of ExperimentResultSet here, so the original is never
	//messed with, i.e. sorted
	ExperimentResultSet input_copy( input );//copy constructor

	LessThanFpr ltf;//function object
	std::sort( input_copy.begin(), input_copy.end(), ltf );

	//group_fpr = first first result's fpr.
	//bestgrouptrpindex = 0;//first result
	//
	//for all items begginng 1 past 0
	//	if( item[i]fpr == groupfpr )//same group
	//	{
	//		//test for better tpr in group
	//		if( item[i].tpr > result[bestgrouptrpindex].tpr )
	//		{
	//			bestgrouptrpindex = i;
	//		}
	//	}
	//	else//next group
	//	{
	//		//save previous group's best
	//		besttprresult = new result( item[bestgrouptrpindex] );//copy constructor
	//		push_back (besttprresult);

	//		group_fpr = item[i].fpr;
	//		bestgrouptrpindex = i;
	//	}

	//	besttprresult = new result( item[bestgrouptrpindex] );//copy constructor
	//	push_back (besttprresult);

	double group_fpr = fpr( input_copy.at(0) );
	size_t best_group_tpr_index = 0;

	for( size_t i=1; i < input_copy.size(); i++ )//skip front
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
			ExperimentResult *er = new ExperimentResult( *input_copy.at(best_group_tpr_index) );
			output.push_back( er );

			//setup next group
			group_fpr = fpr( input_copy.at(i) );
			best_group_tpr_index = i;
		}
	}
	
	//save last fpr group
	ExperimentResult *er = new ExperimentResult( *input_copy.at(best_group_tpr_index) );
	output.push_back( er );



	////ExperimentResult *best_er_of_group = new ExperimentResult( *input_copy.front() );//copy constructor
	//size_t best_tpr_index = 0;
	//for( size_t i=0; i < input_copy.size()-1; i++ )//skip front
	//{
	//	//if nextfpr != currentfpr
	//	if( fpr( input_copy.at(i) ) != fpr( input_copy.at(i+1) ) )
	//	{
	//		//we've reached end of an fpr group. save the 'best tpr' result
	//		ExperimentResult *best_er_of_group = new ExperimentResult( *input_copy.at( best_tpr_index ) );
	//		output.push_back( best_er_of_group );//1 pointer pushed to 2 vectors; need copy constructor
	//	}
	//	else//nextfpr == currentfpr
	//	{
	//		if( tpr( input_copy.at(i+1) ) > tpr( input_copy.at(i) ) )
	//		{
	//			best_tpr_index = i+1;
	//		}
	//	}
	//}

	////final one
	//ExperimentResult *best_er_of_group = new ExperimentResult( *input_copy.at( best_tpr_index ) );
	//output.push_back( best_er_of_group );
}

//void ExperimentAnalyzer::prune( const ExperimentResultSet& input, ExperimentResultSet& output )
//{
//
//	//best to make a copy of ExperimentResultSet here, so the original is never
//	//messed with, i.e. sorted
//	ExperimentResultSet input_copy( input );//copy constructor
//
//	LessThanFpr ltf;//function object
//	std::sort( input_copy.begin(), input_copy.end(), ltf );
//
//	ExperimentResult *best_er_of_group = new ExperimentResult( *input_copy.front() );//copy constructor
//	for( size_t i=1; i < input_copy.size(); i++ )//skip front
//	{
//		//if nextfpr != currentfpr
//		if( fpr( input_copy.at(i) ) != fpr( best_er_of_group ) )
//		{
//			//we've reached end of an fpr group. save the 'best tpr' result
//			output.push_back( best_er_of_group );//1 pointer pushed to 2 vectors; need copy constructor
//			best_er_of_group = new ExperimentResult( *input_copy.at(i) );//move to next group of fpr's
//		}
//		else//nextfpr == currentfpr
//		{
//			if( tpr( input_copy.at(i) ) > tpr( best_er_of_group ) )
//			{
//				best_er_of_group = new ExperimentResult( *input_copy.at(i) );//we have a new 'best tpr' of this group
//			}
//		}
//	}
//
//	//what about the final one?
//	//1 pointer pushed to 2 vectors; need copy constructor
//	output.push_back( best_er_of_group );
//}	

	//currentfpr = resultset.fpr.front;
	//best_group_tpr_result = result().tpr.front;
	//
	//for each result in original set//starting at front()+1
	//	if( nextfpr != currentfpr)
	//		pruned_results.pushback(best_group_tpr_result)
	//	if( nextfpr == currentfpr)
	//		if( next-tpr > current_tpr )
	//			best_group_tpr_result = curr_result;


std::vector<std::string> ExperimentAnalyzer::get_ids( const ExperimentResult* result )
{
	std::vector<std::string> ids;

	for( size_t i=0; i < result->data.data_points.size(); i++ )
	{
		ids.push_back( result->data.data_points.at(i)->id );
	}
	return ids;
}

void ExperimentAnalyzer::save_model_selection_ids( const ExperimentResult* result,
				const std::string output_prefix )
{
	std::string output;
	for(size_t i=0; i < result->data.data_points.size(); i++ )
	{
		output.append( result->data.data_points.at(i)->id + '\n' );
	}

	//filename
	std::string file_name = output_prefix + "model_selection_ids.csv";

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

void ExperimentAnalyzer::save_experiment_results_data( const ExperimentResultSet& result_set,
				std::string output_prefix )
{
	//TODO will the string run out of room in big programs?
	//create output
	std::string output;
	output.append( "Log_Cost, Log_Gamma, P-error, DOR, Sensitivity, Specificity, PPV, NPV, Accuracy, Error_Rate, TPR Less FPR, TP, FP, TN, FN, KEY: -1_means_Undefined_Value, DOR = Diagnostic Odds Ratio\n" );
	
	for(size_t i=0; i < result_set.size(); i++ )
	{
		output.append( boost::lexical_cast<std::string>( result_set.at(i)->log_cost ) + ", " );//cost
		output.append( boost::lexical_cast<std::string>( result_set.at(i)->log_gamma ) + ", " );//gamma
		output.append( boost::lexical_cast<std::string>( p_error( result_set.at(i) ) ) + ", " );
		output.append( boost::lexical_cast<std::string>( diagnostic_odds_ratio( result_set.at(i) ) ) + ", " );
		output.append( boost::lexical_cast<std::string>( sensitivity( result_set.at(i) ) ) + ", " );
		output.append( boost::lexical_cast<std::string>( specificity( result_set.at(i) ) ) + ", " );
		output.append( boost::lexical_cast<std::string>( positive_predictive_value( result_set.at(i) ) ) + ", " );
		output.append( boost::lexical_cast<std::string>( negative_predictive_value( result_set.at(i) ) ) + ", " );
		output.append( boost::lexical_cast<std::string>( accuracy( result_set.at(i) ) ) + ", " );
		output.append( boost::lexical_cast<std::string>( error_rate( result_set.at(i) ) ) + ", " );
		output.append( boost::lexical_cast<std::string>( tpr_fpr_difference( result_set.at(i) ) ) + ", " );	

		set_tp_tn_fn_fp_n( result_set.at(i) );//prepare class var's for each result in the result set: TP, TN, FP, FN
		output.append( boost::lexical_cast<std::string>( TP ) + ", " );
		output.append( boost::lexical_cast<std::string>( FP ) + ", " );
		output.append( boost::lexical_cast<std::string>( TN ) + ", " );
		output.append( boost::lexical_cast<std::string>( FN ) + ", " );
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




//void ExperimentAnalyzer::analyze( const ExperimentResultSet& result_set )
//{	
////	save_experiment_info( result_set );//info to recreate experiment
//	save_experiment_results_data( result_set );//all data points and statistics
//	save_all_roc_info( result_set );//for ROC Curve
//}

void ExperimentAnalyzer::analyze( const ExperimentResultSet& result_set, const Options& options )
{	
	options.save_cmd_line_options();
	//options.save_config_file_options();
	save_experiment_results_data( result_set, options.output_prefix );//all data points and statistics
	
	if( options.purpose == "cross-validate" || options.purpose == "self-validate" )
	{
		save_all_roc_info( result_set, options.output_prefix, options.drug );//for ROC Curve. data points and script
	}
}

void ExperimentAnalyzer::save_all_roc_info( const ExperimentResultSet& result_set,
										   const std::string output_prefix, const std::string drug )
{
	ExperimentResultSet pruned_result_set;//only contains best TPR's per FPR
	prune( result_set, pruned_result_set );

	save_roc_script_data_points( pruned_result_set, output_prefix );
	save_gnuplot_script( pruned_result_set, output_prefix, drug );
}

//void ExperimentAnalyzer::save_experiment_info( const Options& options )
//{
//	//Options should handle creating the output string.
//	//SAVE AS 2 FILES? .cfg and .cmd ?
//	//NAH, just make it all one file for now., but easy to cut and paste out of.
//	//cmd line params (line by line for readibility, and also in one line form for cut and paste to rerun)
//	//rerun cmd line should point to this .cfg file
//	//config file. (should be ready to cut and paste into
//
//	//create output
//	std::string output;
//	output.append( "Experiment Type: " + options.purpose + "\n" );
//	output.append( "Drug: " + options.drug + "\n" );
//	//output.append( "Threshold: " + boost::lexical_cast<std::string>( result_set.fold_threshold ) + "\n" );
//	output.append( "Wild Type Sequence: " + options.wild_type_file + "\n" );
//	output.append( "Test Data Set: " + result_set.test_data_path + "\n" );
//	output.append( "Seed for randomizing training/test split: " + boost::lexical_cast<std::string>( result_set.seed ) + "\n" );
//	output.append( "# Training Sequences: " + boost::lexical_cast<std::string>( result_set.num_training_seqs ) + "\n" );
//	
//	//svm params used in this experiment
//	output.append( "SVM Parameters\n" );
//	output.append( "Svm Type: \n" );
//	output.append( "etc: \n" );
//	output.append( "Finish Implementing This Part. \n" );
//
//	//filename
//	std::string file_name = get_file_path( result_set ) + ".txt";
//
//	try
//	{
//		FileIO::write( file_name, output );
//	}
//	catch( boost::filesystem::filesystem_error& e )
//	{
//	    std::cerr << "Exception Type " << typeid(e).name() << std::endl;
//		std::cerr << e.what() << std::endl;
//		std::cerr << "Could not open results file for writing." << std::endl;
//		std::cerr << "Results file name: " << file_name << std::endl;
//		std::cerr << "Sending results to std err instead:" << std::endl;
//		std::cerr << output << std::endl;
//		std::cerr << "Continuing to run hivm..." << std::endl;
//	}
//}
//
////TODO test
//void ExperimentAnalyzer::save_experiment_info( const ExperimentResultSet& result_set )
//{
//	//create output
//	std::string output;
//	output.append( "Experiment Type: " + result_set.experiment_type + "\n" );
//	output.append( "Drug: " + result_set.drug + "\n" );
//	output.append( "Threshold: " + boost::lexical_cast<std::string>( result_set.fold_threshold ) + "\n" );
//	output.append( "Wild Type Sequence: " + result_set.wild_seq_path + "\n" );
//	output.append( "Test Data Set: " + result_set.test_data_path + "\n" );
//	output.append( "Seed for randomizing training/test split: " + boost::lexical_cast<std::string>( result_set.seed ) + "\n" );
//	output.append( "# Training Sequences: " + boost::lexical_cast<std::string>( result_set.num_training_seqs ) + "\n" );
//	
//	//svm params used in this experiment
//	output.append( "SVM Parameters\n" );
//	output.append( "Svm Type: \n" );
//	output.append( "etc: \n" );
//	output.append( "Finish Implementing This Part. \n" );
//
//	//filename
//	std::string file_name = get_file_path( result_set ) + ".txt";
//
//	try
//	{
//		FileIO::write( file_name, output );
//	}
//	catch( boost::filesystem::filesystem_error& e )
//	{
//	    std::cerr << "Exception Type " << typeid(e).name() << std::endl;
//		std::cerr << e.what() << std::endl;
//		std::cerr << "Could not open results file for writing." << std::endl;
//		std::cerr << "Results file name: " << file_name << std::endl;
//		std::cerr << "Sending results to std err instead:" << std::endl;
//		std::cerr << output << std::endl;
//		std::cerr << "Continuing to run hivm..." << std::endl;
//	}
//}

//DEPRECATED
std::string ExperimentAnalyzer::get_file_path( const ExperimentResultSet& result_set )
{
	//create file name leaf
	std::string file_name = Log::get_start_time();
	file_name.append( "_" + result_set.experiment_type );
	file_name.append( "_" + result_set.drug );
	file_name.append( "_t" + boost::lexical_cast<std::string>(result_set.fold_threshold) );
	file_name.append( "_seqs" + boost::lexical_cast<std::string>( result_set.num_training_seqs ) );

	//create file path
	std::string file_path = "../results/" + file_name;

	return file_path;
}


//why don't I collect TP,TN,FP,FN,N all in one place for all functions to use?
void ExperimentAnalyzer::set_tp_tn_fn_fp_n( const ExperimentResult* result )
{
	//zero out all the variables
	TN = 0;
	FN = 0;
	TP = 0;
	FP = 0;
	 N = 0;

	//N
	this->N = result->data.data_points.size();
	
	for( size_t i = 0; i < result->data.data_points.size(); i++ )
	{
		if( result->data.data_points.at(i)->predicted_susceptibility == true)
		{
			if(	result->data.data_points.at(i)->actual_susceptibility  == true )
			{
				TP++;
			}
			if(	result->data.data_points.at(i)->actual_susceptibility  == false )
			{
				FP++;
			}
		}

		if( result->data.data_points.at(i)->predicted_susceptibility == false)
		{
			if(	result->data.data_points.at(i)->actual_susceptibility  == true )
			{
				FN++;
			}
			if(	result->data.data_points.at(i)->actual_susceptibility  == false )
			{
				TN++;
			}
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

// n1 = Positive class, susceptible = TP + FN
// n2 = Negative class, resistant  = TN + FP
// pmax = min(n1, n2)/(n1+n2) = (TN + FP) / ( TP + FN + TN + FP )
double ExperimentAnalyzer::pmax( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( N == 0 )
	{
		return -1;
	}

	double n1 = TP + FN;
	double n2 = TN + FP;
	double min = std::min( n1, n2 );
	return min / ( n1 + n2 );
}

//p-error = error rate / pmax
double ExperimentAnalyzer::p_error( const ExperimentResult* result )
{
	if( pmax( result ) == -1 || pmax( result ) == 0 )
	{
		return -1;
	}

	if( error_rate( result ) == -1 )
	{
		return -1;
	}

	return error_rate( result ) / pmax( result );
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
//FP/(TN + FN)
double ExperimentAnalyzer::fpr( const ExperimentResult* result )
{
	set_tp_tn_fn_fp_n( result );

	if( ( TN + FN ) == 0 )
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
	for( size_t i=0; i < pruned_result_set.size(); i++ )
	{
		output.append( "set label ''" );
		output.append( " at " );
		output.append( boost::lexical_cast<std::string>( fpr( pruned_result_set.at(i) ) ) );//x coordinate
		output.append( "," );
		output.append( boost::lexical_cast<std::string>( tpr( pruned_result_set.at(i) ) ) );//y coordinate
		output.append( " # (" );//gnuplot comment marker
		output.append( boost::lexical_cast<std::string>( pruned_result_set.at(i)->log_cost ) );//cost
		output.append( "," );
		output.append( boost::lexical_cast<std::string>( pruned_result_set.at(i)->log_gamma ) );//gamma
		output.append( ")\n" );
	}

	output.append( "set xrange [ 0 : 1 ] noreverse nowriteback\n" );
	output.append( "set yrange [ 0 : 1.1 ] noreverse nowriteback\n" );
	output.append( "set xlabel 'False Positive Rate'\n" );
	output.append( "set ylabel 'True Positive Rate'\n" );
	output.append( "set title '" + drug + " ROC Curve. (values are logbase2)'\n" );
	output.append( "set terminal png\n" );
	output.append( "set output '" + output_prefix_path.leaf() + "_roc_curve.png'\n" );
	output.append( "plot'" + output_prefix_path.leaf() + "_roc_data_points.csv' using 4:3 with linespoints\n" );
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

