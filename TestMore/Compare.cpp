/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

/*
Check comparison operators between AD< AD<Base> > and Base, int
*/
# include <CppAD/CppAD.h>

namespace { 
	template <class Type>
	bool Compare(void)
	{	bool ok = true;
		using CppAD::AD;
	
		Type      middle = 4;
		AD<double> three = 3;
		AD<double> four  = 4;
		AD<double> five  = 5;
	
		// AD<double> > Type
		ok &= ! (three  >  middle);   
		ok &= ! (four   >  middle);   
		ok &=   (five   >  middle);   
		// Type > AD<double> 
		ok &=   (middle >  three );   
		ok &= ! (middle >  four  );   
		ok &= ! (middle >  five  );   
	
		// AD<double> >= Type
		ok &= ! (three  >= middle);   
		ok &=   (four   >= middle);   
		ok &=   (five   >= middle);   
		// Type > AD<double> 
		ok &=   (middle >= three );   
		ok &=   (middle >= four  );   
		ok &= ! (middle >= five  );   
	
		// AD<double> < Type
		ok &=   (three  <  middle);   
		ok &= ! (four   <  middle);   
		ok &= ! (five   <  middle);   
		// Type > AD<double> 
		ok &= ! (middle <  three );   
		ok &= ! (middle <  four  );   
		ok &=   (middle <  five  );   
	
		// AD<double> <= Type
		ok &=   (three  <= middle);   
		ok &=   (four   <= middle);   
		ok &= ! (five   <= middle);   
		// Type > AD<double> 
		ok &= ! (middle <= three );   
		ok &=   (middle <= four  );   
		ok &=   (middle <= five  );   
	
		return ok;
	}
}
bool Compare(void)
{	bool ok = true;
	ok     &= Compare<int>();
	ok     &= Compare<double>();
	ok     &= Compare< CppAD::AD<double> >();
	return ok;
}
