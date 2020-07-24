#if !defined(DUALTRACKHELICES_DUALTRACKHELICES_H)
#define DUALTRACKHELICES_DUALTRACKHELICES_H
// -*- C++ -*-
//
// Package:     <DualTrackHelices>
// Module:      DualTrackHelices
// 
/**\class DualTrackHelices DualTrackHelices.h DualTrackHelices/DualTrackHelices.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Fri Feb 25 17:13:22 EST 2000
// $Id: DualTrackHelices.h,v 1.3 2001/08/31 20:35:40 mccann Exp $
//
// Revision history
//
// $Log: DualTrackHelices.h,v $
// Revision 1.3  2001/08/31 20:35:40  mccann
// added some diagnostics to DualTrackHelices object
//
// Revision 1.2  2001/08/16 16:44:15  mccann
// Added chisq() and ndof() of constraint to DualTrackHelices object for testing for final-state radiation
//
// Revision 1.1.1.1  2001/01/30 23:57:19  llh14
// imported DualTrackHelices sources
//

// system include files

// user include files

#include "Navigation/NavTrack.h"
#include "HelixIntersection/HIDualTrackConstraint.h"
#include "HelixIntersection/HIHelix.h"

#include "CommandPattern/Parameter.h"
#include "CLHEP/Matrix/Vector.h"

// forward declarations

class DualTrackHelices
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      typedef Count Identifier;
      
      // ---------- Constructors and destructor ----------------

      DualTrackHelices(const Identifier& identifier, double curvature, 
		      double phi0, double d0, double cotTheta, 
		      double z0, STL_VECTOR(double) errVector);

      DualTrackHelices(Identifier identifier,
		      const HIFitConstraint::Results& results,
		      const HIFitHelix& modifiedHelix);

      virtual ~DualTrackHelices();

      // ---------- member functions ---------------------------

      DABoolean operator==(const DualTrackHelices& rhs) const;
      DABoolean operator!=(const DualTrackHelices& rhs) const;

      // methods for binding to streams
//      virtual ActionBase::ActionResult beginRun( Frame& iFrame );

      // ---------- const member functions ---------------------

      const Identifier& identifier() const { return m_identifier;} 
      double curvature() const;
      double phi0() const;
      double d0() const;
      double cotTheta() const;
      double z0() const;
      double chisq() const;
      unsigned int ndof() const;

      //note, the error matrix is compressed into an STL_VECTOR
      STL_VECTOR(double) storeErrMatrix() const;
      HepSymMatrix errMatrix() const;

      HepSymMatrix trackParamErrMatrix() const;

      HepVector deltaTrackParam() const;

      HIHelix helix() const;


      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      //DualTrackHelices( const DualTrackHelices& );//default copy constructor ok

      // ---------- assignment operator(s) ---------------------
 
     const DualTrackHelices& operator=( const DualTrackHelices& ); // stop default

      // ---------- private member functions -------------------


      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

	Count m_arrayStartPoint; 
	Identifier m_identifier;
	STL_VECTOR(double) m_errVector;
        HepVector m_deltaTrackParam;
        HIFitConstraint::Results m_results;
	double m_curvature;
	double m_phi0;
	double m_d0;
	double m_cotTheta;
	double m_z0;
        double m_chisq;
        unsigned int m_ndof;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DualTrackHelices/Template/DualTrackHelices.cc"
//#endif

#endif /* DUALTRACKHELICES_DUALTRACKHELICES_H */



