#ifndef BOOSTTESTS_HPP
#define BOOSTTESTS_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  BOOST_TEST || defined TEST_ALL

#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration
#include <boost/numeric/ublas/matrix.hpp> //matrix class
#include <boost/cast.hpp>				 //safe numerical casts
#include <boost/lexical_cast.hpp>		//safe lexical casts
#include <boost/tokenizer.hpp>			//tokenizer
#include <boost/random/uniform_int.hpp> //rand num generator
#include <boost/random/mersenne_twister.hpp>//rand num generator
#include <boost/random/variate_generator.hpp>//rand num generator
#include <boost/filesystem/path.hpp> // Portable paths

// Sun CC doesn't handle boost::iterator_adaptor yet
#if !defined(__SUNPRO_CC) || (__SUNPRO_CC > 0x530)
#include <boost/generator_iterator.hpp>
#endif

#include <sstream> //stringstreams

#include "../PreProcessor.hpp"
#include "../FileIO.hpp"
#include "../FunctionObjects.cpp"

namespace nu = boost::numeric::ublas;

struct BoostTests_Fixture
{
	//ctor
	BoostTests_Fixture(){}

	//dtor
	~BoostTests_Fixture(){}

	PreProcessor pre_proc;
	FileIO		 file_io;
	std::string input;
	boost::numeric::ublas::matrix< std::string> spread_sheet;
};

BOOST_AUTO_TEST_SUITE( boost_tests );

BOOST_AUTO_TEST_CASE( boost_tests_random_num_generator )
{
	//http://www.boost.org/libs/random/random-generators.html
	
	//typedef boost::minstd_rand base_generator_type;

	boost::mt19937::result_type seed = 42;
	boost::mt19937 rng; 
	rng.seed( seed );
	boost::uniform_int<> three(0,2);
	boost::variate_generator< boost::mt19937&, boost::uniform_int<> > die( rng, three );
	
	 for( int i = 0; i < 1000; i++ )
	 {
		 int my_rand = die();  
		 BOOST_CHECK_PREDICATE( std::greater<int>(), (my_rand) (-1) );
		 BOOST_CHECK_PREDICATE( std::less<int>(), (my_rand) (3) );
	 }

	RandInt Rand( seed );//function object

	//TODO add check to make sure that we don't repeat the same random number each time.
	 for( int i = 0; i < 1000; i++ )
	 {
		 int my_rand = Rand( 2 );  
		 BOOST_CHECK_PREDICATE( std::greater<int>(), (my_rand) (-1) );
		 BOOST_CHECK_PREDICATE( std::less<int>(), (my_rand) (3) );
	 }

	 for( int i = 0; i < 1000; i++ )
	 {
		 int my_rand = Rand( 0, 2 );  
		 BOOST_CHECK_PREDICATE( std::greater<int>(), (my_rand) (-1) );
		 BOOST_CHECK_PREDICATE( std::less<int>(), (my_rand) (3) );
	 }

}

BOOST_AUTO_TEST_CASE( boost_tests_shared_ptrs )
{

	BOOST_MESSAGE("purpose: test boost shared_ptr of type string");

	//std::stringstream* ss;
	//ss	= new std::stringstream("hello", std::stringstream::app);

	//BOOST_CHECK_EQUAL( (*ss).str(), "hello" );
	//*ss << " world";

	boost::shared_ptr< std::string > sharedStr(new std::string("hello world"));
	BOOST_CHECK_EQUAL( sharedStr.get()->c_str(), "hello world" );

	//boost::shared_ptr< std::stringstream > sharedStream;
	//sharedStream = new std::stringstream( "hello beautiful", std::stringstream::app );
	std::string str("hello beautiful");
	boost::shared_ptr< std::stringstream > sharedStream(new std::stringstream(str, 
		std::stringstream::out ));
	//std::cerr << "yo! " << sharedStream.get()->str() << std::endl;
	BOOST_CHECK_EQUAL( sharedStream.get()->str(), "hello beautiful" );

	//std::cerr << sharedStream.get()->str() << std::endl;
	(*sharedStream) << "!";
	//std::cerr << sharedStream.get()->str() << std::endl;
	BOOST_CHECK_EQUAL( sharedStream.get()->str(), "!ello beautiful" );

	*sharedStream << "42";
	//std::cerr << (*sharedStream).str();
	BOOST_CHECK_EQUAL( sharedStream.get()->str(), "!42lo beautiful" );

	std::stringstream tr;
	tr << "hello world";
	tr << "!";

//	std::cerr << tr.str();
	BOOST_CHECK_EQUAL( tr.str(), "hello world!" );
}

BOOST_AUTO_TEST_CASE( boost_tests_matrices )
{
	BOOST_MESSAGE("purpose: read in matrix shared_ptr of type string");

	{
		nu::matrix<int> _m1(3,2);

		unsigned int row = 3;
		unsigned int col = 2;
		BOOST_CHECK_EQUAL(  _m1.size1(), 3 );
		BOOST_CHECK_EQUAL(  _m1.size2(),2 );
		BOOST_CHECK_EQUAL( boost::numeric_cast<unsigned int>( _m1.size1() ), 3 );
		BOOST_CHECK_EQUAL( boost::numeric_cast<unsigned int>( _m1.size2() ), 2 );

		_m1(0,0) = 45;
		BOOST_CHECK_EQUAL( _m1(0,0), 45 );
	}

	{
		nu::matrix<std::string> _m2(3,2);
		_m2(0,0) = "hello world";
		BOOST_CHECK_EQUAL( _m2(0,0), "hello world" );

		_m2(0,0) = "goodbye world";
		BOOST_CHECK_EQUAL( _m2(0,0), "goodbye world" );

		_m2(0,0) = _m2(0,0) + "!";
		BOOST_CHECK_EQUAL( _m2(0,0), "goodbye world!" );

		_m2(0,0) += "?";
		BOOST_CHECK_EQUAL( _m2(0,0), "goodbye world!?" );
	}

	{
		nu::matrix<std::stringstream> _m3(1,1);
		_m3(0,0) << "hello mundo";
		//_m3(0,0) = boost::lexical_cast<std::stringstream>("hello mundo");//doesn't compile
		BOOST_CHECK_EQUAL( _m3(0,0).str(), "hello mundo" );

		_m3(0,0) << "!";
		BOOST_CHECK_EQUAL( _m3(0,0).str(), "hello mundo!" );
	}
	

}



BOOST_AUTO_TEST_CASE( boost_tests_tokenizer )
{
	BoostTests_Fixture f;
	
	try
	{
		f.file_io.read( "../data/BoostTests/strings-small-matrix-tabdelim.csv", f.input );
	}
	catch( boost::filesystem::filesystem_error& e)
	{
		BOOST_FAIL( e.what() );
	}

	{
		//TODO use tokenizer to load up matrix
		//tokenizer has to get a full string, not stringstream.str()
		std::string lines = f.input;
		boost::tokenizer<> tok( lines );
		boost::tokenizer<>::iterator it = tok.begin();
		
		BOOST_CHECK_EQUAL( "now", *it );

		++it;
		BOOST_CHECK_EQUAL( "is", *it );

		++it;
		BOOST_CHECK_EQUAL( "the", *it );

		++it;
		BOOST_CHECK_EQUAL( "time", *it );

		//test reusing tokenizer and iterator
		tok = lines;
		it = tok.begin();
		BOOST_CHECK_EQUAL( "now", *it );
	}
}

//test my understanding of stringstream
BOOST_AUTO_TEST_CASE( boost_tests_Stringstream )
{
	BOOST_MESSAGE( "purpose: test my understanding of stringstream" );

	std::stringstream s;

	s.str( "hi" );
	BOOST_CHECK( s.str() == "hi");

	s.str( "there");
	BOOST_CHECK( s.str() != "hithere");
	BOOST_CHECK( s.str() == "there");

	s << "george";
	BOOST_CHECK_MESSAGE( s.str() != "theregeorge", s.str());

	s.str("");//clears out stringstream, by constructing a new one.

	s << "empty";

	BOOST_CHECK_EQUAL( s.str(), "empty" );

	s << "hi";
	s << "there";
	BOOST_CHECK_MESSAGE( s.str() != "hithere", "Stringstrea.clear() does not empty a stringstream" );

	std::stringstream t;//new stringstream

	t << "hi";
	t.clear();
	t << "there";
	BOOST_CHECK_MESSAGE( t.str() != "there", "Stringstrea.clear() does not empty a stringstream" );

	t << "";
	t << "yo";
	BOOST_CHECK_MESSAGE( t.str() != "yo", "Feeding it an empty string clears a stringstream");
}

BOOST_AUTO_TEST_CASE( boost_tests_filesystem_leaf )
{
	std::string output_prefix = "results/Crossval";
	boost::filesystem::path output_prefix_path( output_prefix );//for getting leaf of output prefix
	
	std::string leaf = output_prefix_path.leaf();
	BOOST_CHECK_EQUAL( "Crossval", output_prefix_path.leaf() );

}

BOOST_AUTO_TEST_SUITE_END();

#endif// #if defined  BOOST_TESTS || defined TEST_ALL

#endif //BOOSTTESTS_HPP
