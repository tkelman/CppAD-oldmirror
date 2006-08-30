# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# This program is free software; you can use it under the terms of the 
#	         Common Public License Version 1.0.
# You should have received a copy of the this license along with this program.
# -----------------------------------------------------------------------------
#
# automake input file
#
# Note that config.h is needed by VC++; i.e., for users that do not run
# the configure script but rather use the project files 
#
if CppAD_POSTFIX
myincludedir = $(includedir)/$(POSTFIX_DIR)
else
myincludedir = $(includedir)
endif
#
nobase_myinclude_HEADERS =  \
	CppAD/config.h \
	CppAD/CheckNumericType.h \
	CppAD/CheckSimpleVector.h \
	CppAD/CppAD.h \
	CppAD/CppAD_vector.h \
	CppAD/ErrorHandler.h \
	CppAD/LuFactor.h \
	CppAD/LuInvert.h \
	CppAD/LuSolve.h \
	CppAD/NearEqual.h \
	CppAD/OdeErrControl.h \
	CppAD/OdeGear.h \
	CppAD/OdeGearControl.h \
	CppAD/Poly.h \
	CppAD/PowInt.h \
	CppAD/RombergOne.h \
	CppAD/RombergMul.h \
	CppAD/Rosen34.h \
	CppAD/Runge45.h \
	CppAD/SpeedTest.h \
	CppAD/TrackNewDel.h \
	CppAD/local/ad_.hpp \
	CppAD/local/ad_fun_.hpp \
	CppAD/local/ad_tape_.hpp \
	CppAD/local/ad_valued_.hpp \
	CppAD/local/abs_.hpp \
	CppAD/local/abs_op_.hpp \
	CppAD/local/acos_op_.hpp \
	CppAD/local/add_.hpp \
	CppAD/local/add_eq_.hpp \
	CppAD/local/add_op_.hpp \
	CppAD/local/arithmetic_.hpp \
	CppAD/local/asin_op_.hpp \
	CppAD/local/atan2_.hpp \
	CppAD/local/atan_op_.hpp \
	CppAD/local/bender_quad_.hpp \
	CppAD/local/bool_fun_.hpp \
	CppAD/local/bool_fun_link_.hpp \
	CppAD/local/bool_valued_.hpp \
	CppAD/local/cap_taylor_.hpp \
	CppAD/local/compare_.hpp \
	CppAD/local/cond_exp_.hpp \
	CppAD/local/constructor_.hpp \
	CppAD/local/convert_.hpp \
	CppAD/local/copy_ad_.hpp \
	CppAD/local/copy_base_.hpp \
	CppAD/local/cppad_error_.hpp \
	CppAD/local/cppad_vector_.hpp \
	CppAD/local/declare_.hpp \
	CppAD/local/default_.hpp \
	CppAD/local/define_.hpp \
	CppAD/local/dependent_.hpp \
	CppAD/local/discrete_.hpp \
	CppAD/local/div_.hpp \
	CppAD/local/div_eq_.hpp \
	CppAD/local/div_op_.hpp \
	CppAD/local/drivers_.hpp \
	CppAD/local/eq_.hpp \
	CppAD/local/erf_.hpp \
	CppAD/local/exp_op_.hpp \
	CppAD/local/for_one_.hpp \
	CppAD/local/for_two_.hpp \
	CppAD/local/for_jac_sweep_.hpp \
	CppAD/local/for_sparse_jac_.hpp \
	CppAD/local/forward_.hpp \
	CppAD/local/forward_sweep_.hpp \
	CppAD/local/fun_check_.hpp \
	CppAD/local/fun_eval_.hpp \
	CppAD/local/fun_construct_.hpp \
	CppAD/local/hessian_.hpp \
	CppAD/local/identical_.hpp \
	CppAD/local/independent_.hpp \
	CppAD/local/integer_.hpp \
	CppAD/local/jacobian_.hpp \
	CppAD/local/log_op_.hpp \
	CppAD/local/mul_.hpp \
	CppAD/local/mul_eq_.hpp \
	CppAD/local/mul_op_.hpp \
	CppAD/local/op_.hpp \
	CppAD/local/op_code_.hpp \
	CppAD/local/ordered_.hpp \
	CppAD/local/output_.hpp \
	CppAD/local/par_var_.hpp \
	CppAD/local/pow_.hpp \
	CppAD/local/print_for_.hpp \
	CppAD/local/lu_ratio_.hpp \
	CppAD/local/math_other_.hpp \
	CppAD/local/near_equal_ext_.hpp \
	CppAD/local/reverse_.hpp \
	CppAD/local/rev_hes_sweep_.hpp \
	CppAD/local/rev_jac_sweep_.hpp \
	CppAD/local/rev_one_.hpp \
	CppAD/local/rev_sparse_jac_.hpp \
	CppAD/local/rev_sparse_hes_.hpp \
	CppAD/local/rev_two_.hpp \
	CppAD/local/reverse_sweep_.hpp \
	CppAD/local/sin_cos_.hpp \
	CppAD/local/sparse_.hpp \
	CppAD/local/sqrt_op_.hpp \
	CppAD/local/std_math_unary_.hpp \
	CppAD/local/sub_.hpp \
	CppAD/local/sub_eq_.hpp \
	CppAD/local/sub_op_.hpp \
	CppAD/local/tape_rec_.hpp \
	CppAD/local/user_ad_.hpp \
	CppAD/local/unary_minus_.hpp \
	CppAD/local/unary_plus_.hpp \
	CppAD/local/vec_ad_.hpp \
	CppAD/local/undef_.hpp \
	CppAD/local/value_.hpp 
#
# Did user specify --with-Example
if CppAD_GETSTARTED
GETSTARTED = GetStarted
else
GETSTARTED =
endif
#
# Did user specify --with-Introduction
if CppAD_INTRODUCTION
INTRODUCTION = Introduction
else
INTRODUCTION =
endif
#
# Did user specify --with-Example
if CppAD_EXAMPLE
EXAMPLE = Example
else
EXAMPLE =
endif
#
# Did user specify --with-TestMore
if CppAD_TESTMORE
TESTMORE = TestMore
else
TESTMORE =
endif
#
# Did user specify --with-Speed
if CppAD_SPEED
SPEED = Speed
else
SPEED =
endif
#
# Did user specify --with-PrintFor
if CppAD_PRINTFOR
PRINTFOR = PrintFor
else
PRINTFOR =
endif
#
# Did user specify --with-SpeedExample
if CppAD_SPEEDEXAMPLE
SPEEDEXAMPLE = SpeedExample
else
SPEEDEXAMPLE =
endif
#
# Did user specify a value for ADOLC_DIR in configure command line
if CppAD_ADOLC
ADOLC = Adolc
else
ADOLC = 
endif
#
# Did user specify a value for FADBAD_DIR in configure command line
if CppAD_FADBAD
FADBAD = Fadbad
else
FADBAD = 
endif
#
SUBDIRS = \
	$(ADOLC) \
	$(EXAMPLE) \
	$(INTRODUCTION) \
	$(FADBAD) \
	$(GETSTARTED) \
	$(PRINTFOR) \
	$(SPEEDEXAMPLE) \
	$(SPEED) \
	$(TESTMORE) 
#
EXTRA_DIST = \
	Build.sh \
	CheckIncludeFile.sh \
	CheckIncludeOmh.sh \
	CheckIncludeDef.sh \
	CleanCppAD.sh \
	CppAD.tex \
	cpl1.0.txt \
	Dev \
	Dev.omh \
	Doc \
	Doc.omh \
	Example/OneTest \
	FixAclocal.sh \
	FixMakefile.sh \
	omh \
	RunOMhelp.sh \
	UWCopy040507.html
#
dist-hook:
	rm -rf `find $(distdir)/omh -name .svn`
