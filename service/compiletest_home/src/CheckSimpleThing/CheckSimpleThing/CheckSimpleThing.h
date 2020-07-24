// -*- C++ -*-
#if !defined(CHECKSIMPLETHING_CHECKSIMPLETHING_H)
#define CHECKSIMPLETHING_CHECKSIMPLETHING_H
//
// Package:     <CheckSimpleThing>
// Module:      CheckSimpleThing
//
/**\class CheckSimpleThing CheckSimpleThing.h CheckSimpleThing/CheckSimpleThing.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Oct 23 15:47:01 EDT 2003
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
#include "HistogramInterface/HINtuple.h"

// forward declarations

class CheckSimpleThing : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
	 kCurvature,
	 kCotTheta,
	 kChiSquare,
	 kDOF,
	 kPPerp,
	 kResidual,

	 kNumEntries
      };

      // ------------ Constructors and destructor ----------------
      CheckSimpleThing( void );                      // anal1 
      virtual ~CheckSimpleThing();                   // anal5 

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
      CheckSimpleThing( const CheckSimpleThing& );

      // ------------ assignment operator(s) ---------------------
      const CheckSimpleThing& operator=( const CheckSimpleThing& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (CheckSimpleThing::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HINtuple* m_ntuple;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* CHECKSIMPLETHING_CHECKSIMPLETHING_H */
