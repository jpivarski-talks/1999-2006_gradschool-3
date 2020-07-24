#if !defined(MINUITINTERFACE_EXAMPLEFCN_H)
#define MINUITINTERFACE_EXAMPLEFCN_H
// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      ExampleFcn
//
/**\class ExampleFcn ExampleFcn.h MinuitInterface/ExampleFcn.h

 Description: An example Fit Fcn - just do a simple line fit to some points

Usage: This is an example Fcn for MinuitInterface to do a simple line
fit. Look at \ref annominuitex for more information.


*/
//
// Author:      Adam Lyon
// Created:     Mon Nov 22 10:08:13 EST 1999
// $Id: ExampleFcn.h,v 1.1 1999/12/14 05:15:01 lyon Exp $
//
// Revision history
//
// $Log: ExampleFcn.h,v $
// Revision 1.1  1999/12/14 05:15:01  lyon
// Renamed LeastSquaresFcn to ExampleFcn
//
// Revision 1.2  1999/12/12 21:12:35  lyon
// Added extensive Doxygen documentation
//
// Revision 1.1.1.1  1999/12/09 15:34:50  lyon
// Imported MinuitInterface sources
//

// system include files

// user include files

// Your Fcn must inherit from MIFcn
#include "MinuitInterface/MIFcn.h"

// forward declarations

class ExampleFcn : public MIFcn
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      //! Enums defining the order of parameters.
      enum params { kConstant, kSlope, kNumParams };

      // ---------- Constructors and destructor ----------------

      //! You use the constructor to pass in a list of points 
      ExampleFcn( int npts, float* x, float* y );

      //! Destructor
      virtual ~ExampleFcn();

      // ---------- member functions ---------------------------

      //! Determine a chi-square for the line fit
      virtual double iterate(double* values);
 
      // ---------- const member functions ---------------------

    // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      ExampleFcn( const ExampleFcn& );  // stop default 

      const ExampleFcn& operator=( const ExampleFcn& rhs );     // stop default 

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      int m_npts;   //  Number of data points
      float* m_x;   //  x's of points
      float* m_y;   //  y's of points

      // ---------- static data members ------------------------

};

// inline function definitions

// Additional Doxygen documentation

/*! \enum ExampleFcn::params
  A list of parameters involved with this Fcn. This enum specifies the 
  index of each parameter for use in accessing arrays and getting
  parameter information from MinuitInterface::parameter().
*/

/*! \var ExampleFcn::params ExampleFcn::kConstant
  Index of the constant parameter.
*/

/*! \var ExampleFcn::params ExampleFcn::kSlope
  Index of the slope parameter.
*/

/*! \var ExampleFcn::params ExampleFcn::kNumParams
  Number of parameters defined. Note this is not really needed.
*/

/*! \page annominuitex Annotated ExampleFcn code

The ExampleFcn is an example MinuitInterface Fcn object that does 
a simple line fit to a set of points. You construct
ExampleFcn with a list of points that you want to fit and then
minimize with MinuitInterface. Let's go through the code in some
detail.

\section lsqheader Header File for ExampleFcn
Here are the important parts of the header file ExampleFcn.h:
\dontinclude MinuitInterface/MinuitInterface/ExampleFcn.h

Since ExampleFcn  must inherit from MIFcn, we need to include
MIFcn's header.
\skipline #include

Now, we declare the class. Note the inheritance
\skip class
\until public:


Since values of the parameters for iterating will be given to us in
arrays, let's define enum's for the parameters we're going to use. Now 
we don't have to remember the order of the parameters for access. The
last enum (kNumParams) is just the number of parameters defined. You
don't really need it, but it may come in handy sometime.
\skipline params

Now we have our constructor and destructor. See how we're going to
pass in arrays of x and y values of the points to the constructor.
\skipline ExampleFcn(
\until ~Example

Here is where we override the iterate() function in MIFcn. We must do
this; after all, we have do some calculations for the minimization.
\c values is the array of parameter values Minuit is passing 
to us for the calculation.
\skipline iterate(

Here are the private data members of our ExampleFcn class.
\skipline private
\skipline data
\until m_y

That's it for the header file. You should probably look at the real
header file for additional information.

\section lsqimplementation Implementation file for ExampleFcn
Here are the important parts of the implementation file.

\dontinclude MinuitInterface/Class/ExampleFcn.cc

Here is the beginning of the code for the constructor for
ExampleFcn. We must call MIFcn's constructor, and then we fill
the member data with the arrays of points.
\skipline ExampleFcn(
\until {

Now we have to specify the parameters we want involved in the
fit. Since the parameters go hand in hand with the function we're
minimizing, it seems to makes sense that the Fcn should specify the
parameters it involves. To do this, we call addInitialParameter() for
each parameter we want to define. Each call to addInitialParameter()
adds the initial parameter information (name, start value, step size)
to a list. Arrays of parameter values will be in the same order as the 
addInitialParamter() order. You should thus call addInitialParameter() in
the same order as the enums defined in the header file. This will
conclude the constructor.
\until }

The other important part of ExampleFcn is doing the line fit 
calculation. This is in the ExampleFcn::iterate() member function.
\skipline iterate
\until {

\c values is an array of doubles of parameter values that Minuit wants 
us to use for the calculation. The array is ordered in the same order
as the addInitialParameter() calls above and thus, the same order as
our enum. Let's use the enum to copy the parameter values to temporary 
variables to make our code simpler.
\skipline kConstant
\until kSlope

Now we loop over our data points and calculate a chi-square for the
line fit.
\skipline chisq
\until }

One of the features of MinuitInterface is specifying diagnostic
levels. Let's print out some diagnostic information, but only if the 
user set the diagnostic level (with MinuitInterface::setDiagLevel()) 
to kIntermediate or larger. These will print for every Minuit iteration.
\skipline Print
\until }

Now that we have our chi-square, we must return it to Minuit.
\until }

And that's all there is to it. For an example on running a
MinuitInterface job with this Fcn, look at MinuitInterface
documentation for example 
code.

*/

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "MinuitInterfaceTemplateExampleFcn.cc"
//#endif

#endif  //MINUITINTERFACE_EXAMPLEFCN_H
