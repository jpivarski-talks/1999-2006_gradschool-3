#if !defined(PACKAGE_HISTOGRAMRESIDUALS_H)
#define PACKAGE_HISTOGRAMRESIDUALS_H
// -*- C++ -*-
//
// Package:     <ChisqForSiAlignment>
// Module:      HistogramResiduals
// 
/**\class HistogramResiduals HistogramResiduals.h package/HistogramResiduals.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Fri Jun  9 11:58:21 EDT 2000
// $Id: HistogramResiduals.h,v 1.1.1.1 2001/02/02 18:03:29 llh14 Exp $
//
// Revision history
//
// $Log: HistogramResiduals.h,v $
// Revision 1.1.1.1  2001/02/02 18:03:29  llh14
// imported ChisqForSiAlignment
//

// system include files

// user include files

#include "ChisqForSiAlignment/ChisqForSiAlignment.h"

#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
#include "FrameAccess/FATable.h"

#include "HelixIntersection/HIIntersectionSurface.h"

// forward declarations

#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "BDLSVclient/DBSVAlignmentGlobal.hh"
#include "BDLSVclient/DBSVAlignmentLayer.hh"
#include "BDLSVclient/DBSVAlignmentLadder.hh"

class HistogramResiduals : public ChisqForSiAlignment
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum ComponentType {kBulk, kClam, kLayer, kLadder, kWafer};

      // ---------- Constructors and destructor ----------------
      HistogramResiduals(const FIFrameIterator& iBegin,
			 const FIFrameIterator& iEnd,
			 CLEOConstantsModifiable<DBSVAlignmentGlobal>&
			 SVGlobalConstants,
			 CLEOConstantsModifiable<DBSVAlignmentLayer>&
			 SVLayerConstants,
			 CLEOConstantsModifiable<DBSVAlignmentLadder>&
			 SVLadderConstants,
			 ComponentType component,
			 Count componentNum=1);

      virtual ~HistogramResiduals();

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
      HistogramResiduals( const HistogramResiduals& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HistogramResiduals& operator=( const HistogramResiduals& ); // stop default

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
//# include "package/Template/HistogramResiduals.cc"
//#endif

#endif /* PACKAGE_HISTOGRAMRESIDUALS_H */
