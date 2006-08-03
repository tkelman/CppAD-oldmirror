# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# This program is free software; you can use it under the terms of the 
#	         Common Public License Version 1.0.
# You should have received a copy of the this license along with this program.
# -----------------------------------------------------------------------------
#
sed \
-e 's/tardir=$(distdir) && $(am__tar) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz/tar -cvf $(distdir).tar $(distdir) ; gzip -f $(GZIP_ENV) $(distdir).tar/'\
	< Makefile  > Makefile.new
diff Makefile     Makefile.new
mv   Makefile.new Makefile
