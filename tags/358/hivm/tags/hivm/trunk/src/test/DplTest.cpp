#ifndef DPLTEST_HPP
#define DPLTEST_HPP

//choose unit tests to run based on Definitions.hpp
#include "Definitions.hpp"
#if defined  DPL_TEST || defined TEST_ALL

#include "../dpl.h"
#include <boost/test/auto_unit_test.hpp> //boost automatic unit test registration

struct Dpl_Fixture
{
	//ctor
    Dpl_Fixture()
	{
		wild_seq = "PQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF";
		seq_type = 1;//aa
		match_matrix = NULL;//default to BLOSUM50
		gap_o = 0.0; //default to -12 penalty
		gap_e = 0.0;// default to -2  penalty	
	}

	//dtor
    ~Dpl_Fixture(){}

	char* wild_seq;
	int seq_type;//aa
	float** match_matrix;//default to BLOSUM50
	float gap_o; //default to -12 penalty
	float gap_e;// default to -2  penalty
};

BOOST_AUTO_TEST_SUITE( DplTests );

/**
 Local Alignment
amino acids
BLOSUM50
gap open penalty: 12
gap extension penalty: 2

Online SSEARCH options:
ftp://ftp.virginia.edu/pub/fasta/ (for download)

FASTA package http://en.wikipedia.org/wiki/FASTA
"In addition to rapid heuristic search methods, the FASTA package provides SSEARCH, an implementation of the optimal Smith Waterman algorithm. "

**/

BOOST_AUTO_TEST_CASE( DplTest_ssearch_basic )
{

/**TEST 1
Test Sequence Set 1:
1. wild type
PQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF
2. CA3809
PQITLWQRPLVTVKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQIPIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF

Results dpl:

Results fasta3 ssearch on my PC using defaults:
 SSEARCH [version 3.4t25 Sept 2, 2005]s

Smith-Waterman (PGopt) (5.0 Nov 2004) function [BL50 matrix (15:-5)], open/ext: -12/-2
Smith-Waterman score: 658;  97.980% identity (97.980% ungapped) in 99 aa overlap (1-99:1-99)	
	*/

	Dpl_Fixture f;

	char* CA3809_seq = "PQITLWQRPLVTVKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQIPIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF";

	float dpl_result = local_alignment( f.wild_seq, CA3809_seq, f.seq_type, f.match_matrix, 
					f.gap_o, f.gap_e );

	BOOST_CHECK_EQUAL( 658, dpl_result );
}

BOOST_AUTO_TEST_CASE( DplTest_ssearch_self )
{
/**TEST 2
Test Sequence Set 2: (local alignment of self)

1. wild type
PQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF
2. wild type
PQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF

Results dpl:


Results fasta3 ssearch on my PC using defaults:
 SSEARCH [version 3.4t25 Sept 2, 2005]
 Smith-Waterman (PGopt) (5.0 Nov 2004) function [BL50 matrix (15:-5)], open/ext: -12/-2
Smith-Waterman score: 668;  100.000% identity (100.000% ungapped) in 99 aa overlap (1-99:1-99)
	*/

	Dpl_Fixture f;

	float dpl_result = local_alignment( f.wild_seq, f.wild_seq, f.seq_type, f.match_matrix, 
					f.gap_o, f.gap_e );


	BOOST_CHECK_EQUAL( 668, dpl_result );

}

BOOST_AUTO_TEST_CASE( DplTest_ssearch_more_subs)
{
/**TEST 3
Test Sequence Set 3: (local alignment of self)

1. wild type
PQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF
2. CA3872
PQITLWQRPLVTIKVGGQLKEALLDTGADDTVLEDMELPGRWKPKMIGGIGGFIKVKQYEDQIPIEICGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF


Results fasta3 ssearch on my PC using defaults:
 s-w opt: 615  Z-score: 908.1  bits: 172.9 E(): 9e-49
Smith-Waterman score: 615;  93.069% identity (94.949% ungapped) in 101 aa overlap (1-99:1-101)

	*/

	Dpl_Fixture f;

	char* CA3872_seq = "PQITLWQRPLVTIKVGGQLKEALLDTGADDTVLEDMELPGRWKPKMIGGIGGFIKVKQYEDQIPIEICGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF";

	float dpl_result = local_alignment( f.wild_seq, CA3872_seq, f.seq_type, f.match_matrix, 
					f.gap_o, f.gap_e );


	BOOST_CHECK_EQUAL( 615, dpl_result );

}

BOOST_AUTO_TEST_CASE( DplTest_ssearch_special_substitutions )
{
/**TEST 4
Test Sequence Set 4: (local alignment of self)

1. wild type
PQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF
2. made_up_seq with Z, B and X substitutions
PQITLWZRPLVTIBVGGQLXEALLDTGADDTVLEDMELPGRWKPKMIGGBGGFIKVKZYEDQIPIEXCGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF

 s-w opt: 581  Z-score: 858.2  bits: 163.7 E(): 5.4e-46
Smith-Waterman score: 581;  87.129% identity (88.889% ungapped) in 101 aa overlap (1-99:1-101)

*/
	Dpl_Fixture f;

	char* zbx_seq = "PQITLWZRPLVTIBVGGQLXEALLDTGADDTVLEDMELPGRWKPKMIGGBGGFIKVKZYEDQIPIEXCGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF";

	float dpl_result = local_alignment( f.wild_seq, zbx_seq, f.seq_type, f.match_matrix, 
					f.gap_o, f.gap_e );

	BOOST_CHECK_EQUAL( 581, dpl_result );
}

BOOST_AUTO_TEST_CASE( DplTest_ssearch_stop_codon )
{
/**TEST 4
Test Sequence Set 4: (local alignment of self)

1. wild type
PQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF
2. made_up_seq with Z, B and X and stop codeon '*' substitutions
PQITLWZRPLVTIBVGGQLXEALL*TGADDTVLEDMELPGRWKPKMIGGBGGFIKVKZYEDQIPIEXCGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF

 s-w opt: 572  Z-score: 845.1  bits: 161.2 E(): 2.9e-45
Smith-Waterman score: 572;  86.139% identity (87.879% ungapped) in 101 aa overlap (1-99:1-101)

*/
	Dpl_Fixture f;

	char* zbx_stopcodon_seq = "PQITLWZRPLVTIBVGGQLXEALL*TGADDTVLEDMELPGRWKPKMIGGBGGFIKVKZYEDQIPIEXCGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF";

	float dpl_result = local_alignment( f.wild_seq, zbx_stopcodon_seq, f.seq_type, f.match_matrix, 
					f.gap_o, f.gap_e );

	BOOST_CHECK_EQUAL( 572, dpl_result );
}

BOOST_AUTO_TEST_CASE( DplTest_ssearch_pass_penalties )
{
/**TEST 4
Test Sequence Set 4: (local alignment of self)

1. wild type
PQITLWQRPLVTIKIGGQLKEALLDTGADDTVLEEMNLPGRWKPKMIGGIGGFIKVRQYDQILIEICGHKAIGTVLVGPTPVNIIGRNLLTQIGCTLNF
2. made_up_seq with Z, B and X and stop codeon '*' substitutions
PQITLWZRPLVTIBVGGQLXEALL*TGADDTVLEDMELPGRWKPKMIGGBGGFIKVKZYEDQIPIEXCGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF

 s-w opt: 572  Z-score: 845.1  bits: 161.2 E(): 2.9e-45
Smith-Waterman score: 572;  86.139% identity (87.879% ungapped) in 101 aa overlap (1-99:1-101)

*/
	Dpl_Fixture f;

	char* zbx_stopcodon_seq = "PQITLWZRPLVTIBVGGQLXEALL*TGADDTVLEDMELPGRWKPKMIGGBGGFIKVKZYEDQIPIEXCGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF";

	float dpl_result = local_alignment( f.wild_seq, zbx_stopcodon_seq, f.seq_type, f.match_matrix, 
					999, 999 );

	//penalties to aa local alignment are not passed
	BOOST_CHECK_EQUAL( 572, dpl_result );
}

BOOST_AUTO_TEST_CASE( DplTest_symmetric_property )
{
	Dpl_Fixture f;
	char* zbx_stopcodon_seq = "PQITLWZRPLVTIBVGGQLXEALL*TGADDTVLEDMELPGRWKPKMIGGBGGFIKVKZYEDQIPIEXCGHKATIGTVLVGPTPVNIIGRNLLTQIGCTLNF";

	float dpl_result_A = local_alignment( f.wild_seq, zbx_stopcodon_seq, f.seq_type, f.match_matrix, 
					0, 0 );

	float dpl_result_B = local_alignment( zbx_stopcodon_seq, f.wild_seq, f.seq_type, f.match_matrix, 
					0, 0 );

	//Test Symmetric property
	BOOST_CHECK_EQUAL( dpl_result_A, dpl_result_B );
}

BOOST_AUTO_TEST_SUITE_END();

#endif //#if defined  DPL_TEST || defined TEST_ALL
#endif //DPLTEST_HPP
