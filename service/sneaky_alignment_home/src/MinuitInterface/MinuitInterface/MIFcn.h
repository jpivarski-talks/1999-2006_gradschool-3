#if !defined(MINUITINTERFACE_MIFCN_H)
#define MINUITINTERFACE_MIFCN_H
// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MIFcn
// 

// The best way to get help on MIFcn is to look up the
// Doxygen generated documentation under the "Automatic Software
// Documentation" web page. (see Cleo III Software web page). Then
// search for the MIFcn class. There's 
// lots of information there, including some example code.
//

/**\class MIFcn MIFcn.h MinuitInterface/MIFcn.h

 Description: The interface for Fcn's used by MinuitInterface (ABSTRACT class)

 Usage:
   MIFcn defines the interface for Fcn classes used by the
   MinuitInterface. In Fortran Minuit, you tell Minuit what Fortran
   function you want to have minimized. For the C++ MinuitInterface,
   you instead defined an Fcn *object*. The advantage here is that an
   object can hold its own data that can be used in the course of
   minimization (such as track objects or perhaps geometry
   information). 

   MIFcn is an abstract class; you can't instantiate one
   explictly. Rather, it helps you define your own Fcn classes by
   giving you an interface to "fill in" so that the MinuitInterface
   object will get the information it expects. 

   Here are steps that you must follow in order to have your Fcn class
   work with the MinuitInterface. An example Fcn, ExampleFcn, 
   in the MinuitInterface package and illustrates all of the items
   below. Also, see \ref annominuitex for more information.
   
   <OL>
   <LI>Your Fcn *must* inherit from MIFcn (this class).</LI>
   <LI>You should use enums to give names to the parameters.</LI>
   <LI>The Fcn constructor must call MIFcn's default constructor.</LI>
   <LI>In the body of the Fcn constructor, you must call
       addInitialParameter() for each parameter you wish to be fit.</LI>
   <LI>Your Fcn *must* have an iterate() method, overridding the pure
       virtual iterate() method in MIFcn. iterate() is what actually
       does the calculation for minimization.</LI>
   </OL>

   Your Fcn may optionally have the following:<BR>
   <UL>
   <LI>Override the dummy initialize() method. initialize() is called
       before the first Minuit ineration. You can use it to load files or
       other initialization proceedures. Note that you can also put
       initialization code in the Fcn's constructor.</LI>

   <LI>Override the dummy finalize() method. finalize() is called after 
       the last Minuit interation. Use it to close files or save important 
       information collected in the Fcn. Note that you can also do those
       things in the Fcn's destructor.</LI>

   <LI>Override the dummy derivatives() method. derivatives() is used
       to calculate your own gradient.</LI>

   <LI>Feel free to add other member functions that may be needed. The
       only mandatory items and functions are described above.</LI>
   </UL>

*/

//
// Author:      Adam Lyon
// Created:     Thu Nov 18 15:48:02 EST 1999
// $Id: MIFcn.h,v 1.4 1999/12/14 05:15:05 lyon Exp $
//
// Revision history
//
// $Log: MIFcn.h,v $
// Revision 1.4  1999/12/14 05:15:05  lyon
// Renamed LeastSquaresFcn to ExampleFcn
//
// Revision 1.3  1999/12/12 21:12:35  lyon
// Added extensive Doxygen documentation
//
// Revision 1.2  1999/12/12 04:25:54  lyon
// Simplified adding parameters
//
// Revision 1.1.1.1  1999/12/09 15:34:50  lyon
// Imported MinuitInterface sources
//

// system include files

// user include files
#include "MinuitInterface/MIInitialParameter.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class MIFcn
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      //! Default constructor
      MIFcn();

      //! Destructor
      virtual ~MIFcn() = 0;

      // ---------- member functions ---------------------------

      //! Initialize Fcn before first Minuit interation
      /*! Optional. Note that you may also put initialization code in your Fcn's 
	  constructor. The default is to do nothing.
      */
      virtual void initialize();

      //! Perform calculations for a Minuit interation
      /*! You MUST override this member function in your Fcn. 
	\param values An array of doubles of parameter values that
	Minuit is trying for the current iteration. Use the ENUM's
	defined in your Fcn to access.
	\return The value of the function given the parameter values
	(e.g. chi-square).
      */
      virtual double iterate(double* values) = 0;
      
      //! Calculate gradient for a Minuit gradiant call
      /*!
	Optional. Used if you tell Minuit to calculate derivatives
	with your code instead of its own. 
	\param values An array of doubles of parameter values that
	Minuit is trying for the current iteration.
	\param derivatives An array of double you fill with the
	derivative of each parameter.
	
	Use the ENUM's you've defined in your Fcn to access the array
	elements.
      */
      virtual void derivatives(double* values, double* derivatives);
  
      //! Finalize the Fcn after the last Minuit iteration.
      /*! Optional. You may also put code in your Fcn's
	destructor. The default is to do nothing.*/
      virtual void finalize();

      //! Set the diagonistics level
      /*! Don't call this yourself. Instead, use MinuitInterface::setDiagLevel()
	  \sa MinuitInterface
      */
      void setDiagLevel(int level);

      //! Return the diagnostics level
      /*! Use this member function to get the current diagnostics
	level. Compare with the enum's defined in
	MinuitInterface::diagLevels.
      */
      inline int diagLevel() const;

      //! Return the set of initial parameters (used by MinuitInterface)
      /*! This member function is used by MinuitInterface. You should
	never have to call it yourself.
      */

      const STL_VECTOR(MIInitialParameter)* initialParameters() const;

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------


      //! Add an initial parameter to the parameter list
      /*! This member function registers a parameter to be fit
	for. Call this member function in your Fcn's constructor 
	for each parameter you want involved in
	the fit (the order should be the same that is followed in the
	enum's you defined in the Fcn). 
      */
      void addInitialParameter(string name, double start, double
			       step);

      //! Add an initial parameter to the parameter list
      /*! Allows you to also specify upper and lower bounds on the
	parameter value.
      */
      void addInitialParameter(string name, double start, double step,
			       double lowBound, double highBound);
  
      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MIFcn( const MIFcn& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MIFcn& operator=( const MIFcn& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      int m_diagLevel;
  
      STL_VECTOR(MIInitialParameter)* m_initialParameters;
  

      // ---------- static data members ------------------------

};

// inline function definitions
int MIFcn::diagLevel() const
{ return m_diagLevel; }



// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "MinuitInterface/Template/MIFcn.cc"
//#endif


#endif /* MINUITINTERFACE_MIFCN_H */
