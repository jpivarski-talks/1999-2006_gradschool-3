// -*- C++ -*-
#if !defined(SELECTBHABHAS_SELECTBHABHAS_H)
#define SELECTBHABHAS_SELECTBHABHAS_H
//
// Package:     <SelectBhabhas>
// Module:      SelectBhabhas
//
/**\class SelectBhabhas SelectBhabhas.h SelectBhabhas/SelectBhabhas.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Jun  4 10:22:37 EDT 2003
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

class SelectBhabhas : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      SelectBhabhas( void );                      // anal1 
      virtual ~SelectBhabhas();                   // anal5 

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
      SelectBhabhas( const SelectBhabhas& );

      // ------------ assignment operator(s) ---------------------
      const SelectBhabhas& operator=( const SelectBhabhas& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (SelectBhabhas::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------
      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* SELECTBHABHAS_SELECTBHABHAS_H */
