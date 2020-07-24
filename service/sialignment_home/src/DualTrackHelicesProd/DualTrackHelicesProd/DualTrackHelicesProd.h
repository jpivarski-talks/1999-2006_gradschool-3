#if !defined(DUALTRACKHELICESPROD_DUALTRACKHELICESPROD_H)
#define DUALTRACKHELICESPROD_DUALTRACKHELICESPROD_H
// -*- C++ -*-
//
// Package:     <DualTrackHelicesProd>
// Module:      DualTrackHelicesProd
//
/**\class DualTrackHelicesProd DualTrackHelicesProd.h DualTrackHelicesProd/DualTrackHelicesProd.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Fri Jul  7 14:15:27 EDT 2000
// $Id: DualTrackHelicesProd.h,v 1.1.1.1 2001/02/01 17:40:13 llh14 Exp $
//
// Revision history
//
// $Log: DualTrackHelicesProd.h,v $
// Revision 1.1.1.1  2001/02/01 17:40:13  llh14
// imported DualTrackHelicesProd
//
//

// system include files

// user include files
#include "Processor/Producer.h"
#include "HistogramInterface/HistogramPackage.h"
#include "CommandPattern/Parameter.h"

// forward declarations

class DualTrackHelicesProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      DualTrackHelicesProd( void  );                      // anal1 
      virtual ~DualTrackHelicesProd();                   // anal5 

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      inline DABoolean useAllHits() const {return m_UseAllHits.value(); }
      inline DABoolean useLay1() const { return m_UseLay1.value(); }
      inline DABoolean useLay2() const { return m_UseLay2.value(); }
      inline DABoolean useLay3() const { return m_UseLay3.value(); }
      inline DABoolean useLay4() const { return m_UseLay4.value(); }
 						
      inline double svrWeight() const { return m_svrWeight.value(); }
      inline double svzWeight() const { return m_svzWeight.value(); }
       
      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // ------------ const member functions ----------------------

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      DualTrackHelicesProd( const DualTrackHelicesProd& );

      // ------------ assignment operator(s) ----------------------
      const DualTrackHelicesProd& operator=( const DualTrackHelicesProd& );

      // ------------ private member functions --------------------

      ProxyBase* makeDualTrackHelicesProxy();

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------

	Parameter < DABoolean > m_UseAllHits;		
	Parameter < DABoolean > m_UseLay1;		
	Parameter < DABoolean > m_UseLay2;		
	Parameter < DABoolean > m_UseLay3;		
	Parameter < DABoolean > m_UseLay4;

	Parameter < double > m_svrWeight;
	Parameter < double > m_svzWeight;

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* DUALTRACKHELICESPROD_DUALTRACKHELICESPROD_H */
