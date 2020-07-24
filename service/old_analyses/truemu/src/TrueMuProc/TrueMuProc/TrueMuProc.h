// -*- C++ -*-
#if !defined(TRUEMUPROC_TRUEMUPROC_H)
#define TRUEMUPROC_TRUEMUPROC_H
//
// Package:     <TrueMuProc>
// Module:      TrueMuProc
//
/**\class TrueMuProc TrueMuProc.h TrueMuProc/TrueMuProc.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Thu Jun 28 12:22:20 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

// this is sizeof( unsigned int )
// but whatever it is, it must stay the same for all systems!!!
#define UNSIGNED_INT 4

// system include files
#include <stdio.h>
#include <stdlib.h>

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"
#include "CommandPattern/Parameter.h"

// forward declarations

class TrueMuProc : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      TrueMuProc( void );                      // anal1 
      virtual ~TrueMuProc();                   // anal5 

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
      TrueMuProc( const TrueMuProc& );

      // ------------ assignment operator(s) ---------------------
      const TrueMuProc& operator=( const TrueMuProc& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (TrueMuProc::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      DABoolean read_run( unsigned int runnum );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter< string > m_cutdir;
      unsigned int m_listsize;
      unsigned int m_index;
      unsigned int* m_listbuffer;
      unsigned int m_runnum;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* TRUEMUPROC_TRUEMUPROC_H */
