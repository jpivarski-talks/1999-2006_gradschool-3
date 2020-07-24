// -*- C++ -*-
#if !defined(WIREVAPORPROC2_WIREVAPORPROC2_H)
#define WIREVAPORPROC2_WIREVAPORPROC2_H
//
// Package:     <WireVaporProc2>
// Module:      WireVaporProc2
//
/**\class WireVaporProc2 WireVaporProc2.h WireVaporProc2/WireVaporProc2.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri Jul 18 13:42:05 EDT 2003
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

class WireVaporProc2 : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      WireVaporProc2( void );                      // anal1 
      virtual ~WireVaporProc2();                   // anal5 

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
      WireVaporProc2( const WireVaporProc2& );

      // ------------ assignment operator(s) ---------------------
      const WireVaporProc2& operator=( const WireVaporProc2& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (WireVaporProc2::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------
      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* WIREVAPORPROC2_WIREVAPORPROC2_H */
