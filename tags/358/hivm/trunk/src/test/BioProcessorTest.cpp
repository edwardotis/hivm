#ifndef BIOPROCESSORTEST_HPP
#define BIOPROCESSORTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  BIOPROCESSOR_TEST || defined TEST_ALL

#include "../BioProcessor.hpp"
#include "../SvmWorkUnit.hpp"
#include "../PreProcessor.hpp"
#include "../Hash.hpp"

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration
#include <boost/test/floating_point_comparison.hpp>//for comparing floating point values

struct BioProcessor_Fixture
{
	//ctor
    BioProcessor_Fixture()
	{
		for( int row=0; row < 4; row++ )
		{
			SvmWUSet* wu_set = new SvmWUSet( "my id", false );

			for(int i = 1; i < 4; i++ )
			{
				wu_set->push_back( i*20 );
			}

			wu_matrix.push_back( wu_set );
		}
	}

	//dtor
    ~BioProcessor_Fixture(){}

	SvmWUMatrix wu_matrix;

};

BOOST_AUTO_TEST_SUITE( BioProcessorTests );

BOOST_AUTO_TEST_CASE( BioProcessorTest_find_min )
{
	BioProcessor_Fixture f;

	BioProcessor b;

	BOOST_CHECK_EQUAL( 20, b.find_min( f.wu_matrix ) );

	SvmWUSet* wu_set = new SvmWUSet( "my id", false );

	wu_set->push_back( 6 );
	wu_set->push_back( 600 );
	wu_set->push_back( .3 );

	f.wu_matrix.push_back( wu_set );

	BOOST_CHECK_EQUAL( .3, b.find_min( f.wu_matrix ) );
		
}

BOOST_AUTO_TEST_CASE( BioProcessorTest_find_max )
{
	BioProcessor_Fixture f;

	BioProcessor b;

	BOOST_CHECK_EQUAL( 60, b.find_max( f.wu_matrix ) );

	SvmWUSet* wu_set = new SvmWUSet( "my id", false );

	wu_set->push_back( 6 );
	wu_set->push_back( 600 );
	wu_set->push_back( .3 );

	f.wu_matrix.push_back( wu_set );

	BOOST_CHECK_EQUAL( 600, b.find_max( f.wu_matrix ) );
}

BOOST_AUTO_TEST_CASE( BioProcessorTest_scale )
{
	//test nothing beyond -1 or 1.
	//put in some numbers that scale easily
	// 100 200 300 == -1, 0, 1?

	BioProcessor b;
	double scaled_result;

	scaled_result = b.scale( 200, 100, 300 );
	BOOST_CHECK_EQUAL( 0, scaled_result );

	scaled_result = b.scale( 100, 100, 300 );
	BOOST_CHECK_EQUAL( -1, scaled_result );

	scaled_result = b.scale( 300, 100, 300 );
	BOOST_CHECK_EQUAL( 1, scaled_result );

	scaled_result = b.scale( 150, 100, 300 );
	BOOST_CHECK_EQUAL( -.5, scaled_result );

	scaled_result = b.scale( 500, 0, 1000 );
	BOOST_CHECK_EQUAL( 0, scaled_result );

	scaled_result = b.scale( 0, 0, 1000 );
	BOOST_CHECK_EQUAL( -1, scaled_result );

	scaled_result = b.scale( 750, 0, 1000 );
	BOOST_CHECK_EQUAL( .5, scaled_result );

	scaled_result = b.scale( 900, 0, 1000 );
	BOOST_CHECK_EQUAL( .8, scaled_result );

}

BOOST_AUTO_TEST_CASE( BioProcessorTest_scale_data )
{
	//load wumatrices  0 - 1000, 0, 250, 500, 750

	SvmWUMatrix wu_matrix_train;//5x5 matrix, 200 - 1000
	for( int row=0; row < 5; row++ )
	{
		SvmWUSet* wu_set = new SvmWUSet( "my id", false );

		for(int i = 1; i <= 5; i++ )
		{
			wu_set->push_back( i*200 );
		}

		wu_matrix_train.push_back( wu_set );
	}

	SvmWUMatrix wu_matrix_predictee;//2x5, 0 - 400
	for( int row=0; row < 2; row++ )
	{
		SvmWUSet* wu_set = new SvmWUSet( "my id", false );

		for(int i = 0; i < 5; i++ )
		{
			wu_set->push_back( i*100 );
		}

		wu_matrix_predictee.push_back( wu_set );
	}

	BioProcessor b;
	b.scale_data( wu_matrix_train, wu_matrix_predictee );

	BOOST_CHECK_CLOSE( -.6, wu_matrix_train[0]->at(0), 0.0001 );//200
	BOOST_CHECK_CLOSE( -.2, wu_matrix_train[0]->at(1), 0.0001 );//400
	BOOST_CHECK_CLOSE( .2,  wu_matrix_train[0]->at(2), 0.0001  );//600
	BOOST_CHECK_CLOSE( .6,  wu_matrix_train[0]->at(3), 0.0001  );//800
	BOOST_CHECK_EQUAL( 1,   wu_matrix_train[0]->at(4) );//1000

	BOOST_CHECK_EQUAL( -1, wu_matrix_predictee[0]->at(0) );//000
	BOOST_CHECK_CLOSE( -.2, wu_matrix_predictee[0]->back(), 0.0001 );//400
	BOOST_CHECK_EQUAL( -.8, wu_matrix_predictee[1]->at(1) );//100
	BOOST_CHECK_CLOSE( -.2, wu_matrix_predictee[1]->back(), 0.0001 );//400

}

#if defined  LONG_TESTS || defined TEST_ALL
BOOST_AUTO_TEST_CASE( BioProcessorTest_process )
{

	PreProcessor p;
	PreProcWUSet wu_set;
	Options o;
	
	p.parseInputFiles(
		"../data/PreProcessorTest/PR_2006-05-25_v2.0.tsv",
		"../data/shared/PI_wild.seq",
		"NFV",
		1000,//means all should be non-resistant
		o,
		wu_set
		);

	PreProcWUSet predictee_set;

	//add 2 wu's to it from the end of the inputset
	predictee_set.push_back( wu_set.back() );
	wu_set.pop_back();
	predictee_set.push_back( wu_set.back() );
	wu_set.pop_back();

	//OUTPUT
	SvmWUMatrix train_trix;
	SvmWUMatrix predict_trix;

	BioProcessor b;
	b.process_( wu_set, predictee_set, train_trix, predict_trix );

	//check row size
	BOOST_CHECK_EQUAL( wu_set.size(), train_trix.size() );
	BOOST_CHECK_EQUAL( predictee_set.size(), predict_trix.size() );

	//check column size
	BOOST_CHECK_EQUAL( wu_set.size(), train_trix[0]->size() );
	BOOST_CHECK_EQUAL( train_trix[0]->size(), predict_trix[0]->size() );

}
#endif

//MAKE private version of this without scaling to test local align
//public version will call the private and also scale
BOOST_AUTO_TEST_CASE( BioProcessorTest_process_ )
{
	PreProcessor p;
	PreProcWUSet wu_set;
	Options o;
	
	p.parseInputFiles(
		"../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv",
		"../data/shared/PI_wild.seq",
		"NFV",
		1000,//means all should be susceptible
		o,
		wu_set
		);

	//careful with memory scope here
	//separate input file into 2 sets

	//INPUT
	PreProcWUSet train_set;
	train_set.push_back( wu_set[0] );
	train_set.push_back( wu_set[1] );
	train_set.push_back( wu_set[2] );

	PreProcWUSet predictee_set;
	predictee_set.push_back( wu_set[3] );
	predictee_set.push_back( wu_set[4] );

	//clear out original set to avoid destructor problem.
	//train_set and predictee_set now 'own' the wu_set pointers.
	wu_set.clear();
	
	//OUTPUT
	SvmWUMatrix train_trix;
	SvmWUMatrix predict_trix;

	BioProcessor b;
	b.process_( train_set, predictee_set, train_trix, predict_trix );
        
	//tests
	//dimensions of both output matrices
	BOOST_CHECK_EQUAL( 3, train_trix.size() );//3x3
	BOOST_CHECK_EQUAL( 3, train_trix[2]->size() );

	BOOST_CHECK_EQUAL( 2, predict_trix.size() );//2x3
	BOOST_CHECK_EQUAL( 3, predict_trix[1]->size() );

	//check id's
	BOOST_CHECK_EQUAL( "CA3809", train_trix[0]->get_id() );
	BOOST_CHECK_EQUAL( "CA3872", train_trix[1]->get_id() );
	BOOST_CHECK_EQUAL( "CA3874", train_trix[2]->get_id() );

	BOOST_CHECK_EQUAL( "CA3876", predict_trix[0]->get_id() );
	BOOST_CHECK_EQUAL( "SD-7",   predict_trix[1]->get_id() );

	//check resistance
	BOOST_CHECK_EQUAL( true, train_trix[0]->known_susceptibility() );
	BOOST_CHECK_EQUAL( true, train_trix[1]->known_susceptibility() );
	BOOST_CHECK_EQUAL( true, train_trix[2]->known_susceptibility() );

	BOOST_CHECK_EQUAL( true, predict_trix[0]->known_susceptibility() );
	BOOST_CHECK_EQUAL( true, predict_trix[1]->known_susceptibility() );

	//check local alignment values (not scaled yet)
	char* CA3809 = "PQITLWQRPLVTVKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQIPIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF";
	char* CA3872 = "PQITLWQRPLVTIKVGGQLKEALLDTGADDTVLEDMELPGRWKPKMIGGIGGFIKVKQYEDQIPIEICGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF";
	char* CA3874 = "PITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF";


	//Train ROW 1
	float CA3809_CA3809 = local_alignment( CA3809, CA3809, 1, NULL, 0, 0 );
	float CA3809_CA3872 = local_alignment( CA3809, CA3872, 1, NULL, 0, 0 );
	float CA3809_CA3874 = local_alignment( CA3809, CA3874, 1, NULL, 0, 0 );

	BOOST_CHECK_EQUAL( CA3809_CA3809, train_trix[0]->at(0) );
	BOOST_CHECK_EQUAL( CA3809_CA3872, train_trix[0]->at(1) );
	BOOST_CHECK_EQUAL( CA3809_CA3874, train_trix[0]->at(2) );

	//Train ROW 2
	float CA3872_CA3809 = local_alignment( CA3872, CA3809, 1, NULL, 0, 0 );
	float CA3872_CA3872 = local_alignment( CA3872, CA3872, 1, NULL, 0, 0 );
	float CA3872_CA3874 = local_alignment( CA3872, CA3874, 1, NULL, 0, 0 );

	BOOST_CHECK_EQUAL( CA3872_CA3809, train_trix[1]->at(0) );
	BOOST_CHECK_EQUAL( CA3872_CA3872, train_trix[1]->at(1) );
	BOOST_CHECK_EQUAL( CA3872_CA3874, train_trix[1]->at(2) );

	//Train ROW 3
	float CA3874_CA3809 = local_alignment( CA3874, CA3809, 1, NULL, 0, 0 );
	float CA3874_CA3872 = local_alignment( CA3874, CA3872, 1, NULL, 0, 0 );
	float CA3874_CA3874 = local_alignment( CA3874, CA3874, 1, NULL, 0, 0 );
	
	BOOST_CHECK_EQUAL( CA3874_CA3809, train_trix[2]->at(0) );
	BOOST_CHECK_EQUAL( CA3874_CA3872, train_trix[2]->at(1) );
	BOOST_CHECK_EQUAL( CA3874_CA3874, train_trix[2]->at(2) );


	//------------
	char* CA3876 = "AGQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNP";
	char* SD_7 = "PQITLWQRPIVTIKIGGQILKEALIDTGADDTVLEEMNLPGRWKPKLIGGIGGFAVKVRQYDQIPIEICGHKLVRGTVLVGPTPANIIGRNLMLTQLGFCTLNF";

	//Predict ROW 1
	float CA3876_CA3809 = local_alignment( CA3876, CA3809, 1, NULL, 0, 0 );
	float CA3876_CA3872 = local_alignment( CA3876, CA3872, 1, NULL, 0, 0 );
	float CA3876_CA3874 = local_alignment( CA3876, CA3874, 1, NULL, 0, 0 );

	BOOST_CHECK_EQUAL( CA3876_CA3809,   predict_trix[0]->at(0) );
	BOOST_CHECK_EQUAL( CA3876_CA3872,   predict_trix[0]->at(1) );
	BOOST_CHECK_EQUAL( CA3876_CA3874,   predict_trix[0]->at(2) );

	float SD_7_CA3809 = local_alignment( SD_7, CA3809, 1, NULL, 0, 0 );
	float SD_7_CA3872 = local_alignment( SD_7, CA3872, 1, NULL, 0, 0 );
	float SD_7_CA3874 = local_alignment( SD_7, CA3874, 1, NULL, 0, 0 );

	//Predict ROW 2
	BOOST_CHECK_EQUAL( SD_7_CA3809,   predict_trix[1]->at(0) );
	BOOST_CHECK_EQUAL( SD_7_CA3872,   predict_trix[1]->at(1) );
	BOOST_CHECK_EQUAL( SD_7_CA3874,   predict_trix[1]->at(2) );

	//call local alignment separately.
	//Then compare in matrices with my calculated values.


	//use small3 and create a small4 with 2 entries for prediction?
	//or just separate out one input file into 2 PreProcSets? yes.
	//still might need a new PRsmall dataset.
	//3x3 trainer and 2x3 predict
	//so, grab 5 seqs for new test set.

	//hmmm, easier to test if I don't scale inside process()
	//or I could still put all this shit into a svmwumatrix and 
	//scale it.

	//maybe just test la scores the first time. no scaling.
	//then update the unit test to include scaling if all is well.
}

BOOST_AUTO_TEST_CASE( BioProcessorTest_hash_PreProcWUSet )
{
	PreProcessor p;
	PreProcWUSet wu_set;
	Options o;
	
	p.parseInputFiles(
		"../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv",
		"../data/shared/PI_wild.seq",
		"NFV",
		1000,//means all should be susceptible
		o,
		wu_set
		);

	std::string id1 = Hash::hash( wu_set );

	//INPUT
	PreProcWUSet train_set;
	train_set.push_back( wu_set[0] ); 
	train_set.push_back( wu_set[1] );
	train_set.push_back( wu_set[2] );

	std::string id2 = Hash::hash( train_set );

	PreProcWUSet predictee_set;
	predictee_set.push_back( wu_set[3] );
	predictee_set.push_back( wu_set[4] );

	std::string id3 = Hash::hash( predictee_set );

	BOOST_CHECK_PREDICATE( std::not_equal_to< std::string >(), (id1) (id2) );
	BOOST_CHECK_PREDICATE( std::not_equal_to< std::string >(), (id1) (id3) );
	BOOST_CHECK_PREDICATE( std::not_equal_to< std::string >(), (id2) (id3) );

	//clear out original set to avoid destructor problem.
	//train_set and predictee_set now 'own' the wu_set pointers.
	wu_set.clear();
}

BOOST_AUTO_TEST_CASE( BioProcessorTest_load_save_trainset_cache )
{

	PreProcessor p;
	PreProcWUSet preproc_trainput;
	Options o;
	
	p.parseInputFiles(
		"../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv",
		"../data/shared/PI_wild.seq",
		"NFV",
		1000,//means all should be susceptible
		o,
		preproc_trainput
		);

	SvmWUMatrix svm_matrix_train;
	BioProcessor b;
	b.process_trainset( preproc_trainput, svm_matrix_train );

	//save cache
	b.save_cache( svm_matrix_train ) ;

	//read cache
	SvmWUMatrix svm_matrix_train_cache;
	bool found_cache = b.load_cache( preproc_trainput, svm_matrix_train_cache );
	
	BOOST_CHECK_EQUAL( true, found_cache );
	BOOST_CHECK_EQUAL( svm_matrix_train.size(), svm_matrix_train_cache.size() );
	BOOST_CHECK_EQUAL( preproc_trainput.size(), svm_matrix_train_cache.size() );

	//compare LA scores unscaled for equality
	for( size_t i = 0; i < preproc_trainput.size(); i++ )
	{
		BOOST_CHECK_EQUAL( preproc_trainput.at(i)->get_id(), svm_matrix_train_cache.at(i)->get_id() );
		BOOST_CHECK_EQUAL( svm_matrix_train.at(i)->get_id(), svm_matrix_train_cache.at(i)->get_id() );
		
		BOOST_CHECK_EQUAL( preproc_trainput.at(i)->known_susceptibility(), svm_matrix_train_cache.at(i)->known_susceptibility() );
		BOOST_CHECK_EQUAL( svm_matrix_train.at(i)->known_susceptibility(), svm_matrix_train_cache.at(i)->known_susceptibility() );

		std::vector< double >::const_iterator it_orig, it_cache;
		for( it_orig = svm_matrix_train.at(i)->begin(), it_cache = svm_matrix_train_cache.at(i)->begin(); 
			 it_orig != svm_matrix_train.at(i)->end(),  it_cache != svm_matrix_train_cache.at(i)->end();
			 it_orig++, it_cache++ )
		{
			BOOST_CHECK_EQUAL( (*it_orig), (*it_cache) );
		}
	}
}

BOOST_AUTO_TEST_CASE( BioProcessorTest_load_save_predictees_cache )
{
	PreProcessor p;
	PreProcWUSet preproc_fullset;
	Options o;
	
	p.parseInputFiles(
		"../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv",
		"../data/shared/PI_wild.seq",
		"NFV",
		1000,//means all should be susceptible
		o,
		preproc_fullset
		);

	//Split into train and predictee set
	PreProcWUSet train_input_set;
	train_input_set.push_back( preproc_fullset[0] );
	train_input_set.push_back( preproc_fullset[1] );
	train_input_set.push_back( preproc_fullset[2] );

	PreProcWUSet predictee_input_set;
	predictee_input_set.push_back( preproc_fullset[3] );
	predictee_input_set.push_back( preproc_fullset[4] );

	//clear out original set to avoid destructor problem.
	//train_set and predictee_set now 'own' the wu_set pointers.
	preproc_fullset.clear();

	BioProcessor b;
	SvmWUMatrix svm_matrix_predictees, svm_matrix_train;
	b.process_predictees( train_input_set, predictee_input_set, svm_matrix_predictees );
	//b.process_trainset( train_input_set, svm_matrix_train );

	//save cache
	//produced by predictee and trainset LA scores. so HASH needs both sets for unique id of predictees
	//even though we only store LA scores associated with the predictees in the output file
	b.save_cache( train_input_set, svm_matrix_predictees ) ;

	//read cache
	SvmWUMatrix svm_matrix_predictee_cache;
	bool found_cache = b.load_cache( train_input_set, predictee_input_set, svm_matrix_predictee_cache );

	BOOST_CHECK_EQUAL( true, found_cache );

	BOOST_CHECK_EQUAL( svm_matrix_predictees.size(), svm_matrix_predictee_cache.size() );
	BOOST_CHECK_EQUAL( predictee_input_set.size(), svm_matrix_predictee_cache.size() );

	//compare LA scores unscaled for equality
	for( size_t i = 0; i < predictee_input_set.size(); i++ )
	{
		BOOST_CHECK_EQUAL( predictee_input_set.at(i)->get_id(), svm_matrix_predictee_cache.at(i)->get_id() );
		BOOST_CHECK_EQUAL( svm_matrix_predictees.at(i)->get_id(), svm_matrix_predictee_cache.at(i)->get_id() );
		
		BOOST_CHECK_EQUAL( predictee_input_set.at(i)->known_susceptibility(), svm_matrix_predictee_cache.at(i)->known_susceptibility() );
		BOOST_CHECK_EQUAL( svm_matrix_predictees.at(i)->known_susceptibility(), svm_matrix_predictee_cache.at(i)->known_susceptibility() );

		std::vector< double >::const_iterator it_orig, it_cache;
		for( it_orig = svm_matrix_predictees.at(i)->begin(), it_cache = svm_matrix_predictee_cache.at(i)->begin(); 
			 it_orig != svm_matrix_predictees.at(i)->end(),  it_cache != svm_matrix_predictee_cache.at(i)->end();
			 it_orig++, it_cache++ )
		{
			BOOST_CHECK_EQUAL( (*it_orig), (*it_cache) );
		}
	}

}

BOOST_AUTO_TEST_CASE( BioProcessorTest_process_with_cache )
{

	SvmWUMatrix svm_matrix_train_no_cache, svm_matrix_predictees_no_cache;
	SvmWUMatrix svm_matrix_train_with_cache, svm_matrix_predictees_with_cache;

	//process without cache.
	{
		PreProcessor p;
		PreProcWUSet preproc_fullset;
		Options o;
		
		p.parseInputFiles(
			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv",
			"../data/shared/PI_wild.seq",
			"NFV",
			1000,//means all should be susceptible
			o,
			preproc_fullset
			);

		//Split into train and predictee set
		PreProcWUSet train_input_set;
		train_input_set.push_back( preproc_fullset[0] );
		train_input_set.push_back( preproc_fullset[1] );
		train_input_set.push_back( preproc_fullset[2] );

		PreProcWUSet predictee_input_set;
		predictee_input_set.push_back( preproc_fullset[3] );
		predictee_input_set.push_back( preproc_fullset[4] );

		//clear out original set to avoid destructor problem.
		//train_set and predictee_set now 'own' the wu_set pointers.
		preproc_fullset.clear();

		BioProcessor b;
		b.process( train_input_set, predictee_input_set, svm_matrix_train_no_cache, svm_matrix_predictees_no_cache );
	}

	//process with cache.
	{
		PreProcessor p;
		PreProcWUSet preproc_fullset;
		Options o;
		
		p.parseInputFiles(
			"../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv",
			"../data/shared/PI_wild.seq",
			"NFV",
			1000,//means all should be susceptible
			o,
			preproc_fullset
			);

		//Split into train and predictee set
		PreProcWUSet train_input_set;
		train_input_set.push_back( preproc_fullset[0] );
		train_input_set.push_back( preproc_fullset[1] );
		train_input_set.push_back( preproc_fullset[2] );

		PreProcWUSet predictee_input_set;
		predictee_input_set.push_back( preproc_fullset[3] );
		predictee_input_set.push_back( preproc_fullset[4] );

		//clear out original set to avoid destructor problem.
		//train_set and predictee_set now 'own' the wu_set pointers.
		preproc_fullset.clear();

		BioProcessor b;
		b.process_with_cache( train_input_set, predictee_input_set, svm_matrix_train_with_cache, svm_matrix_predictees_with_cache );
	}	
	//compare results. id's, susceptibility, and scaled LA scores.
	BOOST_CHECK_EQUAL( svm_matrix_train_no_cache.size(), svm_matrix_train_with_cache.size() );
	BOOST_CHECK_EQUAL( svm_matrix_predictees_no_cache.size(), svm_matrix_predictees_with_cache.size() );

	//compare LA scores unscaled for equality predictees
	for( size_t i = 0; i < svm_matrix_predictees_no_cache.size(); i++ )
	{
		BOOST_CHECK_EQUAL( svm_matrix_predictees_no_cache.at(i)->get_id(), svm_matrix_predictees_with_cache.at(i)->get_id() );
		BOOST_CHECK_EQUAL( svm_matrix_predictees_no_cache.at(i)->get_id(), svm_matrix_predictees_with_cache.at(i)->get_id() );
		
		BOOST_CHECK_EQUAL( svm_matrix_predictees_no_cache.at(i)->known_susceptibility(), svm_matrix_predictees_with_cache.at(i)->known_susceptibility() );
		BOOST_CHECK_EQUAL( svm_matrix_predictees_no_cache.at(i)->known_susceptibility(), svm_matrix_predictees_with_cache.at(i)->known_susceptibility() );

		std::vector< double >::const_iterator it_orig, it_cache;
		for( it_orig = svm_matrix_predictees_no_cache.at(i)->begin(), it_cache = svm_matrix_predictees_with_cache.at(i)->begin(); 
			 it_orig != svm_matrix_predictees_no_cache.at(i)->end(),  it_cache != svm_matrix_predictees_with_cache.at(i)->end();
			 it_orig++, it_cache++ )
		{
			BOOST_CHECK_EQUAL( (*it_orig), (*it_cache) );
		}
	}	

	//compare LA scores unscaled for equality trainset
	for( size_t i = 0; i < svm_matrix_train_no_cache.size(); i++ )
	{
		BOOST_CHECK_EQUAL( svm_matrix_train_no_cache.at(i)->get_id(), svm_matrix_train_with_cache.at(i)->get_id() );
		BOOST_CHECK_EQUAL( svm_matrix_train_no_cache.at(i)->get_id(), svm_matrix_train_with_cache.at(i)->get_id() );
		
		BOOST_CHECK_EQUAL( svm_matrix_train_no_cache.at(i)->known_susceptibility(), svm_matrix_train_with_cache.at(i)->known_susceptibility() );
		BOOST_CHECK_EQUAL( svm_matrix_train_no_cache.at(i)->known_susceptibility(), svm_matrix_train_with_cache.at(i)->known_susceptibility() );

		std::vector< double >::const_iterator it_orig, it_cache;
		for( it_orig = svm_matrix_train_no_cache.at(i)->begin(), it_cache = svm_matrix_train_with_cache.at(i)->begin(); 
			 it_orig != svm_matrix_train_no_cache.at(i)->end(),  it_cache != svm_matrix_train_with_cache.at(i)->end();
			 it_orig++, it_cache++ )
		{
			BOOST_CHECK_EQUAL( (*it_orig), (*it_cache) );
		}
	}
}

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  BIOPROCESSOR_TEST || defined TEST_ALL
#endif //BIOPROCESSORTEST_HPP
