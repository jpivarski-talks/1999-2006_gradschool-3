// -*- C++ -*-
#if !defined(DRALIGNMENTMOD_DRALIGNMENTMOD_H)
#define DRALIGNMENTMOD_DRALIGNMENTMOD_H
//
// Package:     DRAlignmentMod
// Module:      DRAlignmentMod
//
/**\class DRAlignmentMod DRAlignmentMod.h DRAlignmentMod/DRAlignmentMod.h
 
 Description: see DRAlignmentMod.cc

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon Dec  8 11:50:49 EST 2003
// $Id: 
//
// Revision history
//

// system include files

// user include files
#include "CommandPattern/Module.h"
#include "CommandPattern/Parameter.h"
#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
#include "DRAlignmentMod/DRAlignmentFcn.h"

// forward declarations

class DRAlignmentMod : public FrameIteratorModuleBase
{
      // ---------- friend classes and functions ---------------
      friend class DRAlignmentFcn;

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DRAlignmentMod();
      virtual ~DRAlignmentMod();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      int cake_ring(int layer) const;
      bool full_moon() const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      ///override this function to do the actual iterations
      virtual void iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd );

      CLEOConstantsModifiable<DBDRGeomAlignment> m_geometry;

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DRAlignmentMod( const DRAlignmentMod& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DRAlignmentMod& operator=( const DRAlignmentMod& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      FIHolder<CLEOConstants<DBDRGeomAlignment> > m_geometry_holder;
      bool m_first_time;

      Parameter<string> m_geomName;
      Parameter<unsigned int> m_ring;
      Parameter<double> m_minDrift;
      Parameter<double> m_maxDrift;
      Parameter<DABoolean> m_showPlots;
      Parameter<DABoolean> m_interactive;
      Parameter<DABoolean> m_fakeDriftFunction;
      Parameter<double> m_crossingangle_x;
      Parameter<double> m_crossingangle_y;
      Parameter<double> m_asymmetric_beamz;

      DRAlignmentFcn m_fcn;
      MinuitInterface* m_minuit;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DRAlignmentMod/Template/DRAlignmentMod.cc"
//#endif

#endif /* DRALIGNMENTMOD_DRALIGNMENTMOD_H */
