// -*- C++ -*-
#if !defined(REQUIREKAON_REQUIREKAON_H)
#define REQUIREKAON_REQUIREKAON_H
//
// Package:     <RequireKaon>
// Module:      RequireKaon
//
/**\class RequireKaon RequireKaon.h RequireKaon/RequireKaon.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Mar 20 14:49:46 EST 2003
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

class RequireKaon : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      RequireKaon( void );                      // anal1 
      virtual ~RequireKaon();                   // anal5 

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
      RequireKaon( const RequireKaon& );

      // ------------ assignment operator(s) ---------------------
      const RequireKaon& operator=( const RequireKaon& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (RequireKaon::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------
      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* REQUIREKAON_REQUIREKAON_H */
