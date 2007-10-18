#ifndef LIBSVMADAPTERTEST_HPP
#define LIBSVMADAPTERTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  LIBSVMADAPTER_TEST || defined TEST_ALL

#include "../LibSvmAdapter.hpp"
#include "../PreProcessor.hpp"
#include "../PreProcWorkUnit.hpp"
#include "../BioProcessor.hpp"
#include "../Log.hpp"
#include "../Options.hpp"
#include "../ExperimentResult.hpp"//for prediction results

#include <boost/lexical_cast.hpp> //safe numerical to string casts

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration
#include <boost/test/floating_point_comparison.hpp>//for comparing floating point values

class LibSvmAdapterTest
{

public:
	//default ctor
	LibSvmAdapterTest()
	{}

	//ctor
	//@param hivdb susceptibility file path
	LibSvmAdapterTest( std::string input_data_path )
	{
		p.parseInputFiles(
			input_data_path,
			"../data/shared/PI_wild.seq",
			"NFV",//NFV is standard for unit tests here.
			2,
			preproc_options,
			wu_set
			);	

		Log::append( "LibSvmAdapterTest wu_set.size = " +  boost::lexical_cast<std::string>( wu_set.size() )  );


		if( wu_set.size() > 3 )
		{
			//add 2 wu's to it from the end of the inputset
			predictee_set.push_back( wu_set.back() );//SD-7 if small3.tsv
			wu_set.pop_back();
			predictee_set.push_back( wu_set.back() );//CA3876 if small3.tsv
			wu_set.pop_back();

			//bio_proc.process( wu_set, predictee_set, svm_train_matrix, svm_predictee_matrix );
			bio_proc.process_with_cache( wu_set, predictee_set, svm_train_matrix, svm_predictee_matrix );
		}
		else
		{
			BOOST_FAIL( "Less than 4 sequences loaded intoLibSvmAdapterTest. Test won't work." );
		}	
	}

	//dtor
	~LibSvmAdapterTest()
	{}

	void create_svm_problem_categories()
	{
		LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
		
		LibSvmAdapter l;
		double* testee;
		testee = l.create_svm_problem_categories( f.svm_train_matrix );

		//test for 1 and -1 in correct places
		//NFV threshold 2
		BOOST_CHECK_EQUAL( -1, *testee );//2.2
		BOOST_CHECK_EQUAL( 1, *(testee+1) );//.8
		BOOST_CHECK_EQUAL( 1, *(testee+2) );//1.2

		free( testee );
	}


	void create_svm_node_ptr()
	{
		LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	
		struct svm_node* node;

		LibSvmAdapter l;
		node = l.create_svm_node_ptr( f.svm_train_matrix.front() );

		SvmWUSet* s = f.svm_train_matrix.front();

		//test values. there are 3
		BOOST_CHECK_EQUAL( node->index,   0  );
		BOOST_CHECK_EQUAL( node->value,   s->front()  );// 2.2

		BOOST_CHECK_EQUAL( (node+1)->index,   1  );
		BOOST_CHECK_EQUAL( (node+1)->value, s->at(1) );//  .8

		BOOST_CHECK_EQUAL( (node+2)->index,   2  );
		BOOST_CHECK_EQUAL( (node+2)->value, s->at(2) );// 1.2

		//test special libsvm termination symbol for node ptrs
		BOOST_CHECK_EQUAL( (node+3)->index,   -1  );
		BOOST_CHECK_EQUAL( (node+3)->value,   -1 );// 

		free( node );
	}
	void create_svm_node_ptr_to_ptr()
	{
		LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
		
		struct svm_node** node_matrix;

		LibSvmAdapter l;
		node_matrix = l.create_svm_node_ptr_to_ptr( f.svm_train_matrix );
		
		//test first, second, last rows
		//test 0,0 0,1 0,2  indices and values
		struct svm_node* row_zero = *node_matrix;
		BOOST_CHECK_EQUAL( row_zero->value, f.svm_train_matrix.at(0)->at(0) );
		BOOST_CHECK_EQUAL( (row_zero+1)->value, f.svm_train_matrix.at(0)->at(1) );
		BOOST_CHECK_EQUAL( (row_zero+2)->value, f.svm_train_matrix.at(0)->at(2) );
		BOOST_CHECK_EQUAL( (row_zero+3)->value, -1 );//check termination symbol
		free( row_zero );

		struct svm_node* row_one = *(node_matrix+1);
		BOOST_CHECK_EQUAL( row_one->value, f.svm_train_matrix.at(1)->at(0) );
		BOOST_CHECK_EQUAL( (row_one+1)->value, f.svm_train_matrix.at(1)->at(1) );
		BOOST_CHECK_EQUAL( (row_one+2)->value, f.svm_train_matrix.at(1)->at(2) );
		BOOST_CHECK_EQUAL( (row_one+3)->value, -1 );//check termination symbol
		free( row_one );

		struct svm_node* row_two = *(node_matrix+2);
		BOOST_CHECK_EQUAL( row_two->value, f.svm_train_matrix.at(2)->at(0) );
		BOOST_CHECK_EQUAL( (row_two+1)->value, f.svm_train_matrix.at(2)->at(1) );
		BOOST_CHECK_EQUAL( (row_two+2)->value, f.svm_train_matrix.at(2)->at(2) );
		BOOST_CHECK_EQUAL( (row_two+3)->value, -1 );//check termination symbol
		free( row_two );

		//free( *node_matrix );
		//free( *(node_matrix+1) );
		//free( *(node_matrix+2) );
		free( node_matrix );
	}
	void create_svm_problem()
	{
		LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
		
		struct svm_problem* problem;

		LibSvmAdapter l;
		problem = l.create_svm_problem( f.svm_train_matrix );
			
		BOOST_CHECK_EQUAL( problem->l, f.svm_train_matrix.size() );

		//check on x
		struct svm_node** node_matrix = problem->x;
		struct svm_node* row_zero = *node_matrix;
		BOOST_CHECK_EQUAL( row_zero->value, f.svm_train_matrix.at(0)->at(0) );
		BOOST_CHECK_EQUAL( (row_zero+1)->value, f.svm_train_matrix.at(0)->at(1) );
		BOOST_CHECK_EQUAL( (row_zero+2)->value, f.svm_train_matrix.at(0)->at(2) );
		
		//check end points
		BOOST_CHECK_EQUAL( (row_zero+3)->value, -1 );
		//free( row_zero );

		//check on y
		double* node = problem->y;
		BOOST_CHECK_EQUAL( -1, *node  );

		//check for memory leaks
		l.delete_svm_problem( problem );
		
		//struct svm_problem* null_pointer = NULL;
		//BOOST_CHECK_EQUAL( null_pointer, problem );
	}

	void create_svm_parameter()
	{
		LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );

		LibSvmAdapter l;
		struct svm_problem* problem = l.create_svm_problem( f.svm_train_matrix );
		
		Options o;
		o.lg_cost = 0;// 2^0 == 1
		o.lg_gamma = 0;

		struct svm_parameter* param = l.create_svm_parameter( problem, o );

		BOOST_CHECK_EQUAL( param->C, 1  );
		BOOST_CHECK_EQUAL( param->gamma, 1 );//
		//BOOST_CHECK_CLOSE( param->gamma, 0.33333333, 0.0001 );//
		
		l.delete_svm_parameter( param );

		o.lg_cost = 3;
		o.lg_gamma = -3;
		struct svm_parameter* param2 = l.create_svm_parameter( problem, o  );

		BOOST_CHECK_EQUAL( param2->C, 8  );
		BOOST_CHECK_EQUAL( param2->gamma,  0.125 ); 

		l.delete_svm_parameter( param2 );

		o.lg_cost = 32;
		o.lg_gamma = -31;
		struct svm_parameter* param3 = l.create_svm_parameter( problem, o  );

		BOOST_CHECK_EQUAL( param3->C, 4294967296ULL  );
		BOOST_CHECK_EQUAL( param3->gamma,  0.0000000004656612873077392578125 );

		l.delete_svm_parameter( param3 );

		o.lg_cost = 33;
		o.lg_gamma = -32;
		struct svm_parameter* param4 = l.create_svm_parameter( problem, o );

		BOOST_CHECK_EQUAL( param4->C, 8589934592ULL  );
		BOOST_CHECK_EQUAL( param4->gamma,  0.00000000023283064365386962890625 );

		l.delete_svm_parameter( param4 );

		o.lg_cost = 0;
		o.lg_gamma = -1;
		struct svm_parameter* param5 = l.create_svm_parameter( problem, o  );

		BOOST_CHECK_EQUAL( param5->C, 1  );
		BOOST_CHECK_EQUAL( param5->gamma,  .5 );

		l.delete_svm_parameter( param5 );

		l.delete_svm_problem( problem );
	}
	void train_model_pr_small3()
	{
			//test. um, what's the model look like?
		//similar to this. so let's create in python libsvm by hand
		//and compare?

		//also search memory leaks

		//create a controlled svmwumatrix from small3
		LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );

		Options o;
		LibSvmAdapter l;
		struct svm_problem* problem = l.create_svm_problem( f.svm_train_matrix );//NEW SVM_PROBLEM
		struct svm_parameter* param = l.create_svm_parameter( problem, o );
		struct svm_model* train_model;//svm.h

		train_model = l.train( f.svm_train_matrix, param, problem );

		//BEGIN fix category label problem
		//test for 1 and -1 in correct places

		double* testee = problem->y;

		//NFV threshold 2
		BOOST_CHECK_EQUAL( -1, *testee );//2.2
		BOOST_CHECK_EQUAL( 1, *(testee+1) );//.8
		BOOST_CHECK_EQUAL( 1, *(testee+2) );//1.2

		//END

		//save and eyeball it. easiest first step.
		svm_save_model( "../data/LibSvmAdapterTest/PR_small3_model.txt", train_model );//svm.h

		//check model size somehow
		//by counting number of nodes in train_model->SV
	/**
	//TODO IF YOU WANT TO TEST DIRECTLY ON train_model, you have to move the definition
	of struct train_model into svm.h It's currently in svm.cpp for some stupid
	reason, so it won't link when you try to access the members vars of it
	*/
		
		//	BOOST_CHECK_EQUAL( f.svm_train_matrix.size(), train_model->l );
	//	BOOST_CHECK_EQUAL( f.svm_train_matrix.at(0)->at(0), train_model->SV[0]->value );

		svm_destroy_model( train_model );//DELETE SVM_MODEL
		l.delete_svm_parameter( param );//DELETE SVM_PARAMETER
		l.delete_svm_problem( problem );//DELETESVM_PROBLEM
	}
	void train_model_pr_full()
	{

		LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0.tsv" );

		Options o;
		LibSvmAdapter l;
		struct svm_problem* problem = l.create_svm_problem( f.svm_train_matrix );//NEW SVM_PROBLEM
		struct svm_parameter* param = l.create_svm_parameter( problem, o);
		struct svm_model* train_model;//svm.h

		train_model = l.train( f.svm_train_matrix, param, problem );

		Log::append( "LibSvmAdapterTest_train_model_pr_full" );
		Log::append( "train_model = l.train( f.svm_train_matrix, param, problem );" );
		Log::append( "f.svm_train_matrix.size = " + boost::lexical_cast<std::string>(f.svm_train_matrix.size() ) );
		Log::append( "problem->l" +  boost::lexical_cast<std::string>(problem->l)  );
		

	//	save and eyeball it. easiest first step.
		svm_save_model( "../data/LibSvmAdapterTest/PR_model-NFV.txt", train_model );//svm.h

		svm_destroy_model( train_model );//DELETE SVM_MODEL
		l.delete_svm_parameter( param );//DELETE SVM_PARAMETER
		l.delete_svm_problem( problem );//DELETESVM_PROBLEM
	}

	void test_number_of_classes()
	{
		//Evaluates to 1 class
		{
				//vars
			PreProcessor p;
			//PreProcWUSet wu_set;
			//PreProcWUSet predictee_set;
			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			BioProcessor bio_proc;
			Options options;
			options.probability = 1;//turn on probability
			int seed = 42;

			options.save_cmd_line_options( "testversion_pre-parseinput" );

			//OUTPUT
			SvmWUMatrix svm_train_matrix;  
			SvmWUMatrix svm_predictee_matrix;

			p.parseInputFiles(
				"../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv",
				"../data/shared/PI_wild.seq",
				"NFV",//NFV is standard for unit tests here.
				2,
				options,
				seed,
				input_train_set,
				input_test_set
				);	

			bio_proc.process_with_cache( input_train_set, input_test_set, svm_train_matrix, svm_predictee_matrix );
		
			LibSvmAdapter l;
			int classes = l.get_number_of_classes( svm_train_matrix, options );
			BOOST_CHECK_EQUAL( classes, 1 );
		}

		//Evaluates to 2 classes
		{
				//vars
			PreProcessor p;
			//PreProcWUSet wu_set;
			//PreProcWUSet predictee_set;
			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			BioProcessor bio_proc;
			Options options;
			options.probability = 1;//turn on probability
			int seed = 42;

			options.save_cmd_line_options( "testversion_pre-parseinput" );

			//OUTPUT
			SvmWUMatrix svm_train_matrix;  
			SvmWUMatrix svm_predictee_matrix;

			p.parseInputFiles(
				"../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv",
				"../data/shared/PI_wild.seq",
				"NFV",//NFV is standard for unit tests here.
				10,
				options,
				seed,
				input_train_set,
				input_test_set
				);	

			bio_proc.process_with_cache( input_train_set, input_test_set, svm_train_matrix, svm_predictee_matrix );
			
			LibSvmAdapter l;
			int classes = l.get_number_of_classes( svm_train_matrix, options );
			BOOST_CHECK_EQUAL( classes, 2 );
		}

	}


	void predict_collection_1class_predictions_short()
	{
		//without probability
		//Evaluates to 1 class
		{
				//vars
			PreProcessor p;
			//PreProcWUSet wu_set;
			//PreProcWUSet predictee_set;
			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			BioProcessor bio_proc;
			Options options;
			options.probability = 0;//turn off probability
			int seed = 42;

			//options.save_cmd_line_options( "testversion_pre-parseinput" );

			//OUTPUT
			SvmWUMatrix svm_train_matrix;  
			SvmWUMatrix svm_predictee_matrix;

			p.parseInputFiles(
				"../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv",
				"../data/shared/PI_wild.seq",
				"NFV",//NFV is standard for unit tests here.
				0,
				options,
				seed,
				input_train_set,
				input_test_set
				);	

			bio_proc.process_with_cache( input_train_set, input_test_set, svm_train_matrix, svm_predictee_matrix );
		
			LibSvmAdapter l;
			int classes = l.get_number_of_classes( svm_train_matrix, options );
			BOOST_CHECK_EQUAL( classes, 1 );

			ExperimentResult* result = new ExperimentResult();	
			l.predict_susceptibility( svm_train_matrix, svm_predictee_matrix, options, result );

			BOOST_CHECK_EQUAL( result->size(), svm_predictee_matrix.size() );

			for( int i = 0; i < result->size(); i++ )
			{
				bool known = svm_train_matrix.front()->known_susceptibility();
				bool predicted = result->at(i)->predicted_susceptibility;
				BOOST_CHECK_EQUAL( known, predicted );
			}

			delete result;

		}

		//with probabilty
		//Evaluates to 1 class
		{
				//vars
			PreProcessor p;
			//PreProcWUSet wu_set;
			//PreProcWUSet predictee_set;
			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			BioProcessor bio_proc;
			Options options;
			options.probability = 1;//turn off probability
			int seed = 42;

			//options.save_cmd_line_options( "testversion_pre-parseinput" );

			//OUTPUT
			SvmWUMatrix svm_train_matrix;  
			SvmWUMatrix svm_predictee_matrix;

			p.parseInputFiles(
				"../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv",
				"../data/shared/PI_wild.seq",
				"NFV",//NFV is standard for unit tests here.
				0,
				options,
				seed,
				input_train_set,
				input_test_set
				);	

			bio_proc.process_with_cache( input_train_set, input_test_set, svm_train_matrix, svm_predictee_matrix );
		
			LibSvmAdapter l;
			int classes = l.get_number_of_classes( svm_train_matrix, options );
			BOOST_CHECK_EQUAL( classes, 1 );

			ExperimentResult* result = new ExperimentResult();	
			l.predict_susceptibility( svm_train_matrix, svm_predictee_matrix, options, result );

			BOOST_CHECK_EQUAL( result->size(), svm_predictee_matrix.size() );

			for( int i = 0; i < result->size(); i++ )
			{
				bool known = svm_train_matrix.front()->known_susceptibility();
				bool predicted = result->at(i)->predicted_susceptibility;
				BOOST_CHECK_EQUAL( known, predicted );
			}

			delete result;
		}
	}


	void predict_collection_1class_predictions_long()
	{
		//without probability
		//Evaluates to 1 class
		{
				//vars
			PreProcessor p;
			//PreProcWUSet wu_set;
			//PreProcWUSet predictee_set;
			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			BioProcessor bio_proc;
			Options options;
			options.probability = 0;//turn off probability
			int seed = 42;

			//options.save_cmd_line_options( "testversion_pre-parseinput" );

			//OUTPUT
			SvmWUMatrix svm_train_matrix;  
			SvmWUMatrix svm_predictee_matrix;

			p.parseInputFiles(
				"../data/shared/PR_2006-05-25_v2.0.tsv",
				"../data/shared/PI_wild.seq",
				"NFV",//NFV is standard for unit tests here.
				0,
				options,
				seed,
				input_train_set,
				input_test_set
				);	

			bio_proc.process_with_cache( input_train_set, input_test_set, svm_train_matrix, svm_predictee_matrix );
		
			LibSvmAdapter l;
			int classes = l.get_number_of_classes( svm_train_matrix, options );
			BOOST_CHECK_EQUAL( classes, 1 );

			ExperimentResult* result = new ExperimentResult();	
			l.predict_susceptibility( svm_train_matrix, svm_predictee_matrix, options, result );

			BOOST_CHECK_EQUAL( result->size(), svm_predictee_matrix.size() );

			for( int i = 0; i < result->size(); i++ )
			{
				bool known = svm_train_matrix.front()->known_susceptibility();
				bool predicted = result->at(i)->predicted_susceptibility;
				BOOST_CHECK_EQUAL( known, predicted );
			}

			delete result;

		}

		//without probabilty
		//Evaluates to 1 class
		{
				//vars
			PreProcessor p;
			//PreProcWUSet wu_set;
			//PreProcWUSet predictee_set;
			PreProcWUSet input_train_set;
			PreProcWUSet input_test_set;
			BioProcessor bio_proc;
			Options options;
			options.probability = 1;//turn off probability
			int seed = 42;

			//options.save_cmd_line_options( "testversion_pre-parseinput" );

			//OUTPUT
			SvmWUMatrix svm_train_matrix;  
			SvmWUMatrix svm_predictee_matrix;

			p.parseInputFiles(
				"../data/shared/PR_2006-05-25_v2.0.tsv",
				"../data/shared/PI_wild.seq",
				"NFV",//NFV is standard for unit tests here.
				0,
				options,
				seed,
				input_train_set,
				input_test_set
				);	

			bio_proc.process_with_cache( input_train_set, input_test_set, svm_train_matrix, svm_predictee_matrix );
		
			LibSvmAdapter l;
			int classes = l.get_number_of_classes( svm_train_matrix, options );
			BOOST_CHECK_EQUAL( classes, 1 );

			ExperimentResult* result = new ExperimentResult();	
			l.predict_susceptibility( svm_train_matrix, svm_predictee_matrix, options, result );

			BOOST_CHECK_EQUAL( result->size(), svm_predictee_matrix.size() );

			for( int i = 0; i < result->size(); i++ )
			{
				bool known = svm_train_matrix.front()->known_susceptibility();
				bool predicted = result->at(i)->predicted_susceptibility;
				BOOST_CHECK_EQUAL( known, predicted );
			}

			delete result;

		}

	}



	void predict_with_probability_collection_order_train_test_parseinputfiles()
	{

		//LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small2.tsv" );
		//LibSvmAdapterTest f( "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv" );

		//vars
		PreProcessor p;
		//PreProcWUSet wu_set;
		//PreProcWUSet predictee_set;
		PreProcWUSet input_train_set;
		PreProcWUSet input_test_set;
		BioProcessor bio_proc;
		Options options;
		options.probability = 1;//turn on probability
		int seed = 42;

		options.save_cmd_line_options( "testversion_pre-parseinput" );

		//OUTPUT
		SvmWUMatrix svm_train_matrix;  
		SvmWUMatrix svm_predictee_matrix;

		p.parseInputFiles(
			"../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv",
			"../data/shared/PI_wild.seq",
			"NFV",//NFV is standard for unit tests here.
			2,
			options,
			seed,
			input_train_set,
			input_test_set
			);	

		options.save_cmd_line_options( "testversion_post-parseinput" );

		int trainsize = input_train_set.size();
		int testsize = input_test_set.size();

		//bio_proc.process( wu_set, predictee_set, svm_train_matrix, svm_predictee_matrix );
		bio_proc.process_with_cache( input_train_set, input_test_set, svm_train_matrix, svm_predictee_matrix );

		//Are all workunits in svm_train_matrix unique?
		for( int b=0; b < svm_train_matrix.size(); b++ )
		{
			int duplicates = 0;
			for( int c=0; c < svm_train_matrix.size(); c++ )
			{
				if( svm_train_matrix.at(b)->get_id() == svm_train_matrix.at(c)->get_id() &&
					b != c )//svm_train_matrix(3) should equal svm_train_matrix(3).
				{
					duplicates++;
				}
			}

			//make sure each entry occurs exactly one time
			BOOST_CHECK_EQUAL( 0, duplicates );
		}


		//are all workunits in svm_predictee_matrix unique? or are there duplicates? 
		for( int b=0; b < svm_predictee_matrix.size(); b++ )
		{
			int duplicates = 0;
			for( int c=0; c < svm_predictee_matrix.size(); c++ )
			{
				if( svm_predictee_matrix.at(b)->get_id() == svm_predictee_matrix.at(c)->get_id() &&
					b != c )//svm_predictee_matrix(3) should equal svm_predictee_matrix(3).
				{
					duplicates++;
				}
			}

			//make sure each entry occurs exactly one time, i.e. zero duplicates
			BOOST_CHECK_EQUAL( 0, duplicates );
		}

		//just want to view the data here
		//std::string id;
		//bool suscep;
		//std::cout << "\nid, suscep trainset TEST Veresion known\n";
		//for( int d=0; d < svm_train_matrix.size(); d++ )
		//{
		//	id = svm_train_matrix.at(d)->get_id();
		//	suscep = svm_train_matrix.at(d)->known_susceptibility();
		//	std::cout << id <<  " " << suscep  << '\n';
		//}

		//std::cout << "\nid, suscep predictee set known\n";
		//for( int d=0; d < svm_predictee_matrix.size(); d++ )
		//{
		//	id = svm_predictee_matrix.at(d)->get_id();
		//	suscep = svm_predictee_matrix.at(d)->known_susceptibility();
		//	std::cout << id <<  " " << suscep  << '\n';
		//}



		Log::append( "LibSvmAdapterTest svm_predictee_matrix.size = " +  boost::lexical_cast<std::string>( svm_predictee_matrix.size() )  );

		int svmtrainsize = svm_train_matrix.size();
		int svmtestsize = svm_predictee_matrix.size();

		LibSvmAdapter l;
		int predsize = svm_predictee_matrix.size();

		ExperimentResult* result = new ExperimentResult();	
		l.predict_susceptibility( svm_train_matrix, svm_predictee_matrix, options, result );
		std::vector<double> probabilities;

		//just want to view the data here
		//std::cout << "\nid, suscep predictee predicted\n";
		//for( int e=0; e < result->size(); e++ )
		//{
		//	id = result->at(e)->id;
		//	suscep = result->at(e)->predicted_susceptibility;
		//	std::cout << id <<  " " << suscep  << '\n';
		//}

		/*
			ok. in libsvm for 2 classes with my svm_model.
			class 1 is susceptible
			class 0 is resistant

			a. we could be getting the equal to here.
		*/

		int classes = l.get_number_of_classes( svm_train_matrix, options );
		BOOST_CHECK_EQUAL( classes, 1 );

		//test for result class having greater or equal probability
		for( int i = 0; i < result->size(); i++ )
		{
			probabilities = result->at(i)->probabilities;

			//test for 2 classes since I'm currently only allowing binary predictions. anything else is wrong
			BOOST_CHECK_EQUAL( probabilities.size(), 2 );

			////check for special case of 1 class
			if( l.get_number_of_classes( svm_train_matrix, options ) < 2 )
			{
				if( result->at(i)->predicted_susceptibility == 1 )//susceptible
				{
					//100% probability that it's susceptible
					BOOST_CHECK_EQUAL( probabilities.at(1), 1 ); 
					BOOST_CHECK_EQUAL( probabilities.at(0), 0 ); 
				}
				else//resistant
				{
					//100% probability that it's resistant
					BOOST_CHECK_EQUAL( probabilities.at(1), 0 ); 
					BOOST_CHECK_EQUAL( probabilities.at(0), 1 ); 
				}
			}

			//I SET probabilities as -1 in Libsvmadapter. probability should be between 0 and 100
			// so a -1 means that the values were never set properly by libsvm.
			for( int m=0; m< probabilities.size(); m++ )
			{
				BOOST_CHECK_PREDICATE( std::not_equal_to<double>(), (-1.0) ( probabilities.at( m ) ) );
			}

			double sucep = probabilities.at( 1 );
			double resist = probabilities.at( 0 );
			int size = result->size();

			BOOST_CHECK_CLOSE( 1.0, ( sucep + resist ), 0.0001 );
			if( result->at(i)->predicted_susceptibility == true )//suceptible
			{
				BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( 1 ) ) (probabilities.at( 0 ) ) );
			}

			if( result->at(i)->predicted_susceptibility  == false )//resistant
			{
				BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( 0 ) ) ( probabilities.at( 1 ) ) );
			}
		}

		delete result;
	}


	void predict_susceptibility_vs_libsvm_predict()
	{
		//setup a call directly to svm_predict
		//begin setup
		LibSvmAdapterTest f2( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
		LibSvmAdapter l2;
		Options o2;	
		o2.lg_cost = 4;
		o2.lg_gamma = 5;
		
		struct svm_problem* problem = l2.create_svm_problem( f2.svm_train_matrix );//NEW SVM_PROBLEM
		struct svm_parameter* param = l2.create_svm_parameter( problem, o2 );//NEW SVM_PARAMETER
		struct svm_model* train_model = l2.train( f2.svm_train_matrix, param, problem );//NEW SVM_MODEL

		double svm_result;

		struct svm_node* predictee_ = l2.create_svm_node_ptr( f2.svm_predictee_matrix[0] );//CALLOC SVM_NODE*
		svm_result = svm_predict( train_model, predictee_ );
		free( predictee_ );//FREE SVM_NODE*

		svm_destroy_model( train_model );//DELETE SVM_MODEL
		l2.delete_svm_parameter( param );//DELETE SVM_PARAMETER
		l2.delete_svm_problem( problem );//DELETESVM_PROBLEM
		//end setup

		//setup use of libsvmadapter version
		//begin setup 2
		LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
		LibSvmAdapter l;
		Options o;
		o.lg_cost = 4;
		o.lg_gamma = 5;
		//end setup 2

		//I chose labels for libsvm.
		// 1 susceptible
		// -1 not susceptible.
		// I did not use 0,1,2,3, etc.

	//	std::vector<bool> results;
		//ExperimentResultSet result_set;
		ExperimentResult* result = new ExperimentResult();	
		l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix, o, result );

		BOOST_CHECK_EQUAL( 4, result->lg_cost );
		BOOST_CHECK_EQUAL( 5, result->lg_gamma);

		//TODO make cleaner interface to data points in ExperimentResults
		if( svm_result == -1 )
		{
			bool is_libsvm_susceptible = false;

			//check that these two are opposite of each other.
			BOOST_CHECK_EQUAL( is_libsvm_susceptible, result->front()->predicted_susceptibility );
		}
		else if( svm_result == 1 )
		{
			bool is_libsvm_susceptible = true;

			//check that these two are opposite of each other.
			BOOST_CHECK_EQUAL( is_libsvm_susceptible, result->front()->predicted_susceptibility  );
		}
		else
		{
			BOOST_FAIL("svm_predict should have returned -1 or 1");
		}

		delete result;
	}
	void svm_destroy_model_with_probability()
	{
	//	run without probability
		{
			//create a controlled svmwumatrix from small3
			LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );

			Options o;
			o.probability = 0;
			LibSvmAdapter l;
			struct svm_problem* problem = l.create_svm_problem( f.svm_train_matrix );//NEW SVM_PROBLEM
			struct svm_parameter* param = l.create_svm_parameter( problem, o );
			struct svm_model* train_model;//svm.h

			train_model = l.train( f.svm_train_matrix, param, problem );

			//BEGIN fix category label problem
			//test for 1 and -1 in correct places

			double* testee = problem->y;
			
			//save and eyeball it. easiest first step.
			svm_save_model( "../data/LibSvmAdapterTest/PR_small3_model.txt", train_model );//svm.h
		
			svm_destroy_model( train_model );//DELETE SVM_MODEL
			l.delete_svm_parameter( param );//DELETE SVM_PARAMETER
			l.delete_svm_problem( problem );//DELETESVM_PROBLEM
		}
		
	//	run with probability
		{
			int size = sizeof( double );//8 bytes, 
			int size2 = sizeof( double* );//4 bytes, 

			//create a controlled svmwumatrix from small3
			LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );

			Options o;
			o.probability = 1;
			LibSvmAdapter l;
			struct svm_problem* problem = l.create_svm_problem( f.svm_train_matrix );//NEW SVM_PROBLEM
			struct svm_parameter* param = l.create_svm_parameter( problem, o );
			struct svm_model* train_model;//svm.h

			//struct svm_parameter* param2 = l.create_svm_parameter( problem, o );

			train_model = l.train( f.svm_train_matrix, param, problem );
			
			

			/*int i = *train_model->nr_class;
			std::cout <<  "train_model->nr_class: " << boost::lexical_cast<std::string>( i );
			*///BEGIN fix category label problem
			//test for 1 and -1 in correct places

			//double* testee = problem->y;
			
			//save and eyeball it. easiest first step.
			svm_save_model( "../data/LibSvmAdapterTest/PR_small3_model_prob.txt", train_model );//svm.h
		
			svm_destroy_model( train_model );//DELETE SVM_MODEL
			l.delete_svm_parameter( param );//DELETE SVM_PARAMETER
			l.delete_svm_problem( problem );//DELETESVM_PROBLEM

		}
	}


	void predict_with_probability_collection_order_train_test_parseinputfiles_2class() 
	{
		//LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small2.tsv" );
		//LibSvmAdapterTest f( "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv" );

		//vars
		PreProcessor p;
		//PreProcWUSet wu_set;
		//PreProcWUSet predictee_set;
		PreProcWUSet input_train_set;
		PreProcWUSet input_test_set;
		BioProcessor bio_proc;
		Options options;
		options.lg_cost = 7.5;//optimal for t10
		options.lg_gamma = -5.5;//optimal for t10
		options.probability = 1;//turn on probability
		int seed = 42;

		options.save_cmd_line_options( "testversion_pre-parseinput" );

		//OUTPUT
		SvmWUMatrix svm_train_matrix;  
		SvmWUMatrix svm_predictee_matrix;

		p.parseInputFiles(
			"../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv",
			"../data/shared/PI_wild.seq",
			"NFV",//NFV is standard for unit tests here.
			10,
			options,
			seed,
			input_train_set,
			input_test_set
			);	

		//std::cerr << "2nd. Train Set:" << "\n\n";
		//for( int k=0; k < input_train_set.size(); k++ )
		//{
		//	std::cerr << input_train_set.at(k)->get_id() << ", ";
		//	std::cerr << input_train_set.at(k)->known_susceptibility() << ", ";
		//	std::cerr << input_train_set.at(k)->get_data() << "\n";
		//}	

		//	std::cerr << "2nd. Test Set:" << "\n\n";
		//for( int k=0; k < input_test_set.size(); k++ )
		//{
		//	std::cerr << input_test_set.at(k)->get_id() << ", ";
		//	std::cerr << input_test_set.at(k)->known_susceptibility() << ", ";
		//	std::cerr << input_test_set.at(k)->get_data() << "\n";
		//}	

		options.save_cmd_line_options( "testversion_post-parseinput" );

		int trainsize = input_train_set.size();
		int testsize = input_test_set.size();

		//bio_proc.process( wu_set, predictee_set, svm_train_matrix, svm_predictee_matrix );
		bio_proc.process_with_cache( input_train_set, input_test_set, svm_train_matrix, svm_predictee_matrix );

		//Are all workunits in svm_train_matrix unique?
		for( int b=0; b < svm_train_matrix.size(); b++ )
		{
			int duplicates = 0;
			for( int c=0; c < svm_train_matrix.size(); c++ )
			{
				if( svm_train_matrix.at(b)->get_id() == svm_train_matrix.at(c)->get_id() &&
					b != c )//svm_train_matrix(3) should equal svm_train_matrix(3).
				{
					duplicates++;
				}
			}

			//make sure each entry occurs exactly one time
			BOOST_CHECK_EQUAL( 0, duplicates );
		}


		//are all workunits in svm_predictee_matrix unique? or are there duplicates? 
		for( int b=0; b < svm_predictee_matrix.size(); b++ )
		{
			int duplicates = 0;
			for( int c=0; c < svm_predictee_matrix.size(); c++ )
			{
				if( svm_predictee_matrix.at(b)->get_id() == svm_predictee_matrix.at(c)->get_id() &&
					b != c )//svm_predictee_matrix(3) should equal svm_predictee_matrix(3).
				{
					duplicates++;
				}
			}

			//make sure each entry occurs exactly one time, i.e. zero duplicates
			BOOST_CHECK_EQUAL( 0, duplicates );
		}

		//just want to view the data here
		//std::string id;
		//bool suscep;
		//std::cout << "\nid, suscep trainset TEST Veresion known\n";
		//for( int d=0; d < svm_train_matrix.size(); d++ )
		//{
		//	id = svm_train_matrix.at(d)->get_id();
		//	suscep = svm_train_matrix.at(d)->known_susceptibility();
		//	std::cout << id <<  " " << suscep  << '\n';
		//}

		//std::cout << "\nid, suscep predictee set known\n";
		//for( int d=0; d < svm_predictee_matrix.size(); d++ )
		//{
		//	id = svm_predictee_matrix.at(d)->get_id();
		//	suscep = svm_predictee_matrix.at(d)->known_susceptibility();
		//	std::cout << id <<  " " << suscep  << '\n';
		//}

		Log::append( "LibSvmAdapterTest svm_predictee_matrix.size = " +  boost::lexical_cast<std::string>( svm_predictee_matrix.size() )  );

		int svmtrainsize = svm_train_matrix.size();
		int svmtestsize = svm_predictee_matrix.size();

		LibSvmAdapter l;
		int predsize = svm_predictee_matrix.size();

		ExperimentResult* result = new ExperimentResult();	
		l.predict_susceptibility( svm_train_matrix, svm_predictee_matrix, options, result );
		std::vector<double> probabilities;

		//just want to view the data here
		//std::cout << "\nid, suscep predictee predicted\n";
		//for( int e=0; e < result->size(); e++ )
		//{
		//	id = result->at(e)->id;
		//	suscep = result->at(e)->predicted_susceptibility;
		//	std::cout << id <<  " " << suscep  << '\n';
		//}

		/*
			ok. in libsvm for 2 classes with my svm_model.
			class 1 is susceptible
			class 0 is resistant

			a. we could be getting the equal to here.
		*/

		int classes = l.get_number_of_classes( svm_train_matrix, options );
		BOOST_CHECK_EQUAL( classes, 2 );

		//test for result class having greater or equal probability
		for( int i = 0; i < result->size(); i++ )
		{
			probabilities = result->at(i)->probabilities;

			//test for 2 classes since I'm currently only allowing binary predictions. anything else is wrong
			BOOST_CHECK_EQUAL( probabilities.size(), 2 );


			//I SET probabilities as -1 in Libsvmadapter. probability should be between 0 and 100
			// so a -1 means that the values were never set properly by libsvm.
			for( int m=0; m< probabilities.size(); m++ )
			{
				BOOST_CHECK_PREDICATE( std::not_equal_to<double>(), (-1.0) ( probabilities.at( m ) ) );
			}

			double sucep = probabilities.at( 1 );
			double resist = probabilities.at( 0 );
			int size = result->size();

			BOOST_CHECK_CLOSE( 1.0, ( sucep + resist ), 0.0001 );

			if( result->at(i)->predicted_susceptibility == true )//suceptible
			{
				BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( 1 ) ) (probabilities.at( 0 ) ) );
			}

			if( result->at(i)->predicted_susceptibility  == false )//resistant
			{
				BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( 0 ) ) ( probabilities.at( 1 ) ) );
			}
		}

		delete result;
	}


public:
	PreProcessor p;
	PreProcWUSet wu_set;
	PreProcWUSet predictee_set;
	BioProcessor bio_proc;
	Options preproc_options;

	//OUTPUT
	SvmWUMatrix svm_train_matrix;  //size 3x3 if small3.tsv
	SvmWUMatrix svm_predictee_matrix;//size 2x3 if small3.tsv
};

//test private member data with these unit tests
BOOST_AUTO_TEST_SUITE( LibSvmAdapterTests_private );

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_create_svm_problem_categories )
{
	LibSvmAdapterTest t;
	t.create_svm_problem_categories();
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_create_svm_node_ptr )
{
	LibSvmAdapterTest t;
	t.create_svm_node_ptr();
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_create_svm_node_ptr_to_ptr )
{
	LibSvmAdapterTest t;
	t.create_svm_node_ptr_to_ptr();
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_create_svm_problem )
{
	LibSvmAdapterTest t;
	t.create_svm_problem();
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_create_svm_parameter )
{
	LibSvmAdapterTest t;
	t.create_svm_parameter();
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_train_model_pr_small3 )
{
	LibSvmAdapterTest t;
	t.train_model_pr_small3();	
}

#if defined  LONG_TESTS || defined TEST_ALL
BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_train_model_pr_full )
{
	LibSvmAdapterTest t;
	t.train_model_pr_full();	
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_pr_full)
{
		//well, this is the theory part, right? does this svm thing work?
		//we can run predict here with a known sequence that is included
		//in the training set.
		//However, a true or false won't mean jack.
		//but it's good to check for memory leaks, etc.

		LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0.tsv" );
		LibSvmAdapter l;
		Options o;

		//TODO make something more useful out of this?
		//std::vector<bool> results;
		//ExperimentResultSet result_set;
		ExperimentResult* result = new ExperimentResult();			
		l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix, o, result );
		delete result;
		//BOOST_CHECK_EQUAL( results.at(0), l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[0], o ) );
		//BOOST_CHECK_EQUAL( results.at(1), l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix[1], o ) );
}

#endif

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_pr_small3 )
{
	//well, this is the theory part, right? does this svm thing work?
	//we can run predict here with a known sequence that is included
	//in the training set.
	//However, a true or false won't mean jack.
	//but it's good to check for memory leaks, etc.

	LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	LibSvmAdapter l;
	Options o;

	//TODO make something more useful out of this?
	//std::vector<bool> results;
	//ExperimentResultSet result_set;
	ExperimentResult* result = new ExperimentResult();	
	l.predict_susceptibility( f.svm_train_matrix, f.svm_predictee_matrix, o, result );
	delete result;
}




//Test trainset details of single class problem are logged, and alert goes to std::err. //todo EYEBALL test
//test function to get number of classes in a training model.
BOOST_AUTO_TEST_CASE(LibSvmAdapterTest_test_number_of_classes)
{
	LibSvmAdapterTest t;
	t.test_number_of_classes();
}
//
////If one class is used, all predictions will be for the same known class
BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_collection_1class_predictions_short )
{
	LibSvmAdapterTest t;
	t.predict_collection_1class_predictions_short();
}

#if defined  LONG_TESTS || defined TEST_ALL

//If one class is used, all predictions will be for the same known class
BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_collection_1class_predictions_long )
{
	LibSvmAdapterTest t;
	t.predict_collection_1class_predictions_long();
}

#endif //#if defined  LONG_TESTS || defined TEST_ALL

//TEST BEHAVIOR WHEN all training data falls into a single class.
BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_with_probability_collection_order_train_test_parseinputfiles )
{
	LibSvmAdapterTest t;
	t.predict_with_probability_collection_order_train_test_parseinputfiles();
}


//////same as above test, but with guaranteed 2 classes.
BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_with_probability_collection_order_train_test_parseinputfiles_2class )
{
	LibSvmAdapterTest t;
	t.predict_with_probability_collection_order_train_test_parseinputfiles_2class();
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_vs_libsvm_predict )
{
	LibSvmAdapterTest t;
	t.predict_susceptibility_vs_libsvm_predict();
}

BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_with_probability_single_pr_small3_probabilty_collection_order )
{

	{
		double prob_estimates[ 2 ];
		prob_estimates[0] = 3;
		prob_estimates[1] = 4;
	}

	//LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small2.tsv" );
	//LibSvmAdapterTest f( "../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV25.tsv" );

	//vars
	PreProcessor p;
	PreProcWUSet wu_set;
	PreProcWUSet predictee_set;
	BioProcessor bio_proc;
	Options options;
    options.lg_cost = 7.5;//optimal for t10
    options.lg_gamma = -5.5;//optimal for t10
	int seed = 42;

	options.probability = 1;//turn on probability

	options.save_cmd_line_options( "productionversion_pre-parseinput" );

	//OUTPUT
	SvmWUMatrix svm_train_matrix;  //size 3x3 if small3.tsv
	SvmWUMatrix svm_predictee_matrix;//size 2x3 if small3.tsv

	//UNCOMMENT ME
	p.parseInputFiles(
	"../data/ExperimentAnalyzerTest/PR_2006-05-25_v2.0_NFV6.tsv",
	"../data/shared/PI_wild.seq",
	"NFV",//NFV is standard for unit tests here.
	10,
	options,
	wu_set
	);	

		

	options.save_cmd_line_options( "productionversion_post-parseinput" );

	int trainsize = wu_set.size();
	int testsize = predictee_set.size();//0

	//ok, find the two seq's from below and put them into 
	//the prediction.

	//std::cerr << "Before cleaning up:" << "\n\n";
	//for( int k=0; k < wu_set.size(); k++ )
	//{
	//	std::cerr << wu_set.at(k)->get_id() << ", ";
	//	std::cerr << wu_set.at(k)->known_susceptibility() << ", ";
	//	std::cerr << wu_set.at(k)->get_data() << "\n";
	//}	

	//This makes the 2 unit tests (previous and this one) have identical train and test sets.
	//PreProcWorkUnit* wu1 = new PreProcWorkUnit( *(wu_set.at(0)) );
	//PreProcWorkUnit* wu2 = new PreProcWorkUnit( *(wu_set.at(1)) );
	//predictee_set.push_back( wu1 );
	//predictee_set.push_back( wu2 );

	//std::vector<PreProcWorkUnit*>::iterator it = wu_set.begin();
	//it = wu_set.erase( it );//erase 1st spot
	//wu_set.erase( it );//erase 2nd spot

	////remove usused slots in vector
	//std::vector<PreProcWorkUnit*>(wu_set).swap(wu_set);

	//for( int k=0; k < wu_set.size(); k++ )
	//{
	//	it++;

	//	if( wu_set.at(k)->get_id() == "CA2634" ||
	//		wu_set.at(k)->get_id() == "CA3872"
	//		)
	//	{	
	//		//create new preprocwu pointer using copy constructor, and save to predictees
	//		PreProcWorkUnit* wu = new PreProcWorkUnit( *(wu_set.at(k)) );
	//		predictee_set.push_back( wu );

	//		//clean up training vector
	//		wu_set.erase( it );
	//		int temp = wu_set.size();
	//		int x;

	//	}
		//add 8 wu's to it from the end of the inputset
		//predictee_set.push_back( wu_set.back() );
		//wu_set.pop_back();
	//}

	//this makes the first label different in this unit test
	//from the previous unit test.
	for( int k=0; k < 2; k++ )
	{
		//add x wu's to it from the end of the inputset
		predictee_set.push_back( wu_set.back() );
		wu_set.pop_back();
	}


	//std::cerr << "After cleaning up: Train" << "\n\n";
	//for( int k=0; k < wu_set.size(); k++ )
	//{
	//	std::cerr << wu_set.at(k)->get_id() << ", ";
	//	std::cerr << wu_set.at(k)->known_susceptibility() << ", ";
	//	std::cerr << wu_set.at(k)->get_data() << "\n";
	//}

	//std::cerr << "After cleaning up: Test" << "\n\n";
	//for( int k=0; k < predictee_set.size(); k++ )
	//{
	//	std::cerr << predictee_set.at(k)->get_id() << ", ";
	//	std::cerr << predictee_set.at(k)->known_susceptibility() << ", ";
	//	std::cerr << predictee_set.at(k)->get_data() << "\n";
	//}

	//for( int k=0; k < 2; k++ )
	//{
	//	//add 8 wu's to it from the end of the inputset
	//	predictee_set.push_back( wu_set.back() );
	//	wu_set.pop_back();
	//}

	trainsize = wu_set.size();
	testsize = predictee_set.size();//8

	//bio_proc.process( wu_set, predictee_set, svm_train_matrix, svm_predictee_matrix );
	bio_proc.process_with_cache( wu_set, predictee_set, svm_train_matrix, svm_predictee_matrix );

	int svmtrainsize = svm_train_matrix.size();
	int svmtestsize = svm_predictee_matrix.size();

	//svm_train_matrix.at(1)->_known_susceptibility = 0;

	//just want to view the data here
	//std::string id;
	//bool suscep;
	//std::cout << "id, suscep trainset production version\n";
	//for( int d=0; d < svm_train_matrix.size(); d++ )
	//{
	//	id = svm_train_matrix.at(d)->get_id();
	//	suscep = svm_train_matrix.at(d)->known_susceptibility();
	//	std::cout << id <<  " " << suscep  << '\n';
	//}

	//std::cout << "id, suscep predictee set\n";
	//for( int d=0; d < svm_predictee_matrix.size(); d++ )
	//{
	//	id = svm_predictee_matrix.at(d)->get_id();
	//	suscep = svm_predictee_matrix.at(d)->known_susceptibility();
	//	std::cout << id <<  " " << suscep  << '\n';
	//}

	Log::append( "LibSvmAdapterTest svm_predictee_matrix.size = " +  boost::lexical_cast<std::string>( svm_predictee_matrix.size() )  );

	LibSvmAdapter l;

	int predsize = svm_predictee_matrix.size();

	ExperimentResult* result = new ExperimentResult();	
	l.predict_susceptibility( svm_train_matrix, svm_predictee_matrix, options, result );
	

	/*
		ok. in libsvm for 2 classes with my svm_model.
		class 1 is susceptible
		class 0 is resistant

		a. we could be getting the equal to here.
		make this a loop and test dozens in this this test.
	*/

	//test for result class having greater or equal probability
	for( int i = 0; i < result->size(); i++ )
	{
		std::vector<double> probabilities = result->at(i)->probabilities;

		//test for 2 classes since I'm currently only allowing binary predictions. anything else is wrong
		BOOST_CHECK_EQUAL( probabilities.size(), 2 );

		//I SET probabilities as -1 in Libsvmadapter. probability should be between 0 and 100
		// so a -1 means that the values were never set properly by libsvm.
		for( int m=0; m< probabilities.size(); m++ )
		{
			BOOST_CHECK_PREDICATE( std::not_equal_to<double>(), (-1.0) ( probabilities.at( m ) ) );
		}

		double sucep = probabilities.at( 1 );
		double resist = probabilities.at( 0 );
		int size = result->size();

		if( result->at(i)->predicted_susceptibility == true )//suceptible
		{
			BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( 1 ) ) (probabilities.at( 0 ) ) );
		}

		if( result->at(i)->predicted_susceptibility  == false )//resistant
		{
			BOOST_CHECK_PREDICATE( std::greater_equal<double>(), ( probabilities.at( 0 ) ) ( probabilities.at( 1 ) ) );
		}
	}

	delete result;
}


BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_predict_susceptibility_probabilities_vs_predict_susceptibility )
{

	//begin setup predict_susceptibility_with_probabilities 
	//Options o;
	LibSvmAdapterTest f( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	LibSvmAdapter l;
	f.preproc_options.probability = 0;
	std::vector<double> probabilities_output;
	//end setup 1

	//begin setup predict_susceptibility (no probability)
	LibSvmAdapterTest f2( "../data/PreProcessorTest/PR_2006-05-25_v2.0_small3.tsv" );
	LibSvmAdapter l2;
	Options o2;
	o2.probability = 0;//don't use probability
	//end setup 1

//l.predict_susceptibility_with_probabilities( f.svm_train_matrix, f.svm_predictee_matrix[0], f.preproc_options, probabilities_output );
	
//BOOST_CHECK_EQUAL( l.predict_susceptibility_with_probabilities( f.svm_train_matrix, f.svm_predictee_matrix[0], f.preproc_options, probabilities_output ),
	//	l2.predict_susceptibility( f2.svm_train_matrix, f2.svm_predictee_matrix[0], o2 ) );
}



//testing for memory leaks
BOOST_AUTO_TEST_CASE( LibSvmAdapterTest_svm_destroy_model_with_probability )
{
	LibSvmAdapterTest t;
	t.svm_destroy_model_with_probability();
}

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  LIBSVMADAPTER_TEST || defined TEST_ALL
#endif //LIBSVMADAPTERTEST_HPP
