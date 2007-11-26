//  (C) Copyright Gennadiy Rozental 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile: progress_monitor.hpp,v $
//
//  Version     : $Revision: 1.1 $
//
//  Description : defines simple text based progress monitor
// ***************************************************************************

#ifndef BOOST_TEST_PROGRESS_MONITOR_HPP_020105GER
#define BOOST_TEST_PROGRESS_MONITOR_HPP_020105GER

// Boost.Test
#include <boost/test/test_observer.hpp>
#include <boost/test/utils/trivial_singleton.hpp>

// STL
#include <iosfwd>   // for std::ostream&

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************                progress_monitor              ************** //
// ************************************************************************** //

class progress_monitor_t : public test_observer, public singleton<progress_monitor_t> {
public:
    // test observer interface
    void    test_start( counter_t test_cases_amount );
    void    test_finish() {}
    void    test_aborted();

    void    test_unit_start( test_unit const& ) {}
    void    test_unit_finish( test_unit const&, unsigned long );
    void    test_unit_skipped( test_unit const& );
    void    test_unit_aborted( test_unit const& ) {}

    void    assertion_result( bool passed ) {}
    void    exception_caught( execution_exception const& ) {}

    // configuration
    void    set_stream( std::ostream& );

private:
    BOOST_TEST_SINGLETON_CONS( progress_monitor_t );
}; // progress_monitor_t

BOOST_TEST_SINGLETON_INST( progress_monitor )

} // namespace unit_test

} // namespace boost

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log: progress_monitor.hpp,v $
//  Revision 1.1  2006/08/07 05:24:30  ed
//  load boost files for making libraries for each architecture
//
//  Revision 1.2  2005/06/24 04:09:34  rogeeff
//  added missing iosfwd
//
//  Revision 1.1  2005/02/20 08:27:06  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
// ***************************************************************************

#endif // BOOST_TEST_PROGRESS_MONITOR_HPP_020105GER
