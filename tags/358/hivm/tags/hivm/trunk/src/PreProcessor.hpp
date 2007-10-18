/**
 * @class PreProcessor
 * Base class to parse the input files and get them
 * ready for SVM
 */

#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

//#include <boost/numeric/ublas/matrix.hpp> //matrix
#include <vector> //vector
#include <list>

#include "FileIO.hpp"
#include "Types.hpp" //shared typedefs
#include "PreProcWorkUnit.hpp"
#include "Options.hpp"//options for selecting entries from hivdb susecptibility files


//namespace nu = boost::numeric::ublas;

class PreProcessor{


public: 

	//default constructor
	PreProcessor();	

	//destructor
	virtual ~PreProcessor();

	//TODO this should be only public interface

	/**
	Return reference to set (of pointers) if we use smartptrs.
	Otherwise, copy. No, if we copy them, then we set of 

	If we don't use smartpointers, then we need to pass the WUSet into PreProcessor.
	Otherwise, PreProcessor destructor will erase WUSet.

	With smartptrs, WUSet will not be deleted b/c it's reference count will not be zero. 

	ok, first. smartptr?
	or create without smartptr first?

	or easy way first? Just pass in the WUSet.
	this will reliance on boost smaller.

	so, PreProcWUSet will be autoref. when it goes out of scope.
	it will call vector destructor. This will call destructor
	of PreProcWU ptr's. which will call their own destructor
	*/

/**
PRODUCTION
@purpose For use in production environment. aka, not for testing
However, this was also used for SelfTestExperiment. Which does
not split into separate training and testing tests
*/
	//DEPRECATED
	void parseInputFiles( 
		const std::string dataset_path,
		const std::string wild_sequence_path,
		const std::string drug,
		const double resistance_threshold,
		const Options& preproc_options,
		PreProcWUSet& output
		);

//@post thresholds are sorted
//@pre resistance_thresholds is sorted in ascending order
	void parseInputFiles( 
		const std::string dataset_path,
		const std::string wild_sequence_path,
		const std::string drug,
		const std::vector<double>& resistance_thresholds,
		const Options& preproc_options,
		PreProcWUSet& output
		);

/**
TESTING (CROSS-VALIDATION AND SELF-VALIDATION)
@purpose Parse input files and create a set from training data
and a set from test data.
2/3 total data = training data
1/3 total data = test data
Split by a seeded random shuffle of the total input data.
Test data should not be touched until time to run final
tests on it after all cross validation
*/
//@post thresholds are sorted
//@pre resistance_thresholds is sorted in ascending order

	//DEPRECATED
	void parseInputFiles( 
		const std::string dataset_path,
		const std::string wild_sequence_path,
		const std::string drug,
		const double resistance_threshold,
		const Options& preproc_options,
		size_t seed,//for randomly picking training and test sets
		PreProcWUSet& train_output,
		PreProcWUSet& test_output
		);

//@pre resistance_thresholds is sorted in ascending order
	void parseInputFiles( 
		const std::string dataset_path,
		const std::string wild_sequence_path,
		const std::string drug,
		const std::vector<double>& resistance_thresholds,
		const Options& preproc_options,
		size_t seed,//for randomly picking training and test sets
		PreProcWUSet& train_output,
		PreProcWUSet& test_output
		);

	string_spread_sheet load_spread_sheet( const std::string input_path );

	//@purpose load and parse the seqs
	//@pre seq files are in FASTA format
	std::string load_wild_seq( const std::string input_path );

	//@purpose blend wild type sequence with mutations
	//@pre Row has already been screened for unknown mutation types and other errors
	//@post return mutation list
	std::string create_mutation_string( const std::string wild_sequence, const string_spread_sheet_row& );

	//@purpose screen mutation list for unknown characters
	//@post Return true if all mutation recognized, otherwise false.
	//	Log these mutation lists that fail this function
	//bool is_known_mutation_list( const std::string mutation_list ) const;

	//@purpose remove rows where drug has no test result or result is outside assay limits
	//@ref: http://hivdb.stanford.edu/cgi-bin/GenoPhenoDS.cgi
	//@pre  spread sheet has been loaded with HIVDB dataset
	//@post spread sheet doesn't change contents
	//@post returns a container of indices to rows that should be removed
	std::vector<size_t> screen_drug ( const std::string drug, const string_spread_sheet& );

	//@purpose make sure a row has correct number of columns
	bool is_correct_num_columns( std::string line, size_t num_header_columns  );

	//@purpose make sure a row has correct number of columns
	//@pre string is tab delimited
	size_t calc_number_columns( const std::string input );

	//@purpose remove rows from spreadsheet
	//@params spread sheet and container of indices of rows to delete
	//@post spreadsheet dimensions will be smaller
	//@post header row will not be delted
	void erase_rows( string_spread_sheet&, std::vector<size_t>& rows_to_delete );

	//@purpose scan dataset for usable rows
	bool is_invalid_drug_fold( const spread_sheet_row_const_iterator&, size_t drug_fold_column );
	bool is_invalid_drug_foldmatch( const spread_sheet_row_const_iterator&, size_t drug_foldmatch_column );

	bool are_invalid_mutations( const spread_sheet_row_const_iterator& row_it );

	std::string find_isolate_name( const string_spread_sheet_row& );

	//DEPRECATED
	bool is_susceptible( const size_t drug_column, const string_spread_sheet_row& row,  double threshold );

	//@pre assums sorted thresholds
	bool is_susceptible( const size_t drug_column, const string_spread_sheet_row& row,  std::vector<double> thresholds );

	size_t find_drug_fold_col( const std::string drug, const string_spread_sheet& spread_sheet );

	//@purpose clean out indices that represent rows with no usable drug information
	void update_usable_input_indices( const std::vector<size_t>& rows_to_remove_indices, 
								 std::list<size_t>& output_indices );

	//@purpose split input dataset into a training and a testing group.
	// 2/3 to training, 1/3 to testing
	// splits raw dataset before cleaning out unusable rows. Why?
	//So, the split won't be perfect 2/3 to 1/3, but it should be within about 5% of that ratio
	void split_input_into_training_and_testing( const string_spread_sheet&,
										   const size_t seed,
										   std::list<size_t>& rows_for_training_indices,
										   std::list<size_t>&  rows_for_testing_indices );

	//@purpose find all duplicate mutated sequences for a drug
	//and  cull the duplicates so that only one sequence is left.
	//All the remaining duplicates will have their indices added
	//to the rows_to_remove output
	//@pre already ran screen_drug to find first group rows to remove
	//@note The duplicate that is kept is the first one found sequentially
	//searching the spreadsheet. All subsequent identical seqs are removed.
	//TODO Investigate whether randomizing the duplicate kept should be implemented
	void remove_duplicate_entries( 
		const std::string drug,
		const std::string wild_sequence_path,
		const string_spread_sheet& spread_sheet,
		std::vector<size_t>& rows_to_remove );

	//@purpose for situations with 2 thresholds. high and low.
	//This adds indices of all rows that fall into medium category to
	// the rows_to_remove output vector
	//@pre thresholds is sorted least to greatest
	void remove_medium_fold_entries( 
		size_t drug_column,
		const string_spread_sheet& spread_sheet,
		const std::vector<double> threholds,
		std::vector<size_t>& rows_to_remove
		);

	void log_duplicate_entry(
		const string_spread_sheet& spread_sheet,
		const std::string drug,
		const size_t base_row,
		const size_t test_row,
		const std::string base_mut_seq,
		const std::string test_mut_seq
		);

	////@purpose return a seeded random int in range 0-(range-1) inclusive
	//int random( const int seed, const int range );

private:


	//TODO remove magic numbers
	int P1_INDEX;
	int P99_INDEX;
	int ISOLATE_NAME_COLUMN;
	std::string VALID_MUTATION_CHARS;


	//@purpose helper to load spreadsheet columns
	void tokenize_columns( const std::string line, const int cur_row, 
		std::vector< std::vector < std::string > >& _spread_sheet );

	//@purpose screen out rows based on hivdb Type
	void screen_type( 
		const std::string drug, 
		const string_spread_sheet&, 
		const std::string susceptibility_type, 
		std::vector<size_t>& rows_to_remove_indices
		);

	//@purpose find any column header in spreadsheet. ex. Type, Method, IsolateName
	size_t find_header_col( const std::string header_name, const string_spread_sheet& spread_sheet );

	//copy constructor
	PreProcessor( const PreProcessor& );

	//assignment operator
	PreProcessor& operator= ( const PreProcessor& );

};

#endif // PREPROCESSOR_HPP
