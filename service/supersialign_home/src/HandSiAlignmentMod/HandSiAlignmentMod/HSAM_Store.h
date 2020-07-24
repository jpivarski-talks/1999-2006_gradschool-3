#if !defined(HANDSIALIGNMENTMOD_HSAM_STORE_H)
#define HANDSIALIGNMENTMOD_HSAM_STORE_H
// -*- C++ -*-
//
// Package:     <HandSiAlignmentMod>
// Module:      HSAM_Store
// 
/**\class HSAM_Store HSAM_Store.h HandSiAlignmentMod/HSAM_Store.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Fri May 10 15:10:36 EDT 2002
// $Id$
//
// Revision history
//
// $Log$

// system include files

// user include files

#include "../../HelixIntersection/HelixIntersection/HIHelix.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"

// forward declarations

class HSAM_Store
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef enum {
	 kNone,
	 kSVRonly,
	 kSVZonly,
	 kBoth
      } StoreKind;

      // ---------- Constructors and destructor ----------------
      HSAM_Store();
      HSAM_Store( HSAM_Store* head,
		  double curvature, double phi0, double d0, double cotTheta, double z0,
		  float mat_cc, float mat_cp, float mat_cd, float mat_ct, float mat_cz,
		  float mat_pp, float mat_pd, float mat_pt, float mat_pz,
		  float mat_dd, float mat_dt, float mat_dz,
		  float mat_tt, float mat_tz,
		  float mat_zz );
      virtual ~HSAM_Store();

      // ---------- member functions ---------------------------

      // the user must delete this!!!
      HIHelix* unsafe_helix() const;

      // ---------- const member functions ---------------------

      StoreKind kind() const { return m_kind; };
      HSAM_Store* next() const { return m_next; };

      // ---------- static member functions --------------------

   protected:
      StoreKind m_kind;

      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HSAM_Store( const HSAM_Store& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HSAM_Store& operator=( const HSAM_Store& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      double m_track_params[5];
      float m_matrix_elements[15];

      HSAM_Store* m_next;

      // ---------- static data members ------------------------

};

class HSAM_StoreSVRonly : public HSAM_Store
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HSAM_StoreSVRonly();
      HSAM_StoreSVRonly( HSAM_Store* head,
			 double curvature, double phi0, double d0, double cotTheta, double z0,
			 float mat_cc, float mat_cp, float mat_cd, float mat_ct, float mat_cz,
			 float mat_pp, float mat_pd, float mat_pt, float mat_pz,
			 float mat_dd, float mat_dt, float mat_dz,
			 float mat_tt, float mat_tz,
			 float mat_zz,

			 CalibratedSVRphiHit::Identifier rphi_identifier,
			 SmallCount rphi_parent,
			 SmallCount rphi_hybrid,
			 SmallCount rphi_layer,
			 SmallCount rphi_sensor,
			 SmallCount rphi_width,
			 CalibratedSVRphiHit::PicoCoul rphi_charge,
			 CalibratedSVRphiHit::PicoCoul rphi_charge_error,
			 float rphi_coord,
			 float rphi_coord_error,
			 CalibratedSVRphiHit::QualHit rphi_quality,
			 DABoolean rphi_saturated );
      virtual ~HSAM_StoreSVRonly();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      const CalibratedSVRphiHit* rphi_hit() const { return &m_rphi_hit; };

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HSAM_StoreSVRonly( const HSAM_Store& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HSAM_Store& operator=( const HSAM_Store& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      CalibratedSVRphiHit m_rphi_hit;

      // ---------- static data members ------------------------

};

class HSAM_StoreSVZonly : public HSAM_Store
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HSAM_StoreSVZonly();
      HSAM_StoreSVZonly( HSAM_Store* head,
			 double curvature, double phi0, double d0, double cotTheta, double z0,
			 float mat_cc, float mat_cp, float mat_cd, float mat_ct, float mat_cz,
			 float mat_pp, float mat_pd, float mat_pt, float mat_pz,
			 float mat_dd, float mat_dt, float mat_dz,
			 float mat_tt, float mat_tz,
			 float mat_zz,

			 CalibratedSVZHit::Identifier z_identifier,
			 SmallCount z_parent,
			 SmallCount z_hybrid,
			 SmallCount z_layer,
			 SmallCount z_sensor,
			 SmallCount z_width,
			 CalibratedSVZHit::PicoCoul z_charge,
			 CalibratedSVZHit::PicoCoul z_charge_error,
			 float z_coord,
			 float z_coord_error,
			 CalibratedSVZHit::QualHit z_quality,
			 DABoolean z_saturated );
      virtual ~HSAM_StoreSVZonly();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      const CalibratedSVZHit* z_hit() const { return &m_z_hit; };

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HSAM_StoreSVZonly( const HSAM_Store& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HSAM_Store& operator=( const HSAM_Store& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      CalibratedSVZHit m_z_hit;

      // ---------- static data members ------------------------

};

class HSAM_StoreBoth : public HSAM_StoreSVRonly
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HSAM_StoreBoth();
      HSAM_StoreBoth( HSAM_Store* head,
		      double curvature, double phi0, double d0, double cotTheta, double z0,
		      float mat_cc, float mat_cp, float mat_cd, float mat_ct, float mat_cz,
		      float mat_pp, float mat_pd, float mat_pt, float mat_pz,
		      float mat_dd, float mat_dt, float mat_dz,
		      float mat_tt, float mat_tz,
		      float mat_zz,

		      CalibratedSVRphiHit::Identifier rphi_identifier,
		      SmallCount rphi_parent,
		      SmallCount rphi_hybrid,
		      SmallCount rphi_layer,
		      SmallCount rphi_sensor,
		      SmallCount rphi_width,
		      CalibratedSVRphiHit::PicoCoul rphi_charge,
		      CalibratedSVRphiHit::PicoCoul rphi_charge_error,
		      float rphi_coord,
		      float rphi_coord_error,
		      CalibratedSVRphiHit::QualHit rphi_quality,
		      DABoolean rphi_saturated,

		      CalibratedSVZHit::Identifier z_identifier,
		      SmallCount z_parent,
		      SmallCount z_hybrid,
		      SmallCount z_layer,
		      SmallCount z_sensor,
		      SmallCount z_width,
		      CalibratedSVZHit::PicoCoul z_charge,
		      CalibratedSVZHit::PicoCoul z_charge_error,
		      float z_coord,
		      float z_coord_error,
		      CalibratedSVZHit::QualHit z_quality,
		      DABoolean z_saturated );
      virtual ~HSAM_StoreBoth();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      const CalibratedSVZHit* z_hit() const { return &m_z_hit; };

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HSAM_StoreBoth( const HSAM_Store& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HSAM_Store& operator=( const HSAM_Store& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      CalibratedSVZHit m_z_hit;

      // ---------- static data members ------------------------

};


// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HandSiAlignmentMod/Template/HSAM_Store.cc"
//#endif

#endif /* HANDSIALIGNMENTMOD_HSAM_STORE_H */
