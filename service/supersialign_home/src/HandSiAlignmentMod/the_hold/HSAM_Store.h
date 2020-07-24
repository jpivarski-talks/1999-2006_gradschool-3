// -*- C++ -*-
#if !defined(HANDSIALIGNMENTMOD_HSAM_STORE_H)
#define HANDSIALIGNMENTMOD_HSAM_STORE_H
//
// Package:     HandSiAlignmentMod
// Module:      Hsam_Store
//
/**\class HandSiAlignmentMod Hsam_Store.h HandSiAlignmentMod/Hsam_Store.h
 
 Description: storage classes for helices and hits

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Tue Apr  9 15:11:19 EDT 2002
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "../../HelixIntersection/HelixIntersection/HIHelix.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"

class HSAM_Store
{
   public:
      typedef enum {
	 kNone,
	 kSVRonly,
	 kSVZonly,
	 kBoth
      } StoreKind;

      HSAM_Store( HSAM_Store* head,
		  double curvature, double phi0, double d0, double cotTheta, double z0,
		  float mat_cc, float mat_cp, float mat_cd, float mat_ct, float mat_cz,
		  float mat_pp, float mat_pd, float mat_pt, float mat_pz,
		  float mat_dd, float mat_dt, float mat_dz,
		  float mat_tt, float mat_tz,
		  float mat_zz );

      // the user must delete this!!!
      HIHelix* unsafe_helix() const;

      StoreKind kind() const { return m_kind; };
      HSAM_Store* next() const { return m_next; };

   protected:
      StoreKind m_kind;

   private:
      double m_track_params[5];
      float m_matrix_elements[15];

      HSAM_Store* m_next;
};

class HSAM_StoreSVRonly : public HSAM_Store
{
   public:
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

      const CalibratedSVRphiHit* rphi_hit() const { return &m_rphi_hit; };

   private:
      CalibratedSVRphiHit m_rphi_hit;
};

class HSAM_StoreSVZonly : public HSAM_Store
{
   public:
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

      const CalibratedSVZHit* z_hit() const { return &m_z_hit; };

   private:
      CalibratedSVZHit m_z_hit;
};

class HSAM_StoreBoth : public HSAM_StoreSVRonly
{
   public:
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

      const CalibratedSVZHit* z_hit() const { return &m_z_hit; };

   private:
      CalibratedSVZHit m_z_hit;
};

#endif /* HANDSIALIGNMENTMOD_HSAM_STORE_H */
