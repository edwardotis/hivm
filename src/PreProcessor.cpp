#include "PreProcessor.hpp"
#include "Log.hpp" //logging
#include "FunctionObjects.cpp"


#include <algorithm>  //std algorithms
#include <functional> //binding

#include<boost/tokenizer.hpp>//tokenizer
#include <boost/lexical_cast.hpp> //safe numerical to string casts

PreProcessor::PreProcessor()
{
  //TODO remove column magic number
  ISOLATE_NAME_COLUMN = 5;
  // if mutation columns have anything other than this list of
  // characters, trash that row
  //Reference:
  //http://en.wikipedia.org/wiki/FASTA_format#Sequence_representation
  /**
     <empty string> no mutation
     - no mutation
     . no mutation
     * stop codon - means poor quality entry from HIVDB according to syrhee@stanford.edu
     ~ deletion
     B Aspartic acid or Asparagine
     Z Glutamate or Glutamine
     X any
  */
  VALID_MUTATION_CHARS = ".-~*ARNDCQEGHILKMFPSTWYVBZX#";
}

PreProcessor::~PreProcessor()
{}

//TODO rename. drop the 'v'

string_spread_sheet  PreProcessor::load_spread_sheet( const std::string input_path )
{

	std::string input;
	try
	{
		FileIO::read( input_path, input ); 
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::cout << e.what() << std::endl;
		Log::append( "Error in: " );
		Log::append( "vspread_sheet  PreProcessor::load_spread_sheet( const std::string input_path )");
		Log::append( e.what() );
	}
	//count new lines
	int new_lines = std::count_if( 
		input.begin(),
		input.end(),
		std::bind2nd( std::equal_to<char>(), '\n')
		);

	//b/c there's no new line at end of last row
	int rows = new_lines + 1;

	std::size_t first_nl_pos  = input.find( '\n' );
	std::string line( input.begin(), input.begin()+first_nl_pos);

	int cols;//# of columns
	cols = calc_number_columns( line );
	
	//Log dataset name and dimensions
	Log::append( input_path + " dimensions:" );
	Log::append( "Raw Spreadsheet Rows: " + boost::lexical_cast<std::string>(rows) );
	Log::append( "Raw Spreadsheet Columns: " + boost::lexical_cast<std::string>(cols) );

	//declare spread sheet
	string_spread_sheet spread_sheet(rows);
	std::fill(spread_sheet.begin(), spread_sheet.end(), std::vector<std::string>(cols, "") );
	
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
	
		//check that each row had same # tabs as header row.
		if( !is_correct_num_columns( line, cols ) )
		{
			Log::append( "Error in PreProcessor.cpp" );
			Log::append( "The following line does not have " + boost::lexical_cast<std::string>(cols) + " columns" );
			Log::append( line );
			std::cout << "Error in PreProcessor.cpp. Check logs. Exiting.." << std::endl;
			exit(1);
		}
		
		//tokenize columns, enter into spread sheet
		tokenize_columns( line, cur_row, spread_sheet );
	}

	set_P_index(spread_sheet);

	return spread_sheet;
}

void PreProcessor::tokenize_columns( const std::string line, const int cur_row, 
		string_spread_sheet& _spread_sheet )
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

	//columns, tokenize on tabs
	boost::char_separator<char> sep_col("\t", "", boost::keep_empty_tokens );//tokenize on tabs
	tokenizer col_tokens( line, sep_col); //tokenize
	int cur_col;
	tokenizer::iterator tok_iter_col;

	for (	tok_iter_col = col_tokens.begin(), cur_col = 0;
			tok_iter_col != col_tokens.end();
			tok_iter_col++, cur_col++ )
	{
		//add each token into matrix(row,column)
		std::string temp = *tok_iter_col;
		_spread_sheet[cur_row][cur_col] = *tok_iter_col;
	}
}


	

int PreProcessor::calc_number_columns( const std::string line )
{
	//find end of first row

	//boost::numeric_cast<int>(first_nl_pos) = input.find( '\n' );

	int tabs = std::count_if( 
		line.begin(),
		line.end(),
		std::bind2nd( std::equal_to<char>(), '\t')
		);

	//count tabs to get columns.
	//int tabs = std::count_if( 
	//	input.begin(),
	//	input.begin() + first_nl_pos,
	//	std::bind2nd( std::equal_to<char>(), '\t')
	//	);

	int cols = tabs + 1;

	//TODO remove
	//deal with trailing tabs which Stanford DB sometimes adds, sometimes doesn't
	//if char before new line is a tab, then columns == number of tabs.
	//if( input.at( first_nl_pos - 1 ) == '\t' )
	
	//line.back() = *(line.end()-1)
	//if( *(line.end()-1) == '\t' )
	//{
	//	cols = tabs;
	//}
	//else
	//{
	//	cols = tabs + 1;
	//}

	return cols;
}

bool PreProcessor::is_correct_num_columns( std::string line, int num_header_columns )
{
	int cur_col_count =	calc_number_columns( line );
	if( cur_col_count != num_header_columns )
	{
		return false;
	}
	
	return true;
}




std::string PreProcessor::load_wild_seq( const std::string input_path )
{
	std::string input;

	try
	{
		FileIO::read( input_path, input ); 
	}
	catch( boost::filesystem::filesystem_error& e )
	{
		std::cout << e.what() << std::endl;
		Log::append( "Error in: " );
		Log::append( "std::string PreProcessor::load_wild_seq( const std::string input_path )");
		Log::append( e.what() );
	}

	//find end of first row
	std::size_t first_nl_pos  = input.find( '\n' );

	//remove first line
	input.erase( 
		input.begin(), 
		input.begin()+first_nl_pos+1
		);

	//remove new lines
	std::string::iterator new_end = std::remove(input.begin(), input.end(), '\n');
	input.erase( new_end, input.end() );

	return input;
}

std::vector<int> PreProcessor::screen_drug ( const std::string drug, const string_spread_sheet& spread_sheet )
{
  std::vector<int> rows_to_remove;
  int rr;
  bool invalid_drug_fold;
  bool invalid_drug_foldmatch;
  bool invalid_mutation;
  int invalid_drug_fold_cntr = 0;
  int invalid_drug_foldmatch_cntr = 0;
  int invalid_mutation_cntr = 0;
  
  //find header column
  int drug_fold_col = find_drug_fold_col( drug, spread_sheet );
  
  for( int row = 1; row < spread_sheet.size(); row++ )//skip header row
    {
      spread_sheet_row_const_iterator row_it = spread_sheet.begin()+row;
      invalid_drug_fold = is_invalid_drug_fold(row_it, drug_fold_col);
      invalid_drug_foldmatch = is_invalid_drug_foldmatch(row_it, drug_fold_col+1);
      invalid_mutation = are_invalid_mutations(row_it);
	  if (invalid_drug_fold){
		invalid_drug_fold_cntr++;
	  }    
	  if (invalid_drug_foldmatch){
		invalid_drug_foldmatch_cntr++;
	  }
	  if (invalid_mutation){
		invalid_mutation_cntr++;
	  }

	  if(invalid_drug_fold || invalid_drug_foldmatch || invalid_mutation){
		rows_to_remove.push_back(row);
	  }
    }
  rr = rows_to_remove.size(); // for debugging
  printf("rows to remove: %d\n", rr);
  printf("invalid folds: %d\n", invalid_drug_fold_cntr);
  printf("invalid foldmatches: %d\n", invalid_drug_foldmatch_cntr);
  printf("invalid mutations: %d\n", invalid_mutation_cntr);
  return rows_to_remove;
}

int PreProcessor::find_drug_fold_col( const std::string drug, const string_spread_sheet& spread_sheet )
{
//make drug name match header in local var
	std::string drug_header = drug;
	drug_header.append( " Fold");

	//find iterator to header column
	spread_sheet_column_const_iterator drug_it;
	drug_it = std::find( spread_sheet[0].begin(), spread_sheet[0].end(), drug_header );
	
	//Log error if drug not found
	if( drug_it == spread_sheet[0].end() )
	{
		Log::append( "Drug not found in spread sheet." );
		Log::append( "Drug: " + drug );
		std::cout << "Drug not found: " << drug;
		std::cout << "Exiting";
		exit(1);
	}

	return drug_it - spread_sheet[0].begin();
	
}

int PreProcessor::find_header_col( const std::string header_name, const string_spread_sheet& spread_sheet )
{

	//find iterator to header column
	spread_sheet_column_const_iterator it;
	it = std::find( spread_sheet[0].begin(), spread_sheet[0].end(), header_name );
	
	//Log error if drug not found
	if( it == spread_sheet[0].end() )
	{
		Log::append( "Type header not found in spread sheet." );
		std::cout << "Exiting";
		exit(1);
	}

	return it - spread_sheet[0].begin();
}

bool PreProcessor::is_invalid_drug_fold( const spread_sheet_row_const_iterator& row_it, int drug_fold_column )
{
  bool invalid = true;
  bool valid   = false;
  
  std::string drug_fold = (*row_it).at(drug_fold_column);
  for (int jdx = 0; jdx < drug_fold.size(); jdx++)
    drug_fold[jdx] = toupper(drug_fold[jdx]);
  if( drug_fold == "NA" || drug_fold == "" )
    {
      return invalid;
    }
  return valid;
}

//
// Function effectively disabled 04/01/2008. Why remove entries with
// very low or very high fold values? We know they belong to
// resistant/susceptible categories. We don't need to know the exact
// value of the fold.
//
bool PreProcessor::is_invalid_drug_foldmatch( const spread_sheet_row_const_iterator& row_it, int drug_foldmatch_column )
{
	bool valid   = false;

	return valid;
}

bool PreProcessor::are_invalid_mutations( const spread_sheet_row_const_iterator& row_it )
{
	bool invalid = true;
	bool valid   = false;
	
	spread_sheet_column_const_iterator col_it = (*row_it).begin();
	for( int col = PSTART_INDEX; col <= PEND_INDEX; col++ )
	{
		std::string current_mutation = col_it[col];
		for (int jdx = 0; jdx < current_mutation.size(); jdx++)
		  current_mutation[jdx] = toupper(current_mutation[jdx]);
		if( current_mutation == "" )//no empty mutation allowed
		{
			Log::append( "Found Invalid Blank Mutation: " );
			return invalid;
		}
		
		//if any character in a mutation string is NOT in my valid list, then the mutation is invalid
		std::string::const_iterator str_it;
		for( str_it = current_mutation.begin(); str_it != current_mutation.end(); str_it++ )
		{
			if( std::find( VALID_MUTATION_CHARS.begin(), VALID_MUTATION_CHARS.end(), *str_it ) == VALID_MUTATION_CHARS.end() )
			{
				Log::append( "Found Invalid Mutation: " + current_mutation );
				return invalid;
			}
		}	
	}

	return valid;//all mutation were valid

}

/**
Pass in a string of amino acids, and return a randomly selected single amino acid from that param
Randomness is statictly seeded, so the method will repeatably always return same aa from a given string of aa's
@param aas amino acid string
@return single amino acid
*/
 char pickRandomAA( std::string aas){
	RandInt Rand( 52 );
	int rangeHigh = aas.length()-1 ;
    unsigned int randomPosition = Rand( 0, rangeHigh );
	return aas.at(randomPosition);
}

void PreProcessor::erase_rows( string_spread_sheet& spread_sheet, std::vector<int>& rows_to_delete )
{
	string_spread_sheet temp_sheet;

	std::sort( rows_to_delete.begin(), rows_to_delete.end(), std::less<int>() );

	// copy header
	temp_sheet.push_back( spread_sheet.front() );
	int rows_kept_counter = 1;

	//check each row to make sure that row's index isn't in the list of rows to remove
	for( int row_index = 1; row_index < spread_sheet.size(); row_index++ )
	{
		if( std::find( rows_to_delete.begin(), rows_to_delete.end(), row_index ) == rows_to_delete.end()  )
		{
			spread_sheet_row_iterator row_it = (spread_sheet.begin()+row_index);
			temp_sheet.push_back( *row_it );
			rows_kept_counter++;
		}
	}

	//Log::append( "Number of rows kept (including header row): " + boost::lexical_cast<std::string>(rows_kept_counter) );
	//int dropped = spread_sheet.size() - rows_kept_counter;
	//Log::append( "Number of rows dropped: " + boost::lexical_cast<std::string>(dropped) );

	spread_sheet.clear();
	spread_sheet = temp_sheet;

	//DON't erase like this. create temp. clear passed in. copy temp to it.
	//otherwise indices mess up. make test in small for this.
}
//TODO refactor function to use string_spread_sheet_row instead of passing an iterator.
std::string PreProcessor::create_mutation_string(const std::string wild_sequence,
						 const string_spread_sheet_row& row)
{
  //check for accidentally passing in header row
  if( row[0] == "SeqID" )
    {
      std::string msg = "Passed in header row to create_mutation_string()\nExiting program.";
      Log::append( msg );
      std::cerr << msg << std::endl;
      exit(1);
    }
  
  std::string result = wild_sequence;
  
  for (int i = 0; i < row.size(); i++)
    {
      // printf("%s", row[i].c_str());
    }
  // printf("\n");
  
  //copy p1 - p99 into a container in order to zero out index
  std::vector<std::string> mut_vector;
  for( int i = PSTART_INDEX; i <= PEND_INDEX; i++ )
    {
      std::string mut = row[i];
      mut_vector.push_back( mut );
    }
  
  //handles difference from the P1-P99 index as 
  int index_offest = 0;//can be negative or positive
  
  for( int pos = 0; pos < mut_vector.size(); pos++ )
    {
      std::string mutation = mut_vector[pos];
      std::string::iterator str_pos_iter = result.begin() + (pos + index_offest);
      
      if( mutation == ""  ||
	  mutation == "." ||
	  mutation == "-")
	{
	  //do nothing
	}
      else if ( mutation == "~" )
	{
	  //delete; delete that position at index + index_offest	
	  result.erase( str_pos_iter );
	  index_offest--;
	}
      else// insertions and substitutions
	{
	  int length = mutation.size();
	  //
	  // In case of insertion (mutation with a trailing `#'),
	  // remove the `#' from the mutation, and insert the rest
	  // after the current amino-acid. Else just replace the old
	  // aa with the mutation.
	  //
	  if (mutation.at(length-1) == '#') {
	    mutation.erase(length-1);
	    result.insert(pos+index_offest+1, mutation); //insert new aa(s) after the current position
	    index_offest += mutation.size()+1;
	  } else {//substitution
	  //Updating this code to handle aa mixture's properly. They should result in single substitution, not multiple substitution.
	    result.erase( str_pos_iter );//remove old aa
		if(length >= 2){
			if(length>2){
			  Log::append("Found a mutation mixture > 2 aa's: " + mutation + "\n"  );
			  //just use first aa found in this case for now.
			 }else if(length==2){
					//Log::append("Found a mutation mixture of size 2 aa's: " + mutation + "\n"  );	
			}
				    //Randomly select one of the aa's from the aa mixture substitution
			std::string rndMixtureAA = "";
			rndMixtureAA.push_back(pickRandomAA(mutation));

			if(rndMixtureAA.length() != 1){
				std::cerr <<"Error inserting amino acid mixture!\n";
			}
			//Log::append("Inserting randomly selected aa out of the mutation mixture: " + rndMixtureAA);
			result.insert( (pos + index_offest), rndMixtureAA );//insert new aa
		}else{//length == 1 (usual case)
			if(mutation.length() != 1){
				std::cerr <<"Error inserting amino acid!\n";
			}
			result.insert( (pos + index_offest), mutation );//insert new aa  
		}
	    //index_offest += mutation.size()-1;
	  }
	}
    }
  
  return result;
}


std::string PreProcessor::find_isolate_name( const string_spread_sheet_row& row )
{
	return row[ISOLATE_NAME_COLUMN];
}

//
// Determine and set offsets of first and last mutation column (`P
// columns') by parsing header row of input file.
//
//void PreProcessor::set_P_index(const std::string fname)
void PreProcessor::set_P_index(const string_spread_sheet& spread_sheet)
{
  int i;
  
  //string_spread_sheet spread_sheet = this->load_spread_sheet(fname);
  string_spread_sheet_row row = spread_sheet[0];
  for (i = 0; i < row.size(); i++)
    {
      if (row[i] == "P1")
	PSTART_INDEX = i;
      else if (row[i] == "CompMutList")
	PEND_INDEX = i-1;
    }
}

int PreProcessor::is_susceptible(const int drug_column, const string_spread_sheet_row& row, 
				 const std::vector<double> thresholds )
{

  double fold_strength = boost::lexical_cast<double>( row[drug_column] );
  
  if( thresholds.size() > 2 )
    {
      std::string msg;
      msg.append( "PreProcessor::is_susceptible() not equipped to handle more than 2 thresholds!" );
      msg.append( "\nExiting..." );
      Log::append( msg );
      std::cerr << msg;
      exit(1);
    }
  
  if(  fold_strength <= thresholds.at(0)  )
    return Options::hivm_susceptible;
  
  if( thresholds.size() == 2 )
    {
      if( fold_strength > thresholds.at(0) &&
	  fold_strength < thresholds.at(1) )
	return Options::hivm_intermediate;
    }
  
  return Options::hivm_resistant;
}


//TODO DRY
void PreProcessor::parseInputFiles( 
	const std::string dataset_path,
	const std::string wild_sequence_path,
	const std::string drug,
	const double resistance_threshold,
	const Options& preproc_options,
	PreProcWUSet& output
	)
{

  std::vector<double> thresholds;
  thresholds.push_back( resistance_threshold );
  
  parseInputFiles(
		  dataset_path,
		  wild_sequence_path,
		  drug,
		  thresholds,
		  preproc_options,
		  output
		  );
}

//TODO DRY (remove single threshold version)
void PreProcessor::parseInputFiles(const std::string dataset_path,
				   const std::string wild_sequence_path,
				   const std::string drug,
				   const std::vector<double>& resistance_thresholds,
				   const Options& preproc_options,
				   PreProcWUSet& output)
{
  std::string aa_seq;
  //sort least to highest
  //std::sort( resistance_thresholds.begin(), resistance_thresholds.end(), std::less<double>() );

  Log::append( "Parsing for drug: " + drug );
  
  //load spreadsheet
  string_spread_sheet	spread_sheet = this->load_spread_sheet( dataset_path );
  
  //Log size less header
  Log::append( "Pre-screening train dataset size: " + 
	       boost::lexical_cast<std::string>( spread_sheet.size()-1 ) );
  
  //screen
  std::vector<int> rows_to_remove_indices = this->screen_drug( drug, spread_sheet );
  remove_duplicate_entries( drug, wild_sequence_path, spread_sheet, rows_to_remove_indices );
  screen_type( drug, spread_sheet, preproc_options.susceptibility_type, rows_to_remove_indices );
  
  int fold_col = this->find_drug_fold_col( drug, spread_sheet );
  remove_medium_fold_entries( fold_col, spread_sheet, resistance_thresholds, rows_to_remove_indices );
  screen_type( drug, spread_sheet, preproc_options.susceptibility_type, rows_to_remove_indices );
  
  //clean
  this->erase_rows( spread_sheet, rows_to_remove_indices );
  
  //Log size less header
  Log::append( "Total rows screened out: " +  
	       boost::lexical_cast<std::string>( rows_to_remove_indices.size() ) );
  Log::append( "Post-screening train dataset size: " + 
	       boost::lexical_cast<std::string>( spread_sheet.size()-1 ) );
  
  //for each row left in spread_sheet. skipping header row
  for( int row = 1; row < spread_sheet.size(); row++ )
    {
      //get data for WU
      //id
      std::string id = this->find_isolate_name( spread_sheet[row] );
      //mutated_seq
      std::string wild_seq = this->load_wild_seq( wild_sequence_path );
      std::string mut_seq = this->create_mutation_string( wild_seq, spread_sheet[row] );
      //resistance
      int susceptibility = this->is_susceptible( fold_col, spread_sheet[row], resistance_thresholds );
      
      //create new WU 
      PreProcWorkUnit* wu = new PreProcWorkUnit( id, mut_seq, susceptibility );
      //
      // Dump the raw sequences, for analyses outside hivm.
      // Each row has one labeled sequence.
      //
      if (preproc_options.dump_sequences == 1) {
	aa_seq = wu->get_data();
	printf("%2d, %20s, %s\n", susceptibility, (char *) id.c_str(), (char *) aa_seq.c_str());
	fflush(stdout);
      }
      
      //add new WU to output set
      output.push_back( wu );
    }//end loop
  if (preproc_options.dump_sequences == 1)
    exit(0);

}

//TODO DRY
void PreProcessor::parseInputFiles( 
	const std::string dataset_path,
	const std::string wild_sequence_path,
	const std::string drug,
	const double resistance_threshold,
	const Options& preproc_options,
	int seed,//for randomly picking training and test sets
	PreProcWUSet& train_output,
	PreProcWUSet& test_output
	)
{

	std::vector<double> thresholds;
	thresholds.push_back( resistance_threshold );
	
	parseInputFiles(
		dataset_path,
		wild_sequence_path,
		drug,
		thresholds,
		preproc_options,
		seed,
		train_output,
		test_output
		);
}

//TODO DRY
void PreProcessor::parseInputFiles(const std::string dataset_path,
				   const std::string wild_sequence_path,
				   const std::string drug,
				   const std::vector<double>& resistance_thresholds,
				   const Options& preproc_options,
				   int seed,//for randomly picking training and test sets
				   PreProcWUSet& train_output,
				   PreProcWUSet& test_output)
{
  ////sort least to highest
  //std::sort( resistance_thresholds.begin(), resistance_thresholds.end(), std::less<double>() );
  
  Log::append( "Parsing for drug " + drug );
  
  //load spreadsheet
  string_spread_sheet	spread_sheet = this->load_spread_sheet( dataset_path );
  
  std::list<int> rows_for_training_indices;
  std::list<int> rows_for_testing_indices;
  
  //2/3 training, 1/3 testing.
  split_input_into_training_and_testing( spread_sheet, seed, rows_for_training_indices,
					 rows_for_testing_indices );
  
  //screen spreadsheet
  std::vector<int> rows_to_remove_indices;
  rows_to_remove_indices = screen_drug( drug, spread_sheet );
  remove_duplicate_entries( drug, wild_sequence_path, spread_sheet, rows_to_remove_indices );
  screen_type( drug, spread_sheet, preproc_options.susceptibility_type, rows_to_remove_indices );
  
  int fold_col = this->find_drug_fold_col( drug, spread_sheet );
  remove_medium_fold_entries( fold_col, spread_sheet, resistance_thresholds, rows_to_remove_indices );
  
  //screen and remove unusable rows from training and testing index sets
  update_usable_input_indices( rows_to_remove_indices, rows_for_training_indices );
  update_usable_input_indices( rows_to_remove_indices, rows_for_testing_indices );
  
  //Log sizes
  Log::append( "Total rows screened out: " +  boost::lexical_cast<std::string>( rows_to_remove_indices.size() ) );
  Log::append( "Post-screening train dataset size: " + boost::lexical_cast<std::string>( rows_for_training_indices.size() ) );
  Log::append( "Post-screening test dataset size: " + boost::lexical_cast<std::string>( rows_for_testing_indices.size() ) );
  
  //clean REMOVED!! (switching to all indices solution instead of altering size of spreadsheet)
  //this->erase_rows( spread_sheet, rows_to_remove_indices );
  
  //for each row left in spread_sheet. 
  for( int row = 1; row < spread_sheet.size(); row++ )
    {
      //printf("Analyzing row %d...\n", row);
      //if current row is NOT in rows to be removed
      if( std::count( rows_to_remove_indices.begin(), rows_to_remove_indices.end(), row ) == 0 )
	{
	  //get data for WU
	  //id
	  std::string id = this->find_isolate_name( spread_sheet[row] );
	  //mutated_seq
	  std::string wild_seq = this->load_wild_seq( wild_sequence_path );
	  std::string mut_seq = this->create_mutation_string( wild_seq, spread_sheet[row] );
	  //resistance
	  int susceptibility = this->is_susceptible( fold_col, spread_sheet[row], resistance_thresholds );
	  
	  //create new WU 
	  PreProcWorkUnit* wu = new PreProcWorkUnit(id, mut_seq, susceptibility);
	  
	  //NOW, decide whether it goes into training set or testing set
	  if( std::count( rows_for_training_indices.begin(), rows_for_training_indices.end(), row ) == 1 )
	    train_output.push_back( wu );
	  else if( std::count( rows_for_testing_indices.begin(), rows_for_testing_indices.end(), row ) == 1  )//put into testing set
	    test_output.push_back( wu );
	  else//error
	    {
	      std::string msg = "Error splitting of training and testing input data sets.\n";
	      msg.append( "Error in: PreProcessor::parseInputFiles\n" );
	      msg.append( "Exiting..." );
	      Log::append( msg );
	      std::cerr << msg;
	      exit(1);
	    }
	}
      
    }//end loop
  Log::append( "Parsing completed.");
}

void PreProcessor::split_input_into_training_and_testing( const string_spread_sheet& spread_sheet,
										const int seed,
										std::list<int>& rows_for_training_indices,
										std::list<int>&  rows_for_testing_indices )
{
	//srand(seed);//don't set seed in each iteration. just once.
	Log::append( "seed: " + boost::lexical_cast<std::string>(  seed ) );
	//Log::append( "RAND_MAX: " + boost::lexical_cast<std::string>(  RAND_MAX ) );

	RandInt Rand( seed );

	for( int i=1; i < spread_sheet.size(); i++ )//skip header index
	{	
		//make 2/3 chance of adding index to training ( or 1/3 chance adding to testging )
		//int range = 3;//0 through 2
	    //unsigned int rand_result = rand_num % range;
		unsigned int range_low = 0;
		unsigned int range_high = 9;
		unsigned int rand_result = Rand( range_low, range_high );

		//Log::append( "Rand() " + boost::lexical_cast<std::string>( rand_result ) );

		if( rand_result == range_high )
		{
			rows_for_testing_indices.push_back( i );
		}
		else
		{
			rows_for_training_indices.push_back( i );
		}
	}
	//Log sizes
	Log::append( "Pre-screening train dataset size: " + boost::lexical_cast<std::string>( rows_for_training_indices.size() ) );
	Log::append( "Pre-screening test dataset size: " + boost::lexical_cast<std::string>( rows_for_testing_indices.size() ) );
}

//
// The function disabled 12/20/2007 due to introduction of
// intermediate resistance category.
//
// Placeholder retained to simplify changes in the rest of code.
//
void PreProcessor::remove_medium_fold_entries( 
	int drug_column,
	const string_spread_sheet& spread_sheet,
	const std::vector<double> thresholds,
	std::vector<int>& rows_to_remove
	)
{
  return;
}

//#include <stdlib.h>
// this
//int PreProcessor::random( const int seed, const int range )
//{
//    //srand(seed);
//	int random = rand();
//	int result = random % range;
//	return result;
//}

//@pre already ran screen_drug to find first group rows to remove
void PreProcessor::remove_duplicate_entries( 
	const std::string drug,
	const std::string wild_sequence_path,
	const string_spread_sheet& spread_sheet,
	std::vector<int>& rows_to_remove )
{
	//initi wild_seq
	std::string wild_seq = this->load_wild_seq( wild_sequence_path );

	int num_duplicates_removed = 0;

	//for each row in spreadsheet
	for( int base_row=1; base_row < spread_sheet.size(); base_row++ )//skip header row
	{
		//if( base_row is NOT already set for removal)
		if( std::count( rows_to_remove.begin(), rows_to_remove.end(), base_row ) == 0 )
		{
			//find base mutated seq for base_row
			std::string base_mut_seq = this->create_mutation_string( wild_seq, spread_sheet[base_row] );
			
			//for every other row in spreadsheet
			for( int test_row=1; test_row < spread_sheet.size(); test_row++ )//skip header row
			{
				//don't test base_row against iself
				if( base_row != test_row )
				{
					//if( current index NOT already set for removal)
					if( std::count( rows_to_remove.begin(), rows_to_remove.end(), test_row ) == 0 )
					{
						std::string test_mut_seq = this->create_mutation_string( wild_seq, spread_sheet[test_row] );
						if( base_mut_seq == test_mut_seq )
						{
							//add current_mut_seq index to rows_to_remove
							rows_to_remove.push_back( test_row );
							num_duplicates_removed++;

							//Log the base seq and current seq and fold values
							log_duplicate_entry( spread_sheet, drug, base_row, test_row,
								base_mut_seq, test_mut_seq);
						}
					}
				}
			}
		}
	}

	Log::append( "Total duplicates removed: " + boost::lexical_cast<std::string>(num_duplicates_removed) );
	//for each row in spreadsheet
	//	if( row is NOT already set for removal)
	//		find base mutated seq for that row
	//		for every other row in spreadsheet
	//			if( current index NOT already set for removal)
	//				if( base_mut_seq == current_mut_seq )
	//					add current_mut_seq index to rows_to_remove
	//					Log the base seq and current seq and fold values
}

void PreProcessor::log_duplicate_entry(
		const string_spread_sheet& spread_sheet,
		const std::string drug,
		const int base_row,
		const int test_row,
		const std::string base_mut_seq,
		const std::string test_mut_seq
		)
{
	//collect data for Log entry

	//id's
	std::string base_id = this->find_isolate_name( spread_sheet[base_row] );
	std::string test_id = this->find_isolate_name( spread_sheet[test_row] );

	//resistance's
	int fold_col = this->find_drug_fold_col( drug, spread_sheet );
	std::string base_fold_strength = spread_sheet[base_row][fold_col];
	std::string test_fold_strength = spread_sheet[test_row][fold_col];

	std::string msg;
	msg.append( "Found duplicate mutation sequence for drug: " + drug + "\n"  );
	msg.append( "Keeping: " + base_id + "\tFold: " + base_fold_strength + "\n" );
	msg.append( "Removed: " + test_id + "\tFold: " + test_fold_strength + "\n" );

	Log::append( msg );

}

void PreProcessor::update_usable_input_indices( const std::vector<int>& rows_to_remove_indices, 
								 std::list<int>& output_indices )
{
	std::vector<int>::const_iterator it;
	for( it=rows_to_remove_indices.begin(); it != rows_to_remove_indices.end(); it++ )
	{
		int count = std::count( output_indices.begin(), output_indices.end(), *it );
		if( count > 1)//error
		{
			std::string msg = "Error splitting of training and testing input data sets.\n";
			msg.append( "Error in: PreProcessor::update_usable_input_indices\n" );
			msg.append( "Exiting..." );
			Log::append( msg );
			std::cerr << msg;
			exit(1);
		}

		if( count == 1 )//the current index should be removed from output collection
		{
			output_indices.remove( *it );
		}	
	}

	//now remove all of them.
}

void PreProcessor::screen_type( 
	const std::string drug, 
	const string_spread_sheet& spread_sheet, 
	const std::string susceptibility_type, 
	std::vector<int>& rows_to_remove_indices
	)
{
	//find header column
	int type_col = find_header_col( "Type", spread_sheet );

	for( int row = 1; row < spread_sheet.size(); row++ )//skip header row
	{
		spread_sheet_row_const_iterator row_it = spread_sheet.begin()+row;

		std::string type = (*row_it).at(type_col);

		if( susceptibility_type != "All" &&
				susceptibility_type != (*row_it).at(type_col)
		  )
		{
			rows_to_remove_indices.push_back( row );
		}
	}	
}
