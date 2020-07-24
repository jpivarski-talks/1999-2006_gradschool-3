// -*- C++ -*-
#if !defined(D0MISSTHETADEPENDENCE_D0MISSTHETADEPENDENCE_H)
#define D0MISSTHETADEPENDENCE_D0MISSTHETADEPENDENCE_H
//
// Package:     <D0MissThetaDependence>
// Module:      D0MissThetaDependence
//
/**\class D0MissThetaDependence D0MissThetaDependence.h D0MissThetaDependence/D0MissThetaDependence.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri Mar 21 15:52:00 EST 2003
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
#include "HistogramInterface/HINtuple.h"

// forward declarations

class D0MissThetaDependence : public Processor
{
      // ------------ friend classses and functions --------------

      enum {
	 kPlusPPerp,
	 kPlusPZ,
	 kPlusD0,
	 kPlusPhi0,
	 kPlusCotTheta,
	 kPlusZ0,

	 kMinusPPerp,
	 kMinusPZ,
	 kMinusD0,
	 kMinusPhi0,
	 kMinusCotTheta,
	 kMinusZ0,

	 kPX,
	 kPY,
	 kPPerp,
	 kPZ,
	 kD0,
	 kPhi0,
	 kCotTheta,
	 kZ0,

	 kNumEntries
      };

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      D0MissThetaDependence( void );                      // anal1 
      virtual ~D0MissThetaDependence();                   // anal5 

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
      //virtual ActionBase::ActionResult geometry( Frame& iFrame);
      //virtual ActionBase::ActionResult hardware( Frame& iFrame);
      //virtual ActionBase::ActionResult user( Frame& iFrame);

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      D0MissThetaDependence( const D0MissThetaDependence& );

      // ------------ assignment operator(s) ---------------------
      const D0MissThetaDependence& operator=( const D0MissThetaDependence& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (D0MissThetaDependence::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HINtuple* m_ntuple;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* D0MISSTHETADEPENDENCE_D0MISSTHETADEPENDENCE_H */
