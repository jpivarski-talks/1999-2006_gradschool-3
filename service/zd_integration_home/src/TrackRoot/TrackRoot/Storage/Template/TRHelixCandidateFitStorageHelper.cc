#if !defined(TRACKROOT_TRHELIXCANDIDATEFITSTORAGEHELPER_CC)
#define TRACKROOT_TRHELIXCANDIDATEFITSTORAGEHELPER_CC
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixCandidateFitStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri May 14 17:00:02 EDT 1999
// $Id: TRHelixCandidateFitStorageHelper.cc,v 1.4 2002/04/04 21:26:54 cdj Exp $
//
// Revision history
//
// $Log: TRHelixCandidateFitStorageHelper.cc,v $
// Revision 1.4  2002/04/04 21:26:54  cdj
// avoid creation of temporary KTHelix when reading back HelixFits
//
// Revision 1.3  2002/04/02 15:04:54  cdj
// minor speed optimization for reading back data from storage
//
// Revision 1.2  2000/06/16 02:49:03  marsh
// Purged tool box typedefs.
//
// Revision 1.1  2000/01/05 18:51:29  cdj
// first submission
//
// Revision 1.1.1.1  1999/09/13 21:21:45  cdj
// imported package
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
#include "TrackRoot/Storage/TRHelixCandidateFitStorageHelper.h"
#include "StorageManagement/SMSinkStream.h"
#include "StorageManagement/SMSourceStream.h"
#include "StorageManagement/SMPackInfo.h"

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

//static const char* const kHCFSFacilityString = "TrackRoot.TRHelixCandidateFitStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
//static const char* const kIdString  = "$Id: TRHelixCandidateFitStorageHelper.cc,v 1.4 2002/04/04 21:26:54 cdj Exp $";
//static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
TRHelixCandidateFitStorageHelper<H>::TRHelixCandidateFitStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &TRHelixCandidateFitStorageHelper<H>::deliverV1 );
}

// TRHelixCandidateFitStorageHelper<H>::TRHelixCandidateFitStorageHelper( const TRHelixCandidateFitStorageHelper<H>& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//TRHelixCandidateFitStorageHelper<H>::~TRHelixCandidateFitStorageHelper()
//{
//}

//
// assignment operators
//
// const TRHelixCandidateFitStorageHelper& TRHelixCandidateFitStorageHelper::operator=( const TRHelixCandidateFitStorageHelper& rhs )
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


//Determination of Packing Values for Error Matrix
// In CLEO II, the error matrix used fixed packing with values of
//       diagonal elements: min= 0 max=2 precision 1E-6
//   off diagonal elements: min=-1 max=1 precision 1E-3
//
// the reason the diagonal elements were stored with such high precision was
//  because the off diagonal elements were actually stored as
//
//         N  =  Vij / sqrt( Vii ) / sqrt( Vjj )
//
//  therefore Vii and Vjj (actually sqrt(Vii) sqrt(Vjj)) had to be stored 
//  with high precision to avoid the truncation error on Vii and Vjj being 
//  the dominant error on Vij
//
// For this packing strategy, we are going to store the diagonal elements
//  using fractional packing, i.e.
//
//  ((stored sqrt(Vii)) - (actual sqrt(Vii))) / (actual sqrt(Vii) ) <= delta 
//
// But what size does delta have to be inorder to not dominate the error
//  on Vij?  This can be determined by using error propegation.  The variables
//  in the calcuation are
//    V       := off diagonal element Vij
//    A, B    := sqrt of the two diagonal elements Vii Vjj
//    N        = V / A * B    normalized value that is stored
//    sigma_* := the 'truncation' error on quantity *, where *=V,A,B,N
//
//  the error on V is given by
//    sigma_V^2/V^2 = sigma_N^2/N^2 + sigma_A^2/A^2+sigma_B^2/B^2
//  
//  if we substitute delta = sigma_A/A = sigma_B/B
//  (which is actually an overestimate of sigma_A and sigma_B)
//
//    sigma_V^2/V^2 =  sigma_N^2/N^2 + 2 delta^2
//
//  then if we want the truncation error on A and B to only be a small
//  contribution to sigma_V^2 then
//
//    sigma_N^2 >> 2 abs(N)^2 * delta^2
//
//  setting N at it's max value of 1 gives
//    1 >> 2 * delta^2 /sigma_N^2
//
//  then the fractional increase in the value of sigma_V due to a non-zero
//   delta is
//         sqrt( 1 + 2 * delta^2/sigma_N^2)
//
//   if we set delta/sigma_N = 0.25 in the above formula we get 1.061, i.e.
//   at the absolute worse case (N=1) the contribution to sigma_N from the
//   maximum truncation error of the diagonal elements is only 6%
//

static const float kOffDiagMaxTruncationError = 1.0e-3;
static const float kDiagMaxTruncationError = kOffDiagMaxTruncationError/4.0 ;
#define PACK_DIAG( element_ ) \
sm_pack_fraction( sm_field( "errorMatrixElement" # element_ "2", sqrt(errorMatrix(KTHelix:: k ## element_, KTHelix:: k ## element_ ) ) ), 1e-6, 4, kDiagMaxTruncationError)

//if one of the diagonal elements is zero, store 0
#define PACK_OFF( element1_, element2_ ) \
   sm_pack( sm_field( "errorMatrixElement" # element1_ # element2_ "Normalized", 0.0 == errorMatrix(KTHelix::k ## element1_, KTHelix::k ## element1_) * errorMatrix( KTHelix::k ## element2_, KTHelix::k ## element2_ ) ? 0 * errorMatrix(KTHelix::k ## element1_, KTHelix::k ## element2_ ) : errorMatrix(KTHelix::k ## element1_, KTHelix::k ## element2_ ) / sqrt( errorMatrix(KTHelix::k ## element1_, KTHelix::k ## element1_) * errorMatrix( KTHelix::k ## element2_, KTHelix::k ## element2_ ) ) ), -1.0, 1.0, kOffDiagMaxTruncationError )

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
void 
TRHelixCandidateFitStorageHelper<H>::implementStore( 
   SMSinkStream& iSink,
   const TRHelixCandidateFit<H>& iData ) 
{
   const HepSymMatrix& errorMatrix = iData.errorMatrix();

  // using compression values similar to cleoii.ini.
   iSink 
     << sm_pack( SM_VAR( iData, identifier )           , 1 , 50000, 1)           
      //      << sm_pack( SM_VAR( iData, curvature )  , -10, 10, 1e-5)
// ranges from 10 MeV to 20 GeV with enough precision to store the curvature 
// for 1 micron drift chamber resolution (MC).
// dc/c = 5*1e-3 for 100 micron resolution -> use 500 more 
     << sm_pack_fraction( SM_VAR( iData, curvature )  , 0.01, 25     , 1e-5) 
     << sm_pack( SM_VAR( iData, phi0 )                , 0   , 2*M_PI , 1e-5)
     << sm_pack( SM_VAR( iData, d0 )                  , -.5 , .5     , 1e-6)
     << sm_pack( SM_VAR( iData, cotTheta )            , -10., 10.    , 1e-5)
     << sm_pack( SM_VAR( iData, z0 )                  , -1. , 1.     , 1e-6)
     << sm_pack( SM_VAR( iData, referencePoint().x )  , -3. , 3.     , 1e-6)
     << sm_pack( SM_VAR( iData, referencePoint().y )  , -3. , 3.     , 1e-6)
     << sm_pack( SM_VAR( iData, referencePoint().z )  , -3. , 3.     , 1e-6)
     << PACK_DIAG( Curvature )
     << PACK_DIAG( Phi0 )
     << PACK_DIAG( D0 )
     << PACK_DIAG( CotTheta )
     << PACK_DIAG( Z0 )
     << PACK_OFF( Curvature, Phi0 )
     << PACK_OFF( Curvature, D0 )
     << PACK_OFF( Curvature, CotTheta )
     << PACK_OFF( Curvature, Z0 )
     << PACK_OFF( Phi0, D0 )
     << PACK_OFF( Phi0, CotTheta )
     << PACK_OFF( Phi0, Z0 )
     << PACK_OFF( D0, CotTheta )
     << PACK_OFF( D0, Z0 )
     << PACK_OFF( CotTheta, Z0 )
     ;
}
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
TRHelixCandidateFit<H>* 
TRHelixCandidateFitStorageHelper<H>::deliverV1( SMSourceStream& iSource )
{
   
   TRHelixCandidateFit<H> ::Identifier identifier; 
   double curvature; 
   double phi0; 
   double d0; 
   double cotTheta; 
   double z0; 

   double x, y, z;

   //variables to hold stored pieces of error matrix;
   double emCC, emPP, emDD, emTT, emZZ;
   double       emCP, emCD, emCT, emCZ;
   double             emPD, emPT, emPZ;
   double                   emDT, emDZ;
   double                         emTZ;

   iSource 
      >> identifier 
      >> curvature 
      >> phi0 
      >> d0 
      >> cotTheta 
      >> z0 
      >> x
      >> y
      >> z
      >> emCC >> emPP >> emDD >> emTT >> emZZ
              >> emCP >> emCD >> emCT >> emCZ
                      >> emPD >> emPT >> emPZ
                              >> emDT >> emDZ
                                      >> emTZ
	 ;

   HepPoint3D referencePoint(x,y,z);
   
   //rebuild the error Matrix
   double sqrt_emCC = emCC;
   double sqrt_emPP = emPP;
   double sqrt_emDD = emDD;
   double sqrt_emTT = emTT;
   double sqrt_emZZ = emZZ;

   HepSymMatrix errorMatrix(5);
   errorMatrix( KTHelix::kCurvature, KTHelix::kCurvature ) = emCC*emCC;
   errorMatrix( KTHelix::kPhi0,      KTHelix::kPhi0 )      = emPP*emPP;
   errorMatrix( KTHelix::kD0,        KTHelix::kD0 )        = emDD*emDD;
   errorMatrix( KTHelix::kCotTheta,  KTHelix::kCotTheta )  = emTT*emTT;
   errorMatrix( KTHelix::kZ0,        KTHelix::kZ0 )        = emZZ*emZZ;


   errorMatrix( KTHelix::kCurvature, KTHelix::kPhi0     ) = emCP*sqrt_emCC*sqrt_emPP;
   errorMatrix( KTHelix::kCurvature, KTHelix::kD0       ) = emCD*sqrt_emCC*sqrt_emDD;
   errorMatrix( KTHelix::kCurvature, KTHelix::kCotTheta ) = emCT*sqrt_emCC*sqrt_emTT;
   errorMatrix( KTHelix::kCurvature, KTHelix::kZ0       ) = emCZ*sqrt_emCC*sqrt_emZZ;
   
   errorMatrix( KTHelix::kPhi0,      KTHelix::kD0       ) = emPD*sqrt_emPP*sqrt_emDD;
   errorMatrix( KTHelix::kPhi0,      KTHelix::kCotTheta ) = emPT*sqrt_emPP*sqrt_emTT;
   errorMatrix( KTHelix::kPhi0,      KTHelix::kZ0       ) = emPZ*sqrt_emPP*sqrt_emZZ;

   errorMatrix( KTHelix::kD0,        KTHelix::kCotTheta ) = emDT*sqrt_emDD*sqrt_emTT;
   errorMatrix( KTHelix::kD0,        KTHelix::kZ0       ) = emDZ*sqrt_emDD*sqrt_emZZ;

   errorMatrix( KTHelix::kCotTheta,  KTHelix::kZ0       ) = emTZ*sqrt_emTT*sqrt_emZZ;

   return new TRHelixCandidateFit<H>( 
				       identifier,  
				       curvature,  
				       phi0,  
				       d0,  
				       cotTheta,  
				       z0,
				       errorMatrix,
				       referencePoint ) ; 
}

//
// const member functions
//
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const TRHelixCandidateFit<H>& 
TRHelixCandidateFitStorageHelper<H>::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   static const TRHelixCandidateFit<H> temp(
      TRHelixCandidateFit<H> ::Identifier ( 1 ), // identifier 
      KTHelix( double ( 2 ), // curvature 
	       double ( 3 ), // phi0 
	       double ( 4 ), // d0 
	       double ( 5 ), // cotTheta 
	       double ( 6 ) ) // z0
      );
   return temp;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
DABoolean 
TRHelixCandidateFitStorageHelper<H>::compare( 
   const TRHelixCandidateFit<H>* iNewData, 
   const TRHelixCandidateFit<H>& iOldData ) const
{
   //write a comparison operation if operator== doesn't exist for TRHelixCandidateFit<H>
   return (*iNewData == iOldData ) ;
}

//
// static member functions
//

//TRHelixCandidateFit<H>*
//SMStorageHelper<TRHelixCandidateFit<H>>::make( const TRHelixCandidateFit<H>& iTempData ) const
//{
   //change this line if TRHelixCandidateFit<H> does not have a copy constructor
//   return new TRHelixCandidateFit<H>( iTempData );
//}

//------------------------------------------------------
// DO NOT DELETE 

//------------------------------------------------------
#endif
