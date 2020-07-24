#if !defined(SNEAK_SNEAKFCN_H)
#define SNEAK_SNEAKFCN_H
// -*- C++ -*-
//
// Package:     <Sneak>
// Module:      SneakFcn
// 
/**\class SneakFcn SneakFcn.h Sneak/SneakFcn.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Mar 12 17:28:57 EST 2003
// $Id$
//
// Revision history
//
// $Log$

// system include files

// user include files

#include "MinuitInterface/MIFcn.h"
#include "KinematicTrajectory/KTHelix.h"
#include "CLHEP/Geometry/Point3D.h"

// forward declarations

class SneakFcn : public MIFcn
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum {
	 kDeltaD0
      };

      // ---------- Constructors and destructor ----------------
      SneakFcn();
      SneakFcn(HepPoint3D& origin,
	       KTHelix&    fixed_helix,    KTHelix&    variable_helix,
	       Hep3Vector& fixed_momentum, Hep3Vector& variable_momentum);
      virtual ~SneakFcn();

      // ---------- member functions ---------------------------

      double iterate(double* values);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      SneakFcn( const SneakFcn& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const SneakFcn& operator=( const SneakFcn& ); // stop default

      // ---------- private member functions -------------------
      double energy_constraint(KTHelix& trial_helix,
			       double fixed_phi, double trial_phi);
      double theta_constraint(KTHelix& trial_helix,
			      double fixed_phi, double trial_phi,
			      Hep3Vector kaon_flight);
      double phi_constraint(KTHelix& trial_helix,
			    double fixed_phi, double trial_phi,
			    Hep3Vector kaon_flight);

      double edm(KTHelix& trial_helix);

      DABoolean SneakFcn::calc_intersection( double& x, double& y, double& z, double& zsep,
					     double& fixed_phi, double& trial_phi,
					     KTHelix& trial_helix );
      DABoolean SneakFcn::arclength_test( double x, double y,
					  KTHelix& trial_helix,
					  double& aarclength, double& barclength );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      HepPoint3D m_origin;
      KTHelix    m_fixed_helix;
      KTHelix    m_variable_helix;
      Hep3Vector m_fixed_momentum;
      Hep3Vector m_variable_momentum;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "Sneak/Template/SneakFcn.cc"
//#endif

#endif /* SNEAK_SNEAKFCN_H */
