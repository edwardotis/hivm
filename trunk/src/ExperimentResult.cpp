#include "ExperimentResult.hpp"
#include "Log.hpp"

ExperimentResult::ExperimentResult()
{}

ExperimentResult::ExperimentResult(	double _lg_cost, double _lg_gamma ) :
  lg_cost( _lg_cost ),
  lg_gamma( _lg_gamma )
{}

ExperimentResult::~ExperimentResult()
{
  //free memory
  std::vector< data_point* >::const_iterator it;
  for( it = this->begin(); it != this->end(); it++ )
    {
      data_point* dp = (*it);
      delete dp;
      dp = NULL;
    }
  
  //removes all elements from the vector
  this->clear();	
}

void ExperimentResult::add(int predicted_susceptibility, int actual_susceptibility,
			   const std::string id, const std::vector<double>& probabilities_)
{
  struct data_point* dp;
  try
    {
      dp = new data_point(predicted_susceptibility, actual_susceptibility,
			  id, probabilities_);
    }
  catch( std::exception& e )
    {
      std::string msg = "\nException caught in ExperimentResult::add() new data_point: \n";
      msg +=   e.what();
      msg += "\n\nElapsed Time: ";
      msg += Log::elapsed_time();
      msg += "\nExiting...";
      Log::append( msg );
      std::cerr << msg;
      exit(1);
    }
  
  push_back( dp );
}

//copy constructor
ExperimentResult::ExperimentResult( const ExperimentResult& rhs )
{
  lg_cost = rhs.lg_cost;
  lg_gamma = rhs.lg_gamma;

  for( int i=0; i < rhs.size(); i++)
    {
      struct data_point* dp;
      try
	{
	  dp = new data_point(rhs.at(i)->predicted_susceptibility,
			      rhs.at(i)->actual_susceptibility,
			      rhs.at(i)->id,
			      rhs.at(i)->probabilities);
	}
      catch( std::exception& e )
	{
	  std::string msg = "\nException caught in ExperimentResult::ExperimentResult() new data_point: \n";
	  msg +=   e.what();
	  msg += "\n\nElapsed Time: ";
	  msg += Log::elapsed_time();
	  msg += "\nExiting...";
	  Log::append( msg );
	  std::cerr << msg;
	  exit(1);
	}
      
      push_back( dp );
    }
}

//--------ExperimentResultSet-----------

//copy constructor
ExperimentResultSet::ExperimentResultSet( const ExperimentResultSet& rhs ) 
{
  for( int i=0; i < rhs.size(); i++)
    {
      ExperimentResult *er;
      try
	{
	  er = new ExperimentResult( *rhs.at(i) );
	}
      catch( std::exception& e )
	{
	  std::string msg = "\nException caught in ExperimentResultSet::ExperimentResultSet( const ExperimentResultSet& rhs ) new ExperimentResult: \n";
	  msg +=   e.what();
	  msg += "\n\nElapsed Time: ";
	  msg += Log::elapsed_time();
	  msg += "\nExiting...";
	  Log::append( msg );
	  std::cerr << msg;
	  exit(1);
	}
      this->push_back( er );		
    }
}

ExperimentResultSet::ExperimentResultSet()
{}

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

void ExperimentResultSet::add( ExperimentResult* result )
{
  this->push_back( result );
}
