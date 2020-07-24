// -*- C++ -*-
//
// Package:     <ChisqForSiAlignment>
// Module:      ChisqForLayer
// 
// Description: <one line class summary>
// Feeds minuit the correct constants for one silicon Layer
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Sat Jun  3 09:59:34 EDT 2000
// $Id: ChisqForLayer.cc,v 1.2 2001/02/06 20:31:34 llh14 Exp $
//
// Revision history
//
// $Log: ChisqForLayer.cc,v $
// Revision 1.2  2001/02/06 20:31:34  llh14
// Added dof, and MaxEvent parameters
//
// Revision 1.1.1.1  2001/02/02 18:03:29  llh14
// imported ChisqForSiAlignment
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
#include "Experiment/units.h"
#include "ChisqForSiAlignment/ChisqForLayer.h"
#include "ChisqForSiAlignment/ChisqForSiAlignment.h"
#include "STLUtility/fwd_vector.h"

#include "DataHandler/Frame.h"

#include "SiGeom/SiAlignedGeometryProxy.h"
#include "ASiStorePro/ASiStore.h"

#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAConstants.h"

#include "MinuitInterface/MinuitInterface.h"
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

static const char* const kFacilityString = "package.ChisqForLayer" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqForLayer.cc,v 1.2 2001/02/06 20:31:34 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ChisqForLayer::ChisqForLayer(const FIFrameIterator& iBegin, 
			     const FIFrameIterator& iEnd,       
			     CLEOConstantsModifiable<DBSVAlignmentLayer>&  
			     SVLayerConstants, float normRphi, float normZ, 
			     float cutRphi, float cutZ, long int maxEvents,
			     Count layer, DABoolean varyX, DABoolean varyY, 
			     DABoolean varyZ, DABoolean varyPhiX, 
			     DABoolean varyPhiY, DABoolean varyPhiZ) :  
   m_maxEvents(maxEvents), 
   m_iBegin(iBegin), 
   m_iEnd(iEnd), 
   m_SVLayerConstants(SVLayerConstants ),
   m_normRphi(normRphi),
   m_normZ(normZ),
   m_cutRphi(cutRphi),
   m_cutZ(cutZ),
   m_layer(layer),   
   m_varyX(varyX),
   m_varyY(varyY),
   m_varyZ(varyZ),
   m_varyPhiX(varyPhiX),
   m_varyPhiY(varyPhiY),
   m_varyPhiZ(varyPhiZ)

{
   // prompt for the component number
   report(INFO, kFacilityString) 
      << "\n the number is:  " << m_layer << endl;

   if ( !( 0 < m_layer <= 4) )
   { report( ERROR, kFacilityString )
      <<"Invalid layer number passed to ChisqForLayer!!" << endl;
   }

   //Make initial parameters for Minuit
   if(m_varyX)
   {
      addInitialParameter("Delta X", m_SVLayerConstants[m_layer-1].get_deltaX(), 
			  1*k_um, -k_cm, k_cm);
   }

   if(m_varyY)
   {
      addInitialParameter("Delta Y", m_SVLayerConstants[m_layer-1].get_deltaY(), 
			  1*k_um, -k_cm, k_cm);
   }

   if(m_varyZ)
   {
      addInitialParameter("Delta Z", m_SVLayerConstants[m_layer-1].get_deltaZ(),50*k_um, -k_cm, k_cm);
   }

   if(m_varyPhiX)
   {
      addInitialParameter("Delta PhiX", m_SVLayerConstants[m_layer-1].get_phiX(), .000001, -0.1, 0.1);
   }

   if(m_varyPhiY)
   {
      addInitialParameter("Delta PhiY", m_SVLayerConstants[m_layer-1].get_phiY(), .000001, -0.1, 0.1);
   }

   if(m_varyPhiZ)
   {
      addInitialParameter("Delta PhiZ", m_SVLayerConstants[m_layer-1].get_phiZ(), 
			  .000001, -0.1, 0.1);
   }

}

// ChisqForLayer::ChisqForLayer( const ChisqForLayer& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ChisqForLayer::~ChisqForLayer()
{
}

//
// assignment operators
//
// const ChisqForLayer& ChisqForLayer::operator=( const ChisqForLayer& rhs )
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

double
ChisqForLayer::iterate(double* values)
{
   Count useParam[kDoF];
   useParam[kDeltaX] = ( m_varyX ? 0 : 1 );
   useParam[kDeltaY] = ( m_varyY ? 0 : 1 );
   useParam[kDeltaZ] = ( m_varyZ ? 0 : 1 );
   useParam[kPhiX] = ( m_varyPhiX ? 0 : 1 );
   useParam[kPhiY] = ( m_varyPhiY ? 0 : 1 );
   useParam[kPhiZ] = ( m_varyPhiZ ? 0 : 1 );

   //Now Minuit controls the iterations

   if(m_varyX)
   {
      double deltaX = values[kDeltaX];
      m_SVLayerConstants[m_layer-1].set_deltaX( deltaX );
   }

   if(m_varyY)   
   {
      Count indxY = kDeltaY - useParam[kDeltaX] ;

      double deltaY = values[indxY];
      m_SVLayerConstants[m_layer-1].set_deltaY( deltaY );
   }

   if(m_varyZ)   
   {
      Count indxZ = kDeltaZ - useParam[kDeltaX] - useParam[kDeltaY];
      
      double deltaZ = values[indxZ];
      m_SVLayerConstants[m_layer-1].set_deltaZ( deltaZ );
   }

   if(m_varyPhiX)   
   {
      Count indxPhiX = kPhiX - useParam[kDeltaX] - useParam[kDeltaY]
	                - useParam[kDeltaZ];
      double phiX = values[indxPhiX];
      m_SVLayerConstants[m_layer-1].set_phiX( phiX );
   }

   if(m_varyPhiY)
   {
      Count indxPhiY = kPhiY - useParam[kDeltaX] - useParam[kDeltaY]
	               - useParam[kDeltaZ] - useParam[kPhiX];
      double phiY = values[indxPhiY];
      m_SVLayerConstants[m_layer-1].set_phiY( phiY );
   }

   if(m_varyPhiZ)
   {
      Count indxPhiZ = kPhiZ - useParam[kDeltaX] - useParam[kDeltaY]
	               - useParam[kDeltaZ] - useParam[kPhiX]
	               - useParam[kPhiY];
      double phiZ = values[indxPhiZ];
      m_SVLayerConstants[m_layer-1].set_phiZ( phiZ );
  
   }

   report(INFO, kFacilityString)
      << "/n Value for delta x is: " 
      << m_SVLayerConstants[m_layer-1].get_deltaX() 
      << "/n Value for delta y is: " 
      << m_SVLayerConstants[m_layer-1].get_deltaY() 
      << "/n Value for delta z is: " 
      << m_SVLayerConstants[m_layer-1].get_deltaZ() 
      << "/n Value for delta phix is: " 
      << m_SVLayerConstants[m_layer-1].get_phiX() 
      << "/n Value for delta phiy is: " 
      << m_SVLayerConstants[m_layer-1].get_phiY() 
      << "/n Value for delta phiz is: " 
      << m_SVLayerConstants[m_layer-1].get_phiZ() 
      << endl;

   // initialize chi square value
   double chisq = 0.0;
   Count eventCtr = 0;

   calcResiduals(m_iBegin, m_iEnd, eventCtr, chisq, m_maxEvents,
		 m_normRphi, m_normZ, m_cutRphi, m_cutZ);

   //Place here anything to be done at the end 
   report(INFO, kFacilityString) 
   << "The number of events:  " << eventCtr << endl;

   //Print diagnostics?
   if ( diagLevel() >= MinuitInterface::kIntermediate )
   {

      //report( INFO, kFacilityString) 
      cout.precision(10);
      cout << "chisq = " << chisq 
	   << endl;

      cout.precision(6);
   }

   return chisq;
}

//////////////////////////////////////////////////////////////////////////////

DABoolean
ChisqForLayer::selectComponent(SmallCount sensorNumber,
				FAItem<ASiStore>& iStore)

{

   //use the ladder number of the hybrid number
   SmallCount hybrid = iStore->ladderForSensor(sensorNumber);
   
   //check to see if its in the clamshell
   if ( (m_layer <=2) && (iStore->clamshellForHybrid(hybrid) == m_layer) )
   {
      return true; 
   }

   //check to see if its in layer 3 or 4
   if ( (m_layer >= 3) && (iStore->layerForSensor(sensorNumber) == m_layer) )
   {
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
