//@purpose - Parse cmd line and config files for options.

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <boost/program_options.hpp>//library allows program developers to obtain program options, that is (name, value) pairs from the user

class Options{

public:

	//default constructor. JUST FOR UNIT TESTS!
	Options();	

	//constructor to parse with cmd line
	Options( int argc, char* argv[] );

	//destructor
	virtual ~Options();

	//@purpose parse all options cmd line and config files
	void parse_options( int ac, char* av[], boost::program_options::variables_map& vm );
	
	//@purpose returns ref to object containing all options
	boost::program_options::variables_map& get_options();

	////@purpose save cmd line and config file
	//void save_all_options( std::string file_name_prefix );

	//@purpose save cmd line options to file
	//@post saves to path: output_prefix_cmdline.txt
	void save_cmd_line_options() const;
	void save_cmd_line_options( const std::string file_name_prefix ) const;

	void save_cmd_line_options_( const std::string file_name_prefix ) const;

	//@purpose save configuration file options to file
	//@post saves to prefix.cfg
	//void save_config_file_options( std::string file_name_prefix );

//OPTIONS SET IN CMDLINE

//configuration file - NOT USED YET
	//std::string hivm_config_file;

//did the user ask for help aka display usage request?
	bool help_flag;

//parameters for hivm
	std::string purpose;//enum?
	std::string drug; //enum?

	std::vector<double> thresholds;
	double threshold_default;
	std::string wild_type_file;
	std::string susceptibility_file;
	std::string susceptibility_type;//clinical, lab, both (enum?)
	std::string output_prefix;

//params for model-selection and model-validation
	int seed; //splitting suscep file into training and testing
	//double crossval_seed;//OPTIONAL randomizing cross validation runs
	
//parameters for model-validation only
	//Only for advanced user that has produced a hivdb format
	//file for testing. Implies that all of susceptibility_file will
	//be used as a training dataset
	std::string susceptibility_file_test_dataset;

//parameters for model-selection only
	//use entire susceptibility file for model selection. 
	//if true, seed ignored.
	bool use_entire_susceptbility_file;

//cost and gamma grid search 
	double lg_cost_low;
	double lg_cost_high;
	double lg_cost_inc;

	double lg_gamma_low;
	double lg_gamma_high;
	double lg_gamma_inc;

	//TODO how to handle this vs the grid search? be careful
	double lg_cost;
	double lg_gamma;

//  parameters for the svm. SET IN CONFIG FILE ONLY

	int svm_type;
	enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };	/* svm_type */
	
	int kernel_type;
	enum { LINEAR, POLY, RBF, SIGMOID };	/* kernel_type */

	int degree;	/* for poly */
	double coef0;	/* for poly/sigmoid */

	/* these are for training only */
	double cache_size; /* in MB */
	double eps;	/* stopping criteria */
	int nr_weight;		/* for C_SVC */
	int *weight_label;	/* for C_SVC */
	double* weight;		/* for C_SVC */
	double nu;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
	double p;	/* for EPSILON_SVR */
	int shrinking;	/* use the shrinking heuristics */
	int probability; /* do probability estimates */

	//TODO unit test this
	bool validate_required_options_();

private:

	/**
	@purpose Check  susceptibility file was split into pieces using
	seed. Certain parameters trump seed, and the susceptibility file
	is not split in these cases.
	*/
	bool is_seed_used_() const;

	void _initialize( int argc, char* argv[] );

	//structure to hold options from cmd line and config file
	boost::program_options::variables_map vm;

	//copy constructor
	Options( const Options& );

	//assignment operator
	Options& operator= ( const Options& );

};

#endif//OPTIONS_HPP
