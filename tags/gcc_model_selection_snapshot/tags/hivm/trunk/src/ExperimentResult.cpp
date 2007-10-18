#include "ExperimentResult.hpp"

ExperimentResult::ExperimentResult()
{}

ExperimentResult::ExperimentResult(	double _log_cost, double _log_gamma ) :
	log_cost( _log_cost ),
	log_gamma( _log_gamma )
{
	//datum = new data_point_set();
}

ExperimentResult::~ExperimentResult()
{
//	delete datum;
}

//copy constructor
ExperimentResult::ExperimentResult( const ExperimentResult& rhs )
{
	log_cost = rhs.log_cost;
	log_gamma = rhs.log_gamma;

	for( size_t i=0; i < rhs.datum.data_points.size(); i++)
	{
		struct ExperimentResult::data_point* dp = new ExperimentResult::data_point(
			rhs.datum.data_points.at(i)->predicted_susceptibility,
			rhs.datum.data_points.at(i)->actual_susceptibility,
			rhs.datum.data_points.at(i)->id
			);
		
		datum.data_points.push_back( dp );
	}
}


//--------ExperimentResultSet-----------

ExperimentResultSet::ExperimentResultSet() :
	experiment_type( "Experiment_Type"),
	drug( "drug" ),
	fold_threshold( -1 ),
	wild_seq_path( "wild_seq_path" ),
	test_data_path( "test_data_path" ),
	num_training_seqs(  -2 )
{}

ExperimentResultSet::ExperimentResultSet(
		std::string _experiment_type,
		std::string _drug,
		double _fold_threshold,
		std::string _wild_seq_path,
		std::string _test_data_path,
		size_t _num_training_seqs
		) :
			experiment_type( _experiment_type),
			drug( _drug ),
			fold_threshold( _fold_threshold ),
			wild_seq_path( _wild_seq_path ),
			test_data_path( _test_data_path ),
			num_training_seqs(  _num_training_seqs )
{
}

//copy constructor
ExperimentResultSet::ExperimentResultSet( const ExperimentResultSet& rhs ) :
		experiment_type( rhs.experiment_type),
		drug( rhs.drug ),
		fold_threshold( rhs.fold_threshold ),
		wild_seq_path( rhs.wild_seq_path ),
		test_data_path( rhs.test_data_path ),
		num_training_seqs(  rhs.num_training_seqs ),
		seed( rhs.seed )
{
	for( size_t i=0; i < rhs.size(); i++)
	{
		ExperimentResult *er = new ExperimentResult( *rhs.at(i) );
		this->push_back( er );		
	}


}




ExperimentResultSet::~ExperimentResultSet()
{
	//free memory
	std::vector< ExperimentResult *>::const_iterator it;
	for( it = this->begin(); it != this->end(); it++ )
	{
		ExperimentResult* er = (*it);
		delete er;
		er = NULL;
	}

	//removes all elements from the vector
	this->clear();
}

void ExperimentResultSet::set_seed( size_t _seed )
{
	seed = _seed;
}

size_t ExperimentResultSet::get_seed()
{
	return seed;
}


void ExperimentResultSet::set_params(
	std::string _experiment_type,
	std::string _drug,
	double _fold_threshold,
	std::string _wild_seq_path,
	std::string _test_data_path,
	size_t _num_training_seqs
	)
{
	experiment_type = _experiment_type;
	drug = _drug;
	fold_threshold = _fold_threshold;
	wild_seq_path = _wild_seq_path;
	test_data_path = _test_data_path;
	num_training_seqs =  _num_training_seqs;
}

