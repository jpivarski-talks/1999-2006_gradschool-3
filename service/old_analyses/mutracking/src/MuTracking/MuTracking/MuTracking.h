// -*- C++ -*-
#if !defined(MUTRACKING_MUTRACKING_H)
#define MUTRACKING_MUTRACKING_H
//
// Package:     <MuTracking>
// Module:      MuTracking
//
/**\class MuTracking MuTracking.h MuTracking/MuTracking.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Fri Oct 26 13:25:51 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

#include <fstream>
#include <iostream>

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"
#include "CommandPattern/Parameter.h"
#include "MuTracking/MomentumFitFunc.h"

// forward declarations

class MuTracking : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      MuTracking( void );                      // anal1 
      virtual ~MuTracking();                   // anal5 

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

      void finish_run( unsigned int run, ofstream& log, ofstream& kumac );

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      MuTracking( const MuTracking& );

      // ------------ assignment operator(s) ---------------------
      const MuTracking& operator=( const MuTracking& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (MuTracking::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter< string > m_logName;
      Parameter< string > m_kumacName;
      HIHistoManager* m_hm;
      HIHist1D* m_working;
      ofstream* m_log;
      ofstream* m_kumac;
      unsigned int m_current_run;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* MUTRACKING_MUTRACKING_H */
