# ! /bin/bash 
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
flag="+"
if [ "$flag" == "" ]
then
	echo "./svn_status.sh <automake flag>"
	echo "where automake flag is either + or -"
	exit 1
fi
yyyy_mm_dd="2010-01-01"
#
svn status | sed  \
	-e '/^[?].*\.[0-9]*$/d'                               \
	-e '/^[?].*\.tmp$/d'                                  \
	-e '/^[?].*\.out$/d'                                  \
	-e '/^[?].*\.gz$/d'                                   \
	-e '/^[?].*\.tgz$/d'                                  \
	-e '/^[?].*\.zip$/d'                                  \
	-e '/^[?].*\.swp$/d'                                  \
	-e '/^[?].*\.cache$/d'                                \
	-e '/^[?].*\.exe$/d'                                  \
	-e '/^[?].*\.ncb$/d'                                  \
	-e '/^[?].*\.suo$/d'                                  \
	-e '/^[?].*\.deps$/d'                                 \
	-e '/^[?].*\.a$/d'                                    \
	-e '/^[?].*\.log$/d'                                  \
	-e '/^[?].*\.stackdump$/d'                            \
	-e '/^[?].*\/test_one.cpp$/d'                         \
	-e '/^[?].*\/Debug$/d'                                \
	-e '/^[?].*\/Release$/d'                              \
	-e '/^[?].*\/stamp-h1$/d'                             \
	-e '/^[?].*\/makefile$/d'                             \
	-e '/^[?].*\/_adolc-*/d'                              \
	-e '/^[?] *makefile$/d'                               \
	-e '/^[?].*\/junk$/d'                                 \
	-e '/^[?] *junk$/d'                                   \
	-e '/^[?].*\/junk\.[0-9a-zA-Z]*$/d'                   \
	-e '/^[?] *junk\.[0-9a-zA-Z]*$/d'                     \
	-e '/^[?] *dev$/d'                                    \
	-e '/^[?] *config.status$/d'                          \
	-e '/^[?] *doc$/d'                                    \
	-e '/^[?] *doxydoc$/d'                                \
	-e '/^[?] *dev$/d'                                    \
	-e '/^[?] *aclocal.m4$/d'                             \
	-e '/^[?] *cppad-[0-9]\{8\}.*$/d'                     \
	-e '/^[?] *doc.omh$/d'                                \
	-e '/^[?] *doxyfile$/d'                               \
	-e '/^[?] *svn_dist$/d'                               \
	-e '/^[?] *omh\/install_unix.omh$/d'                  \
	-e '/^[?] *omh\/install_windows.omh$/d'               \
	-e '/^[?] *example\/test_one.sh$/d'                   \
	-e '/^[?] *test_more\/test_one.sh$/d'                 \
	-e '/^[?] *example\/example$/d'                       \
	-e '/^[?] *ipopt_cppad\/ipopt_cppad$/d'               \
	-e '/^[?] *ipopt_cppad\/ipopt_ode_speed$/d'           \
	-e '/^[?] *print_for\/print_for$/d'                   \
	-e '/^[?] *speed\/adolc\/adolc$/d'                    \
	-e '/^[?] *speed\/cppad\/cppad$/d'                    \
	-e '/^[?] *speed\/double\/double$/d'                  \
	-e '/^[?] *speed\/example\/example$/d'                \
	-e '/^[?] *speed\/fadbad\/fadbad$/d'                  \
	-e '/^[?] *speed\/profile\/profile$/d'                \
	-e '/^[?] *speed\/sacado\/sacado$/d'                  \
	-e '/^[?] *test_more\/test_more$/d'                   \
	-e '/^[?] *ipopt_cppad\/include$/d'                   \
	-e '/^[?] *ipopt_cppad\/lib$/d'                       \
	-e '/cygwin_package$/d'
