/**
@purpose - Convert input data from aminio acids to numeric values based upon 
application of different pairwise biological algorithms, such as Local Alignment.

*/

#ifndef BIOPROCESSOR_HPP
#define BIOPROCESSOR_HPP

#include "Types.hpp" //shared typedefs
#include "PreProcWorkUnit.hpp"//input data container
#include "SvmWorkUnit.hpp"//output data container
#include "dpl.h" //local alignment function

class BioProcessor{

friend class BioProcessorTest;//unit tests

public: 

	//default constructor
	BioProcessor();	

	//destructor
	virtual ~BioProcessor();

	//@purpose Convert input data from aminio acids to numeric values
	//while checking for cached values.
	//If no cache found, it creates cache on filesystem.
	//@note caches are not scaled
	//@note uses Local Alignment similar to FASTA ssearch
	void process_with_cache( 
		const PreProcWUSet& train_input, 
		const PreProcWUSet& predictee_input, 
		SvmWUMatrix& train_output,
		SvmWUMatrix& predictee_output
		);


	private:

	//@purpose Convert input data from aminio acids to numeric values
	//@note uses Local Alignment similar to FASTA ssearch
	void process(
			const PreProcWUSet& train_input, 
			const PreProcWUSet& predictee_input, 
			SvmWUMatrix& train_output,
			SvmWUMatrix& predictee_output
			);

	//@purpose save trainset LA values to cache on filesystem.
	void save_cache( const SvmWUMatrix& train_output );//just trainset LA on itself


	/**
		@purpose save predictees LA values to cache on filesystem.
		//produced by predictee and trainset LA scores. so HASH needs both sets for unique id of predictees
		//even though we only store LA scores associated with the predictees in the output file
	*/
	void save_cache( const PreProcWUSet& train_input, const SvmWUMatrix& predictee_output );

	//@purpose load trainset LA values from cache on filesystem, if available and hash id's match
	//@returns true if cache hit, else false 
	bool load_cache( const PreProcWUSet& train_input, SvmWUMatrix& train_output );

	//@purpose load trainset LA values from cache on filesystem, if available and hash id's match
	//@returns true if cache hit, else false 
	bool load_cache( const PreProcWUSet& train_input, const PreProcWUSet& predictee_input, SvmWUMatrix& predictees_output );

	//@purpose processing happens here privately.
	void process_(
			const PreProcWUSet& train_input, 
			const PreProcWUSet& predictee_input, 
			SvmWUMatrix& train_output,
			SvmWUMatrix& predictee_output
			);

	void process_predictees(		
		const PreProcWUSet& train_input, 
		const PreProcWUSet& predictee_input, 
		SvmWUMatrix& predictee_output 
		);

	void process_trainset( const PreProcWUSet& train_input, SvmWUMatrix& train_output);

	
	/**

	////@purpose scales data according to LibSvm recommendations
	//@purpose scales EACH ATTRIBUTE in data set to [-1,1]
	//x'' =  [ ( 2* (x-mini) )/(Maxi-mini) ] - 1

	//@pre 
	//@post each pairwise value is between  [-1,1] inclusive	
	//*/
	void scale_data( SvmWUMatrix& train_data, SvmWUMatrix& predictee_data );

	//@purpose scale a single set of data
	void scale_data( SvmWUMatrix& data_set, double min, double max );

	///**
	//@purpose function that actually scales data set [0-1]
	//x' = (x-min)/(Max-min)
	//*/
	double scale( const double input, const double min, const double max );



	///**
	//@purpose finds max value of Matrix
	//*/
	double find_max( const SvmWUMatrix& ) const;

	/**
	@purpose finds min value of Matrix
	*/
	double find_min( const SvmWUMatrix& ) const;

	//copy constructor
	BioProcessor( const BioProcessor& );

	//assignment operator
	BioProcessor& operator= ( const BioProcessor& );

};

#endif // BIOPROCESSOR_HPP
