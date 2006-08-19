# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# This program is free software; you can use it under the terms of the 
#	         Common Public License Version 1.0.
# You should have received a copy of the this license along with this program.
# -----------------------------------------------------------------------------
#
if [ "$1" != "Doc" ] && [ "$1" != "Dev" ]
then
	echo "RunOMhelp target: where target is Doc or Dev"
	exit
fi
if [ "$1" = Dev ]
then
	rm -r -f Dev
	#
	echo "Building developer documentation"
	mkdir Dev
	cd    Dev
	if ! omhelp ../Dev.omh -noframe -xml -debug > ../OMhelp.Dev.log
	then
		grep "^OMhelp Error:" ../OMhelp.Dev.log
		echo "OMhelp could not build developer documentation."
		echo "See the complete error message in OMhelp.Dev.log"
		exit 1
	fi
	omhelp ../Dev.omh -noframe -debug
	cd ..
	if grep "^OMhelp Warning:" OMhelp.Dev.log
	then
		echo "See the complete warning messages in OMhelp.Dev.log."
	fi
fi
if [ "$1" = Doc ]
then
	#
	rm -r -f Doc
	echo "Building user documentation"
	#
	mkdir Doc
	cd    Doc
	if ! omhelp ../Doc.omh > ../OMhelp.Doc.log \
		-l http://www.coin-or.org/CppAD/ \
		-noframe \
		-xml \
		-debug
	then
		grep "^OMhelp Error:" ../OMhelp.Doc.log
		echo "OMhelp could not build user documentation."
		echo "See the complete error message in OMhelp.Doc.log."
		exit 1
	fi
	omhelp ../Doc.omh  \
                -l http://www.coin-or.org/CppAD/ \
		-noframe \
		-debug
	cd ..
	if grep "^OMhelp Warning:" OMhelp.Doc.log
	then
		echo "See the complete warning messages in OMhelp.Doc.log."
	fi
fi
exit 0
