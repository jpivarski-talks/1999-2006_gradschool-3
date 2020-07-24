// -*- C++ -*-
#if !defined(LOOKFORBIGERRORMATRIXBUG_LOOKFORBIGERRORMATRIXBUG_H)
#define LOOKFORBIGERRORMATRIXBUG_LOOKFORBIGERRORMATRIXBUG_H
//
// Package:     <LookForBigErrorMatrixBug>
// Module:      LookForBigErrorMatrixBug
//
/**\class LookForBigErrorMatrixBug LookForBigErrorMatrixBug.h LookForBigErrorMatrixBug/LookForBigErrorMatrixBug.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon May 12 13:44:53 EDT 2003
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

class LookForBigErrorMatrixBug : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      LookForBigErrorMatrixBug( void );                      // anal1 
      virtual ~LookForBigErrorMatrixBug();                   // anal5 

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
      LookForBigErrorMatrixBug( const LookForBigErrorMatrixBug& );

      // ------------ assignment operator(s) ---------------------
      const LookForBigErrorMatrixBug& operator=( const LookForBigErrorMatrixBug& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (LookForBigErrorMatrixBug::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      int m_run;
      int m_bad;
      int m_bad2;
      int m_total;
      int m_total2;
      int m_real_total;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* LOOKFORBIGERRORMATRIXBUG_LOOKFORBIGERRORMATRIXBUG_H */
