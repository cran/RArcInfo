#ifndef RINFO_H
#define RINFO_H

#include<Rversion.h>
#include<Rinternals.h>
#include<Rdefines.h>

#if R_VERSION < R_Version(1,2,0)
#define STRING_ELT(x,i)		(STRING(x)[i])
#define VECTOR_ELT(x,i)	 	(VECTOR(x)[i])
#define SET_STRING_ELT(x,i,v)	(STRING(x)[i]=(v))
#define SET_VECTOR_ELT(x,i,v)	(VECTOR(x)[i]=(v))
#endif


SEXP get_names_of_coverages(SEXP directory);
SEXP get_table_names(SEXP directory);
SEXP get_table_fields(SEXP info_dir, SEXP table_name);
SEXP get_arc_data(SEXP directory, SEXP coverage, SEXP filename) ;
SEXP get_bnd_data(SEXP info_dir, SEXP tablename);
SEXP get_pal_data(SEXP directory, SEXP coverage, SEXP filename);
SEXP get_lab_data(SEXP directory, SEXP coverage, SEXP filename);
SEXP get_cnt_data(SEXP directory, SEXP coverage, SEXP filename);
SEXP get_tol_data(SEXP directory, SEXP coverage, SEXP filename);

#endif
