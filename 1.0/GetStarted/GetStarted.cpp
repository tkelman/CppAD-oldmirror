/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
$begin GetStarted.cpp$$
$spell
      http://www.coin-or.org/CppAD/
      getstarted
      namespace
      iostream
      const
      std
      powx
      Jacobian
      jac
      endl
      da
      cout
$$

$section Getting Started Using CppAD$$
$index getstarted$$
$index simple, example$$
$index example, simple$$
$index start, using CppAD$$

$head Purpose$$
Demonstrate the use of CppAD by computing the derivative 
of a simple example function.

$head Function$$
The example function $latex y : \R \rightarrow \R$$ is defined by 
$latex \[
      y(x) = a_0 + a_1 * x^1 + \cdots + a_{k-1} * x^{k-1}
\] $$
where $italic a$$ is a fixed vector of length $italic k$$.

$head Derivative$$
The derivative of $latex y(x)$$ is given by
$latex \[
      y'(x) = a_1 + 2 * a_2 * x +  \cdots + (k-1) * a_{k-1} * x^{k-2} 
\] $$

$head Value$$
For the particular case in this example,
$latex k$$ is equal to 5, 
$latex a = (1, 1, 1, 1, 1)$$, and 
$latex x = 3$$.
If follows that 
$latex \[
      y'( 3 ) = 1 + 2 * 3 + 3 * 3^2 + 4 * 3^3 = 142
\] $$

$head Poly$$
The routine $code Poly$$ is defined below for this particular application.
A general purpose polynomial evaluation routine is documented and
distributed with CppAD (see $xref/Poly/$$).


$head Source Code$$
$codep */
#include <iostream>      // standard input/output 
#include <vector>        // standard vector
#include <CppAD/CppAD.h> // the CppAD package http://www.coin-or.org/CppAD/

namespace { 
      // define y(x) = Poly(a, x) in the empty namespace
      template <class Type>
      Type Poly(const std::vector<double> &a, const Type &x)
      {     size_t k  = a.size();
            Type y   = 0.;  // initialize summation
            Type x_i = 1.;  // initialize x^i
            size_t i;
            for(i = 0; i < k; i++)
            {     y   += a[i] * x_i;  // y   = y + a_i * x^i
                  x_i *= x;           // x_i = x_i * x
            }
            return y;
      }
}
// main program
int main(void)
{     using CppAD::AD;           // use AD as abbreviation for CppAD::AD
      using std::vector;         // use vector as abbreviation for std::vector
      size_t i;                  // a temporary index

      // vector of polynomial coefficients
      size_t k = 5;              // number of polynomial coefficients
      vector<double> a(k);       // vector of polynomial coefficients
      for(i = 0; i < k; i++)
            a[i] = 1.;           // value of polynomial coefficients

      // domain space vector
      size_t n = 1;              // number of domain space variables
      vector< AD<double> > X(n); // vector of domain space variables
      X[0] = 3.;                 // value corresponding to operation sequence

      // declare independent variables and start recording operation sequence
      CppAD::Independent(X);

      // range space vector
      size_t m = 1;              // number of ranges space variables
      vector< AD<double> > Y(m); // vector of ranges space variables
      Y[0] = Poly(a, X[0]);      // value during recording of operations

      // store operation sequence in f: X -> Y and stop recording
      CppAD::ADFun<double> f(X, Y);

      // compute derivative using operation sequence stored in f
      vector<double> jac(m * n); // Jacobian of f (m by n matrix)
      vector<double> x(n);       // domain space vector
      x[0] = 3.;                 // argument value for derivative
      jac  = f.Jacobian(x);      // Jacobian for operation sequence

      // print the results
      std::cout << "y'(3) computed by CppAD = " << jac[0] << std::endl;

      // check if the derivative is correct
      int error_code;
      if( jac[0] == 142. )
            error_code = 0;      // return code for correct case
      else  error_code = 1;      // return code for incorrect case

      return error_code;
}
/* $$
$head Output$$
The program above will generate the following output:
$code
$verbatim%GetStarted/GetStarted.out%$$
$$
$end
*/
