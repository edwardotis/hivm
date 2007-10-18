// **************************************************************
//						Copyright (C) 2003 - 2005 
//			Martin Eggenberger, Ljubomir Buturovic, SFSU
//
// **************************************************************
//
// Permission is hereby granted, free of charge, to any person obtaining 
// a copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// **************************************************************
//
// PhaseMachine is a program that allows the classification of 
// two species based on the individual genetic sequecne as 
// obtained by the phase program.
//			
// For further information, please read the documentation to the 
// phasemachine program or conact.
//
//	meggenbe@sfsu.edu
//	ljubomir@sfsu.edu
// 
// **************************************************************

#ifndef DPL_
#define DPL_ 1

#ifdef __cplusplus
extern "C" {
#endif

//
//  Compute score of the optimal local alignment between nucleotide or
//  amino-acid sequences 'seq1' and 'seq2', using scoring matrix in
//  'match_matrix', and 'gap_o' and 'gap_e' gap opening and extension
//  penalties (positive numbers).
//
//  For 'seq_type' == 0, it is assumed that 'seq1' and 'seq2' are
//  nucleotide sequences; otherwise, they are considered to be
//  amino-acid sequences.
//
//  If match_matrix is NULL, the default scoring matrix and gap
//  penalties are used. For nucleotide alignment, the scores are 5 for
//  nucleotide match, -4 for mismatch, 16 for opening a gap, 2 for
//  extending.  For amino-acid alignment, BLOSUM50 matrix and gap
//  penalties of 12 and 2 are used. These values are identical to the
//  defaults used in the FASTA package (program ssearch).
//
float local_alignment(char *seq1, char *seq2, int seq_type, float **match_matrix, 
		      float gap_o, float gap_e);


#ifdef __cplusplus
}
#endif


#endif
