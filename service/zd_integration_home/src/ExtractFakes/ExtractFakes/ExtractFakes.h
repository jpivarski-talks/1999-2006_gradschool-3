// -*- C++ -*-
#if !defined(EXTRACTFAKES_EXTRACTFAKES_H)
#define EXTRACTFAKES_EXTRACTFAKES_H
//
// Package:     <ExtractFakes>
// Module:      ExtractFakes
//
/**\class ExtractFakes ExtractFakes.h ExtractFakes/ExtractFakes.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri Jun 27 13:42:34 EDT 2003
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
#include "CommandPattern/Parameter.h"

// forward declarations

class ExtractFakes : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      ExtractFakes( void );                      // anal1 
      virtual ~ExtractFakes();                   // anal5 

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
      ExtractFakes( const ExtractFakes& );

      // ------------ assignment operator(s) ---------------------
      const ExtractFakes& operator=( const ExtractFakes& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (ExtractFakes::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter<DABoolean> m_explicitExtraction;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* EXTRACTFAKES_EXTRACTFAKES_H */
