// -*- C++ -*-
//
// Package:     <ChisqForSiAlignment>
// Module:      ChisqForSensor
// 
// Description: <one line class summary>
// Feeds minuit the correct constants for one silicon sensor
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Sat Jun  3 09:59:34 EDT 2000
// $Id: ChisqForSensor.cc,v 1.2 2001/02/06 20:31:34 llh14 Exp $
//
// Revision history
//
// $Log: ChisqForSensor.cc,v $
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
#include "ChisqForSiAlignment/ChisqForSensor.h"
#include "ChisqForSiAlignment/ChisqForSiAlignment.h"
#include "STLUtility/fwd_vector.h"

#include "DataHandler/Frame.h"

#include "SiGeom/SiAlignedGeometryProxy.h"
#include "ASiStorePro/ASiStore.h"

#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAConstants.h"
#include "FrameAccess/extract.h"

#include "FrameIterate/FIHolder.h"

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

static const char* const kFacilityString = "package.ChisqForSensor" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqForSensor.cc,v 1.2 2001/02/06 20:31:34 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ChisqForSensor::ChisqForSensor(const FIFrameIterator& iBegin, 
			       const FIFrameIterator& iEnd,       
			       CLEOConstantsModifiable<DBSVAlignmentSensor>&  
			       SVSensorConstants,float normRphi, 
			       float normZ, float cutRphi, float cutZ, 
			       long int maxEvents, long int sensor, 
			       DABoolean varyX, DABoolean varyY, 
			       DABoolean varyZ, DABoolean varyPhiX, 
			       DABoolean varyPhiY, DABoolean varyPhiZ ) : 
 
   m_maxEvents(maxEvents), 
   m_iBegin(iBegin), 
   m_iEnd(iEnd), 
   m_SVSensorConstants(SVSensorConstants ),
   m_compCtr(0),   
   m_normRphi(normRphi),
   m_normZ(normZ),
   m_cutRphi(cutRphi),
   m_cutZ(cutZ),
   m_SensorNumber(sensor),
   m_varyX(varyX),
   m_varyY(varyY),
   m_varyZ(varyZ),
   m_varyPhiX(varyPhiX),
   m_varyPhiY(varyPhiY),
   m_varyPhiZ(varyPhiZ)
{
   if ( !( 0 < m_SensorNumber <= 447) )
   { report( ERROR, kFacilityString )
      <<"Invalid sensor number passed to ChisqForSensor!!" << endl;
   }

   //Make initial parameters for Minuit
   if(m_varyX)
   {
      addInitialParameter("Delta X", m_SVSensorConstants[m_SensorNumber-1].get_deltaX(), 1*k_um, -k_cm, k_cm);
   }
   
   if(m_varyY)
   {
      addInitialParameter("Delta Y", m_SVSensorConstants[m_SensorNumber-1].get_deltaY(), 1*k_um, -k_cm, k_cm);
   }

   if(m_varyZ)
   {
      addInitialParameter("Delta Z", m_SVSensorConstants[m_SensorNumber-1].get_deltaZ(), 10*k_um, -k_cm, k_cm);
   }

   if(m_varyPhiX)
   {
      addInitialParameter("Delta PhiX", m_SVSensorConstants[m_SensorNumber-1].get_phiX(), .000001, -0.1, 0.1);
   }

   if(m_varyPhiY)
   {
      addInitialParameter("Delta PhiY", m_SVSensorConstants[m_SensorNumber-1].get_phiY(), .000001, -0.1, 0.1);
   }
   
   if(m_varyPhiZ)
   {
      addInitialParameter("Delta PhiZ", m_SVSensorConstants[m_SensorNumber-1].get_phiZ(), .00001, -0.1, 0.1);
   }

}

// ChisqForSensor::ChisqForSensor( const ChisqForSensor& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ChisqForSensor::~ChisqForSensor()
{
}

//
// assignment operators
//
// const ChisqForSensor& ChisqForSensor::operator=( const ChisqForSensor& rhs )
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
ChisqForSensor::iterate(double* values)
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
      m_SVSensorConstants[m_SensorNumber-1].set_deltaX( deltaX );
   }

   if(m_varyY)   
   {
      Count indxY = kDeltaY - useParam[kDeltaX] ;
      
      double deltaY = values[indxY];
      m_SVSensorConstants[m_SensorNumber-1].set_deltaY( deltaY );
   }

   if(m_varyZ)   
   {
      Count indxZ = kDeltaZ - useParam[kDeltaX] - useParam[kDeltaY];
      
      double deltaZ = values[indxZ];
      m_SVSensorConstants[m_SensorNumber-1].set_deltaZ( deltaZ );
      
   }

   if(m_varyPhiX)   
   {
      Count indxPhiX = kPhiX - useParam[kDeltaX] - useParam[kDeltaY]
	 - useParam[kDeltaZ];
      
      double phiX = values[indxPhiX];
      m_SVSensorConstants[m_SensorNumber-1].set_phiX( phiX );
   }

   if(m_varyPhiY)
   {
      Count indxPhiY = kPhiY - useParam[kDeltaX] - useParam[kDeltaY]
	               - useParam[kDeltaZ] - useParam[kPhiX];

      double phiY = values[indxPhiY];
      m_SVSensorConstants[m_SensorNumber-1].set_phiY( phiY );
   }

   if(m_varyPhiZ)
   {
      Count indxPhiZ = kPhiZ - useParam[kDeltaX] - useParam[kDeltaY]
	               - useParam[kDeltaZ] - useParam[kPhiX]
	               - useParam[kPhiY];

      double phiZ = values[indxPhiZ];
      m_SVSensorConstants[m_SensorNumber-1].set_phiZ( phiZ );
   }
  
   report( INFO, kFacilityString)
      << "\nValue for delta x is: " 
      << m_SVSensorConstants[m_SensorNumber-1].get_deltaX() 
      << "\nValue for delta y is: " 
      << m_SVSensorConstants[m_SensorNumber-1].get_deltaY() 
      << "\nValue for delta z is: " 
      << m_SVSensorConstants[m_SensorNumber-1].get_deltaZ() 
      << "\nValue for delta phix is: " 
      << m_SVSensorConstants[m_SensorNumber-1].get_phiX() 
      << "\nValue for delta phiy is: " 
      << m_SVSensorConstants[m_SensorNumber-1].get_phiY() 
      << "\nValue for delta phiz is: " 
      << m_SVSensorConstants[m_SensorNumber-1].get_phiZ() 
      << endl;

   // initialize chi square value
   double chisq = 0.0;
   Count eventCtr = 0;

   calcResiduals(m_iBegin, m_iEnd, eventCtr, chisq, m_maxEvents,
		 m_normRphi, m_normZ, m_cutRphi, m_cutZ);

   //Place here anything to be done at the end 
   report(INFO, kFacilityString)
      << "The number of events:  " << eventCtr
      << endl;
   

//////For DEBUGGING ONLY!!!/////////////////////////////////////////////////
   if( chisq == 0)
   {  
      int fakeResidual = 1;
      //There are ~6 hits per event, per side
      chisq = (eventCtr*6)*(2)*(1/0.00004)*(1/0.00004);
   }

/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////
DABoolean
ChisqForSensor::selectComponent(SmallCount sensorNumber, 
				FAItem<ASiStore>& iStore)

{
   if ( sensorNumber == m_SensorNumber)
      
   { 	 
      return true; }

   return false;

}

//
// const member functions
//

//
// static member functions
//