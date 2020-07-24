// -*- C++ -*-
//
// Package:     <Sneak>
// Module:      SneakFcn
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Mar 12 17:29:07 EST 2003
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
#include "Sneak/SneakFcn.h"
#include <assert.h>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Sneak.SneakFcn" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: skeleton.cc,v 1.6 2001/01/03 16:25:23 cdj Exp $";
static const char* const kTagString = "$Name:  $";

static const double M_KMASS = 0.497672;
static const double M_PIMASS = 0.13957018;

//
// static data member definitions
//

//
// constructors and destructor
//
SneakFcn::SneakFcn() : MIFcn()
{
   addInitialParameter(string("delPhi"),   0., 0.1);
   addInitialParameter(string("delD0"),    0., 0.001);
   addInitialParameter(string("delCotTh"), 0., 0.1);
}

SneakFcn::SneakFcn(HepPoint3D& origin,
		   KTHelix&    fixed_helix,    KTHelix&    variable_helix,
		   Hep3Vector& fixed_momentum, Hep3Vector& variable_momentum)
   : MIFcn()
   , m_origin(origin)
   , m_fixed_helix(fixed_helix)
   , m_variable_helix(variable_helix)
   , m_fixed_momentum(fixed_momentum)
   , m_variable_momentum(variable_momentum)
{
   addInitialParameter(string("delPhi"),   0., 0.1);
   addInitialParameter(string("delD0"),    0., 0.001);
   addInitialParameter(string("delCotTh"), 0., 0.1);
}

// SneakFcn::SneakFcn( const SneakFcn& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

SneakFcn::~SneakFcn()
{
}

//
// assignment operators
//
// const SneakFcn& SneakFcn::operator=( const SneakFcn& rhs )
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

double SneakFcn::energy_constraint(KTHelix& trial_helix,
				   double fixed_phi, double trial_phi)
{
   double pperpt(m_variable_helix.curvature() / trial_helix.curvature() * m_variable_momentum.perp());
   Hep3Vector trial_momentum(cos(trial_helix.phi0()) * pperpt,
			     sin(trial_helix.phi0()) * pperpt,
			     trial_helix.cotTheta() * pperpt);

   Hep3Vector fixed_p(cos(fixed_phi) * m_fixed_momentum.perp(),
		      sin(fixed_phi) * m_fixed_momentum.perp(),
		      m_fixed_momentum.z());
   Hep3Vector trial_p(cos(trial_phi) * trial_momentum.perp(),
		      sin(trial_phi) * trial_momentum.perp(),
		      trial_momentum.z());

   double constraint(sqrt((fixed_p + trial_p).mag2() + sqr(M_KMASS)) -
		     sqrt(trial_p.mag2() + sqr(M_PIMASS)) - sqrt(fixed_p.mag2() + sqr(M_PIMASS)));

   // This is -d constraint / d pperpt
   double derivative(pperpt * (1. + sqr(trial_helix.cotTheta())) / sqrt(trial_p.mag2() + sqr(M_PIMASS)) -
		     ((pperpt*cos(trial_phi) + fixed_p.x()) * cos(trial_phi) +
		      (pperpt*sin(trial_phi) + fixed_p.y()) * sin(trial_phi) +
		      (pperpt*trial_helix.cotTheta() + fixed_p.z()) * trial_helix.cotTheta()) /
		     sqrt((fixed_p + trial_p).mag2() + sqr(M_KMASS)));
   // This is d constraint / d curvature
   derivative *= m_variable_helix.curvature() / sqr(trial_helix.curvature()) * m_variable_momentum.perp();

   // One step of Newton's method
   trial_helix.setCurvature(trial_helix.curvature() - constraint/derivative);

   cout << "    CURVCONSTR " << constraint << endl;

   return constraint;
}

double SneakFcn::theta_constraint(KTHelix& trial_helix,
				  double fixed_phi, double trial_phi,
				  Hep3Vector kaon_flight)
{
   double pperpt(m_variable_helix.curvature() / trial_helix.curvature() * m_variable_momentum.perp());
   Hep3Vector trial_momentum(cos(trial_helix.phi0()) * pperpt,
			     sin(trial_helix.phi0()) * pperpt,
			     trial_helix.cotTheta() * pperpt);

   Hep3Vector fixed_p(cos(fixed_phi) * m_fixed_momentum.perp(),
		      sin(fixed_phi) * m_fixed_momentum.perp(),
		      m_fixed_momentum.z());
   Hep3Vector trial_p(cos(trial_phi) * trial_momentum.perp(),
		      sin(trial_phi) * trial_momentum.perp(),
		      trial_momentum.z());
   Hep3Vector kaon_p(fixed_p + trial_p);

   // The direction of the kaon's displacement
   double flight_cotTheta(kaon_flight.z() / kaon_flight.perp());

   // Require kaon displacement to happen at the same angle as kaon momentum
   double constraint(kaon_p.z() / kaon_p.perp() - flight_cotTheta);

   // This is d constraint / d trial_cotTheta
   double derivative(pperpt / kaon_p.perp());

   // One step of Newton's method
   trial_helix.setCotTheta(trial_helix.cotTheta() - constraint/derivative);

   cout << "        THETACONSTR " << constraint << endl;

   return constraint;
}

double SneakFcn::phi_constraint(KTHelix& trial_helix,
				double fixed_phi, double trial_phi,
				Hep3Vector kaon_flight)
{
   double pperpt(m_variable_helix.curvature() / trial_helix.curvature() * m_variable_momentum.perp());
   Hep3Vector trial_momentum(cos(trial_helix.phi0()) * pperpt,
			     sin(trial_helix.phi0()) * pperpt,
			     trial_helix.cotTheta() * pperpt);

   Hep3Vector fixed_p(cos(fixed_phi) * m_fixed_momentum.perp(),
		      sin(fixed_phi) * m_fixed_momentum.perp(),
		      m_fixed_momentum.z());
   Hep3Vector trial_p(cos(trial_phi) * trial_momentum.perp(),
		      sin(trial_phi) * trial_momentum.perp(),
		      trial_momentum.z());
   Hep3Vector kaon_p(fixed_p + trial_p);

   // I derived this with the law of sines and the fact that a
   // triangle has 180 degrees
   double sinQ(fixed_p.perp() / trial_p.perp() * sin(kaon_flight.phi() - fixed_phi));
   double Q1(asin(sinQ));
   double Q2(M_PI - Q1);

   double Q;
   // If you're on the right track
   if ( ((kaon_p.x()*kaon_flight.x() + kaon_p.y()*kaon_flight.y())
	 / kaon_p.perp() / kaon_flight.perp()) > 0. ) {
      // Take the solution which is closest to what you already have
      if ( abs(cos(Q1 + kaon_flight.phi()) - cos(trial_phi)) <
	   abs(cos(Q2 + kaon_flight.phi()) - cos(trial_phi))   )
	 Q = Q1; else Q = Q2;
   }
   else {
      // Do the opposite
      if ( abs(cos(Q1 + kaon_flight.phi()) - cos(trial_phi)) <
	   abs(cos(Q2 + kaon_flight.phi()) - cos(trial_phi))   )
	 Q = Q2; else Q = Q1;
   }

   cout << "(phi = " << trial_helix.phi0() << ") (delta phi = "
	<< Q + kaon_flight.phi() - trial_phi << ")";

   // This would take you right to the solution if nothing else moved
   trial_helix.setPhi0(trial_helix.phi0() + Q + kaon_flight.phi() - trial_phi);
   
   cout << "    PHICONSTR "
	<< ((kaon_p.x()*kaon_flight.x() + kaon_p.y()*kaon_flight.y())
	    / kaon_p.perp()
	    / kaon_flight.perp() - 1.)
	<< endl;
   
   return ((kaon_p.x()*kaon_flight.x() + kaon_p.y()*kaon_flight.y())
	   / kaon_p.perp()
	   / kaon_flight.perp() - 1.);
}

double SneakFcn::iterate(double* values)
{
   KTHelix trial_helix(m_variable_helix.curvature(),
		       m_variable_helix.phi0(),
		       m_variable_helix.d0(),
		       m_variable_helix.cotTheta(),
		       m_variable_helix.z0()        );
   
   double x(-1000.), y(-1000.), z(-1000.), zsep(-1000.), fixed_phi(-1000.), trial_phi(-1000.);

   do {
      do {
	 if( !calc_intersection(x, y, z, zsep, fixed_phi, trial_phi, trial_helix) ) return 1000.;

	 while ( abs(theta_constraint(trial_helix, fixed_phi, trial_phi,
				      Hep3Vector(x, y, z) - m_origin)) > 1e-10 );

      } while ( abs(energy_constraint(trial_helix, fixed_phi, trial_phi)) > 1e-10 );

      do {
	 if( !calc_intersection(x, y, z, zsep, fixed_phi, trial_phi, trial_helix) ) return 1000.;
	 double phiconstr;
	 do {
	    double phidiff(trial_phi - trial_helix.phi0());
	    phiconstr =
	       phi_constraint(trial_helix, fixed_phi, trial_phi, Hep3Vector(x, y, z) - m_origin);
	    trial_phi = trial_helix.phi0() + phidiff;
	 } while ( abs(phiconstr) > 1e-10 );

	 if( !calc_intersection(x, y, z, zsep, fixed_phi, trial_phi, trial_helix) ) return 1000.;

      } while ( abs(phi_constraint(trial_helix, fixed_phi, trial_phi,
				   Hep3Vector(x, y, z) - m_origin)) > 1e-10 );

      cout << "EDM " << edm(trial_helix) << endl;

   } while ( edm(trial_helix) > 1e-10 );

      




	 
   cout << "DONE!" << endl;      
      
   return 0.;
}


double SneakFcn::edm(KTHelix& trial_helix)
{
   // Calculate the momentum of the trial helix by scaling the
   // starting helix (so that it inherits any fine corrections)
   double pperpt(m_variable_helix.curvature() / trial_helix.curvature() * m_variable_momentum.perp());
   Hep3Vector trial_momentum(cos(trial_helix.phi0()) * pperpt,
			     sin(trial_helix.phi0()) * pperpt,
			     trial_helix.cotTheta() * pperpt);

   // Calculate the new intersection point of this with the fixed
   // helix, getting a lot of variables out of the calculation
   double x(-1000.), y(-1000.), z(-1000.), zsep(-1000.), fixed_phi(-1000.), trial_phi(-1000.);
   if ( !calc_intersection(x, y, z, zsep, fixed_phi, trial_phi, trial_helix) ) zsep *= 100.;
   
   // First constraint: no mismatch in z (normalization is to help convergence)
//   double edm1(sqr(zsep)/sqr(0.01));
   double edm1(0.);

   // Rotate the momentum vectors to the intersection point
   Hep3Vector fixed_p(cos(fixed_phi) * m_fixed_momentum.perp(),
		      sin(fixed_phi) * m_fixed_momentum.perp(),
		      m_fixed_momentum.z());
   Hep3Vector trial_p(cos(trial_phi) * trial_momentum.perp(),
		      sin(trial_phi) * trial_momentum.perp(),
		      trial_momentum.z());

   // Calculate the (sin of the) angle between the kaon's
   // (reconstructed) flight path and the kaons's momentum
   Hep3Vector kaon_flight(Hep3Vector(x, y, z) - m_origin);
   Hep3Vector kaon_p(fixed_p + trial_p);
   Hep3Vector cross_product(kaon_flight.cross(kaon_p));
   
   // Second constraint: momentum and flight path are aligned
   double edm2(cross_product.mag2() / kaon_flight.mag2() / kaon_p.mag2());

   // Calculate the energies of the pions and reconstructed kaon
   double fixed_e(sqrt(fixed_p.mag2() + sqr(M_PIMASS)));
   double trial_e(sqrt(trial_p.mag2() + sqr(M_PIMASS)));
   double kaon_e(sqrt(kaon_p.mag2() + sqr(M_KMASS)));

   // Third constraint: energy conservation
   double edm3(sqr(kaon_e - fixed_e - trial_e) / sqr(kaon_e));

   report(DEBUG, kFacilityString)
      << "Trying " << trial_helix.curvature()
      << " " << trial_helix.phi0()
      << " " << trial_helix.d0()
      << " " << trial_helix.cotTheta()
      << " " << trial_helix.z0()
      << " with momentum " << trial_momentum
      << " has EDM1,2,3 " << edm1 << " " << edm2 << " " << edm3 << endl;

   return edm1 + edm2 + edm3;
}

DABoolean SneakFcn::calc_intersection( double& x, double& y, double& z, double& zsep,
				       double& fixed_phi, double& trial_phi,
				       KTHelix& trial_helix )
{
   x = y = z = zsep = fixed_phi = trial_phi = 1000.;  // To more easily find unchecked failures

   // Express the two helices in terms of (x - xa)^2 + (y - ya)^2 == ra^2
   double ra( 1./2./m_fixed_helix.curvature() );
   double xa( -(m_fixed_helix.d0() + ra) * sin(m_fixed_helix.phi0()) );
   double ya( (m_fixed_helix.d0() + ra) * cos(m_fixed_helix.phi0()) );

   double rb( 1./2./trial_helix.curvature() );
   double xb( -(trial_helix.d0() + rb) * sin(trial_helix.phi0()) );
   double yb( (trial_helix.d0() + rb) * cos(trial_helix.phi0()) );

   // Zero or infinitely many solutions
   // I don't want to deal with either case
   if ( xa == xb  &&  ya == yb ) return false;

   double disc( (pow(4.*sqr(ra)*(xa - xb) - 
		     4.*(sqr(rb)*(xa - xb) + 
			(xa + xb)*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)))
		     ,2.) - 16.*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb))*
		 (pow(xa,4.) - 2.*sqr(xa)*sqr(xb) + pow(xb,4.) + 
		  2.*sqr(xb)*(sqr(ra) - sqr(rb) + sqr(ya - yb)) + 
		  2.*sqr(xa)*(-sqr(ra) + sqr(rb) + sqr(ya - yb)) + 
		  pow(-sqr(ra) + sqr(rb) + sqr(ya - yb),2.) - 
		  4.*sqr(rb)*sqr(ya - yb)))/
		(64.*pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.)) );
   // no real solutions
   if ( disc < 0 ) return false;

   double pre( (sqr(rb)*(xa - xb) + sqr(ra)*(-xa + xb) + 
		(xa + xb)*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)))/
	       (2.*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb))) );

   // The two x solutions
   double xsol1( pre + sqrt( disc ) );
   double xsol2( pre - sqrt( disc ) );

   // Two possibilities for the y solution to each x solution
   double ysol1a( yb + sqrt( sqr(rb) - sqr(xsol1 - xb) ) );
   double ysol1b( yb - sqrt( sqr(rb) - sqr(xsol1 - xb) ) );
   double ysol1;
   if ( abs( sqr(xsol1 - xa) + sqr(ysol1a - ya) - sqr(ra) ) <
	abs( sqr(xsol1 - xa) + sqr(ysol1b - ya) - sqr(ra) )   )
      ysol1 = ysol1a;
   else
      ysol1 = ysol1b;

   double ysol2a( yb + sqrt( sqr(rb) - sqr(xsol2 - xb) ) );
   double ysol2b( yb - sqrt( sqr(rb) - sqr(xsol2 - xb) ) );
   double ysol2;
   if ( abs( sqr(xsol2 - xa) + sqr(ysol2a - ya) - sqr(ra) ) <
	abs( sqr(xsol2 - xa) + sqr(ysol2b - ya) - sqr(ra) )   )
      ysol2 = ysol2a;
   else
      ysol2 = ysol2b;

   // Calculate the arclength of each solution along the two tracks.
   // Throw out a solution if the particle needs to travel (too far)
   // backward to get to it. If both solutions are okay, take the one
   // closest to the origin.
   double aarclength(1000.), barclength(1000.);
   double aarclength1(1000.), aarclength2(1000.), barclength1(1000.), barclength2(1000.);

   DABoolean arclength1_ok( arclength_test( xsol1, ysol1, trial_helix, aarclength1, barclength1 ) );
   DABoolean arclength2_ok( arclength_test( xsol2, ysol2, trial_helix, aarclength2, barclength2 ) );
   int which_num;
   char which_let;
   
   if ( !arclength1_ok  &&  !arclength2_ok ) return false;

   else if ( arclength1_ok  &&  !arclength2_ok ) {
      x = xsol1;
      y = ysol1;
      aarclength = aarclength1;
      barclength = barclength1;
   }

   else if ( !arclength1_ok  &&  arclength2_ok ) {
      x = xsol2;
      y = ysol2;
      aarclength = aarclength2;
      barclength = barclength2;
   }      

   else if ( sqr(xsol1) + sqr(ysol1) < sqr(xsol2) + sqr(ysol2) ) {
      x = xsol1;
      y = ysol1;
      aarclength = aarclength1;
      barclength = barclength1;
   }

   else {
      x = xsol2;
      y = ysol2;
      aarclength = aarclength2;
      barclength = barclength2;
   }      

   // Calculate where the intersection happens in Z
   double az( m_fixed_helix.cotTheta() * aarclength + m_fixed_helix.z0() );
   double bz( trial_helix.cotTheta() * barclength + trial_helix.z0() );

   z = az;  // so that the zsep condition can be applied last, without
	    // affecting anything else
//   z = (az + bz)/2.;

   zsep = az - bz;

   // Calculate phi for each circle
   fixed_phi = m_fixed_helix.phi0() + abs(aarclength) * 2. * m_fixed_helix.curvature();
   trial_phi =   trial_helix.phi0() + abs(barclength) * 2. *   trial_helix.curvature();

   return true;
}

DABoolean SneakFcn::arclength_test( double x, double y, KTHelix& trial_helix, double& aarclength, double& barclength )
{
   const double tailCut = 0.005;

   double asinphi1( 2. * m_fixed_helix.curvature() * (x * cos(m_fixed_helix.phi0()) + y * sin(m_fixed_helix.phi0())) );
   double acosphi1( 2. * m_fixed_helix.curvature() * (x * sin(m_fixed_helix.phi0()) - y * cos(m_fixed_helix.phi0())
						      + m_fixed_helix.d0()) + 1. );
   aarclength = atan2(asinphi1, acosphi1) / 2. / m_fixed_helix.curvature();

   double bsinphi1( 2. * trial_helix.curvature() * (x * cos(trial_helix.phi0()) + y * sin(trial_helix.phi0())) );
   double bcosphi1( 2. * trial_helix.curvature() * (x * sin(trial_helix.phi0()) - y * cos(trial_helix.phi0())
						    + trial_helix.d0()) + 1. );
   barclength = atan2(bsinphi1, bcosphi1) / 2. / trial_helix.curvature();

   return ( aarclength > -tailCut  &&  aarclength < M_PI/2./abs(m_fixed_helix.curvature()) )  &&
      ( barclength > -tailCut  &&  barclength < M_PI/2./abs(trial_helix.curvature()) );
}

//
// const member functions
//

//
// static member functions
//
