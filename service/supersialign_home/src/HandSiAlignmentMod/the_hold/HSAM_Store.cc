// -*- C++ -*-
//
// Package:     <HandSiAlignmentMod>
// Module:      HSAM_Store
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Tue Apr  9 15:11:19 EDT 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files

#include "HandSiAlignmentMod/HSAM_Store.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

HSAM_Store::HSAM_Store( HSAM_Store* head,
			double curvature, double phi0, double d0, double cotTheta, double z0,
			float mat_cc, float mat_cp, float mat_cd, float mat_ct, float mat_cz,
			float mat_pp, float mat_pd, float mat_pt, float mat_pz,
			float mat_dd, float mat_dt, float mat_dz,
			float mat_tt, float mat_tz,
			float mat_zz )
{
   m_track_params[0] = curvature;
   m_track_params[1] = phi0;
   m_track_params[2] = d0;
   m_track_params[3] = cotTheta;
   m_track_params[4] = z0;

   m_matrix_elements[0] = mat_cc;
   m_matrix_elements[1] = mat_cp;
   m_matrix_elements[2] = mat_cd;
   m_matrix_elements[3] = mat_ct;
   m_matrix_elements[4] = mat_cz;
   m_matrix_elements[5] = mat_pp;
   m_matrix_elements[6] = mat_pd;
   m_matrix_elements[7] = mat_pt;
   m_matrix_elements[8] = mat_pz;
   m_matrix_elements[9] = mat_dd;
   m_matrix_elements[10] = mat_dt;
   m_matrix_elements[11] = mat_dz;
   m_matrix_elements[12] = mat_tt;
   m_matrix_elements[13] = mat_tz;
   m_matrix_elements[14] = mat_zz;

   m_kind = kNone;
   m_next = head;
}

HIHelix* HSAM_Store::unsafe_helix() const
{
   HepSymMatrix tmpMat(5);
   tmpMat(1,1) = m_matrix_elements[0];
   tmpMat(1,2) = m_matrix_elements[1];
   tmpMat(1,3) = m_matrix_elements[2];
   tmpMat(1,4) = m_matrix_elements[3];
   tmpMat(1,5) = m_matrix_elements[4];
   tmpMat(2,2) = m_matrix_elements[5];
   tmpMat(2,3) = m_matrix_elements[6];
   tmpMat(2,4) = m_matrix_elements[7];
   tmpMat(2,5) = m_matrix_elements[8];
   tmpMat(3,3) = m_matrix_elements[9];
   tmpMat(3,4) = m_matrix_elements[10];
   tmpMat(3,5) = m_matrix_elements[11];
   tmpMat(4,4) = m_matrix_elements[12];
   tmpMat(4,5) = m_matrix_elements[13];
   tmpMat(5,5) = m_matrix_elements[14];

   return new HIHelix( m_track_params[0],
		       m_track_params[1],
		       m_track_params[2],
		       m_track_params[3],
		       m_track_params[4],
		       tmpMat );
}

HSAM_StoreSVRonly::HSAM_StoreSVRonly( HSAM_Store* head,
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
				      DABoolean rphi_saturated )
   : HSAM_Store( head,
		 curvature, phi0, d0, cotTheta, z0,
		 mat_cc, mat_cp, mat_cd, mat_ct, mat_cz,
		 mat_pp, mat_pd, mat_pt, mat_pz,
		 mat_dd, mat_dt, mat_dz,
		 mat_tt, mat_tz,
		 mat_zz )
   , m_rphi_hit( rphi_identifier, rphi_parent, rphi_hybrid, rphi_layer, rphi_sensor, rphi_width,
		 rphi_charge, rphi_charge_error, rphi_coord, rphi_coord_error,
		 rphi_quality, rphi_saturated )
{
   m_kind = kSVRonly;
}

HSAM_StoreSVZonly::HSAM_StoreSVZonly( HSAM_Store* head,
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
				      DABoolean z_saturated )
   : HSAM_Store( head,
		 curvature, phi0, d0, cotTheta, z0,
		 mat_cc, mat_cp, mat_cd, mat_ct, mat_cz,
		 mat_pp, mat_pd, mat_pt, mat_pz,
		 mat_dd, mat_dt, mat_dz,
		 mat_tt, mat_tz,
		 mat_zz )
   , m_z_hit( z_identifier, z_parent, z_hybrid, z_layer, z_sensor, z_width,
	      z_charge, z_charge_error, z_coord, z_coord_error,
	      z_quality, z_saturated )
{
   m_kind = kSVZonly;
}

HSAM_StoreBoth::HSAM_StoreBoth( HSAM_Store* head,
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
				DABoolean z_saturated )
   : HSAM_StoreSVRonly( head,
			curvature, phi0, d0, cotTheta, z0,
			mat_cc, mat_cp, mat_cd, mat_ct, mat_cz,
			mat_pp, mat_pd, mat_pt, mat_pz,
			mat_dd, mat_dt, mat_dz,
			mat_tt, mat_tz,
			mat_zz,
			rphi_identifier, rphi_parent, rphi_hybrid, rphi_layer, rphi_sensor, rphi_width,
			rphi_charge, rphi_charge_error, rphi_coord, rphi_coord_error,
			rphi_quality, rphi_saturated )
   , m_z_hit( z_identifier, z_parent, z_hybrid, z_layer, z_sensor, z_width,
	      z_charge, z_charge_error, z_coord, z_coord_error,
	      z_quality, z_saturated )
{
   m_kind = kBoth;
}
