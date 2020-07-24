// -*- C++ -*-
//
// Package:     <DRFittingWeights>
// Module:      DRFittingWeights
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Mon Jul 30 11:00:22 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "Experiment/report.h"
#include "DRFittingWeights/DRFittingWeights.h"
#include "DataHandler/Stream.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "DataHandler/Frame.h"

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
static const char* const kFacilityString = "DRFittingWeights.DRFittingWeights" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule.cc,v 1.2 2000/12/04 19:11:05 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
DRFittingWeights::DRFittingWeights()
   : FrameIteratorModuleBase( "DRFittingWeights", "This is my module" )
   //,m_header(0), m_headerHolder(&m_header)
{
   //register the Proxy Factory created by the FIHolder
#if 0
   registerProxyFactory( Stream::kBeginRun,
			 m_headerHolder.makeFactory(),
			 UsageTag() );
#endif
   //You must set what streams you which to iterate over
   //  that is, what events should the Frames be stopped on
   iterateOver( Stream::kEvent );

}

// DRFittingWeights::DRFittingWeights( const DRFittingWeights& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DRFittingWeights::~DRFittingWeights()
{
}

//
// assignment operators
//
// const DRFittingWeights& DRFittingWeights::operator=( const DRFittingWeights& rhs )
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

//
// const member functions
//
void
DRFittingWeights::iterate( const FIFrameIterator& iBegin,
			   const FIFrameIterator& iEnd )
{
   report( INFO, kFacilityString )
      << "starting iteration " << endl;
   
   for( unsigned int timesIterated = 1;
	timesIterated < 5;
	++timesIterated )
   {

      report( INFO, kFacilityString )
	 << "start iteration" << timesIterated << endl;
      
      //loop over all the Frames
      for( FIFrameIterator itFrame = iBegin;
	   itFrame != iEnd;
	   ++itFrame )
      {
	 
      }
   }
}

//
// static member functions
//

//
// nested classes
//

double DRFittingWeights::bin::iterate( double* values )
{
   double fw2 = sqr( values[ 0 ] );

   double rms = 0.;
   for ( int i = 0;  i < DRFittingWeights::kBinFull;  i++ )
   {
      double norm_resid2 = ( l_residual2[ i ]
			     / ( l_e_dca2[ i ] + fw2 * l_e_drift2[ i ] ) );
      rms += norm_resid2;
   }

   return sqr( rms - 1. );
}

void DRFittingWeight::bin::take(
   double dca, double drift, double e_dca, double e_drift )
{
   double residual = dca - drift;
   double e_residual = sqrt( sqr( e_dca ) + sqr( e_drift ) );

   l_residual[ index ] = residual;
   l_residual2[ index ] = sqr( residual );
   l_e_dca2[ index ] = sqr( e_dca );
   l_e_drift2[ index ] = sqr( e_drift );
   index++;
   
   h_dca->fill( dca );
   h_drift->fill( drift );
   h_e_dca->fill( e_dca );
   h_e_drift->fill( e_drift );
   h_residual->fill( residual );
   h_e_residual->fill( e_residual );
   h_norm_resid->fill( residual / e_residual );
}

void DRFittingWeights::bin::fill_after( double fw )
{
   for ( int i = 0;  i < DRFittingWeights::kBinFull;  i++ )
   {
      double norm_resid = ( l_residual[ i ]
			    / sqrt( e_dca2[ i ] + fw2 * e_drift2[ i ] ) );
      h_norm_resid_after->fill( norm_resid );
   }
}

void DRFittingWeights::bin::setup(
   HIHistoManager* hm, BinType myType, unsigned int binNum )
{
   index = 0;

   for ( int iteration = 0;
	 iteration < DRFittingWeights::kIterations;
	 iteration++ )
   {
      char c_iteration = iteration + '0';
      string s_iteration( "iteration " );
      s_iteration += c_iteration;
   
      char c_binNum[3];
      sprintf( c_binNum, "%02d", binNum );
      string s_binNum( "" );
      switch ( myType )
      {
	 case kDrift: s_binNum = "layer-drift "; break;
	 case kTheta: s_binNum = "layer-theta "; break;
	 case kAlpha: s_binNum = "drift-alpha "; break;
	 case kPhi:   s_binNum = "layer-phi "; break;
	 case kLayer: s_binNum = "layer "; break;
	 default:
	    assert( 0 );
      }
      s_binNum += "bin ";
      s_binNum += c_binNum;

      h_dca = hm->histogram( iteration * 100000 + 1000 + myType * 100 + binNum,
			     ( s_iteration + " dca " + s_binNum ).c_str(),
			     30, -0.015, 0.015 );
      h_drift = hm->histogram( iteration * 100000 + 2000 + myType * 100 + binNum,
			       ( s_iteration + " drift " + s_binNum ).c_str(),
			       30, -0.015, 0.015 );
      h_e_dca = hm->histogram( iteration * 100000 + 3000 + myType * 100 + binNum,
			       ( s_iteration + " edca " + s_binNum ).c_str(),
			       30, 0., 0.05E-2 );
      h_e_drift = hm->histogram( iteration * 100000 + 4000 + myType * 100 + binNum,
				 ( s_iteration + " edrift " + s_binNum ).c_str(),
				 30, 0., 0.05E-2 );
      h_residual = hm->histogram( iteration * 100000 + 5000 + myType * 100 + binNum,
				  ( s_iteration + " residual " + s_binNum ).c_str(),
				  100, -0.002, 0.002 );
      h_e_residual = hm->histogram( iteration * 100000 + 6000 + myType * 100 + binNum,
				    ( s_iteration + " eresidual " + s_binNum ).c_str(),
				    30, 0., 0.05E-2 );
      h_norm_resid = hm->histogram( iteration * 100000 + 7000 + myType * 100 + binNum,
				    ( s_iteration + " normresid " + s_binNum ).c_str(),
				    100, -3.5, 3.5 );
      h_norm_resid_after = hm->histogram( iteration * 100000 + 8000 + myType * 100 + binNum,
					  ( s_iteration + " fitted-normresid "
					    + s_binNum ).c_str(),
					  100, -3.5, 3.5 );
   }

   for ( int i = 0;  i < DRFittingWeights::kBinFull;  i++ )
      l_residual[ i ] = l_residual2[ i ] = l_e_dca2[ i ] = l_e_drift2[ i ] = 0.;
}

