// -*- C++ -*-
#if !defined(POINTTOERROR_POINTTOERROR_H)
#define POINTTOERROR_POINTTOERROR_H
//
// Package:     <PointToError>
// Module:      PointToError
//
/**\class PointToError PointToError.h PointToError/PointToError.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Dec 26 17:01:41 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"
#include "HbookHistogram/HbookNtuple.h"

// forward declarations

class PointToError : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
	 kCurv, kPhi0, kD0, kCotTheta, kZ0,
	 kXCurv, kXPhi0, kXD0, kXCotTheta, kXZ0,
	 kECurv, kEPhi0, kED0, kECotTheta, kEZ0,
	 kChiSquare, kDOF, kHitsExp, kHits, kFit, kFitAbort,
	 kXChiSquare, kXDOF, kXHitsExp, kXHits, kXFit, kXFitAbort,
	 kSVRave, kSVRrms,
	 kSVZave, kSVZrms,
	 kAXave, kAXrms,
	 kSTave, kSTrms,
	 kCATHave, kCATHrms,
	 kRICHanal,

	 kEntries
      };

      // ------------ Constructors and destructor ----------------
      PointToError( void );                      // anal1 
      virtual ~PointToError();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult event( Frame& iFrame );
      //virtual ActionBase::ActionResult beginRun( Frame& iFrame);
      //virtual ActionBase::ActionResult endRun( Frame& iFrame);

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      PointToError( const PointToError& );

      // ------------ assignment operator(s) ---------------------
      const PointToError& operator=( const PointToError& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (PointToError::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HINtuple* m_ntuple;
      HIHist1D* m_healthy_z[1500];
      HIHist1D* m_sick_z[1500];

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* POINTTOERROR_POINTTOERROR_H */
