// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiLadder
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
#include "SiGeom/SiGeomDescription.h"
#include "ASiStorePro/ASiLadder.h"
#include "ASiStorePro/ASiStore.h"
#include "ASiStorePro/ASiSensorInfo.h"
#include "ASiStorePro/ASiSensorAddress.h"
#include "ASiStorePro/ASiHybrid.h"

#include "DetectorGeometry/DGDetectorStore.h"
#include "DetectorGeometry/DGVolumePtr.h"
#include "DetectorGeometry/DGLabVolume.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "DetectorGeometry/DGConstPlacedVolumePtr.h"
#include "DetectorGeometry/DGConstVolumePtr.h"
#include "DetectorGeometry/DGConstVolumeType.h"
#include "DetectorGeometry/DGVolumeBox.h"
#include "DetectorGeometry/DGVolumeTrapezoidSym.h"
#include "DetectorGeometry/DGPlacedVolume.h"
#include "DetectorGeometry/DGPath.h"
#include "DetectorGeometry/DGVolumeParent.h"
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DetectorGeometry/DGTransform.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"
#include "CLHEP/Geometry/Transform3D.h"
// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ASiStorePro.ASiLadder" ;

//
// static data member definitions
//

//
// constructors and destructor
//
ASiLadder::ASiLadder() {}

ASiLadder::ASiLadder( const FAItem<DGDetectorStore >& iStore, 
		      ASiStore* iStorePro, 
		      SmallCount cl_index, 
		      SmallCount c_index) 
   : m_SiStore( iStore ),
     m_Store( iStorePro ),
     m_layer(cl_index),
     m_index(c_index),
     m_ladder( (*iStorePro).ladderForLayer(cl_index) + c_index -1 ),
     m_spSLab( iStore->labVolume( (iStore->findPathTo(SiGeomDescription
				    ::kLadderName[m_ladder-1], 1)
	                      ) ) )

   // Added 5/4/00  
//  m_spSLab = iStore->labVolume( (iStore->findPathTo(SiGeomDescription
//                             ::kLadderName[m_ladder-1], 1)) );

{
// moved outside body of constructor 5/4/00
//   SmallCount FirstLadder = (*m_Store).ladderForLayer(m_layer);
//   m_ladder = FirstLadder + m_index - 1;

   int n_Hybrids(2);
   m_hybrids = new ASiHybrid[n_Hybrids];
//   cout << " ASiLadder::ASiLadder " << n_Hybrids << "**" << m_index << "**"  << cl_index << "**" << m_ladder << endl;

  for( SmallCount kHybrid(0); kHybrid < n_Hybrids; kHybrid++ ) {
     m_hybrids[kHybrid] = ASiHybrid(  m_SiStore, m_Store, m_layer, 
				      m_ladder, kHybrid + 1 );
  }

//  report( INFO, kFacilityString )
//     << "In ASiLadder's Constructor! " << m_spSLab.isNull()
//     << endl;
   
}

// ASiLadder::ASiLadder(const ASiLadder& ) 
// {
// }

ASiLadder::~ASiLadder()
{
//   cout << " CALLING DESTRUCTOR ASiLadder " << endl;
   delete [] m_hybrids;  
}

//
// assignment operators
//
const ASiLadder& ASiLadder::operator=( const ASiLadder& param ) 
{
   if( this != &param )
   {
//     cout << " ASSIGNMENT OF ASiLadder !!!!! " << endl;
      m_SiStore = param.m_SiStore ;
      m_Store = param.m_Store ;
      m_index = param.m_index ;
      m_ladder = param.m_ladder ; 
      m_layer = param.m_layer ; 
      SmallCount n_Max(2);
      m_hybrids = new ASiHybrid[n_Max];
      // Added 5/4/00 for caching
      m_spSLab = param.m_spSLab;

      for( SmallCount kIndex(0); kIndex < n_Max; kIndex++ ) {
	 m_hybrids[kIndex] = param.m_hybrids[kIndex];
      }
   }

   return *this;
}

//
// member functions
//

void 
ASiLadder::setNumberOfLadder( SmallCount iIndex, SmallCount iIndexLayer )
{
  
   m_index = iIndex;
   
   m_layer = iIndexLayer;

   SmallCount FirstLadder = (*m_Store).ladderForLayer(m_layer);

   m_ladder = FirstLadder + m_index - 1;

}

//
// const member functions
//
SmallCount 
ASiLadder::number() const
{
   return m_ladder;
}

SmallCount 
ASiLadder::layer() const
{
   return m_layer;
}

HepTransform3D 
ASiLadder::transToSi() const
{
   // In new ladder definition, entire trapezoid containing diamond
   // is the ladder, not the assumption here, so translate and rotate
   HepTransform3D tHepSLab =  m_spSLab->localToLabAligned()*
      HepRotateZ3D( ( 4 == m_layer ? M_PI : 0 ) ) *
      HepTranslateY3D( ( 4 == m_layer ? -1 : 1 )*
	 	       SiGeomDescription::kyLadderOffset ) ;

   return tHepSLab;
}


HepPoint3D
ASiLadder::coorInSi( HepPoint3D SPoint ) const
{
   return transToSi()*SPoint;
}

HepVector3D 
ASiLadder::unitVectorZInSi() const
{
//   DGPath pathToLadder = m_SiStore->findPathTo(
//      SiGeomDescription::kLadderName[m_ladder-1], 1);

//   DGPath pathLS =  m_pathToLadder;

//   const DGConstLabVolumePtr spSLab = m_SiStore->labVolume( m_pathToLadder );

   DGTransform tSLab = m_spSLab->localToLabAligned();

   HepTransform3D tHepSLab =  tSLab;

   HepPoint3D unitZ(0.,0.,1.);

   return HepTransform3D( tHepSLab*HepRotateZ3D( 4 == m_layer ? M_PI : 0 )
      ).getRotation()*unitZ;
}

HepVector3D 
ASiLadder::unitVectorXInSi() const
{

//   DGPath pathToLadder = m_SiStore->findPathTo( 
//      SiGeomDescription::kLadderName[m_ladder-1], 1);

//   DGPath pathLS =  m_pathToLadder;
   
//   const DGConstLabVolumePtr spSLab = m_SiStore->labVolume( m_pathToLadder );
   DGTransform tSLab = m_spSLab  -> localToLabAligned();

   HepTransform3D tHepSLab =  tSLab;

   HepPoint3D unitX(1.,0.,0.);

   return  HepTransform3D( tHepSLab*HepRotateZ3D( 4 == m_layer ? M_PI : 0 ) 
      ).getRotation()*unitX;
}

HepVector3D 
ASiLadder::unitVectorYInSi() const
{

//   DGPath pathToLadder = m_SiStore->findPathTo( 
//      SiGeomDescription::kLadderName[m_ladder-1], 1);

//   DGPath pathLS =  m_pathToLadder;
   
//   const DGConstLabVolumePtr spSLab = m_SiStore->labVolume( m_pathToLadder );
   DGTransform tSLab = m_spSLab  -> localToLabAligned();

   HepTransform3D tHepSLab =  tSLab;

   HepPoint3D unitY(0.,1.,0.);

   return  HepTransform3D( tHepSLab*HepRotateZ3D( 4 == m_layer ? M_PI : 0 )
      ).getRotation()*unitY;
}

HepVector3D 
ASiLadder::rPhiPointer() const
{
   SmallCount isen = ASiSensorAddress::ggetSensorForHybrid(m_ladder-1);
//   HepVector3D vsen=getUnitVectorYForSensorInSi(isen);

   HepVector3D vsen = sensor(1,1).unitVectorYInSi();
   HepVector3D vlad = unitVectorYInSi();

   float check = vsen.x()*vlad.x()+vsen.y()*vlad.y();

   HepVector3D un;
   if (check > 0) {
      un = vlad; 
   } 
   else {
      HepTransform3D m=HepReflectY3D();
      HepTransform3D m1=HepReflectX3D();
      HepTransform3D m2=HepReflectZ3D();
      un = m*m1*m2*vlad;
   }
   
   return un;
}

HepVector3D 
ASiLadder::zPointer() const
{
   SmallCount isen = ASiSensorAddress::ggetSensorForHybrid(m_ladder-1);
   HepVector3D vsen = sensor(1,1).unitVectorYInSi();
   HepVector3D vlad=unitVectorYInSi();

   float check = vsen.x()*vlad.x()+vsen.y()*vlad.y();

   HepVector3D un;
   if (check > 0) {
      HepTransform3D m=HepReflectY3D();
      HepTransform3D m1=HepReflectX3D();
      HepTransform3D m2=HepReflectZ3D();
      un = m*m1*m2*vlad;}
   else { 
      un = vlad; 
   } 

   return un;
}

float 
ASiLadder::phiInSi() const
{
   SmallCount ladder=ASiSensorAddress::ggetLadderInLayerForHybrid(m_ladder-1);
   HepPoint3D tLad = coorInSi(ladder);
   return tLad.phi();
}

float 
ASiLadder::phiRotationInSi() const
{
   HepVector3D  vX = unitVectorXInSi();     
   return vX.phi();
}

float 
ASiLadder::radiusInSi () const
{
   SmallCount ladder=ASiSensorAddress::ggetLadderInLayerForHybrid(m_ladder-1);
   HepPoint3D tLad = coorInSi(ladder);
   return tLad.mag();
}

float 
ASiLadder::halfLengthX() const
{
//   SmallCount NLadder = m_ladder - 1;
//   DGPath pathToLadder = m_SiStore->findPathTo( 
//      SiGeomDescription::kLadderName[NLadder], 1 );
//   const DGConstLabVolumePtr spLadder = m_SiStore->labVolume( pathToLadder );
   DGConstVolumePtr pVolume = m_spSLab -> volume();
   DGConstVolumeType< DGVolumeParent > pParent = pVolume;
   if( ! pParent.isValid() ){
      report( WARNING, kFacilityString )
	 <<  "Could not convert pointer to DGVolume into pointer to DGVolumeParent "
	 << endl;
      return 0;
   }
   
//   DGConstVolumeType< DGVolumeTrapezoidSym > pBox = (*pParent).boundingVolume();
   float halfLengthX = SiGeomDescription::kSensorWidth/2;
   return halfLengthX;
}

float 
ASiLadder::halfLengthY() const
{
   float ret = (*m_Store).siThickness()/2.;
   return ret;
}

float 
ASiLadder::halfLengthZ() const 
{
//   SmallCount NLadder = m_ladder - 1;
//   DGPath pathToLadder = m_SiStore->findPathTo( 
//      SiGeomDescription::kLadderName[NLadder], 1 );
//   const DGConstLabVolumePtr spLadder = m_SiStore->labVolume( pathToLadder );

//   DGConstVolumePtr pVolume = m_spSLab -> volume();
//   DGConstVolumeType< DGVolumeParent > pParent = pVolume;
//   if( ! pParent.isValid() ){
//      report( WARNING, kFacilityString )
//	 <<  "Could not convert pointer to DGVolume into pointer to DGVolumeParent "
//	 << endl;
//      return 0;
//   }
   
//   DGConstVolumeType< DGVolumeTrapezoidSym > pBox =
//      (*pParent).boundingVolume();
//   float halfLengthZ = (*pBox).halfLengthZ();
   return (SiGeomDescription::kZStep)*(SiGeomDescription::kNZStep[ m_layer-1 ])/2;
}

//

const ASiHybrid& 
ASiLadder::hybrid( SmallCount iHybrid ) const
{
   assert( ( 1 <= iHybrid ) && ( ASiSensorAddress::kAdNHybrids >= iHybrid ) );

   return m_hybrids[iHybrid - 1 ];
}

const ASiSensorInfo& 
ASiLadder::sensor( SmallCount iHybridInLadder, SmallCount iSensorInHybrid ) const
{
   assert( ( 1 <= iHybridInLadder ) && 
	   ( ASiSensorAddress::kAdNHybrids >= iHybridInLadder ) );
   assert( ( 1 <= iSensorInHybrid ) && 
	   ( ASiSensorAddress::kAdNSensors>=iSensorInHybrid ) );

   return m_hybrids[ iHybridInLadder - 1 ].sensor( iSensorInHybrid );
}

const ASiSensorInfo& 
ASiLadder::sensor( SmallCount iSensorInLadder ) const
{
  assert( ( 1 <= iSensorInLadder ) && 
	  ( ASiSensorAddress::kAdNLaddersInLayer[m_layer - 1] >= iSensorInLadder ) );

  SmallCount firstInHybrid_1 = ASiSensorAddress::ggetSensorForHybrid(m_ladder-1);
  SmallCount gang_1 = ASiSensorAddress::ggetGangForHybrid(m_ladder-1);

  SmallCount firstInHybrid_2 = ASiSensorAddress::ggetSensorForHybrid(m_ladder-1 + ASiSensorAddress::kAdNLadders );
  SmallCount gang_2 = ASiSensorAddress::ggetGangForHybrid(m_ladder-1 + ASiSensorAddress::kAdNLadders);
  
  SmallCount iHybridInLadder;
  SmallCount iSensorInHybrid;
  if(iSensorInLadder <= gang_2) {
     iHybridInLadder = 2;
     iSensorInHybrid = gang_2 - iSensorInLadder + 1;
  }
  else { 
     iHybridInLadder = 1;
     iSensorInHybrid = iSensorInLadder - gang_2;
  }    

  return m_hybrids[ iHybridInLadder - 1 ].sensor( iSensorInHybrid );
}

const ASiLayer&
ASiLadder::layerOwner() const
{
   return (*m_Store).layer( m_layer );
}

const STL_VECTOR( const ASiHybrid* ) 
ASiLadder::v_hybrids() const
{
   STL_VECTOR( const ASiHybrid* ) temp ;
   SmallCount n_Max(2);

   for( SmallCount kIndex(1); kIndex != n_Max + 1; ++kIndex ) {
      temp.push_back(&hybrid(kIndex)) ;
   }
   return temp;
}
//
// static member functions
//









