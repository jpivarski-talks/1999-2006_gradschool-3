// -*- C++ -*-
#if !defined(ONLYHIGHCOTTHETA_ONLYHIGHCOTTHETA_H)
#define ONLYHIGHCOTTHETA_ONLYHIGHCOTTHETA_H
//
// Package:     <OnlyHighCotTheta>
// Module:      OnlyHighCotTheta
//
/**\class OnlyHighCotTheta OnlyHighCotTheta.h OnlyHighCotTheta/OnlyHighCotTheta.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Sep  9 11:54:08 EDT 2003
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

class OnlyHighCotTheta : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      OnlyHighCotTheta( void );                      // anal1 
      virtual ~OnlyHighCotTheta();                   // anal5 

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
      OnlyHighCotTheta( const OnlyHighCotTheta& );

      // ------------ assignment operator(s) ---------------------
      const OnlyHighCotTheta& operator=( const OnlyHighCotTheta& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (OnlyHighCotTheta::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------
      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* ONLYHIGHCOTTHETA_ONLYHIGHCOTTHETA_H */
