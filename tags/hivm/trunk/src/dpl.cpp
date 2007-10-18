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


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef WIN32
#include <values.h>
#endif
#include "dpl.h"

#define GLOBAL     0
#define LOCAL      1

#define NO_WAY     -1
#define OUT_WAY    1

#define GAP        '_'

/*
  Storage for parameters.
*/
struct Param {
  float **match_matrix;
  size_t   n1;
  char  *alphabet1;
  size_t   n2;
  char  *alphabet2;
  int   alignment_type;
  float gc1;
  float ge1;
  float gc2;
  float ge2;
};

/*
  Auxiliary matrix cell. These values are used to calculate the value
  for the score matrix.
*/
struct Entry {
  float g;
  float e;
  float f;

};

/*
  Score matrix entry. 'up', 'left' and 'diag' are pointers to previous
  best location(s).
*/
struct OptimalScore {
  float value;
  int   up;
  int   left;
  int   diag;
};

/*
  Function (inefficiently) computes match between letters 'c1', 'c2'
  It should be replaced by a hash-based version.
*/
static float calc_match(struct Param *parameters, char c1, char c2)
{
  size_t   i;
  int   done;
  size_t   idx1;
  size_t   idx2;
  float match; 
  char  *alpha;

  idx1 = -1;
  idx2 = -1;
  alpha = parameters->alphabet1;
  done = 0;
  for (i = 0; i < parameters->n1 && !done; i++)
    if (c1 == alpha[i])
      {
	idx1 = i;
	done = 1;
      }
  alpha = parameters->alphabet2;
  done = 0;
  for (i = 0; i < parameters->n2 && !done; i++)
    if (c2 == alpha[i])
      {
	idx2 = i;
	done = 1;
      }
  if ((idx1 == -1) || (idx2 == -1))
    {
      /*
	One or both of the letters not found.
	
	If they agree, assign score one. If they don't, assign -1.
      */
      if (c1 == c2)
	match = 1;
      else
	match = -1;
    }
  else
    match = parameters->match_matrix[idx1][idx2];
  return match;
}

/*
  Calculate max(f1, f2).
*/
static float calc_max_2(float f1, float f2)
{
  float max;

  max = f1;
  if (f1 < f2)
    max = f2;
  return max;
}

/*
  Calculate max(f1, f2, f3).
*/
static float calc_max_3(float f1, float f2, float f3)
{
  return calc_max_2(calc_max_2(f1, f2), f3);
}

/*
  Create and fill dynamic programming matrix for 'seq1', 'seq2', using
  'parameters'.  Gap weight is specified using formula:

  gap_weight(length) = gc + ge * length 

  The actual formulas used are from Gusfield (Algorithms on Strings,
  Trees and Sequences, Cambridge University Press, 1997, p. 244), and
  they are identical to the ones in Gotoh (An Improved Algorithm for
  Matching Biological Sequences, J. Mol. Biol., 1982, pp. 705-708):

  g(i, j) = value(i-1, j-1)+s;
  e(i, j) = max{e(i, j-1), value(i, j-1)-gc1}-ge1;
  f(i, j) = max{f(i-1, j), value(i-1, j)-gc2}-ge2;
  value = max(g, e, f); 

  value(i, 0) = 0;
  value(0, j) = 0;
  e(i, 0) = -gc1-i*ge1;
  f(0, j) = -gc2-j*ge2;
*/
struct OptimalScore **fill_matrix(struct Param *parameters, char *seq1, char *seq2)
{
  size_t   i;
  size_t   j;
  size_t   len1;
  size_t   len2;
  struct OptimalScore **scores;
  struct OptimalScore *score;
  struct Entry **matrix;
  struct Entry *entry;

  /*printf("%s\n", seq1);*/
  /*printf("%s\n", seq2);*/
  len1 = strlen(seq1);
  len2 = strlen(seq2);
  matrix = (struct Entry **)		malloc((len1+1)*sizeof(struct Entry));
  scores = (struct OptimalScore **) malloc((len1+1)*sizeof(struct OptimalScore));
/*
  Initialize and fill the matrix.
*/
  for (i = 0; i < len1+1; i++)
  {
	matrix[i] = (struct Entry *)		  malloc((len2+1)*sizeof(struct Entry));
    scores[i] = (struct OptimalScore *) malloc((len2+1)*sizeof(struct OptimalScore));
    for (j = 0; j < len2+1; j++)
	{
	  score = &scores[i][j];
	  entry = &matrix[i][j];
	  if (i == 0)
	  {
		entry->f = -parameters->gc2-j*(parameters->ge2);
	    score->value = 0;
	  }
	  else
	  {
	    if (j == 0)
		{
		  entry->e = -parameters->gc1-i*(parameters->ge1);
		  score->value = 0;
		}
	    else
		{
		  entry->g = scores[i-1][j-1].value+calc_match(parameters,
							       seq1[i-1], seq2[j-1]);
		  entry->e = calc_max_2(matrix[i][j-1].e, scores[i][j-1].value-
					parameters->gc1)-parameters->ge1;
		  entry->f = calc_max_2(matrix[i-1][j].f, scores[i-1][j].value-
					parameters->gc2)-parameters->ge2;
		  score->value = calc_max_3(entry->g, entry->e, entry->f);
		  if (parameters->alignment_type == LOCAL)
		    score->value = calc_max_2(score->value, 0);
		}
	  }
	} //end for

  } //end for
  for (i = 0; i < len1 + 1; i++)
  {
	free( matrix[i] );
  }
  free( matrix );
  return scores;
}

/*
  Find 'global' or 'local' maximum in 'scores'. Report location of the
  maximum in 'imax', 'jmax'.
*/
static float get_max(int type, struct OptimalScore **scores, size_t len1, size_t len2,
		     size_t *imax, size_t *jmax)
{
  size_t   i;
  size_t   j;
  float max;

  if (imax)
    *imax = len1;
  if (jmax)
    *jmax = len2;
  max = scores[len1][len2].value;
  if (type == LOCAL)
    {
      for (i = 1; i <= len1; i++)
	for (j = 1; j <= len2; j++)
	  if (scores[i][j].value > max)
	    {
	      max = scores[i][j].value;
	      if (imax)
		*imax = i;
	      if (jmax)
		*jmax = j;
	    }
    }
  else 
    {
      for (i = 1; i <= len1; i++)
	if (scores[i][len2].value > max)
	  {
	    max = scores[i][len2].value;
	    if (imax)
	      *imax = i;
	  }
      for (j = 1; j <= len2; j++)
	if (scores[len1][j].value > max)
	  {
	    max = scores[len1][j].value;
	    if (jmax)
	      *jmax = j;
	  }
    }
  return max;
}

/*
  Compute score of the optimal local alignment between nucleotide or
  amino-acid sequences 'seq1' and 'seq2', using scoring matrix in
  'match_matrix', and 'gap_o' and 'gap_e' gap opening and extension
  penalties (positive numbers).

  For 'seq_type' == 0, it is assumed that 'seq1' and 'seq2' are
  nucleotide sequences; otherwise, they are considered to be
  amino-acid sequences.

  If match_matrix is NULL, the default scoring matrix and gap
  penalties are used. For nucleotide alignment, the scores are 5 for
  nucleotide match, -4 for mismatch, 16 for opening a gap, 2 for
  extending.  For amino-acid alignment, BLOSUM50 matrix and gap
  penalties of 12 and 2 are used. These values are identical to the
  defaults used in the FASTA package (program ssearch).
*/
float local_alignment(char *seq1, char *seq2, int seq_type, float **match_matrix, 
		      float gap_o, float gap_e)
{
  size_t   i;
  size_t   j;
  size_t   len1;
  size_t   len2;
  float score;
  struct OptimalScore **scores;
  struct Param *parameters;

  float blosum50[23][23] = 
              /* A  R  N  D  C  Q  E  G  H  I  L  K  M  F  P  S  T  W  Y  V  B  Z  X */
    { 
      /* A */ {  5,-2,-1,-2,-1,-1,-1, 0,-2,-1,-2,-1,-1,-3,-1, 1, 0,-3,-2, 0,-2,-1,-1 },      
      /* R */ { -2, 7, -1,-2,-4,1, 0,-3, 0,-4,-3, 3,-2,-3,-3,-1,-1,-3,-1,-3,-1, 0,-1 },
      /* N */ { -1,-1, 7, 2,-2, 0, 0, 0, 1,-3,-4, 0,-2,-4,-2, 1, 0,-4,-2,-3, 4, 0 -1 },
      /* D */ { -2,-2, 2, 8,-4, 0, 2,-1,-1,-4,-4,-1,-4,-5,-1, 0,-1,-5,-3,-4, 5, 1,-1 },
      /* C */ { -1,-4,-2,-4,13,-3,-3,-3,-3,-2,-2,-3,-2,-2,-4,-1,-1,-5,-3,-1,-3,-3,-2 },
      /* Q */ { -1, 1, 0, 0,-3, 7, 2,-2, 1,-3,-2, 2, 0,-4,-1, 0,-1,-1,-1,-3, 0, 4,-1 },
      /* E */ { -1, 0, 0, 2,-3, 2, 6,-3, 0,-4,-3, 1,-2,-3,-1,-1,-1,-3,-2,-1, 1, 5,-1 },
      /* G */ {  0,-3, 0,-1,-3,-2,-3, 8,-2,-4,-4,-2,-3,-4,-2, 0,-2,-3,-3,-4,-1,-2,-2 },
      /* H */ { -2, 0, 1,-1,-3, 1, 0,-2,10,-4,-3, 0,-1,-1,-2,-1,-2,-3,-1, 4, 0, 0,-1 },
      /* I */ { -1,-4,-3,-4,-2,-3,-4,-4,-4, 5, 2,-3, 2, 0,-3,-3,-1,-3,-1, 4,-4,-3,-1 },
      /* L */ { -2,-3,-4,-4,-2,-2,-3,-4,-3, 2, 5,-3, 3, 1,-4,-3,-1,-2,-1, 1,-4,-3,-1 },
      /* K */ { -1, 3, 0,-1,-3, 2, 1,-2, 0,-3,-3, 6,-2,-4,-1, 0,-1,-3,-2,-3, 0, 1,-1 },
      /* M */ { -1,-2,-2,-4,-2, 0,-2,-3,-1, 2, 3,-2, 7, 0,-3,-2,-1,-1, 0, 1,-3,-1,-1 },
      /* F */ { -3,-3,-4,-5,-2,-4,-3,-4,-1, 0, 1,-4, 0, 8,-4,-3,-2, 1, 4,-1,-4,-4,-2 },
      /* P */ { -1,-3,-2,-1,-4,-1,-1,-2,-2,-3,-4,-1,-3,-4,10,-1,-1,-4,-3,-3,-2,-1,-2 },
      /* S */ {  1,-1, 1, 0,-1, 0,-1, 0,-1,-3,-3, 0,-2,-3,-1, 5, 2,-4,-2,-2, 0, 0,-1 },
      /* T */ {  0,-1, 0,-1,-1,-1,-1,-2,-2,-1,-1,-1,-1,-2,-1, 2, 5,-3,-2, 0, 0,-1, 0 },
      /* W */ { -3,-3,-4,-5,-5,-1,-3,-3,-3,-3,-2,-3,-1, 1,-4,-4,-3,15, 2,-3,-5,-2,-3 },
      /* Y */ { -2,-1,-2,-3,-3,-1,-2,-3, 2,-1,-1,-2, 0, 4,-3,-2,-2, 2, 8,-1,-3,-2,-1 },
      /* V */ {  0,-3,-3,-4,-1,-3,-3,-4,-4, 4, 1,-3, 1,-1,-3,-2, 0,-3,-1, 5,-4,-3,-1 }, 
      /* B */ { -2,-1, 4, 5,-3, 0, 1,-1, 0,-4,-4, 0,-3,-4,-2, 0, 0,-5,-3,-4, 5, 2,-1 },
      /* Z */ { -1, 0, 0, 1,-3, 4, 5,-2, 0,-3,-3, 1,-1,-4,-1, 0,-1,-2,-2,-3, 2, 5,-1 },
      /* X */ { -1,-1,-1,-1,-2,-1,-1,-2,-1,-1,-1,-1,-1,-2,-2,-1, 0,-3,-1,-1,-1,-1,-1 }
              /* A  R  N  D  C  Q  E  G  H  I  L  K  M  F  P  S  T  W  Y  V  B  Z  X */
    };

  float dna_matrix[17][17] = 
              /* A   T   G   C   U   R   Y   M   K   W   S   B   D   H   V   N   I */
    { 
      /* A */ {  5, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 },      
      /* T */ { -4,  5, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 },
      /* G */ { -4, -4,  5, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 },
      /* C */ { -4, -4, -4,  5, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 },
      /* U */ { -4, -4, -4, -4,  5, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 },
      /* R */ { -4, -4, -4, -4, -4,  5, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 },
      /* Y */ { -4, -4, -4, -4, -4, -4,  5, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 },
      /* M */ { -4, -4, -4, -4, -4, -4, -4,  5, -4, -4, -4, -4, -4, -4, -4, -4, -4 },
      /* K */ { -4, -4, -4, -4, -4, -4, -4, -4,  5, -4, -4, -4, -4, -4, -4, -4, -4 },
      /* W */ { -4, -4, -4, -4, -4, -4, -4, -4, -4,  5, -4, -4, -4, -4, -4, -4, -4 },
      /* S */ { -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  5, -4, -4, -4, -4, -4, -4 },
      /* B */ { -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  5, -4, -4, -4, -4, -4 },
      /* D */ { -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  5, -4, -4, -4, -4 },
      /* H */ { -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  5, -4, -4, -4 },
      /* V */ { -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  5, -4, -4 },
      /* N */ { -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  5, -4 },
      /* I */ { -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  5 }
    };

  len1 = strlen(seq1);
  len2 = strlen(seq2);
  
  parameters = (struct Param *) malloc(sizeof(struct Param));
  parameters->alignment_type = LOCAL;
  if (seq_type == 0)
    {
      parameters->n1 = 17;
      parameters->alphabet1 = "ATGCURYMKWSBDHVNI";
      parameters->n2 = 17;
      parameters->alphabet2 = "ATGCURYMKWSBDHVNI";
    }
  else
    {
      parameters->n1 = 23;
      parameters->alphabet1 = "ARNDCQEGHILKMFPSTWYVBZX";
      parameters->n2 = 23;
      parameters->alphabet2 = "ARNDCQEGHILKMFPSTWYVBZX";
    }
  if (!match_matrix)
    {
      /*
	Scoring matrix not supplied, use the defaults.
      */
      parameters->match_matrix =  (float **) malloc(parameters->n1*sizeof(float *));
      for (i = 0; i < parameters->n1; i++)
		parameters->match_matrix[i] = (float *) malloc(parameters->n1*sizeof(float *));
      if (seq_type == 0)
	{
	  for (i = 0; i < parameters->n1; i++)
	    for (j = 0; j < parameters->n1; j++)
	      parameters->match_matrix[i][j] = dna_matrix[i][j];
	  parameters->gc1 = 16; 
	  parameters->ge1 = 4;
	}
      else
	{
	  for (i = 0; i < parameters->n1; i++)
	    for (j = 0; j < parameters->n1; j++)
	      parameters->match_matrix[i][j] = blosum50[i][j];
	  parameters->gc1 = 12;
	  parameters->ge1 = 2; 
	}
    }
  else
    {
      parameters->match_matrix = match_matrix;
      parameters->gc1 = 16; 
      parameters->ge1 = 4;
    }
  parameters->gc2 = parameters->gc1;
  parameters->ge2 = parameters->ge1;
  scores = fill_matrix(parameters, seq1, seq2);
	

  score = get_max(parameters->alignment_type, scores, len1, len2, (size_t *) 0, 
		  (size_t *) 0);
 

  for (i = 0; i < parameters->n1; i++)
  {
	free(parameters->match_matrix[i] );
  }
  free(parameters->match_matrix);
  
  free(parameters);
  // Delete the scores matrix
  for (i =0; i < len1 + 1; i++)
  {
	free(scores[i]);	
  }
  free(scores);

  return score;
}
