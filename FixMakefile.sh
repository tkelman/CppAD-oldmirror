# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# -----------------------------------------------------------------------------
#
sed \
-e 's/tardir=$(distdir) && $(am__tar) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz/tar -cvf $(distdir).tar $(distdir) ; gzip -f $(GZIP_ENV) $(distdir).tar/'\
	< Makefile  > Makefile.new
diff Makefile     Makefile.new
mv   Makefile.new Makefile
