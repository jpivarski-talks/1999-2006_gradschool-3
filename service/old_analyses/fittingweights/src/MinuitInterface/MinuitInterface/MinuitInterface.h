#if !defined(MINUITINTERFACE_MINUITINTERFACE_H)
#define MINUITINTERFACE_MINUITINTERFACE_H
// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MinuitInterface
// 

// The best way to get help on MinuitInterface is to look up the
// Doxygen generated documentation under the "Automatic Software
// Documentation" web page. (see Cleo III Software web page). Then
// search for the MinuitInterface class. There's 
// lots of information there, including some example code.
//


/**\class MinuitInterface MinuitInterface.h MinuitInterface/MinuitInterface.h

 Description: An interface to the Minuit Minimization system. 

 Usage:

   MinuitInterface is the user's C++ interface to Minuit. 
   It is a singleton class so that the user cannot have conflicting
   Minuit calls.

   In order to use the MinuitInterface, you must have an Fcn
   object. All Fcn objects must inherit from MIFcn (see MIFcn
   documentation to learn how to write an Fcn). An example Fcn
   object, ExampleFcn (a simple line fitter), 
   is given in the MinuitInterface
   package. See  \ref annominuitex for more information.
   Here is an example of driving Minuit with this Fcn:

\code
   // Let's run a little fitter job with ExampleFcn
   // ExampleFcn is a simple line fitter
   // Make the points that we are going to fit
   float x[] = {10, 15, 20, 25, 30};
   float y[] = {1003, 1005, 1010, 1011, 1014};
   int npts = 5;

   // Make the Fcn
   ExampleFcn exFcn(5, x, y);

   // Get the instance of Minuit
   MinuitInterface* mi = MinuitInterface::instance();

   // Load the FCN into the MinuitInterface
   mi->loadFcn(exFcn);

   // Change the diagnostics level to print out more information
   mi->setDiagLevel(MinuitInterface::kIntermediate);

   // Tell it to go
   int migradOk = mi->runMigrad();

   if ( migradOk != 0 ) return; // Migrad failed

   // Run Minos too
   int minosOk = mi->runMinos();

   if ( minosOk != 0 ) return; // Minos failed

   // Get the Minuit status
   MIStats stats( mi->minuitStats() );

   // Report the information
   report(INFO, kFacilityString) << stats << endl;

   // Get the parameter information
   MIParameter constant( mi->parameter(ExampleFcn::kConstant) );
   MIParameter slope( mi->parameter(ExampleFcn::kSlope) );
   
   // Report the results
   report(INFO, kFacilityString) << constant << endl;
   report(INFO, kFacilityString) << slope << endl;

   // Now get the covariance matrix
   HepMatrix covMtrx( mi->covarianceMatrix() );

   // Print it out
   report(INFO, kFacilityString) << "Covariance Matrix:\n"
				 << covMtrx << endl;

   // Let's get the correlation between the constant and the slope
   // Note the use of the [] and the enum from ExampleFcn
   // (warning: using the () accessors on the matrix will give wrong results
   //  since () assume the first element is (1)(1). 
   //  The enums are always defined to start at zero. So using [], where the 
   //  first element is [0][0] is correct).
   double corr = covMtrx[ExampleFcn::kConstant][ExampleFcn::kSlope];
   
\endcode

*/
//
// Author:      Adam Lyon
// Created:     Thu Nov 18 15:23:06 EST 1999
// $Id: MinuitInterface.h,v 1.7 2000/07/13 15:21:43 lyon Exp $
//
// Revision history
//
// $Log: MinuitInterface.h,v $
// Revision 1.7  2000/07/13 15:21:43  lyon
// rumMigrad now returns command status ; flush fortran buffers so printouts appear correctly ; prescale minuit command error message
//
// Revision 1.6  2000/06/21 14:03:49  lyon
// Report to the user if there is a fit failure, instead of an assertion
//
// Revision 1.5  1999/12/14 05:15:06  lyon
// Renamed LeastSquaresFcn to ExampleFcn
//
// Revision 1.4  1999/12/12 21:12:37  lyon
// Added extensive Doxygen documentation
//
// Revision 1.3  1999/12/12 04:23:19  lyon
// Added Doxygeniated documenation
//
// Revision 1.2  1999/12/09 15:39:40  lyon
// Make variable names consistent -- oops
//
// Revision 1.1.1.1  1999/12/09 15:34:50  lyon
// Imported MinuitInterface sources
//

// system include files

#include "Experiment/fortran_types.h"
#include "STLUtility/fwd_vector.h"

#include "MinuitInterface/MIFcn.h"
#include "MinuitInterface/MIParameter.h"
#include "MinuitInterface/MIStats.h"
#include "CLHEP/Matrix/Matrix.h"

// user include files

// forward declarations

class MinuitInterface
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      //! Minuit Diagnostics level (same as Minuit print level)
      enum diagLevels { 
	/*! No output [default] */       kMute = -1, 
	/*! Minimal output */            kQuiet, 
        /*! Regular Minuit output */     kNormal, 
	/*! Show intermediate results */ kIntermediate,
	/*! Maximal diagnostic output */ kMax
      };

      // ---------- Constructors and destructor ----------------

      //! Destructor
      virtual ~MinuitInterface();

      // ---------- member functions ---------------------------
  
      //! Tell Minuit what Fcn to use (resets Minuit)
      void loadFcn(MIFcn& theFcn);

      //! Changes the start value for a parameter (and the current value)
      void changeParameterStart(unsigned int paramIndex, double startValue);

      //! Change the step size or resolution of a parameter
      void changeParameterStep(unsigned int paramIndex, double step);

      //! Change the upper and lower limits of a parameter
      void changeParameterLimits(unsigned int paramIndex, 
				 double low, double high);

      //! Fix the parameter's value to its start value
      void fixParameter(unsigned int paramIndex);
  
      //! Allow this parameter to be fit
      void releaseParameter(unsigned int paramIndex);
      
      //! Run MiGrad
      int runMigrad();
  
      //! Run Minos
      int runMinos();

      //! Go into Minuit interactive mode
      void interact();

      //! Set the diagnoistics level (default is mute)
      void setDiagLevel(int level);

      // ---------- const member functions ---------------------

      //! Return a pointer to the Fcn (for Minuit interactions)
      MIFcn* fcn() const;

      //! 	Get a parameter used in minimization. 
      MIParameter parameter(int paramIndex) const;

      //! Get the covariance matrix
      HepMatrix covarianceMatrix() const;

      //! Obtain status and statistics of Minuit
      MIStats minuitStats() const;
  
      // ---------- static member functions --------------------

      //! Get THE one and only instance of the Minuit interface
      static MinuitInterface* instance();

   protected:
      // ---------- protected member functions -----------------
      MinuitInterface();  // Do not allow user to create MinuitInterface

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MinuitInterface( const MinuitInterface& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MinuitInterface& operator=( const MinuitInterface& );
                // stop default

      // ---------- private member functions -------------------

      // Set the parameter information within Minuit
      void setMinuitParameter(unsigned int paramNum);

      // Common code for running a Minuit command without parameters
      // Returns the minuit return code (0 is ok)
      int runMinuitCommand(string comm);

      // Put results from a minimization/error seek into parameters
      void resultsToParameters();

      // Flush output buffer
      void flush() const;

      // ---------- private const member functions -------------

      // Check that an Fcn is loaded (assert if not)
      void checkForFcn() const;

      // Check that a parameter number is valid (assert if not)
      void checkParamNum(unsigned int paramNum) const;

      // ---------- data members -------------------------------
  
      // The current Fcn to use
      MIFcn* m_fcn;

      // Vector of parameters
      STL_VECTOR(MIParameter)* m_parameters;

      // The covariance matrix
      HepMatrix* m_covMatrix;

      // Diag level
      int m_diagLevel;

      // ---------- static data members ------------------------
      static MinuitInterface* m_instance;

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "MinuitInterface/Template/MinuitInterface.cc"
//#endif

#endif /* MINUITINTERFACE_MINUITINTERFACE_H */
