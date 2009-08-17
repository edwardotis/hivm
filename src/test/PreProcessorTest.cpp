#ifndef PREPROCESSORTEST_HPP
#define PREPROCESSORTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  PREPROCESSOR_TEST || defined TEST_ALL

#include <boost/test/auto_unit_test.hpp>
#include <boost/lexical_cast.hpp> //safe numerical to string casts

#include "../PreProcessor.hpp"
#include "../FileIO.hpp"
#include "../Options.hpp"
#include "../Log.hpp" //logging
#include <iostream>

class PreProcessorTest
{

public:
	
	//default ctor
	PreProcessorTest()
	{
			//pre_proc.set_P_index("../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv");
	}

	//dtor
	~PreProcessorTest()
	{}


	void spreadsheet_dimensions()
	{

		//copy, not efficient, but who cares?
		spread_sheet = pre_proc.load_spread_sheet( 
			"../data/PreProcessorTest/strings-small-matrix-tabdelim.tsv"
			);

		//spreadsheet tests here
		BOOST_CHECK_EQUAL( spread_sheet.size(), 3 );//# rows
		BOOST_CHECK_EQUAL( spread_sheet.at(2).size(), 3 );//# cols

		BOOST_CHECK_EQUAL( spread_sheet[0][0], "now" );
		BOOST_CHECK_EQUAL( spread_sheet[0][1], "is" );
		BOOST_CHECK_EQUAL( spread_sheet[0][2], "the" );
		BOOST_CHECK_EQUAL( spread_sheet[1][0], "time" );
		BOOST_CHECK_EQUAL( spread_sheet[1][1], "for" );
		BOOST_CHECK_EQUAL( spread_sheet[1][2], "all" );
		BOOST_CHECK_EQUAL( spread_sheet[2][0], "good" );
		BOOST_CHECK_EQUAL( spread_sheet[2][1], "men" );
		BOOST_CHECK_EQUAL( spread_sheet[2][2], "to" );
	}


	void calc_cols()
	{
		std::string PR_small_last_line = "68210	B	Virologic	1151	Clinical	PNT_pat1	1.3	=	0.7	=	3	=	38	=	2.5	=	2	=	0.2	=	-	-	-	-	-	-	-	-	-	-	-	S	V	-	V	-	-	-	I	-	-	-	-	-	-	-	-	-	-	-	-	I	-	-	-	-	-	-	-	-	-	-	-	-	-	IM	A	-	-	-	-	-	-	-	-	-	-	-	-	-	N	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	T12S, I13V, I15V, L19I, V32I, M46IM, I47A, Q61N	V32I, M46IM, I47A	";
		int cols = pre_proc.calc_number_columns( PR_small_last_line );

		BOOST_CHECK_EQUAL( cols, 122 );
	}
	void spreadsheet_PR_small()
	{
		spread_sheet = pre_proc.load_spread_sheet( 
			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small.tsv"
			);

		BOOST_CHECK_EQUAL( spread_sheet.size(), 7 );//# rows
		BOOST_CHECK_EQUAL( spread_sheet.at(5).size(), 122 );//# cols

		BOOST_CHECK_EQUAL( spread_sheet[0][0],   "SeqID" );
		BOOST_CHECK_EQUAL( spread_sheet[0][119], "CompMutList" );
		BOOST_CHECK_EQUAL( spread_sheet[0][120], "MajorMutList" );
		BOOST_CHECK_EQUAL( spread_sheet[0][121], "MinorMutList" );

		BOOST_CHECK_EQUAL( spread_sheet[0][5], "IsolateName" );

		BOOST_CHECK_EQUAL( spread_sheet[1][0], "2996" );
		BOOST_CHECK_EQUAL( spread_sheet[6][0], "68210" );
		BOOST_CHECK_EQUAL( spread_sheet[1][121], "L10I, M36V, L63P, A71V, I93L" );
		BOOST_CHECK_EQUAL( spread_sheet[6][121], "" );
		
		BOOST_CHECK_EQUAL( spread_sheet[0][101], "P82" );
		BOOST_CHECK_EQUAL( spread_sheet[1][101], "A" );
		BOOST_CHECK_EQUAL( spread_sheet[2][101], "-" );
		BOOST_CHECK_EQUAL( spread_sheet[3][101], "AV" );
		BOOST_CHECK_EQUAL( spread_sheet[4][101], "Z" );
		BOOST_CHECK_EQUAL( spread_sheet[6][101], "-" );

   		//check boundary
		BOOST_CHECK_EQUAL( spread_sheet[6][121], "" );

		//check row with blanks
		BOOST_CHECK_EQUAL( spread_sheet[5][0], "54402" );
		BOOST_CHECK_EQUAL( spread_sheet[5][10], "180.0" );
		BOOST_CHECK_EQUAL( spread_sheet[5][11], "" );
		BOOST_CHECK_EQUAL( spread_sheet[5][12], "100.7" );
		BOOST_CHECK_EQUAL( spread_sheet[5][13], "=" );
		BOOST_CHECK_EQUAL( spread_sheet[5][14], "143.6" );
		BOOST_CHECK_EQUAL( spread_sheet[5][119], "L10I, G48M, I54V, L63P, A71I, V77I, V82A, L90M" );	

	}




	void parse_input_files_pr_single_set()
	{
		std::vector<std::string> drugs;
		drugs.push_back( "APV" );
		drugs.push_back( "ATV" );
		drugs.push_back( "IDV" );
		drugs.push_back( "LPV" );
		drugs.push_back( "NFV" );
		drugs.push_back( "RTV" );
		drugs.push_back( "SQV" );

		for( int i = 0; i < drugs.size(); i++ )
		{
			

			PreProcWUSet wu_set;

			pre_proc.parseInputFiles(
				"../data/PreProcessorTest/PR_2006-05-25_v2.0.tsv",
				"../data/shared/PI_wild.seq",
				drugs[i],
				3.4,
				options,
				wu_set
				);

			//each wuset should be <= total possible dataset size
			BOOST_CHECK_PREDICATE( std::less_equal< int >(), 
					(wu_set.size()) (621)
					);

			//tests
			for( int wu_i = 0; wu_i < wu_set.size(); wu_i++ )
			{
				//no empty ids
				BOOST_CHECK_PREDICATE( std::not_equal_to< std::string>(),
					("") ( wu_set[ wu_i ]->get_id() ) 
					);

				//id doesn't equal header id
				BOOST_CHECK_PREDICATE( std::not_equal_to< std::string>(),
					("IsolateName") ( wu_set[ wu_i ]->get_id() ) 
					);
				
				//no mut_seqs that equal wild_seq
				BOOST_CHECK_PREDICATE( std::not_equal_to< std::string>(),
					( wu_set[ wu_i ]->get_data() )("PQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF")  
					);
			}
		}
	}


	void parse_train_test_no_crossover()
	{
		//- make sure there is no cross-over in full data set.
		//run through id's of train and test set.
		//make sure each id is unique.
		//-do this for mutation sequences as well.
		
		int seed = 42;

		PreProcessor p_split;
		PreProcWUSet train_set_1;
		PreProcWUSet test_set_1;

		PreProcessor p_single;
		PreProcWUSet full_set;
				
		p_split.parseInputFiles(
			"../data/shared/PR_2006-05-25_v2.0.tsv",
			"../data/shared/PI_wild.seq",
			"NFV",
			1000,//means all should be non-resistant
			options,
			seed,
			train_set_1,
			test_set_1
			);	

		p_single.parseInputFiles(
			"../data/shared/PR_2006-05-25_v2.0.tsv",
			"../data/shared/PI_wild.seq",
			"NFV",
			1000,//means all should be non-resistant
			options,
			full_set
			);	

		//now test that train and test sum to total rows:
		int combined_size = train_set_1.size() + test_set_1.size();
		BOOST_CHECK_EQUAL( combined_size, full_set.size() );

		//Now test that there's no crossover between test and train
		for( int i=0; i < train_set_1.size(); i++ )
		{
			for( int j=0; j < test_set_1.size(); j++ )
			{
				//check id's
				BOOST_CHECK_PREDICATE( std::not_equal_to< std::string>(),
				( train_set_1.at(i)->get_id() ) ( test_set_1.at(j)->get_id() ) 
				);

				////check mutated seq's
				//if( train_set_1.at(i)->get_data() == test_set_1.at(j)->get_data() )
				//{
				//	std::cout << "Found duplicate seqs:\n";
				//	std::cout << train_set_1.at(i)->get_id() << "\n";
				//	std::cout << test_set_1.at(j)->get_id() << "\n";
				//}

				BOOST_CHECK_PREDICATE( std::not_equal_to< std::string>(),
				( train_set_1.at(i)->get_data() ) ( test_set_1.at(j)->get_data() ) 
				);
			}
		}
	}


	void load_wild_seq_PR()
	{
		std::string pi_seq = pre_proc.load_wild_seq( "../data/shared/PI_wild.seq" );

		BOOST_CHECK_EQUAL( pi_seq, 
			"PQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF"
			);
	}



	void is_correct_num_columns()
	{
		std::string line;
		line = "Hi	there	you";
		
		BOOST_CHECK_EQUAL(
			pre_proc.is_correct_num_columns( line, 3 ), true
			);

		line = "Hi there";
		BOOST_CHECK_EQUAL(
			pre_proc.is_correct_num_columns( line, 3 ), false
			);

		line = "Hi	there	\n";
		BOOST_CHECK_EQUAL(
			pre_proc.is_correct_num_columns( line, 3 ), true
			);
	}

	void spreadsheet_PR_check_size()	
	{
		spread_sheet = pre_proc.load_spread_sheet( 
			"../data/PreProcessorTest/PR_2006-05-25_v2.0.tsv"
			);

		
		BOOST_CHECK_EQUAL( spread_sheet.size(), 622 );//# rows
		BOOST_CHECK_EQUAL( spread_sheet.at(621).size(), 122 );//# cols
	}
	void screen_drug_and_erase_rows_pr()	
	{
		std::vector<std::string> drugs;
		drugs.push_back( "APV" );
		drugs.push_back( "ATV" );
		drugs.push_back( "IDV" );
		drugs.push_back( "LPV" );
		drugs.push_back( "NFV" );
		drugs.push_back( "RTV" );
		drugs.push_back( "SQV" );

		int base_index = 6;
		int fold_index = base_index;
		int foldmatch_index = fold_index + 1;

		for( int i = 0; i < drugs.size(); i++ )
		{
			fold_index = base_index + 2*i;
			foldmatch_index = fold_index + 1;

			

			seq = pre_proc.load_wild_seq( "../data/shared/PI_wild.seq" );

			spread_sheet = pre_proc.load_spread_sheet( 
				"../data/PreProcessorTest/PR_2006-05-25_v2.0.tsv"
				); 

			BOOST_CHECK_EQUAL( spread_sheet.size(), 622 );

			//screen based on drug
			rows_to_remove = pre_proc.screen_drug( drugs[i], spread_sheet );

			//remove the unnecessary rows
			pre_proc.erase_rows( spread_sheet, rows_to_remove );

			//check results of screening
			for( int row = 1; row < spread_sheet.size(); row++  )//skip header row
			{
				std::string msg = drugs[i] + " ";
				msg.append( "[" + boost::lexical_cast<std::string>(row+1) + "," + boost::lexical_cast<std::string>(fold_index+1) + "]" );
							
				//msg.append( " spread_sheet[" + boost::lexical_cast<std::string>(row) + "][" ) ;
				//msg.append( boost::lexical_cast<std::string>(fold_index) + "]\n" );
				//msg.append( 
				//	boost::lexical_cast<std::string>( spread_sheet[row][fold_index] ) + "should not equal 'na' \n" 
				//	);
				//BOOST_CHECK_MESSAGE( spread_sheet[row][fold_index] != "na", msg );//fold	
				BOOST_CHECK_PREDICATE( std::not_equal_to< std::string >(), 
					(spread_sheet[row][fold_index])("na") );

				//BOOST_CHECK_PREDICATE( std::not_equal_to< std::string >(), 
				//	(spread_sheet[row][fold_index])("") );

				std::string msg2 = drugs[i] + " ";
				msg2.append( "[" + boost::lexical_cast<std::string>(row+1) + "," + boost::lexical_cast<std::string>(foldmatch_index+1) + "]" );
				//msg2.append( "Row: " + boost::lexical_cast<std::string>(row) + "\n" );
				//msg2.append( " spread_sheet[" + boost::lexical_cast<std::string>(row) + "][" ) ;
				//msg2.append( boost::lexical_cast<std::string>(foldmatch_index) + "]\n" );
				//msg2.append( 
				//	boost::lexical_cast<std::string>( spread_sheet[row][foldmatch_index] ) + "!= '=' \n" 
				//	);

				BOOST_CHECK_MESSAGE( spread_sheet[row][foldmatch_index] == "=", msg2 );//fold
				BOOST_CHECK_EQUAL( spread_sheet[row][foldmatch_index] ,  "="  );//fold match

				//no good way to test create_mutation_string for all datasets, but I'll run it here to make sure
				//it doesn't crash the system, etc. 
				//I tested create_mutation_string with a small sample set in another test
				std::string mutated_seq = pre_proc.create_mutation_string( seq, spread_sheet[row] );
			}
		}
	}

	void erase_rows_pr_small2()
	{
	spread_sheet = pre_proc.load_spread_sheet( 
			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small2.tsv"
			);

		BOOST_CHECK_EQUAL( spread_sheet.size(), 16 );

		//erase_rows should check to not remove 0 index
		rows_to_remove.push_back( spread_sheet.size()-1 );//seqid 8674
		rows_to_remove.push_back( 1 );//seqid 8648
		rows_to_remove.push_back( 9 );//seqid 12930
		rows_to_remove.push_back( 9 );//seqid 12930 duplicate removal
		rows_to_remove.push_back( 0 );//header row (should not be deleted)
		rows_to_remove.push_back( 15 );//seqid 8674 duplicate removal

		pre_proc.erase_rows( spread_sheet, rows_to_remove );

		BOOST_CHECK_EQUAL( spread_sheet.size(), 13 );
		BOOST_CHECK_EQUAL( spread_sheet[0][0], "SeqID" );//retain header

		for( int row=1; row < spread_sheet.size(); row++ )
		{
			BOOST_CHECK_PREDICATE( std::not_equal_to<std::string>(), (spread_sheet[row][0]) ("8648") );
			BOOST_CHECK_PREDICATE( std::not_equal_to<std::string>(), (spread_sheet[row][0]) ("8674") );
			BOOST_CHECK_PREDICATE( std::not_equal_to<std::string>(), (spread_sheet[row][0]) ("12930") );
		}
	}

	void screen_drug_pr_small2()
	{
		{
			spread_sheet = pre_proc.load_spread_sheet( 
				"../data/PreProcessorTest/PR_2006-05-25_v2.0_small2.tsv"
				);

			rows_to_remove = pre_proc.screen_drug( "APV", spread_sheet );

			//size == # rows
			BOOST_CHECK_EQUAL( spread_sheet.size(), 16 );
			BOOST_CHECK_EQUAL( rows_to_remove.size(), 12 );
		}

		{
			spread_sheet = pre_proc.load_spread_sheet( 
				"../data/PreProcessorTest/PR_2006-05-25_v2.0_small2.tsv"
				);

			rows_to_remove = pre_proc.screen_drug( "SQV", spread_sheet );

			//size == # rows
			BOOST_CHECK_EQUAL( spread_sheet.size(), 16 );
			BOOST_CHECK_EQUAL( rows_to_remove.size(), 7 );

			//check header
			BOOST_CHECK_EQUAL( spread_sheet[0][18], "SQV Fold" );
			BOOST_CHECK_EQUAL( spread_sheet[0][19], "SQV FoldMatch" );

		}

		{
			spread_sheet = pre_proc.load_spread_sheet( 
				"../data/PreProcessorTest/PR_2006-05-25_v2.0_small2.tsv"
				);

			rows_to_remove = pre_proc.screen_drug( "LPV", spread_sheet );

			//size == # rows
			BOOST_CHECK_EQUAL( spread_sheet.size(), 16 );
			BOOST_CHECK_EQUAL( rows_to_remove.size(), 14 );

			//for( spread_sheet_row_const_iterator row_it = spread_sheet.begin()+1;//skip header row
			//	row_it != spread_sheet.end();
			//	row_it++
			//	)
			//{
			//	BOOST_CHECK( (*row_it)[12] != "na" );//fold
			//	BOOST_CHECK_EQUAL( (*row_it)[13] , "=" );//fold match
			//}
		}
	}



	void create_mutation_string_pr_small()
	{
		seq = pre_proc.load_wild_seq( "../data/shared/PI_wild.seq" );

		spread_sheet = pre_proc.load_spread_sheet( 
			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv"
			);

		//todo Refactor this as a requirement for PreProcessor ctor
//		pre_proc.set_P_index("../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv");
		//screen and clean datasets
		rows_to_remove = pre_proc.screen_drug( "APV", spread_sheet );
		pre_proc.erase_rows( spread_sheet, rows_to_remove );

		std::string mutated_seq;

		//single substitutions
		mutated_seq = pre_proc.create_mutation_string( seq, spread_sheet[1] );//use first nonheader row
		BOOST_CHECK_EQUAL( spread_sheet[1][0], "4432" );//do this each to verify testee row is the one I want
		BOOST_CHECK_EQUAL( mutated_seq, 
			"PQITLWQRPLVTVKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQIPIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF" 
		);

		//deletion
		mutated_seq = pre_proc.create_mutation_string( seq, spread_sheet[3] );
		BOOST_CHECK_EQUAL( spread_sheet[3][0], "1" );//do this each to verify testee row is the one I want
		BOOST_CHECK_EQUAL( mutated_seq, 
			"PITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF" 
			);

		////boundaries
		mutated_seq = pre_proc.create_mutation_string( seq, spread_sheet[4] );
		BOOST_CHECK_EQUAL( spread_sheet[4][0], "2" );//do this each to verify testee row is the one I want
		BOOST_CHECK( 
			mutated_seq == "GQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNP" ||
			mutated_seq == "AQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNP"
			);

		//random mixture substituions (formerly double)
		//D60ED, I72TI are the mixtures to test
		mutated_seq = pre_proc.create_mutation_string( seq, spread_sheet[2] );
		BOOST_CHECK_EQUAL( spread_sheet[2][0], "4387" );//do this each to verify testee row is the one I want
		//todo there are 4 possible outcomes. make OR statement to check that one is true.
		Log::append("mutated_seq: " + mutated_seq );
		BOOST_CHECK( 
			mutated_seq == "PQITLWQRPLVTIKVGGQLKEALLDTGADDTVLEDMELPGRWKPKMIGGIGGFIKVKQYEQIPIEICGHKATGTVLVGPTPVNIIGRNLLTQIGCTLNF" ||
			mutated_seq == "PQITLWQRPLVTIKVGGQLKEALLDTGADDTVLEDMELPGRWKPKMIGGIGGFIKVKQYEQIPIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF" ||
			mutated_seq == "PQITLWQRPLVTIKVGGQLKEALLDTGADDTVLEDMELPGRWKPKMIGGIGGFIKVKQYDQIPIEICGHKATGTVLVGPTPVNIIGRNLLTQIGCTLNF" ||
			mutated_seq == "PQITLWQRPLVTIKVGGQLKEALLDTGADDTVLEDMELPGRWKPKMIGGIGGFIKVKQYDQIPIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF" 
			
			);	
	}


	void parseInputFiles_pr_small3()
	{
		{
				PreProcWUSet wu_set;

				pre_proc.parseInputFiles(
					"../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv",
					"../data/shared/PI_wild.seq",
					"APV",
					3.4,
					options,
					wu_set
					);

				BOOST_CHECK_EQUAL( 4, wu_set.size() );

				BOOST_CHECK_EQUAL( "CA3809", wu_set[0]->get_id() );
				BOOST_CHECK_EQUAL( true, wu_set[0]->known_susceptibility() );
				BOOST_CHECK_EQUAL( "PQITLWQRPLVTVKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQIPIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF",
					wu_set[0]->get_data() );

				BOOST_CHECK_EQUAL( "CA3872", wu_set[1]->get_id() );
				BOOST_CHECK_EQUAL( false, wu_set[1]->known_susceptibility() );
				BOOST_CHECK_EQUAL( "PQITLWQRPLVTIKVGGQLKEALLDTGADDTVLEDMELPGRWKPKMIGGIGGFIKVKQYEDQIPIEICGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF",
					wu_set[1]->get_data() );

				BOOST_CHECK_EQUAL( "CA3876", wu_set[3]->get_id() );
				BOOST_CHECK_EQUAL( false, wu_set[3]->known_susceptibility() );
				BOOST_CHECK_EQUAL( "AGQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNP",
					wu_set[3]->get_data() );


			}

			BOOST_CHECKPOINT( "No memory leaks here from PreProcessorWUSet?" );
	}


	void parseInputFiles_pr_small7highlow_two_thresholds()
	{
		//Test two thresholds, but dumping medium group of input seq's. i.e. only high and low are kept

		PreProcWUSet wu_set;
		std::vector<double> thresholds;
		thresholds.push_back( 2 );//low threshold
		thresholds.push_back( 10);//high threshold

		//using the selftest version of parseInputFiles
		pre_proc.parseInputFiles(
			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small7_highlow.tsv",
			"../data/shared/PI_wild.seq",
			"NFV",
			thresholds,
			options,
			wu_set
			);

		//*** Breakdown of file by hand based on 2 and 10 thresholds inclusive.
		//no duplicates or >, < results.
		//index, H = high, L = low
		//0 header row
		//1 H
		//2 L
		//3 H
		//4 M
		//5 H
		//6 M
		//7 L

		//post preprocess indices
		//0 H 
		//1 L
		//2 H
		//3 H
		//4 L

		BOOST_CHECK_EQUAL( 5, wu_set.size() );//7 minus two medium resistant

		BOOST_CHECK_EQUAL( "CA2634", wu_set[0]->get_id() );
		BOOST_CHECK_EQUAL( false, wu_set[0]->known_susceptibility() );

		BOOST_CHECK_EQUAL( "SD-10", wu_set[1]->get_id() );
		BOOST_CHECK_EQUAL( true, wu_set[1]->known_susceptibility() );

		BOOST_CHECK_EQUAL( "ALD-pt611", wu_set[2]->get_id() );
		BOOST_CHECK_EQUAL( false, wu_set[2]->known_susceptibility() );

		BOOST_CHECK_EQUAL( "V2031", wu_set[3]->get_id() );
		BOOST_CHECK_EQUAL( false, wu_set[3]->known_susceptibility() );

		BOOST_CHECK_EQUAL( "CA3872", wu_set[4]->get_id() );
		BOOST_CHECK_EQUAL( true, wu_set[4]->known_susceptibility() );
	}

	void Destructors2()
	{
		{
			PreProcessor p;
			
			
			PreProcWUSet wu_set;
			
			p.parseInputFiles(
				"../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv",
				"../data/shared/PI_wild.seq",
				"NFV",
				1000,//means all should be non-resistant,
				options,
				wu_set
				);

			PreProcWUSet train_set;
			train_set.push_back( wu_set[0] );
			train_set.push_back( wu_set[1] );
			train_set.push_back( wu_set[2] );

			PreProcWUSet predictee_set;
			predictee_set.push_back( wu_set[3] );
			predictee_set.push_back( wu_set[4] );

			wu_set.clear();//clean out original full set.

			predictee_set.at(0)->get_id();
		}//what happens here when 3 destructors are called here, and same memory
		//has 2 pointers? it breaks unless I clear out.

	}



	void parse_train_test_sets_seeded_shuffle()
	{
		//tests
		//ok, we are shuffling with seed.

		//- we could test that shuffling is seeded correctly each time.
		//i.e. run 2x in here. save the id's from each, and make sure
		//they are identical.
		
		int seed = 42;

		std::vector<std::string> train_id_set_1;
		std::vector<std::string> train_id_set_2;

		std::vector<std::string> test_id_set_1;
		std::vector<std::string> test_id_set_2;

		{
			
			PreProcessor p1;
			PreProcWUSet train_set_1;
			PreProcWUSet test_set_1;
			
			
			p1.parseInputFiles(
				"../data/shared/PR_2006-05-25_v2.0.tsv",
				"../data/shared/PI_wild.seq",
				"NFV",
				1000,//means all should be non-resistant
				options,
				seed,
				train_set_1,
				test_set_1
				);

			//load id's from trainset1
			for( int i=0; i < train_set_1.size(); i++)
			{
				train_id_set_1.push_back( train_set_1.at(i)->get_id() );
			}
			
			//load id's from testset1
			for( int i=0; i < test_set_1.size(); i++)
			{
				test_id_set_1.push_back( test_set_1.at(i)->get_id() );
			}	
		}


		{
			
			PreProcessor p2;
			PreProcWUSet train_set_2;
			PreProcWUSet test_set_2;
			
			p2.parseInputFiles(
				"../data/shared/PR_2006-05-25_v2.0.tsv",
				"../data/shared/PI_wild.seq",
				"NFV",
				1000,//means all should be non-resistant
				options,
				seed,
				train_set_2,
				test_set_2
				);

			//load id's from trainset2
			for( int i=0; i < train_set_2.size(); i++)
			{
				train_id_set_2.push_back( train_set_2.at(i)->get_id() );
			}
			
			//load id's from testset2
			for( int i=0; i < test_set_2.size(); i++)
			{
				test_id_set_2.push_back( test_set_2.at(i)->get_id() );
			}	
		}

		//ID's
		//now test make sure seeded shuffle was same both times with seed 42
		BOOST_CHECK_EQUAL( train_id_set_1.size(), train_id_set_2.size() );

		for( int i=0; i < train_id_set_1.size(); i++ )
		{
			BOOST_CHECK_EQUAL( train_id_set_1.at(i), train_id_set_2.at(i) );
		}

		BOOST_CHECK_EQUAL( test_id_set_1.size(), test_id_set_2.size() );

		for( int i=0; i < test_id_set_1.size(); i++ )
		{
			BOOST_CHECK_EQUAL( test_id_set_1.at(i), test_id_set_2.at(i) );
		}
	}

	void parse_train_test_sizes()
	{
		PreProcessor p_split;
		PreProcWUSet train_set_1;
		PreProcWUSet test_set_1;
		int seed = 42;

		p_split.parseInputFiles(
			"../data/shared/PR_2006-05-25_v2.0.tsv",
			"../data/shared/PI_wild.seq",
			"NFV",
			1000,//means all should be non-resistant
			options,
			seed,
			train_set_1,
			test_set_1
			);	

		//now test that train and test sum to total rows:
		int combined_size = train_set_1.size() + test_set_1.size();

		double two_thirds_size = (double)combined_size * ( 2.0/3.0 );
		double one_third_size =  (double)combined_size * ( 1.0/3.0 );

		BOOST_CHECK_EQUAL( combined_size, (two_thirds_size + one_third_size) );

		//train set should be within 8% of 2/3's give or take
		double five_percent_two_thirds = two_thirds_size * .08;

		BOOST_CHECK_PREDICATE( std::greater_equal< double >(),
		( train_set_1.size() ) ( two_thirds_size - five_percent_two_thirds ) 
		);

		BOOST_CHECK_PREDICATE( std::less_equal< double >(),
		( train_set_1.size() ) ( two_thirds_size + five_percent_two_thirds ) 
		);

		//test set should be within 8% of 1/3's give or take
		double five_percent_one_third = one_third_size * .08;

		BOOST_CHECK_PREDICATE( std::greater_equal< double >(),
		( test_set_1.size() ) ( one_third_size - five_percent_one_third ) 
		);

		BOOST_CHECK_PREDICATE( std::less_equal< double >(),
		( test_set_1.size() ) ( one_third_size + five_percent_one_third ) 
		);
	}


	void parseInputFiles_versions_produce_identical_results()
	{
		int seed = 42;

		PreProcessor p_split;
		PreProcWUSet train_set_1;
		PreProcWUSet test_set_1;

		PreProcessor p_single;
		PreProcWUSet full_set;
				
		p_split.parseInputFiles(
			"../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv",
			"../data/shared/PI_wild.seq",
			"NFV",
			1000,//means all should be non-resistant
			options,
			seed,
			train_set_1,
			test_set_1
			);	

		p_single.parseInputFiles(
			"../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv",
			"../data/shared/PI_wild.seq",
			"NFV",
			1000,//means all should be non-resistant
			options,
			full_set
			);	

		int trainsize = train_set_1.size();
		int testsize = test_set_1.size();
		int fullsize = full_set.size();

		//put train and test together into comboset
		PreProcWUSet comboset;
		for( int j=0; j < trainsize; j++ )
		{
			comboset.push_back( train_set_1.back() );
			train_set_1.pop_back();
		}
		for( int k=0; k < testsize; k++ )
		{
			comboset.push_back( test_set_1.back() );
			test_set_1.pop_back();
		}

		//check sizes
		BOOST_CHECK_EQUAL( 0, test_set_1.size() );
		BOOST_CHECK_EQUAL( 0, train_set_1.size() );
		BOOST_CHECK_EQUAL( full_set.size(), comboset.size() );

		//Are all workunits in fullset unique?
		for( int b=0; b < full_set.size(); b++ )
		{
			int duplicates = 0;
			for( int c=0; c < full_set.size(); c++ )
			{
				if( full_set.at(b)->get_id() == full_set.at(c)->get_id() &&
					b != c )//fullset(3) should equal fullset(3).
				{
					duplicates++;
				}
			}

			//make sure each entry occurs exactly one time
			BOOST_CHECK_EQUAL( 0, duplicates );
		}


		//are all workunits in comboset unique? or are there duplicates? 
		for( int b=0; b < comboset.size(); b++ )
		{
			int duplicates = 0;
			for( int c=0; c < comboset.size(); c++ )
			{
				if( comboset.at(b)->get_id() == comboset.at(c)->get_id() &&
					b != c )//fullset(3) should equal fullset(3).
				{
					duplicates++;
				}
			}

			//make sure each entry occurs exactly one time, i.e. zero duplicates
			BOOST_CHECK_EQUAL( 0, duplicates );
		}

		//now test that both versions of parseInput work the same given identical options and input.
		//i.e. wuset = trainset + testset but with different order of items
		// go through each entry in fullset and comboset
		for( int b=0; b < full_set.size(); b++ )
		{
			int count = 0;
			for( int c=0; c < comboset.size(); c++ )
			{
				if( full_set.at(b)->get_id() == comboset.at(c)->get_id() )
				{
					//make sure entries are identical when id's match
					BOOST_CHECK_EQUAL( full_set.at(b)->get_data(), comboset.at(c)->get_data() ); 
					BOOST_CHECK_EQUAL( full_set.at(b)->known_susceptibility(), comboset.at(c)->known_susceptibility() );

					count++;
				}
			}

			//make sure each entry occurs exactly one time
			BOOST_CHECK_EQUAL( 1, count );
		}
	}
	void find_isolate_name_pr_small2()
	{
		spread_sheet = pre_proc.load_spread_sheet( 
			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small2.tsv"
			);

		BOOST_CHECK_EQUAL( "SD-21", pre_proc.find_isolate_name( spread_sheet[1] ) ); 
		BOOST_CHECK_EQUAL( "SD-2", pre_proc.find_isolate_name( spread_sheet[15] ) ); 
		BOOST_CHECK_EQUAL( "IsolateName", pre_proc.find_isolate_name( spread_sheet[0] ) ); 
		BOOST_CHECK_EQUAL( "CA10670", pre_proc.find_isolate_name( spread_sheet[8] ) ); 
	}


	void is_resistant_pr_small2()
	{
		spread_sheet = pre_proc.load_spread_sheet( 
			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small2.tsv"
			);

		std::string drug = "APV";
		int fold_col = pre_proc.find_drug_fold_col( drug, spread_sheet );
		double threshold = 5.2;

		//TODO fix tests
//		BOOST_CHECK_EQUAL( false, pre_proc.is_susceptible(fold_col, spread_sheet[8], threshold) ) ; 
//		BOOST_CHECK_EQUAL( true, pre_proc.is_susceptible(fold_col, spread_sheet[9], threshold) ) ; 
//		BOOST_CHECK_EQUAL( true, pre_proc.is_susceptible(fold_col, spread_sheet[10], threshold) ) ; 
	}
	void find_duplicate_entries()
	{
		
		std::string wild_seq_path = "../data/shared/PI_wild.seq";

		//TODO make a special small spreadsheet with duplicates so you can 
		//make sure they are getting removed.
		//but let's run the full thing once for fun!
		spread_sheet = pre_proc.load_spread_sheet( 
			"../data/PreProcessorTest/PR_2006-05-25_v2.0.tsv"
			);

		pre_proc.remove_duplicate_entries( "NFV", wild_seq_path, spread_sheet, rows_to_remove );
	}

	void find_duplicate_entries_small_NFV_dups()
	{
		std::string wild_seq_path = "../data/shared/PI_wild.seq";

		//TODO make a special small spreadsheet with duplicates so you can 
		//make sure they are getting removed.
		//but let's run the full thing once for fun!
		spread_sheet = pre_proc.load_spread_sheet( 
			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small8_with_NFV_duplicates.tsv"
			);

		pre_proc.remove_duplicate_entries( "NFV", wild_seq_path, spread_sheet, rows_to_remove );

		//tests
		//find some duplicates by hand and make sure they are being removed
		//search with mutation list (if it's not all jacked.)
		
		//My test data has 8 seqs.
		//1 dup with 2 entries
		//1 dup with 3 entries
		BOOST_CHECK_EQUAL( 3, rows_to_remove.size() );

		int first_dup_index = 2;//SD-18
		BOOST_CHECK_EQUAL( first_dup_index, rows_to_remove.at( 0 ) );

		int second_dup_index = 7;//SD-5_44
		BOOST_CHECK_EQUAL( second_dup_index, rows_to_remove.at( 1 ) );

		int third_dup_index = 8;//WAC-3
		BOOST_CHECK_EQUAL( third_dup_index, rows_to_remove.at( 2 ) );
	}

	void update_usable_input_indices()
	{
		std::vector<int> rows_to_remove_indices;
		std::list<int> training_indices;

		rows_to_remove_indices.push_back( 0 );
		rows_to_remove_indices.push_back( 2 );
		rows_to_remove_indices.push_back( 4 );
		rows_to_remove_indices.push_back( 5 );

		training_indices.push_back( 0 );
		training_indices.push_back( 1 );
		training_indices.push_back( 2 );
		training_indices.push_back( 3 );
		training_indices.push_back( 4 );

		pre_proc.update_usable_input_indices( rows_to_remove_indices, training_indices );

		BOOST_CHECK_EQUAL( training_indices.size(), 2 );
		BOOST_CHECK_EQUAL( training_indices.front(), 1 );
		BOOST_CHECK_EQUAL( training_indices.back(), 3 );
	}



private:

	PreProcessor pre_proc;
	FileIO		 file_io;
	Options options;
	std::string input;
	std::string seq;
	string_spread_sheet  spread_sheet;
	std::vector<int> rows_to_remove;
};

//test private member data with these unit tests
BOOST_AUTO_TEST_SUITE( PreProcessorTests_private );

BOOST_AUTO_TEST_CASE( PreProcessorTest_spreadsheet_PR_check_size )
{
	PreProcessorTest t;
	t.spreadsheet_PR_check_size ();
}


BOOST_AUTO_TEST_CASE( PreProcessorTest_update_usable_input_indices )
{
	PreProcessorTest t;
	t.update_usable_input_indices ();
}


BOOST_AUTO_TEST_CASE( PreProcessorTest_load_wild_seq_PR )
{
	PreProcessorTest t;
	t.load_wild_seq_PR ();
}


BOOST_AUTO_TEST_CASE( PreProcessorTest_is_correct_num_columns )
{
	PreProcessorTest t;
	t.load_wild_seq_PR ();
}
BOOST_AUTO_TEST_CASE( PreProcessorTest_spreadsheet_dimensions )
{
	PreProcessorTest t;
	t.spreadsheet_dimensions();
}

BOOST_AUTO_TEST_CASE( PreProcessorTest_calc_cols )
{
	PreProcessorTest t;
	t.calc_cols();	
}

BOOST_AUTO_TEST_CASE( PreProcessorTest_spreadsheet_PR_small)
{
	PreProcessorTest t;
	t.spreadsheet_PR_small();	
}


//BOOST_AUTO_TEST_CASE( PreProcessorTest_screen_drug_and_erase_rows_pr )
//{
//	PreProcessorTest t;
//	t.screen_drug_and_erase_rows_pr();	
//
//}


BOOST_AUTO_TEST_CASE( PreProcessorTest_erase_rows_pr_small2 )
{
	PreProcessorTest t;
	t.erase_rows_pr_small2 ();	
}


//BOOST_AUTO_TEST_CASE( PreProcessorTest_screen_drug_pr_small2 )
//{
//	PreProcessorTest t;
//	t.screen_drug_pr_small2 ();	
//}


BOOST_AUTO_TEST_CASE( PreProcessorTest_create_mutation_string_pr_small )
{
	PreProcessorTest t;
	t.create_mutation_string_pr_small ();	
}


BOOST_AUTO_TEST_CASE( PreProcessorTest_find_isolate_name_pr_small2 )
{	
	PreProcessorTest t;
	t.find_isolate_name_pr_small2();	
}

BOOST_AUTO_TEST_CASE( PreProcessorTest_is_resistant_pr_small2 )
{	
	PreProcessorTest t;
	t.is_resistant_pr_small2();		
}

//BOOST_AUTO_TEST_CASE( PreProcessorTest_parseInputFiles_pr_small3 )
//{
//	PreProcessorTest t;
//	t.parseInputFiles_pr_small3 ();	
//}


//BOOST_AUTO_TEST_CASE( PreProcessorTest_parseInputFiles_pr_small7highlow_two_thresholds )
//{
//	PreProcessorTest t;
//	t.parseInputFiles_pr_small7highlow_two_thresholds();		
//}


BOOST_AUTO_TEST_CASE( PreProcessorTest_Destructors2 )
{
	PreProcessorTest t;
	t.Destructors2 ();	
}

BOOST_AUTO_TEST_CASE( PreProcessorTest_find_duplicate_entries_small_NFV_dups )
{
	PreProcessorTest t;
	t.find_duplicate_entries_small_NFV_dups();	
}



//#endif
//- test that the sizes are roughly 2/3 and 1/3
//how? calculate 2/3's. add 5%, subtract 5%.
//and use greaterthan and lessthan predicates

#if defined  LONG_TESTS || defined TEST_ALL

BOOST_AUTO_TEST_CASE( PreProcessorTest_parse_input_files_pr_single_set )
{
	PreProcessorTest t;
	t.parse_input_files_pr_single_set();	
}


BOOST_AUTO_TEST_CASE( PreProcessorTest_parse_train_test_no_crossover )
{
	PreProcessorTest t;
	t.parse_train_test_no_crossover();	
}

BOOST_AUTO_TEST_CASE( PreProcessorTest_parse_train_test_sizes )
{
	PreProcessorTest t;
	t.parse_train_test_sizes();	
}

BOOST_AUTO_TEST_CASE( PreProcessorTest_parse_train_test_sets_seeded_shuffle )
{
	PreProcessorTest t;
	t.parse_train_test_sets_seeded_shuffle ();	
}

//#if defined  LONG_TESTS || defined TEST_ALL
BOOST_AUTO_TEST_CASE( PreProcessorTest_find_duplicate_entries )
{
	PreProcessorTest t;
	t.find_duplicate_entries();	
}

//Test the choosing hibdb result type: clinical, lab or both

//now test that both versions of parseInput work the same given identical options and input.
	//i.e. wuset = trainset + testset but with different order of items

BOOST_AUTO_TEST_CASE( PreProcessorTest_parseInputFiles_versions_produce_identical_results )
{
	PreProcessorTest t;
	t.parseInputFiles_versions_produce_identical_results();	
}

#endif//#if defined  LONG_TESTS || defined TEST_ALL

BOOST_AUTO_TEST_SUITE_END();//private

//test only pure public interfaces
//BOOST_AUTO_TEST_SUITE( PreProcessorTests_public );
//
//BOOST_AUTO_TEST_CASE( PreProcessorTest_hivdb_susceptibility_type )
//{
//
//	//TEST WITH NO SPLITTING OF TRAIN AND TEST DATA
//	//test by checking sizes
//	//clinical
//	{
//		PreProcWUSet train_set;
//		PreProcessor p;
//		Options o;
//		o.susceptibility_type = "Clinical";
//		
//		p.parseInputFiles(
//			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small.tsv",
//			"../data/shared/PI_wild.seq",
//			"NFV",
//			1,//threshold
//			o,
//			train_set
//			);
//
//		BOOST_CHECK_EQUAL( train_set.size(), 5 );	
//	}
//
//	//lab
//	{
//		PreProcWUSet train_set;
//		PreProcessor p;
//		Options o;
//		o.susceptibility_type = "Lab";
//		
//		p.parseInputFiles(
//			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small.tsv",
//			"../data/shared/PI_wild.seq",
//			"NFV",
//			1,//threshold
//			o,
//			train_set
//			);
//
//		BOOST_CHECK_EQUAL( train_set.size(), 1 );	
//		BOOST_CHECK_EQUAL( train_set.at(0)->get_id(), "V2031" );
//	}
//
//	//all
//	{
//		PreProcWUSet train_set;
//		PreProcessor p;
//		Options o;
//		o.susceptibility_type = "All";
//		
//		p.parseInputFiles(
//			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small.tsv",
//			"../data/shared/PI_wild.seq",
//			"NFV",
//			1,//threshold
//			o,
//			train_set
//			);
//
//		BOOST_CHECK_EQUAL( train_set.size(), 6 );	
//	}
//
//
//	//TEST WITH SPLITTING OF TRAIN AND TEST DATA
//	//test by checking sizes
//	//clinical
//	{
//		PreProcWUSet train_set;
//		PreProcWUSet test_set;
//		PreProcessor p;
//		Options o;
//		o.susceptibility_type = "Clinical";
//		int seed = 42;
//		
//		p.parseInputFiles(
//			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small.tsv",
//			"../data/shared/PI_wild.seq",
//			"NFV",
//			1,//threshold
//			o,
//			seed,
//			train_set,
//			test_set
//			);
//
//		BOOST_CHECK_EQUAL( ( train_set.size() + test_set.size() ), 5 );	
//	}
//
//	//lab
//	{
//		PreProcWUSet train_set;
//		PreProcWUSet test_set;
//		PreProcessor p;
//		Options o;
//		o.susceptibility_type = "Lab";
//		int seed = 42;
//		
//		p.parseInputFiles(
//			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small.tsv",
//			"../data/shared/PI_wild.seq",
//			"NFV",
//			1,//threshold
//			o,
//			seed,
//			train_set,
//			test_set
//			);
//
//		BOOST_CHECK_EQUAL( ( train_set.size() + test_set.size() ), 1 );	
//		//BOOST_CHECK_EQUAL( train_set.at(0)->get_id(), "V2031" );
//	}
//
//	//all
//	{
//		PreProcWUSet train_set;
//		PreProcWUSet test_set;
//		PreProcessor p;
//		Options o;
//		o.susceptibility_type = "All";
//		int seed = 42;
//		
//		p.parseInputFiles(
//			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small.tsv",
//			"../data/shared/PI_wild.seq",
//			"NFV",
//			1,//threshold
//			o,
//			seed,
//			train_set,
//			test_set
//			);
//
//		BOOST_CHECK_EQUAL( ( train_set.size() + test_set.size() ), 6 );	
//	}
//}
//BOOST_AUTO_TEST_SUITE_END();//PreProcessorTests_public
//
//BOOST_AUTO_TEST_SUITE( PreProcWorkUnitTests );
//
//BOOST_AUTO_TEST_CASE( PreProcWorkUnitTest_WorkUnit_copy_constructor )
//{
//	//test that they are equal
//	//test that we don't have any pointers to same stuff
//	{
//		PreProcWorkUnit original( "ID1", "ABCD", true );
//		PreProcWorkUnit copy( original );
//
//		BOOST_CHECK_EQUAL( original.get_id(), copy.get_id() );
//		BOOST_CHECK_EQUAL( original.get_data(), copy.get_data() );
//		BOOST_CHECK_EQUAL( original.known_susceptibility(), copy.known_susceptibility() );
//	}//both destructors called here, so this detects pointing to same memory
//}
//
//BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  PREPROCESSOR_TEST || defined TEST_ALL
#endif //PREPROCESSORTEST_HPP
