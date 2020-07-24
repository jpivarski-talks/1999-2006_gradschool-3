// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiHybrid
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Alex Undrus 
// 

#include "Experiment/Experiment.h"
// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ASiStorePro/ASiStore.h"
#include "ASiStorePro/ASiHybrid.h"
#include "ASiStorePro/ASiLayer.h"
#include "ASiStorePro/ASiLadder.h"
#include "ASiStorePro/ASiSensorAddress.h"
#include "ASiStorePro/ASiSensorInfo.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ASiStorePro.ASiHybrid" ;

//
// static data member definitions
//

//
// constructors and destructor
//
ASiHybrid::ASiHybrid(){}


ASiHybrid::ASiHybrid( const FAItem<DGDetectorStore >& iStore, ASiStore* iStorePro, SmallCount clay_index, SmallCount clad_index, SmallCount c_index ) : m_SiStore( iStore ), m_Store( iStorePro )
////ASiHybrid::ASiHybrid( SmallCount clay_index, SmallCount clad_index, SmallCount c_index ) 
{
   m_index = c_index;
   m_layer = clay_index;
   m_ladder = clad_index;
   m_hybrid = m_ladder + (m_index-1)*ASiSensorAddress::kAdNLadders;

SmallCount gang = ASiSensorAddress::ggetGangForHybrid(m_hybrid-1);
   m_sensors = new ASiSensorInfo [gang];
//   cout << " ASiHybrid::ASiHybrid " << m_layer << "**" << m_ladder << "**" << m_hybrid << "**" << m_index << "**"  << gang << endl;

  for( SmallCount kSensor(0); kSensor < gang; kSensor++ )
  {
    m_sensors[kSensor] = ASiSensorInfo( m_SiStore, m_Store, m_layer, m_ladder, m_hybrid, kSensor + 1);

  }
}

// ASiHybrid::ASiHybrid(const ASiHybrid& ) 
// {
// }


ASiHybrid::~ASiHybrid()
{
//   cout << " CALLING DESTRUCTOR ASiHybrid " << endl;
   delete [] m_sensors;  
}

// 
// assignment operators
//
 const ASiHybrid& ASiHybrid::operator=( const ASiHybrid& param )
{
  if( this != &param )
  {
//     m_sensors = param.m_sensors ;
     m_SiStore = param.m_SiStore ;
     m_Store = param.m_Store ;
     m_index = param.m_index;
     m_hybrid = param.m_hybrid;
     m_layer = param.m_layer;
     m_ladder = param.m_ladder;
  SmallCount n_Max = ASiSensorAddress::ggetGangForHybrid(m_hybrid-1);
  m_sensors = new ASiSensorInfo [n_Max];

  for( SmallCount kIndex(0); kIndex < n_Max; kIndex++ )
  {
    m_sensors[kIndex] = param.m_sensors[kIndex];
  }

//    cout << " ASSIGNMENT OF ASiHybrid !!!!! " << m_index << "_" << m_hybrid << "_" << m_layer << "_"  << m_ladder << endl;
  }

  return *this;
}

//
// member functions
//

void 
ASiHybrid::setNumberOfHybrid( SmallCount iIndex, SmallCount iIndexLayer, SmallCount iIndexLadder )
{
   m_index = iIndex;

   m_layer = iIndexLayer;

   m_ladder = iIndexLadder;

   m_hybrid = iIndexLadder + (m_index-1)*ASiSensorAddress::kAdNLadders;

}


//
// const member functions
//

      SmallCount 
ASiHybrid::number() const
{
   SmallCount ret = m_hybrid;
   return ret;
}

      SmallCount 
ASiHybrid::sensor() const
{
   SmallCount ret = ASiSensorAddress::ggetSensorForHybrid(m_hybrid-1);
   return ret;
}

      SmallCount 
ASiHybrid::gang() const
{
   SmallCount ret = ASiSensorAddress::ggetGangForHybrid(m_hybrid-1);
   return ret;
}

      SmallCount 
ASiHybrid::ladderInLayer() const
{
   SmallCount ret = ASiSensorAddress::ggetLadderInLayerForHybrid(m_hybrid-1)+1;
   return ret;
}

      SmallCount 
ASiHybrid::ladder() const
{
   return m_ladder;
}

      SmallCount 
ASiHybrid::layer() const
{
   return m_layer;
}

//

const ASiSensorInfo& 
ASiHybrid::sensor( SmallCount iSensor ) const
{
  SmallCount max = ASiSensorAddress::kAdNSensors;
  assert( 1 <= iSensor );
  assert( max >= iSensor );

  return m_sensors[iSensor - 1 ];
}

const ASiLayer&
ASiHybrid::layerOwner() const
{
const ASiLayer& ret = (*m_Store).layer( m_layer );
return ret;
}

const ASiLadder&
ASiHybrid::ladderOwner() const
{
return (*m_Store).ladder( m_ladder );
}

const STL_VECTOR( const ASiSensorInfo* ) 
ASiHybrid::v_sensors() const
{
      STL_VECTOR( const ASiSensorInfo* ) temp ;
  SmallCount n_Max = gang();

  for( SmallCount kIndex(1); kIndex != n_Max + 1; ++kIndex )
  {
      temp.push_back(&sensor(kIndex)) ;
  }
return temp;
}
//
// static member functions
//









