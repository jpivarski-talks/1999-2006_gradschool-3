// -*- C++ -*-
//
// Package:     <OverlapHitResiduals>
// Module:      HistogramOverlapResiduals
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Fri Jun  9 11:58:06 EDT 2000
// $Id: HistogramOverlapResiduals.cc,v 1.1.1.1 2001/03/06 22:55:18 llh14 Exp $
//
// Revision history
//
// $Log: HistogramOverlapResiduals.cc,v $
// Revision 1.1.1.1  2001/03/06 22:55:18  llh14
// imported OverlapHitResiduals
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "OverlapHitResiduals/HistogramOverlapResiduals.h"
#include "Experiment/units.h"

#include "DataHandler/Frame.h"

#include "SiGeom/SiAlignedGeometryProxy.h"
#include "ASiStorePro/ASiStore.h"

#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAConstants.h"
#include "FrameAccess/extract.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "package.HistogramOverlapResiduals" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HistogramOverlapResiduals.cc,v 1.1.1.1 2001/03/06 22:55:18 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HistogramOverlapResiduals::HistogramOverlapResiduals(const FIFrameIterator& iBegin, 
				       const FIFrameIterator& iEnd,       
				 CLEOConstantsModifiable<DBSVAlignmentGlobal>&
				       SVGlobalConstants, 
				 CLEOConstantsModifiable<DBSVAlignmentLayer>&
                                       SVLayerConstants,
				 CLEOConstantsModifiable<DBSVAlignmentLadder>&
                                       SVLadderConstants, 
				 ComponentType component, Count componentNum )

 :  
   m_SVGlobalConstants(SVGlobalConstants),
   m_SVLayerConstants(SVLayerConstants),
   m_SVLadderConstants(SVLadderConstants),
   m_component(component),
   m_componentNum(componentNum)

{

}


// HistogramOverlapResiduals::HistogramOverlapResiduals( const HistogramOverlapResiduals& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HistogramOverlapResiduals::~HistogramOverlapResiduals()
{
}

//
// assignment operators
//
// const HistogramOverlapResiduals& HistogramOverlapResiduals::operator=( const HistogramOverlapResiduals& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

DABoolean
HistogramOverlapResiduals::selectComponent(SmallCount sensorNumber, 
				    FAItem<ASiStore>& iStore)

{
   // If component is bulk calculate all residuals except those on layer 3
   if(m_component == kBulk) 
      //&&
      //iStore->layerForSensor(sensorNumber) != 3)   
   { return true; }

   // Code to select hits on the proper ladder
   if( m_component == kLadder &&
       iStore->ladderForSensor(sensorNumber) == m_componentNum )  
   {    

//	 cout << "\n\nSelected Ladder!!!" 
//	      << iStore->ladderForSensor(sensorNumber)
//	      << endl;

      return true; 
   
   }

   // code to select hits on the proper wafer
   if( m_component == kWafer && sensorNumber == m_componentNum)  
   {    
      cout << "selecting wafer !!" 
	   << sensorNumber
	   << endl;

	 return true; 
   }




   return false;
}
//
// const member functions
//

//
// static member functions
//
