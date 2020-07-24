// -*- C++ -*-
//
// Package:     <DRFittingWeights>
// Module:      DRFBin
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Wed Oct 24 16:34:26 EDT 2001
// $Id$
//
// Revision history
//
// $Log$

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
#include "Experiment/report.h"
#include "DRFittingWeights/DRFBin.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DRFittingWeights.DRFBin" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: skeleton.cc,v 1.6 2001/01/03 16:25:23 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DRFBin::DRFBin()
{
}

DRFBin::DRFBin( HIHistoManager* hm, unsigned int id,
		string& nameBase, unsigned int bin, double bottom, double top,
		unsigned int target, unsigned int iterations )
   : m_name()
   , m_id( id )
   , m_workingHist( NULL )
   , m_target( target )
   , m_iterations( iterations )
   , m_hits( 0 )
   , m_state( kEmpty )
   , m_lambda( -1. )
   , m_fitfunc( NULL )
{
   char binName[3];
   sprintf( binName, "%02d", bin );

   char bottomName[6];
   sprintf( bottomName, "%5.2f", bottom );

   char topName[6];
   sprintf( topName, "%5.2f", top );

   m_name = ( nameBase + string( " " ) + string( binName ) +
	      string( " " ) + string( bottomName ) +
	      string( " - " ) + string( topName )             );

   m_workingHist = hm->histogram( m_id, m_name.c_str(), DRF_HISTBINS,
				  -DRF_HISTBOUND, DRF_HISTBOUND );
   double fitStart[] = { double( m_target ), 0., 1. };
   double fitStep[] = { double( m_target ) / 30., 0.1, 0.1 };
   m_workingHist->attachFitFunc(
      m_fitfunc = new HIHistFitGauss( m_workingHist, HIHistFitFunc::kHist1D,
				      fitStart, fitStep ) );

   m_histLog.clear();
   for ( unsigned int iteration = 1;  iteration <= m_iterations;  iteration++ )
   {
      unsigned int id = iteration * 10000 + m_id;
      char iter[3];
      sprintf( iter, "%d", iteration );
      m_histLog.push_back(
	 hm->histogram( id, ( m_name + string( "(" ) + string( iter ) +
			      string( ")" ) ).c_str(),
			DRF_HISTBINS, -DRF_HISTBOUND, DRF_HISTBOUND ) );
   }
}

// DRFBin::DRFBin( const DRFBin& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DRFBin::~DRFBin()
{
}

//
// assignment operators
//
// const DRFBin& DRFBin::operator=( const DRFBin& rhs )
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

string
DRFBin::name()
{
   return m_name;
}

unsigned int
DRFBin::hits()
{
   return m_hits;
}

DRFBin::State
DRFBin::state()
{
   return m_state;
}

double
DRFBin::lambda()
{
   return m_lambda;
}

double
DRFBin::chi2ndf()
{
   return m_chi2ndf;
}

void
DRFBin::reset()
{
   m_workingHist->scale( 0. );
   m_hits = 0;
   m_state = kEmpty;
   m_lambda = -1.;

   m_fw2_already.clear();
   m_residual.clear();
   m_edrift2.clear();
   m_edca2.clear();
   m_layer.clear();
   m_drift.clear();
   m_cosTheta.clear();
//     m_phi.clear();
//     m_sinAlpha.clear();
}

void
DRFBin::take( double residual, double edrift2, double edca2,
	      unsigned int layer, double drift, double cosTheta,
	      double phi, double sinAlpha )
{
   m_hits++;
   double norm_resid = residual / sqrt( edrift2 + edca2 );

   if ( m_state == kEmpty )
      m_state = kFilling;

   if ( m_state == kFilling )
   {
      m_residual.push_back( residual );
      m_edrift2.push_back( edrift2 );
      m_edca2.push_back( edca2 );

      m_layer.push_back( layer );
      m_drift.push_back( drift );
      m_cosTheta.push_back( cosTheta );
//        m_phi.push_back( phi );
//        m_sinAlpha.push_back( sinAlpha );
   }

   if ( m_hits >= m_target )
      m_state = kFilled;
}

void
DRFBin::update( DRFittingWeights* that )
{
   m_fw2_already.clear();
   for ( unsigned int i = 0;  i < m_residual.size();  i++ )
      m_fw2_already.push_back(
	 sqr( that->my_factor( m_layer[i], m_drift[i], m_cosTheta[i],
			       0., 0. ) ) );
//			  m_phi[i], m_alpha[i] ) ) );
   m_state = kUpdated;
}

void
DRFBin::copyHist( unsigned int iteration, ostream& kumac )
{
   assert( iteration > 0  &&  iteration <= m_iterations );
   m_histLog[ iteration-1 ]->copy( *m_workingHist );

   kumac << "h/plot " << m_histLog[ iteration-1 ]->id() << endl;
   if ( m_fitfunc != NULL )
   {
      double area = m_fitfunc->area();
      double mean = m_fitfunc->mean();
      double sigma = abs( m_fitfunc->sigma() );
      double lowBound = m_fitfunc->domainXStart();
      double highBound = m_fitfunc->domainXEnd();

      char comm[512];
      sprintf( comm, "f/plot %.1f*exp(((x-(%.4f))/%.4f)*((x-(%.4f))/%.4f)/(-2.))/sqrt(2.*3.1415926)/%.4f %.4f %.4f s\n",
	       area, mean, sigma, mean, sigma, sigma,
	       lowBound, highBound );
      kumac << comm;
   }
   kumac << endl;
}

DRFBin::State 
DRFBin::optimize()
{
   // fill in here

   m_lambda = 2.;
   double width = residwidth( m_lambda );

   if ( width < 0. )
      return m_state = kFailed;
   else
      return m_state = kOptimized;
}

void
DRFBin::fillHist( double lambda )
{
   double lambda2 = sqr( lambda );
   m_workingHist->scale( 0. );

   double sum = 0.;
   double squaresum = 0.;
   double n = 0.;

   DABoolean fw2_already_setup = ( m_fw2_already.size() == m_residual.size() );
   if ( ! fw2_already_setup )
      report( WARNING, kFacilityString )
	 << "m_fw2_already has not yet been set up." << endl;

   for ( unsigned int i = 0;  i < m_residual.size();  i++ )
   {
      double norm_resid;
      if ( fw2_already_setup )
	 norm_resid = m_residual[i]
	    / sqrt( m_fw2_already[i] * lambda2 * m_edrift2[i] + m_edca2[i] );
      else
	 norm_resid = m_residual[i]
	    / sqrt( lambda2 * m_edrift2[i] + m_edca2[i] );

      if ( abs( norm_resid ) < DRF_HISTBOUND )
      {
	 sum += norm_resid;
	 squaresum += sqr( norm_resid );
	 n += 1.;
      }

      m_workingHist->fill( norm_resid );
   }

   double betterStart[] = { n,
			    sum / n,
			    sqrt( squaresum / n - sqr( sum / n ) ) };
   m_fitfunc->setParamStart( betterStart );
}

double
DRFBin::residwidth( double lambda )
{
   fillHist( lambda );
   m_fitfunc->setDomainFull();
   if ( ! m_fitfunc->fit() )
      return -1.;
   m_fitfunc->setParamStartToFitted();

   double mean = m_fitfunc->mean();
   double sigma = abs( m_fitfunc->sigma() );
   m_fitfunc->setDomain( mean - 2. * sigma, mean + 2. * sigma );
   if ( ! m_fitfunc->fit() )
      return -1.;
   m_chi2ndf = m_fitfunc->chisq() / double( m_fitfunc->degreesOfFreedom() );
   
   return sqr( m_fitfunc->mean() ) + sqr( m_fitfunc->sigma() );
}

pair< double, double >
DRFBin::midpoint( pair< double, double > left, pair< double, double > right )
{
   // fill in here
   return left;
}

double
DRFBin::scan()
{
   // fill in here
   return 1.;
}

//
// const member functions
//

//
// static member functions
//
