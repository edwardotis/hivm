#ifndef FUNCTIONOBJECTS_HPP
#define FUNCTIONOBJECTS_HPP

//@purpose functor class to provide a random number in range
//How good is this? Don't know.
// return an integral random number in the range 0 - (n - 1)

#include <stdlib.h>

#include <boost/random/uniform_int.hpp> //rand num generator
#include <boost/random/mersenne_twister.hpp>//rand num generator
#include <boost/random/variate_generator.hpp>//rand num generator

// Sun CC doesn't handle boost::iterator_adaptor yet
#if !defined(__SUNPRO_CC) || (__SUNPRO_CC > 0x530)
#include <boost/generator_iterator.hpp>
#endif

class RandInt
{
public:

	//default ctor
	RandInt()
   {
		//class_seed = 52;
	    rng.seed( 52 );
   }

   //
   RandInt( boost::mt19937::result_type  seed )
   {
	   //unsigned int seed2 = 42;
	   // boost::mt19937::result_type seed3 = 42;
		//class_seed = _seed;
		rng.seed( seed );
   }

	/**
	Ranges are inclusive. So, range_low 0, range_high 2 gives you 0 or 1 or 2 as a result.
	*/
   unsigned int operator() ( unsigned int range_low, unsigned int range_high )
   {
		//boost::mt19937 rng; //mersenne_twister
		//rng.seed( seed );	//set seed
		boost::uniform_int<> range( range_low, range_high );//set range
		boost::variate_generator< boost::mt19937&, boost::uniform_int<> > die( rng, range );//make die
		
		unsigned int my_rand = die();  
		return my_rand;
   }

   /**
	This operator is compatible with std::shuffle, which only accepts one param
	pre: set_seed must be called before using this operator
   */
   unsigned int operator() ( unsigned int range_high )
   {
	    unsigned int range_low = 0;
		boost::uniform_int<> range( range_low, range_high );//set range
		boost::variate_generator< boost::mt19937&, boost::uniform_int<> > die( rng, range );//make die
		
		unsigned int my_rand = die();  
		return my_rand;
   }

	//void set_seed( unsigned long _seed )
	//{
	//	class_seed = _seed;
	//}

private:

	unsigned long class_seed;
	boost::mt19937 rng; //mersenne_twister


};

#include "ExperimentAnalyzer.hpp"//fpr()
#include "ExperimentResult.hpp"

class LessThanFpr
{
public:
   LessThanFpr() {}

   bool operator() ( const ExperimentResult* r1, const ExperimentResult* r2 )
   {
	   ExperimentAnalyzer ea;
		return ( ea.fpr( r1 ) < ea.fpr( r2 ) );
   }
};

#endif //#ifndef FUNCTIONOBJECTS_HPP
