// -*- C++ -*-
#if !defined(MAKETRUEMU_MAKETRUEMU_H)
#define MAKETRUEMU_MAKETRUEMU_H
//
// Package:     <MakeTrueMu>
// Module:      MakeTrueMu
//
/**\class MakeTrueMu MakeTrueMu.h MakeTrueMu/MakeTrueMu.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Thu Jun 21 12:26:00 EDT 2001
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <vector>
#include <exception.h>
#include <stdexcept>

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"
#include "CommandPattern/Parameter.h"

// forward declarations

class MakeTrueMu : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      MakeTrueMu( void );                      // anal1 
      virtual ~MakeTrueMu();                   // anal5 

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
      MakeTrueMu( const MakeTrueMu& );

      // ------------ assignment operator(s) ---------------------
      const MakeTrueMu& operator=( const MakeTrueMu& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (MakeTrueMu::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      DABoolean write_run( const char* iDir, int iList_size,
			   vector< unsigned int >::const_iterator iList_begin,
			   vector< unsigned int >::const_iterator iList_end );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter< string > m_dir_simplecut;
      Parameter< double > m_d0cut;
      Parameter< double > m_z0cut;

      vector< unsigned int > m_list_simplecut;

      HIHist2D* m_all_d0vphi0;
      HIHist1D* m_all_z0;
      HIHist1D* m_all_phi0;

      HIHist2D* m_simplecut_d0vphi0;
      HIHist1D* m_simplecut_z0;
      HIHist1D* m_simplecut_phi0;

      HIHist1D* m_momentum;

      unsigned int m_prevrun;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* MAKETRUEMU_MAKETRUEMU_H */
