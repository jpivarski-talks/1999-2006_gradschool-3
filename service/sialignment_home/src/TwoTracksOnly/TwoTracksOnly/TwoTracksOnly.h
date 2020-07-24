// -*- C++ -*-
#if !defined(TWOTRACKSONLY_TWOTRACKSONLY_H)
#define TWOTRACKSONLY_TWOTRACKSONLY_H
//
// Package:     <TwoTracksOnly>
// Module:      TwoTracksOnly
//
/**\class TwoTracksOnly TwoTracksOnly.h TwoTracksOnly/TwoTracksOnly.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Fri Dec 14 15:33:35 EST 2001
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

// forward declarations

class TwoTracksOnly : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      TwoTracksOnly( void );                      // anal1 
      virtual ~TwoTracksOnly();                   // anal5 

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
      TwoTracksOnly( const TwoTracksOnly& );

      // ------------ assignment operator(s) ---------------------
      const TwoTracksOnly& operator=( const TwoTracksOnly& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (TwoTracksOnly::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* TWOTRACKSONLY_TWOTRACKSONLY_H */
