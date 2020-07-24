// -*- C++ -*-
#if !defined(DRHIT_DRHIT_H)
#define DRHIT_DRHIT_H
//
// Package:     <DRHit>
// Module:      DRHit
//
/**\class DRHit DRHit.h DRHit/DRHit.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Thu Oct 11 13:56:57 EDT 2001
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

class DRHit : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      DRHit( void );                      // anal1 
      virtual ~DRHit();                   // anal5 

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
      DRHit( const DRHit& );

      // ------------ assignment operator(s) ---------------------
      const DRHit& operator=( const DRHit& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (DRHit::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* DRHIT_DRHIT_H */
