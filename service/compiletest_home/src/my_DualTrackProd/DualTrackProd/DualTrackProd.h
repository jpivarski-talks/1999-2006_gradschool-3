// -*- C++ -*-
#if !defined(DUALTRACKPROD_DUALTRACKPROD_H)
#define DUALTRACKPROD_DUALTRACKPROD_H
//
// Package:     <DualTrackProd>
// Module:      DualTrackProd
//
/**\class DualTrackProd DualTrackProd.h DualTrackProd/DualTrackProd.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Dec 10 11:04:40 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "DualTrackProd/DualTrackProxy.h"
#include "Processor/Producer.h"
#include "HistogramInterface/HistogramPackage.h"
#include "CommandPattern/Parameter.h"
#include "CleoDB/DBCandidate.h"

// forward declarations

class DualTrackProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      DualTrackProd( void );                      // anal1 
      virtual ~DualTrackProd();                   // anal5 

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  
      
      // ------------ const member functions ----------------------

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      DualTrackProd( const DualTrackProd& );

      // ------------ assignment operator(s) ----------------------
      const DualTrackProd& operator=( const DualTrackProd& );

      // ------------ private member functions --------------------
      ProxyBase* makeDualTrackProxy();

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------

      Parameter<string> m_massHypothesis;
      Parameter<double> m_virtualPhotonPx;
      Parameter<double> m_virtualPhotonPy;
      Parameter<double> m_virtualPhotonPz;
      Parameter<DABoolean> m_pointConstraint;
      Parameter<DABoolean> m_ptConstraint;
      Parameter<DABoolean> m_pzConstraint;
      Parameter<DABoolean> m_printOutP;

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* DUALTRACKPROD_DUALTRACKPROD_H */
