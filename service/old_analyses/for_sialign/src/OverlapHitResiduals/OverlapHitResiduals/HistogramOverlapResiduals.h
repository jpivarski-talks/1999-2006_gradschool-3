#if !defined(PACKAGE_HISTOGRAMOVERLAPRESIDUALS_H)
#define PACKAGE_HISTOGRAMOVERLAPRESIDUALS_H
// -*- C++ -*-
//
// Package:     <OverlapHitResiduals>
// Module:      HistogramOverlapResiduals
// 
/**\class HistogramOverlapResiduals HistogramOverlapResiduals.h package/HistogramOverlapResiduals.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Fri Jun  9 11:58:21 EDT 2000
// $Id: HistogramOverlapResiduals.h,v 1.1.1.1 2001/03/06 22:55:18 llh14 Exp $
//
// Revision history
//
// $Log: HistogramOverlapResiduals.h,v $
// Revision 1.1.1.1  2001/03/06 22:55:18  llh14
// imported OverlapHitResiduals
//

// system include files

// user include files

#include "OverlapHitResiduals/OverlapHitResiduals.h"

#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
#include "FrameAccess/FATable.h"

#include "HelixIntersection/HIIntersectionSurface.h"

// forward declarations

#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "BDLSVclient/DBSVAlignmentGlobal.hh"
#include "BDLSVclient/DBSVAlignmentLayer.hh"
#include "BDLSVclient/DBSVAlignmentLadder.hh"

class HistogramOverlapResiduals : public OverlapHitResiduals
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum ComponentType {kBulk, kClam, kLayer, kLadder, kWafer};

      // ---------- Constructors and destructor ----------------
      HistogramOverlapResiduals(const FIFrameIterator& iBegin,
			 const FIFrameIterator& iEnd,
			 CLEOConstantsModifiable<DBSVAlignmentGlobal>&
			 SVGlobalConstants,
			 CLEOConstantsModifiable<DBSVAlignmentLayer>&
			 SVLayerConstants,
			 CLEOConstantsModifiable<DBSVAlignmentLadder>&
			 SVLadderConstants,
			 ComponentType component, Count componentNum=1
			 );

      virtual ~HistogramOverlapResiduals();

      // ---------- member functions ---------------------------

      inline virtual double iterate(double* values) {return 0.0;};
      inline virtual DABoolean selectComponent(SmallCount sensorNumber,
					       FAItem<ASiStore>& iStore);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HistogramOverlapResiduals( const HistogramOverlapResiduals& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HistogramOverlapResiduals& operator=( const HistogramOverlapResiduals& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      CLEOConstantsModifiable<DBSVAlignmentGlobal>& m_SVGlobalConstants;
      CLEOConstantsModifiable<DBSVAlignmentLayer>& m_SVLayerConstants;	
      CLEOConstantsModifiable<DBSVAlignmentLadder>& m_SVLadderConstants;

      ComponentType m_component;
      Count m_componentNum;
								     
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/HistogramOverlapResiduals.cc"
//#endif

#endif /* PACKAGE_HISTOGRAMOVERLAPRESIDUALS_H */
