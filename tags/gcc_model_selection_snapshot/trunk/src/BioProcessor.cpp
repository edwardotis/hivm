
#include "BioProcessor.hpp"
#include <algorithm> //min_element and max_element
#include <cfloat>// for and double max/min values DBL_MAX,DBL_MIN
#include <boost/lexical_cast.hpp> //safe numerical to string casts
#include<boost/tokenizer.hpp>//tokenizer

#include "Log.hpp"
#include "Hash.hpp" //for cache unique id
#include "FileIO.hpp"

BioProcessor::BioProcessor(){}

BioProcessor::~BioProcessor(){}

double BioProcessor::find_min( const SvmWUMatrix& input ) const
{
	double min = DBL_MAX;
	double_iterator it;

	for(int row=0; row < input.size(); row++ )
	{
		it = std::min_element( input[row]->begin(), input[row]->end() );

		if( *it < min )
		{
			min = *it;
		}
	}
	
	return min;	
}

double BioProcessor::find_max( const SvmWUMatrix& input ) const
{
	double max = DBL_MIN;
	double_iterator it;

	for(int row=0; row < input.size(); row++ )
	{
		it = std::max_element( input[row]->begin(), input[row]->end() );

		if( *it > max )
		{
			max = *it;
		}
	}
	
	return max;	
}

double BioProcessor::scale( const double input, const double min, const double max )
{
	double result, numerator;

	double denominator = ( max - min );
	
	if( denominator == 0 )
	{
		return 0;
	}

	numerator = 2*( input - min );
	result = (numerator/denominator) - 1.0;

	return result;
}

void BioProcessor::scale_data( SvmWUMatrix& train_data, SvmWUMatrix& predictee_data )
{
	//find min of both sets and set best min
	double min1  = find_min( train_data );
	double min2  = find_min( predictee_data );
	double min   = ( min1 < min2 ? min1 : min2 );

	//find max of both sets and set best max
	double max1  = find_max( train_data );
	double max2  = find_max( predictee_data );
	double max   = ( max1 > max2 ? max1 : max2 );

	//iterate both sets of data, scaling each number

	//train set data
	scale_data( train_data, min, max );
	scale_data( predictee_data, min, max );
	
}

void BioProcessor::scale_data( SvmWUMatrix& data_set, double min, double max )
{
	for( int row=0; row < data_set.size(); row++ )
	{
		for( int i=0; i < data_set[row]->size(); i++ )
		{
			double scaled = scale( data_set[row]->at(i), min, max );
			data_set[row]->at(i) = scaled;
		}
	}
}

//TODO create cache directory if it ain't where we expectin'
void BioProcessor::save_cache( const SvmWUMatrix& train_output )
{
	std::string output;

	for( int i = 0; i < train_output.size(); i++ )
	{
		//store vector of LA scores per line
		std::vector< double >::const_iterator it;
		for( it = train_output.at(i)->begin(); it != train_output.at(i)->end(); it++ )
		{
			output.append( boost::lexical_cast<std::string>(*it) + "," );
		}

		output.append( "\n" );
	}
	
	std::string filename;
	filename.append( "cache/" );
	filename.append( Hash::hash( train_output ) );
	filename.append( "_train_LA_scores.csv" );

	try
	{
		FileIO::write( filename, output );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::string msg;
		msg.append( "Exception Type: " );
		msg.append( typeid(e).name() );
		msg.append( "\n" );
		msg.append( e.what() );
		msg.append( "Could not open cache file for writing.\n" );
		msg.append( "File path: " + filename );
		msg.append( "\nContinuing to run hivm..." );
		Log::append( msg );
	}
}

void BioProcessor::save_cache( const PreProcWUSet& train_input, const SvmWUMatrix& predictee_output )
{
	std::string output;

	for( int i = 0; i < predictee_output.size(); i++ )
	{
		//store vector of LA scores per line
		std::vector< double >::const_iterator it;
		for( it = predictee_output.at(i)->begin(); it != predictee_output.at(i)->end(); it++ )
		{
			output.append( boost::lexical_cast<std::string>(*it) + "," );
		}

		output.append( "\n" );
	}

	std::string filename;
	filename.append( "cache/" );
	filename.append( Hash::hash( train_input ) );
	filename.append( "_" );
	filename.append( Hash::hash( predictee_output ) );
	filename.append( "_predictee_LA_scores.csv" );

	try
	{
		FileIO::write( filename, output );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::string msg;
		msg.append( "Exception Type: " );
		msg.append( typeid(e).name() );
		msg.append( "\n" );
		msg.append( e.what() );
		msg.append( "Could not open cache file for writing.\n" );
		msg.append( "File path: " + filename );
		msg.append( "\nContinuing to run hivm..." );
		Log::append( msg );
	}

}

bool BioProcessor::load_cache( const PreProcWUSet& train_input, SvmWUMatrix& train_output )
{
	std::string filename;
	filename.append( "cache/" );
	filename.append( Hash::hash( train_input ) );
	filename.append( "_train_LA_scores.csv" );

	std::string input;

	try
	{
		FileIO::read( filename, input );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::string msg;
		msg.append( "Exception Type: " );
		msg.append( typeid(e).name() );
		msg.append( "\n" );
		msg.append( e.what() );
		msg.append( "Could not open cache file for reading.\n" );
		msg.append( "File path: " + filename );
		msg.append( "\nContinuing to run hivm..." );
		Log::append( msg );
		return false;
	}

	//hash name
	//if cannot open file.
		//return false
	//else
		//parse file and load up LA values

	//return true
	

	//TODO DRY with other load_cache
	//parse it, tokenize it
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	
	//tokenize rows on new lines
	boost::char_separator<char> sep_row("\r\n");//tokenize on new lines (\r required for Linux)
	tokenizer row_tokens(input, sep_row); //tokenize
	int cur_row;
	tokenizer::iterator tok_iter_row; 

	for ( tok_iter_row = row_tokens.begin(), cur_row = 0; tok_iter_row != row_tokens.end();
			tok_iter_row++, cur_row++ )
	{
		std::string line = *tok_iter_row;
		
		//TODO make this a separate function below

		//tokenize columns,
		//columns, tokenize on commas, throw away empty tokens (created by extra end of line comma)
		boost::char_separator<char> sep_col( "," ,"", boost::drop_empty_tokens );
		tokenizer col_tokens( line, sep_col); //tokenize
		tokenizer::iterator tok_iter_col;

		//create a new SvmWuSet, grabbing id and suscep from PreProcSet
		SvmWUSet* svmset_train_out = new SvmWUSet( 
			train_input[cur_row]->get_id(), 
			train_input[cur_row]->known_susceptibility()
		);

		//setup the la scores for new svmwuset from cached values
		std::vector< double > svm_wuset_la_scores;
		for (	tok_iter_col =  col_tokens.begin();
				tok_iter_col != col_tokens.end();
				tok_iter_col++ )
		{
			//typedef boost::lexical_cast< double > double_cast;
			svmset_train_out->push_back( boost::lexical_cast< double >( *tok_iter_col ) );
		}

		// and push onto SvmMatrix
		train_output.push_back( svmset_train_out );
	}

	return true;
}


bool BioProcessor::load_cache( const PreProcWUSet& train_input, const PreProcWUSet& predictee_input, SvmWUMatrix& predictees_output )
{
	std::string filename;
	filename.append( "cache/" );
	filename.append( Hash::hash( train_input ) );
	filename.append( "_" );
	filename.append( Hash::hash( predictee_input ) );
	filename.append( "_predictee_LA_scores.csv" );

	std::string input;

	try
	{
		FileIO::read( filename, input );
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::string msg;
		msg.append( "Exception Type: " );
		msg.append( typeid(e).name() );
		msg.append( "\n" );
		msg.append( e.what() );
		msg.append( "Could not open cache file for reading.\n" );
		msg.append( "File path: " + filename );
		msg.append( "\nContinuing to run hivm..." );
		Log::append( msg );
		return false;
	}

	//got it, let's parse it.
	//TODO DRY with other load cache
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	
	//tokenize rows on new lines
	boost::char_separator<char> sep_row("\r\n");//tokenize on new lines (\r required for Linux)
	tokenizer row_tokens(input, sep_row); //tokenize
	int cur_row;
	tokenizer::iterator tok_iter_row; 

	for ( tok_iter_row = row_tokens.begin(), cur_row = 0; tok_iter_row != row_tokens.end();
			tok_iter_row++, cur_row++ )
	{
		std::string line = *tok_iter_row;
		
		//TODO make this a separate function below

		//tokenize columns
		//columns, tokenize on commas, throw away empty tokens (created by extra end of line comma)
		boost::char_separator<char> sep_col( "," ,"", boost::drop_empty_tokens );
		tokenizer col_tokens( line, sep_col); //tokenize
		tokenizer::iterator tok_iter_col;

		//create a new SvmWuSet, grabbing id and susceptibility from PreProcSet
		SvmWUSet* svmset_predictee_out = new SvmWUSet( 
			predictee_input[cur_row]->get_id(), 
			predictee_input[cur_row]->known_susceptibility()
		);

		//setup the la scores for new svmwuset from cached values
		std::vector< double > svm_wuset_la_scores;
		for (	tok_iter_col =  col_tokens.begin();
				tok_iter_col != col_tokens.end();
				tok_iter_col++ )
		{
			//typedef boost::lexical_cast< double > double_cast;
			svmset_predictee_out->push_back( boost::lexical_cast< double >( *tok_iter_col ) );
		}

		// and push onto SvmMatrix
		predictees_output.push_back( svmset_predictee_out );
	}


	return true;

}

void BioProcessor::process_with_cache( 
		const PreProcWUSet& train_input, 
		const PreProcWUSet& predictee_input, 
		SvmWUMatrix& train_output,
		SvmWUMatrix& predictee_output
		)
{
	//process trainset
	if( load_cache( train_input, train_output ) == false )
	{
		process_trainset( train_input,  train_output );
		save_cache( train_output );//just trainset LA on itself
	}

	//process predictees
	if( load_cache( train_input, predictee_input,  predictee_output ) == false )
	{
		process_predictees( train_input, predictee_input,  predictee_output );
		save_cache( train_input, predictee_output );//b/c produce by predictee and trainset LA. so HASH needs both
	}

	scale_data( train_output, predictee_output );
}

void BioProcessor::process( 
		const PreProcWUSet& train_input, 
		const PreProcWUSet& predictee_input, 
		SvmWUMatrix& train_output,
		SvmWUMatrix& predictee_output
		)
{

	process_( train_input, predictee_input, train_output, predictee_output );

	//scale
	scale_data( train_output, predictee_output );
}

void BioProcessor::process_trainset( const PreProcWUSet& train_input, SvmWUMatrix& train_output)
{
	//trainset
	//makes nxn trainingset matrix
	//has identity local alignment computed
	for( int outer = 0; outer < train_input.size(); outer++ )
	{

		SvmWUSet* wu_set = new SvmWUSet( 
			train_input[outer]->get_id(), 
			train_input[outer]->known_susceptibility() 
			);
	
		for( int inner = 0; inner < train_input.size(); inner++ )
		{
			std::string aa_seq_1 = train_input[outer]->get_data();
			std::string aa_seq_2 = train_input[inner]->get_data();
			double score;

			try
			{
				score = local_alignment( 
					(char*)aa_seq_1.c_str(),
					(char*)aa_seq_2.c_str(),
					1, NULL, 0, 0
					);
			}
			catch( std::exception& e )
			{
				std::string msg = "\nException caught in BioProcessor::process_trainset call to dpl.h, local_alignment(): \n";
				msg +=   e.what();
				msg += "\n\nElapsed Time: ";
				msg += Log::elapsed_time();
				msg += "\nExiting...";
				Log::append( msg );
				std::cerr << msg;


		std::cerr << "\n\nHolding program open to see how much memory is in use. Type in a character and 'enter', to exit.\n";
		char wait;
		std::cin >> wait; 

				exit(1);
			}

			wu_set->push_back( score );
		}

		train_output.push_back( wu_set );
	}
}

void BioProcessor::process_predictees(		
		const PreProcWUSet& train_input, 
		const PreProcWUSet& predictee_input, 
		SvmWUMatrix& predictee_output 
		)
{
	////predictees
	////makes an mxn predictee matrix. n is from trainset
	////does not have identity local alignment computed
	for( int outer = 0; outer < predictee_input.size(); outer++ )
	{
		//TODO should resistant be set to int? -1 if not set?
		//having this set makes me concerned for making a mistaked
		//further down the line in the code.
		SvmWUSet* wu_set = new SvmWUSet( 
			predictee_input[outer]->get_id(), 
			predictee_input[outer]->known_susceptibility()
			//true //set susceptible to true as default
			);
		
		for( int inner = 0; inner < train_input.size(); inner++ )
		{
			std::string aa_seq_1 = predictee_input[outer]->get_data();
			std::string aa_seq_2 = train_input[inner]->get_data();
			double score;

			try
			{
				score = local_alignment( 
				(char*)aa_seq_1.c_str(),
				(char*)aa_seq_2.c_str(),
				1, NULL, 0, 0
				);
			}
			catch( std::exception& e )
			{
				std::string msg = "\nException caught in BioProcessor::process_trainset call to dpl.h, local_alignment(): \n";
				msg +=   e.what();
				msg += "\n\nElapsed Time: ";
				msg += Log::elapsed_time();
				msg += "\nExiting...";
				Log::append( msg );
				std::cerr << msg;


		std::cerr << "\n\nHolding program open to see how much memory is in use. Type in a character and 'enter', to exit.\n";
		char wait;
		std::cin >> wait; 

				exit(1);
			}


			wu_set->push_back( score ); 
		}

		predictee_output.push_back( wu_set );
	}
}

void BioProcessor::process_( 
		const PreProcWUSet& train_input, 
		const PreProcWUSet& predictee_input, 
		SvmWUMatrix& train_output,
		SvmWUMatrix& predictee_output
		)
{
	process_trainset( train_input, train_output );
	process_predictees( train_input, predictee_input, predictee_output );
}
