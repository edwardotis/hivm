#ifndef TYPES_HPP
#define TYPES_HPP

#include <string> 
#include <vector>

//TODO move class specific typedefs into each particular class
//They should not be universal
//spread sheet or matrix ADT
typedef std::vector< std::vector < std::string > > string_spread_sheet;
typedef std::vector < std::string > string_spread_sheet_row;
typedef std::vector< std::vector < std::string > >::iterator spread_sheet_row_iterator;
typedef std::vector< std::vector < std::string > >::const_iterator spread_sheet_row_const_iterator;
//typedef std::vector< std::vector < std::string > >::const_iterator spread_sheet_const_iterator;
typedef std::vector < std::string >::const_iterator spread_sheet_column_const_iterator;
typedef std::vector < std::string >::iterator spread_sheet_column_iterator;

typedef std::vector< double >::const_iterator double_iterator;
//PreProcWUSet ADT
//#include "PreProcWorkUnit.hpp"


//global typedefs
//typedef size_t size_t;

#endif
