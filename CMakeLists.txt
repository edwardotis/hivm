#http://www.cmake.org/cmake/help/v3.0/module/FindBoost.html#
#cmake for use with CLion
cmake_minimum_required(VERSION 3.3)
project(hivm)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(BOOST_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_ROOT /home/edwardotis/bin/boost_1_59_0 )
set(Boost_DETAILED_FAILURE_MSG ON)
set(Boost_DEBUG ON)

set(BOOST_INCLUDEDIR /home/edwardotis/bin/boost_1_59_0)
set(BOOST_LIBRARYDIR /home/edwardotis/bin/boost_1_59_0/stage/lib)

find_package( Boost 1.59.0 REQUIRED
        COMPONENTS
        program_options
        system
        filesystem
)
if(Boost_FOUND)

    message("yo, boost was found.")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION-: ${Boost_VERSION}")
    message("CMAKE_INSTALL_PREFIX - ${CMAKE_INSTALL_PREFIX}")

    set(SOURCE_FILES
        src/BioProcessor.cpp
        src/BioProcessor.hpp
        src/CrossValidationExperiment.cpp
        src/CrossValidationExperiment.hpp
        src/dpl.cpp
        src/dpl.h
        src/ExperimentAnalyzer.cpp
        src/ExperimentAnalyzer.hpp
        src/ExperimentResult.cpp
        src/ExperimentResult.hpp
        src/FileIO.cpp
        src/FileIO.hpp
        src/FunctionObjects.cpp
        src/Hash.cpp
        src/Hash.hpp
        src/LibSvmAdapter.cpp
        src/LibSvmAdapter.hpp
        src/LibSvmFacade.h
        src/Log.cpp
        src/Log.hpp
        src/main.cpp
        src/Options.cpp
        src/Options.hpp
        src/PreProcessor.cpp
        src/PreProcessor.hpp
        src/PreProcWorkUnit.cpp
        src/PreProcWorkUnit.hpp
        src/svm.cpp
        src/svm.h
        src/SvmMachine.cpp
        src/SvmMachine.hpp
        src/SvmWorkUnit.cpp
        src/SvmWorkUnit.hpp
        src/Types.hpp
        src/ValidationExperiment.cpp
        src/ValidationExperiment.hpp)

    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(hivm ${SOURCE_FILES})
    target_link_libraries (hivm ${Boost_LIBRARIES})

    install (TARGETS hivm DESTINATION bin)

endif()
