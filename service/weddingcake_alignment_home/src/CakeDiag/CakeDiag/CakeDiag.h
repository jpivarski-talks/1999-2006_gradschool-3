// -*- C++ -*-
#if !defined(CAKEDIAG_CAKEDIAG_H)
#define CAKEDIAG_CAKEDIAG_H
//
// Package:     <CakeDiag>
// Module:      CakeDiag
//
/**\class CakeDiag CakeDiag.h CakeDiag/CakeDiag.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Thu May  2 10:42:29 EDT 2002
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
#include "Navigation/NavTrack.h"
#include "CLHEP/Vector/ThreeVector.h"

// forward declarations

class CakeDiag : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      CakeDiag( void );                      // anal1 
      virtual ~CakeDiag();                   // anal5 

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
      CakeDiag( const CakeDiag& );

      // ------------ assignment operator(s) ---------------------
      const CakeDiag& operator=( const CakeDiag& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (CakeDiag::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HINtuple* m_ntuple;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* CAKEDIAG_CAKEDIAG_H */
