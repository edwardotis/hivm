
HIVM

Usage:

You can type in: hivm --help or hivm -h to see the usage at the command prompt.
Sample scripts are available in samples/windows and samples/linux directories.

Presently, hivm has these two functions:

model-selection ( cross-validate and grid search for cost and gamma
parameters )

prediction (run classification on the test data using c,g parameters chosen
from cross-validation)

Allowed options:
  -h [ --help ]                         produce help message
  -p [ --purpose ] arg                  model-selection or prediction
  -d [ --drug ] arg                     HIV drug to be tested
  -t [ --thresholds ] arg               Thresholds for high  and low drug fold
                                        resistance . Please use only 1 or 2
                                        thresholds. Default: 2 and 10
  -w [ --wild-type ] arg                Wild Type Enzyme Sequence File
  -f [ --hivdb-file ] arg               HIVDB Susceptibility Data File
  -s [ --seed ] arg (=42)               Optional: Seed for randomly splitting
                                        susceptibility file into training and
                                        testing sets. Positive integers only.
                                        Default: 42
  -e [ --suscep-type ] arg (=all)       Optional: Type of susceptibility
                                        experiment used: clinical, lab, or all.
  -c [ --log-cost-c ] arg (=0)          Log Cost.  Required: test operation.
                                        Ignored for CrossValidation and
                                        SelfValidation operations
  -g [ --log-gamma-g ] arg (=0)         Log Gamma. Required: test operation.
                                        Ignored for CrossValidation and
                                        SelfValidation operations
  -x [ --log-cost-low ] arg (=0)        Low Log Cost. Required: cross-validate
                                        . For Grid Search of
                                        Parameters
  -y [ --log-cost-high ] arg (=0)       High Log Cost. Required: cross-validate
                                        . For Grid Search of
                                        Parameters
  -z [ --log-cost-increment ] arg (=0)  Log Cost Increment. Required:
                                        cross-validate . For
                                        Grid Search of Parameters
  -l [ --log-gamma-low ] arg (=0)       Low Log Gamma. Required: cross-validate
                                        . For Grid Search of
                                        Parameters
  -m [ --log-gamma-high ] arg (=0)      High Log Gamma. Required:
                                        cross-validate . For
                                        Grid Search of Parameters
  -n [ --log-gamma-increment ] arg (=0) Log Gamma Increment. Required:
                                        cross-validate . For
                                        Grid Search of Parameters
  -o [ --output ] arg                   Optional: Prefix for output files.
                                        Default: current timestamp



Analysis:

A spreadsheet comparing cross-validation results to test results of a prechosen cost, gamma 
parameter
pair is available in:
samples/test-results.xls

This file contains results for 3 experiments with different threshold categories:
2 fold only
10 fold only
2 fold and 10 fold simultaneously

The "optimal" cost, gamma pair was chosen for the test was the one that created the greatest
difference between True Postive Rate and False Positive Rate. 
i.e. Maximized True Positive Rate and Minimized False Positive Rate

Please use the sample scripts below to run any cost, gamma pairs that you would like to compare.

Samples: (In Linux, executable permissions may need to be set for all scripts and executablers)
The main sample scripts have already been run, and the results are available:
samples/linux/results or samples/windows/results
Precompiled x86, 32bit binaries for linux and windows are  available in the samples directory.

Short Samples:
Short samples are available to play with. They are much less computationally intensive than the
full model selection scripts. Full scripts search a large number of possible cost, gamma 
parameter pairs, and
may take several hours for each experiment to complete on an Intel 3Ghz Xeon.

Samples Cache:
The cache consists of precomputed of Local Alignment scores in comma separated values format.
It is 30MB in size uncompressed, so it was compressed and placed into CVS repository.
Please extract it if you wish to use it:
samples/linux/cache/*.csv
samples/windows/cache/*.csv

Samples Output Files:

Each model selection run results in 5 output files. They are described below:

*results.csv - Statistics and results for every cost, gamma pair tried in an experiment.

*cmdline.txt - Contains all the program options used by hivm in a particular experiment.
Additionally, it creates a one line cmd line script so that the experiment can be easily run 
again.

*.log - A log file that may have some useful messages writ_ten during the program execution.

*gnuplot_script.gpl - A gnuplot script for creating a ROC curve image.

*roc_data_points.csv - Datapoints for the gnuplot script.
--
*.png - ROC curve image output by gnuplot script.



LINUX:

hivm was developed and tested using Boost.org libraries distribution: 3.1.13
The x86 binaries libraries are included in this distribution of hivm.
If these binaries do not work for you:

Run src/build_libs.sh to build the appropriate Boost binaries.

HIVM Build Instructions:
Run src/autogen.sh and src/make
Compilation is controlled using autoconf and automake.
Copy hivm.exe into samples/linux/

Run:
Open a drug script. like IDV.sh
Comment lines in or out to run different model selection or prediction routines
View output in samples/linux/results.

ROC Curve Graphs:
Find *.gpl scripts in samples/linux/results
Run gnuplot script *.gpl to see a ROC curve of model selection results.
From model selection, pick a c,g pair and run it in prediction mode,
and then compare it to model selection.

Linux Unit Tests:

To run unit tests on Linux, first compile a test version of hivm, then
run it.

To compile:

$ cd src
$ make check
$ cp test/testhivm test/Debug 

To run:

$ cd test/Debug
$ ./testhivm

In order to control which tests are run, use src/test/Definitions.hpp

By commenting out definitions, you can control which tests will be
compiled into testhivm.

TEST_ALL: full regression test of every possible test.*

LONG_TESTS: In any given test class, there are some tests that take a
long time to run. This definition can be used to turn these on an
off. Especially useful in conjunction with 'Classname'Tests =
Definitions used to control explicitly which classes you want to test.

*Isolation Tests: Certain classes can only be tested in
Isolation. Turn off (comment out), all other definitions before
running these one at a time.


WINDOWS:

hivm was developed and tested using Boost.org libraries distribution:
3.1.13 The appropriate win32 libraries are included in this
distribution of hivm.  If you need to build the libraries yourself for
any reason, use this command:

Run src/build_libs.bat

HIVM Build Instructions:
Open hivm.sln in MS Visual Studio.(Created with v7.1 aka MS Visual Studio 2003)
Choose Release version of the hivm project and build.
Copy Release/hivm.exe into samples/windows/

Note: MS Visual Studio 2008 users.
If you let MS Visual Studio 2008 update the sln file, you need to do manually update the referenced boost unit test library.
These are located in: hivm\src\3rd_party\boost\lib

Instructions after opening the MS sln file:
Right Click the 'testhivm' project. 

Goto Properties. Select Configuration: All Configurations
Open Linker > Input > Additional Dependencies.
Change 'libboost_unit_test_framework-vc71-mt-sgd-1_33_1.lib' to 'libboost_unit_test_framework-vc80-mt-sgd-1_33_1.lib'

Run:
Open a drug script. like IDV.bat
Use start_IDV.bat to start IDV.at using a Low Process Priority.
Comment lines in or out to run different model selection or prediction routines
View output in samples/windows/results.

ROC Curve Graphs:
Find *.gpl scripts in samples/windows/results
To see a ROC curve of model selection results.
Run gnuplot script *.gpl 

From model selection, pick a c,g pair and run it in prediction mode,
and then compare it to model selection.

Windows Unit Tests:

Building 
Open trunk/hivm.sln in MS Visual Studio. (Created with v7.1)
Choose Release version of the testhivm project and build.

Running:
src/test/Debug/testhivm.exe

In order to control which tests are run use:
src/test/Definitions.hpp

By commenting out definitions, you can control which tests will be compiled into testhivm.exe

TEST_ALL: full regression test of every possible test.*

LONG_TESTS: In any given test class, there are some tests that take a long time to run. 
This definition can be used to turn these on an off. Especially useful in conjunction with 
'Classname'Tests = Definitions used to control explicitly which classes you want to test. 

*Isolation Tests: Certain classes can only be tested in Isolation. Turn off (comment out), all 
other
definitions before running these one at a time.


Other:

# Samples Sizes for single thresholds with seed 42
Train	Test		Drug
300		137		 APV	
53		21		  ATV 
286		136		 IDV 
165		65		 LPV 
308		143		 NFV 
249		119		 RTV 
303		143		 SQV 

Single Class Warning:
"Warning! Training data is all in same class. Predictions were all for same class as well. 
Please view Readme.txt to see ways to avoid this problem. View log to see details of training 
data."

If you saw this warning, then all your training data is in the same class.

Cookbook to solve this problem:
a. Check the drug resistance threshold you used. If it is very low or very high then that will 
force all your training data to be in the same class.
b. Change the seed used to randomly split training and test data. Perhaps you just got a bad 
draw with the previous seed.
c. If available, use more data for training.
