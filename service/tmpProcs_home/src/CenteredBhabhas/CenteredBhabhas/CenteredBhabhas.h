// -*- C++ -*-
#if !defined(CENTEREDBHABHAS_CENTEREDBHABHAS_H)
#define CENTEREDBHABHAS_CENTEREDBHABHAS_H
//
// Package:     <CenteredBhabhas>
// Module:      CenteredBhabhas
//
/**\class CenteredBhabhas CenteredBhabhas.h CenteredBhabhas/CenteredBhabhas.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon Mar 22 05:24:06 EST 2004
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

class CenteredBhabhas : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      CenteredBhabhas( void );                      // anal1 
      virtual ~CenteredBhabhas();                   // anal5 

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
      CenteredBhabhas( const CenteredBhabhas& );

      // ------------ assignment operator(s) ---------------------
      const CenteredBhabhas& operator=( const CenteredBhabhas& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (CenteredBhabhas::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------
      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* CENTEREDBHABHAS_CENTEREDBHABHAS_H */
