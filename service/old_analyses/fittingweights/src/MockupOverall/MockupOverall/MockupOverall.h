// -*- C++ -*-
#if !defined(MOCKUPOVERALL_MOCKUPOVERALL_H)
#define MOCKUPOVERALL_MOCKUPOVERALL_H
//
// Package:     <MockupOverall>
// Module:      MockupOverall
//
/**\class MockupOverall MockupOverall.h MockupOverall/MockupOverall.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Thu Feb 28 11:50:25 EST 2002
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

class MockupOverall : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      MockupOverall( void );                      // anal1 
      virtual ~MockupOverall();                   // anal5 

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
      MockupOverall( const MockupOverall& );

      // ------------ assignment operator(s) ---------------------
      const MockupOverall& operator=( const MockupOverall& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (MockupOverall::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------
      double get_absDrift( unsigned int layer, double drift );
      // ------------ data members -------------------------------

      DABoolean m_layer_info_valid;
      int m_layer_num_cells[47];
      double m_layer_radius[47];

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* MOCKUPOVERALL_MOCKUPOVERALL_H */
