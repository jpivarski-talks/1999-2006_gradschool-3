#if !defined(MINUITINTERFACE_MISTATS_H)
#define MINUITINTERFACE_MISTATS_H
// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MIStats
//
/**\class MIStats MIStats.h MinuitInterface/MIStats.h

 Description: Status and statistical information from Minuit.

 Usage:
   Get an MIStats object from MinuitInterface by calling
   MinuitInterface::minuitStats(). See the example code in MinuitInterface
   documentation for more information. You should not have to call 
   MIStat's constructor yourself.

   Note that the << operator can handle MIStats objects for convenient 
   print outs. For example:
   \code
      MIStats stats = theMinuitInterface->minuitStats();
      report(INFO, kFacilityString) << stats;
   \endcode

*/
//
// Author:      Adam Lyon
// Created:     Mon Nov 22 10:08:13 EST 1999
// $Id: MIStats.h,v 1.3 2002/08/16 15:24:39 cleo3 Exp $
//
// Revision history
//
// $Log: MIStats.h,v $
// Revision 1.3  2002/08/16 15:24:39  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.2  1999/12/12 21:12:37  lyon
// Added extensive Doxygen documentation
//
// Revision 1.1.1.1  1999/12/09 15:34:50  lyon
// Imported MinuitInterface sources
//

// system include files
#include "C++Std/iostream.h"

// user include files

// forward declarations

class MIStats
{
      // ---------- friend classes and functions ---------------
      friend ostream& operator<<(ostream& aStream, 
				 const MIStats& stats);

   public:
      // ---------- constants, enums and typedefs --------------
      //! Enums for error matrix status
      enum errstat { kNotCalculated, kDiagonalApproximation, kForcedPosDef, 
		     kAccurate };
  
      // ---------- Constructors and destructor ----------------
      MIStats( double fmin, double fedm, double errdef, 
	       int npari, int nparx, int istat );

      virtual ~MIStats();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      //! Best function value found so far
      inline double functionMinimum() const;

      //! Esitmated vertical distance remaining to minimum
      inline double verticalDistanceToMinimum() const;

      //! Value of UP defining parameter uncertainties (???)
      inline double errdef() const;
  
      //! Number of currently variable parameters
      inline int numberVariableParameters() const;

      //! Number of parameters defined in the fit
      inline int numberDefinedParameters() const;

      //! Status of Minuit error matrix (returns MIStats::errstat enum)
      inline int errorMatrixStatus() const;
      
      //! Write out information to an output stream (used by <<)
      void dump(ostream& out) const;

    // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      const MIStats& operator=( const MIStats& rhs );     // stop default 

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double m_fmin;
      double m_fedm;
      double m_errdef;
      int m_npari;
      int m_nparx;
      int m_istat;

      // ---------- static data members ------------------------

};

// inline function definitions

double MIStats::functionMinimum() const
{ return m_fmin; }

double MIStats::verticalDistanceToMinimum() const
{ return m_fedm; }

double MIStats::errdef() const
{ return m_errdef; }

int MIStats::numberVariableParameters() const
{ return m_npari; }

int MIStats::numberDefinedParameters() const
{ return m_nparx; }

int MIStats::errorMatrixStatus() const
{ return m_istat; }

// Additional Doxygen documentation

/*! \enum MIStats::errstat
  Enum's specifying the status of the error matrix. Used by errorMatrixStatus().
*/

/*! \var MIStats::errstat MIStats::kNotCalculated
  Error matrix has not been calculated yet.
*/

/*! \var MIStats::errstat MIStats::kDiagonalApproximation
  The error matrix has been approximiated as diagonal.
*/

/*! \var MIStats::errstat MIStats::kForcedPosDef
  The error matrix has been forced to be positive definate.
*/

/*! \var MIStats::errstat MIStats::kAccurate
  The error matrix is accurate - all is well.
*/

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "MinuitInterfaceTemplateMIStats.cc"
//#endif

#endif  //MINUITINTERFACE_MISTATS_H
