/* $Id$ */
# ifndef CPPAD_PTHREAD_TEAM_INCLUDED
# define CPPAD_PTHREAD_TEAM_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cstddef> // for size_t

extern bool start_team(size_t num_threads);
extern bool work_team(void worker(void));
extern bool stop_team(void);

# endif
