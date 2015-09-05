cmake_minimum_required(VERSION 3.3)
project(hivm)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(BOOST_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_ROOT /home/edwardotis/bin/boost_1_59_0 )
#set(BOOST_INCLUDEDIR C:/dev/hivm_project/hivm/3rd_party/boost_1_59_0/)

set(Boost_INCLUDE_DIRS /home/edwardotis/bin/boost_1_59_0)
#set(Boost_LIBRARY_DIRS C:/dev/hivm_project/hivm/3rd_party/boost_1_59_0/stage/lib)
#MESSAGE("yO BOOST_LIBRARYDIR ${BOOST_LIBRARYDIR}")
set(BOOST_LIBRARYDIR /home/edwardotis/bin/boost_1_59_0/stage/lib)

FIND_PACKAGE( Boost 1.59.0 REQUIRED
        COMPONENTS
        filesystem
        program_options

)
if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION-: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

#MESSAGE("Boost information: ")
#MESSAGE("  Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#MESSAGE("  Boost_LIBRARIES: ${Boost_LIBRARIES}")
#MESSAGE("  Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")

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

add_executable(hivm ${SOURCE_FILES})