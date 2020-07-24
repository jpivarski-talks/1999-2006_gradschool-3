// -*- C++ -*-
//
// Package:     DiagnoseHits2
// Module:      DiagnoseHits2
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Fri May 31 12:14:02 EDT 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DiagnoseHits2/DiagnoseHits2.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"


//RICH example 
//Dedx example
//Event Shape example


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
static const char* const kFacilityString = "Processor.DiagnoseHits2" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.29 2002/03/19 15:07:23 cdj Exp $";
static const char* const kTagString = "$Name: v06_05_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
DiagnoseHits2::DiagnoseHits2( void )               // anal1
   : Processor( "DiagnoseHits2" )
   , m_baseDir( "base", this, "../enderskim/" )
   , m_maxHits( "maxHits", this, -1. )
   , m_rphiCut( "rphiCut", this, 0.0002 )
   , m_zCut( "zCut", this, 0.005 )
   , m_pthresh( "pthresh", this,  0.027 )
   , m_nthresh( "nthresh", this, -0.027 )
   , m_diagLadderStart( "diagLadderStart", this, 0. )
   , m_diagLadderStop( "diagLadderStop", this, 0. )
   , m_hsam_store( NULL )
   , m_done( false )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &DiagnoseHits2::event,    Stream::kEvent );
   //bind( &DiagnoseHits2::beginRun, Stream::kBeginRun );
   //bind( &DiagnoseHits2::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)
}

DiagnoseHits2::~DiagnoseHits2()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

void DiagnoseHits2::deleteTracksHits()
{
   report( NOTICE, kFacilityString )
      << "Deleting the old set of hits..." << endl;

   HSAM_Store* tmp;
   while ( m_hsam_store != NULL ) {
      tmp = m_hsam_store;
      m_hsam_store = m_hsam_store->next();
      delete tmp;	
   }
}

DABoolean DiagnoseHits2::switchToContext( int ladder )
{
   deleteTracksHits();

   report( NOTICE, kFacilityString )
      << "Loading hits for ladder " << ladder << "..." << endl;

   char num[3];
   sprintf( num, "%02d", ladder );

   // this is where to change the input directories

   const int numDirs = 4;
   const char* dir[numDirs] = { "mc1", "mc2", "mc3", "mc4" };

//     const int numDirs = 139;
//     const char* dir[numDirs] = { "116368", "116369", "116371", "116372",
//  				"116374", "116375", "116376", "116380",
//  				"116381", "116383", "116384", "116385",
//  				"116387", "116393", "116394", "116395",
//  				"116396", "116398", "116399", "116407",
//  				"116409", "116410", "116412", "116413",
//  				"116414", "116416", "116420", "116421",
//  				"116422", "116423", "116424", "116425",
//  				"116429", "116430", "116432", "116433",
//  				"116436", "116439", "116440", "116441",
//  				"116443", "116450", "116451", "116453",
//  				"116454", "116458", "116460", "116462",
//  				"116463", "116464", "116465", "116468",
//  				"116470", "116475", "116477", "116478",
//  				"116480", "116482", "116483", "116484",
//  				"116485", "116495", "116496", "116498",
//  				"116499", "116500", "116501", "116502",
//  				"116503", "116504", "116505", "116509",
//  				"116510", "116512", "116514", "116515",
//  				"116516", "116517", "116518", "116519",
//  				"116520", "116521", "116568", "116569",
//  				"116570", "116573", "116574", "116577",
//  				"116579", "116580", "116581", "116582",
//  				"116584", "116585", "116588", "116589",
//  				"116590", "116591", "116593", "116594",
//  				"116595", "116596", "116597", "116600",
//  				"116601", "116602", "116603", "116604",
//  				"116609", "116611", "116612", "116614",
//  				"116615", "116616", "116617", "116621",
//  				"116624", "116626", "116628", "116629",
//  				"116630", "116634", "116635", "116636",
//  				"116637", "116639", "116640", "116641",
//  				"116646", "116647", "116650", "116651",
//  				"116652", "116656", "116658", "116659",
//  				"116660", "116662", "116663" };

//     const int numDirs = 239;
//     const char* dir[numDirs] = { "109762", "109763", "109764", "109765",
//  				"109766", "109770", "109771", "109772",
//  				"109774", "109777", "109778", "109781",
//  				"109782", "109788", "109804", "109807",
//  				"109810", "109812", "109813", "109814",
//  				"109815", "109816", "109819", "109822",
//  				"109823", "109824", "109825", "109826",
//  				"109827", "109952", "109953", "109957",
//  				"109959", "109960", "109961", "109962",
//  				"109963", "109964", "109966", "109967",
//  				"109969", "109971", "109972", "109974",
//  				"109976", "109977", "109978", "109979",
//  				"109981", "109983", "109985", "109986",
//  				"109987", "109988", "109990", "109992",
//  				"109993", "109994", "109996", "109997",
//  				"109999", "110000", "110001", "110004",
//  				"110005", "110007", "110008", "110010",
//  				"110011", "110012", "110013", "110015",
//  				"110016", "110018", "110020", "110021",
//  				"110023", "110024", "110026", "110027",
//  				"110028", "110030", "110032", "110033",
//  				"110034", "110035", "110038", "110039",
//  				"110040", "110041", "110042", "110043",
//  				"110044", "110046", "110048", "110049",
//  				"110050", "110053", "110055", "110056",
//  				"110060", "110061", "110062", "110063",
//  				"110066", "110068", "110069", "110070",
//  				"110071", "110073", "110074", "110075",
//  				"110076", "110077", "110078", "110079",
//  				"110081", "110082", "110087", "110088",
//  				"110090", "110092", "110095", "110097",
//  				"110098", "110099", "110101", "110102",
//  				"110104", "110105", "110110", "110111",
//  				"110112", "110113", "110116", "110117",
//  				"110118", "110119", "110120", "110121",
//  				"110124", "110127", "110128", "110129",
//  				"110130", "110133", "110135", "110136",
//  				"110137", "110138", "110143", "110144",
//  				"110145", "110146", "110147", "110149",
//  				"110151", "110152", "110153", "110154",
//  				"110155", "110156", "110160", "110161",
//  				"110162", "110164", "110165", "110167",
//  				"110169", "110170", "110171", "110172",
//  				"110178", "110179", "110180", "110181",
//  				"110182", "110183", "110185", "110186",
//  				"110187", "110188", "110190", "110191",
//  				"110195", "110197", "110198", "110199",
//  				"110200", "110201", "110202", "110203",
//  				"110204", "110205", "110206", "110209",
//  				"110210", "110211", "110212", "110213",
//  				"110215", "110216", "110217", "110219",
//  				"110223", "110225", "110227", "110228",
//  				"110229", "110230", "110232", "110233",
//  				"110234", "110235", "110238", "110239",
//  				"110240", "110242", "110243", "110244",
//  				"110247", "110248", "110249", "110251",
//  				"110252", "110253", "110255", "110256",
//  				"110257", "110259", "110260", "110261",
//  				"110262", "110264", "110265", "110267",
//  				"110268", "110270", "110278" };

   int number_of_hits( 0 );

   // loop over files
   for ( int i = 0;  i < numDirs;  i++ ) {
      if ( m_maxHits.value() != -1.  &&  number_of_hits >= m_maxHits.value() ) break;

      string filename( m_baseDir.value() );
      filename += "/";
      filename += dir[i];
      filename += "/ladder";
//      filename += "/nocath_sto/ladder";
      filename += num;
      filename += ".eskim";
//        report( INFO, kFacilityString ) << "filename is " << filename << endl;

      DABoolean filegood( true );

      FILE* file = fopen( filename.c_str(), "rb" );
      if ( file == NULL ) filegood = false;
//        else
//  	 report( INFO, kFacilityString ) << "opened file " << filename << endl;

      // loop over tracks within a file
      NextInFile next;

      if ( filegood  &&  fread( &next, sizeof( NextInFile ), 1, file ) != 1 )
	 filegood = false;
//        report( INFO, kFacilityString ) << "read record " << next << endl;

      while ( filegood ) {
//  	 report( INFO, kFacilityString ) << "record was a " << next << "..." << endl;

	 if ( next != kTrack ) {
	    filegood = false;
	    break;
	 }

//  	 report( INFO, kFacilityString ) << next << " is a track record" << endl;

	 double curvature( 0. );
	 double phi0( 0. );
	 double d0( 0. );
	 double cotTheta( 0. );
	 double z0( 0. );

	 DABoolean errorMatrixOK;

	 double mat_cc, mat_cp, mat_cd, mat_ct, mat_cz;
	 double mat_pp, mat_pd, mat_pt, mat_pz;
	 double mat_dd, mat_dt, mat_dz;
	 double mat_tt, mat_tz;
	 double mat_zz;

	 DABoolean has_rphi( false );
	 CalibratedSVRphiHit::Identifier rphi_identifier;
	 SmallCount rphi_parent;
	 SmallCount rphi_hybrid;
	 SmallCount rphi_layer;
	 SmallCount rphi_sensor;
	 SmallCount rphi_width;
	 CalibratedSVRphiHit::PicoCoul rphi_charge;
	 CalibratedSVRphiHit::PicoCoul rphi_charge_error;
	 float rphi_coord;
	 float rphi_coord_error;
	 CalibratedSVRphiHit::QualHit rphi_quality;
	 DABoolean rphi_saturated;

	 DABoolean has_z( false );
	 CalibratedSVZHit::Identifier z_identifier;
	 SmallCount z_parent;
	 SmallCount z_hybrid;
	 SmallCount z_layer;
	 SmallCount z_sensor;
	 SmallCount z_width;
	 CalibratedSVZHit::PicoCoul z_charge;
	 CalibratedSVZHit::PicoCoul z_charge_error;
	 float z_coord;
	 float z_coord_error;
	 CalibratedSVZHit::QualHit z_quality;
	 DABoolean z_saturated;

	 if ( fread( &curvature, sizeof( double ), 1, file ) != 1  ||
	      fread( &phi0, sizeof( double ), 1, file ) != 1       ||
	      fread( &d0, sizeof( double ), 1, file ) != 1         ||
	      fread( &cotTheta, sizeof( double ), 1, file ) != 1   ||
	      fread( &z0, sizeof( double ), 1, file ) != 1         ||

	      fread( &errorMatrixOK, sizeof( DABoolean ), 1, file ) != 1  ||

	      fread( &mat_cc, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_cp, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_cd, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_ct, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_cz, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_pp, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_pd, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_pt, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_pz, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_dd, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_dt, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_dz, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_tt, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_tz, sizeof( double ), 1, file ) != 1     ||
	      fread( &mat_zz, sizeof( double ), 1, file ) != 1        ) {
	    filegood = false;
	    break;
	 }
//  	 report( INFO, kFacilityString )
//  	    << "read a track "
//  	    << curvature << " " << phi0 << " " << d0 << " " << cotTheta << " " << z0
//  	    << ", emOK = " << errorMatrixOK << endl;
	 
	 if ( fread( &next, sizeof( NextInFile ), 1, file ) != 1 ) {
	    filegood = false;
	    break;
	 }
//  	 report( INFO, kFacilityString ) << "  read record " << next << endl;
	 
	 if ( next != kSVRphiHit  &&  next != kSVZHit ) break;
//  	 report( INFO, kFacilityString ) << "  " << next << " is a hit record" << endl;

	 do {
	    if ( next == kSVRphiHit ) {
	       has_rphi = true;
//  	       report( INFO, kFacilityString ) << "  in fact, it is an rphi hit record" << endl;

	       if ( fread( &rphi_identifier, sizeof( CalibratedSVRphiHit::Identifier ), 1, file ) != 1   ||
		    fread( &rphi_parent, sizeof( SmallCount ), 1, file ) != 1                            ||
		    fread( &rphi_hybrid, sizeof( SmallCount ), 1, file ) != 1                            ||
		    fread( &rphi_layer, sizeof( SmallCount ), 1, file ) != 1                             ||
		    fread( &rphi_sensor, sizeof( SmallCount ), 1, file ) != 1                            ||
		    fread( &rphi_width, sizeof( SmallCount ), 1, file ) != 1                             ||
		    fread( &rphi_charge, sizeof( CalibratedSVRphiHit::PicoCoul ), 1, file ) != 1         ||
		    fread( &rphi_charge_error, sizeof( CalibratedSVRphiHit::PicoCoul ), 1, file ) != 1   ||
		    fread( &rphi_coord, sizeof( float ), 1, file ) != 1                                  ||
		    fread( &rphi_coord_error, sizeof( float ), 1, file ) != 1                            ||
		    fread( &rphi_quality, sizeof( CalibratedSVRphiHit::QualHit ), 1, file ) != 1         ||
		    fread( &rphi_saturated, sizeof( DABoolean ), 1, file ) != 1                             ) {
		  filegood = false;
		  break;
	       }

//  	       report( INFO, kFacilityString )
//  		  << "  read an rphi hit "
//  		  << rphi_identifier << " " << rphi_parent << " " << rphi_hybrid << " "
//  		  << rphi_layer << " " << rphi_sensor << " " << rphi_width << " "
//  		  << rphi_charge << " " << rphi_charge_error << " " << rphi_coord << " "
//  		  << rphi_coord_error << " " << rphi_quality << " " << rphi_saturated << endl;

	       number_of_hits++;
	    } // end if rphi
	    else if ( next == kSVZHit ) {
	       has_z = true;
//  	       report( INFO, kFacilityString ) << "  in fact, it is an z hit record" << endl;

	       if ( fread( &z_identifier, sizeof( CalibratedSVZHit::Identifier ), 1, file ) != 1      ||
		    fread( &z_parent, sizeof( SmallCount ), 1, file ) != 1                            ||
		    fread( &z_hybrid, sizeof( SmallCount ), 1, file ) != 1                            ||
		    fread( &z_layer, sizeof( SmallCount ), 1, file ) != 1                             ||
		    fread( &z_sensor, sizeof( SmallCount ), 1, file ) != 1                            ||
		    fread( &z_width, sizeof( SmallCount ), 1, file ) != 1                             ||
		    fread( &z_charge, sizeof( CalibratedSVZHit::PicoCoul ), 1, file ) != 1            ||
		    fread( &z_charge_error, sizeof( CalibratedSVZHit::PicoCoul ), 1, file ) != 1      ||
		    fread( &z_coord, sizeof( float ), 1, file ) != 1                                  ||
		    fread( &z_coord_error, sizeof( float ), 1, file ) != 1                            ||
		    fread( &z_quality, sizeof( CalibratedSVZHit::QualHit ), 1, file ) != 1            ||
		    fread( &z_saturated, sizeof( DABoolean ), 1, file ) != 1                             ) {
		  filegood = false;
		  break;
	       }

//  	       report( INFO, kFacilityString )
//  		  << "  read a z hit "
//  		  << z_identifier << " " << z_parent << " " << z_hybrid << " "
//  		  << z_layer << " " << z_sensor << " " << z_width << " "
//  		  << z_charge << " " << z_charge_error << " " << z_coord << " "
//  		  << z_coord_error << " " << z_quality << " " << z_saturated << endl;

	       number_of_hits++;
	    } // end if z

	    if ( fread( &next, sizeof( NextInFile ), 1, file ) != 1 ) {
	       filegood = false;
	       break;
	    }
//  	    report( INFO, kFacilityString ) << "  read record " << next << endl;

	 } while ( next == kSVRphiHit  ||  next == kSVZHit );

	 // now do stuff with them
	 if ( filegood ) {
//  	    report( INFO, kFacilityString ) << "about to write to hsam..." << endl;

	    if ( has_rphi && has_z )
	       m_hsam_store = new HSAM_StoreBoth(
		  m_hsam_store,
		  curvature, phi0, d0, cotTheta, z0,
		  mat_cc, mat_cp, mat_cd, mat_ct, mat_cz,
		  mat_pp, mat_pd, mat_pt, mat_pz,
		  mat_dd, mat_dt, mat_dz,
		  mat_tt, mat_tz,
		  mat_zz,

		  rphi_identifier, rphi_parent, rphi_hybrid, rphi_layer, rphi_sensor, rphi_width,
		  rphi_charge, rphi_charge_error, rphi_coord, rphi_coord_error, rphi_quality, rphi_saturated,

		  z_identifier, z_parent, z_hybrid, z_layer, z_sensor, z_width,
		  z_charge, z_charge_error, z_coord, z_coord_error, z_quality, z_saturated
		  );
	    else if ( has_rphi )
	       m_hsam_store = new HSAM_StoreSVRonly(
		  m_hsam_store,
		  curvature, phi0, d0, cotTheta, z0,
		  mat_cc, mat_cp, mat_cd, mat_ct, mat_cz,
		  mat_pp, mat_pd, mat_pt, mat_pz,
		  mat_dd, mat_dt, mat_dz,
		  mat_tt, mat_tz,
		  mat_zz,

		  rphi_identifier, rphi_parent, rphi_hybrid, rphi_layer, rphi_sensor, rphi_width,
		  rphi_charge, rphi_charge_error, rphi_coord, rphi_coord_error, rphi_quality, rphi_saturated
		  );
	    else if ( has_z )
	       m_hsam_store = new HSAM_StoreSVZonly(
		  m_hsam_store,
		  curvature, phi0, d0, cotTheta, z0,
		  mat_cc, mat_cp, mat_cd, mat_ct, mat_cz,
		  mat_pp, mat_pd, mat_pt, mat_pz,
		  mat_dd, mat_dt, mat_dz,
		  mat_tt, mat_tz,
		  mat_zz,

		  z_identifier, z_parent, z_hybrid, z_layer, z_sensor, z_width,
		  z_charge, z_charge_error, z_coord, z_coord_error, z_quality, z_saturated
		  );

//  	    report( INFO, kFacilityString ) << "wrote to hsam!" << endl;

	 } // end if filegood

	 if ( m_maxHits.value() != -1.  &&  number_of_hits >= m_maxHits.value() ) break;
      } // end while filegood
      fclose( file );
   } // end loop over files

   return true;
}

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
DiagnoseHits2::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
DiagnoseHits2::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
DiagnoseHits2::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   int ladder( 1 );
   for ( ; ladder <= 61;  ladder++ ) {
      m_posZ_rphires_v_phi[ladder-1] = NULL;
      m_negZ_rphires_v_phi[ladder-1] = NULL;
   }

   for ( ladder = m_diagLadderStart.value();  ladder <= m_diagLadderStop.value();  ladder++ ) {
      char num[3];
      sprintf( num, "%02d", ladder );

      m_posZ_rphires_v_phi[ladder-1] = iHistoManager.profile(
	 1000 + ladder, ( string( "posZ rphi residual vs phi. ladder " ) + string( num ) ).c_str(),
	 1000, 0., 2 * 3.1415926,
	 -m_rphiCut.value(), m_rphiCut.value(), HIHistProf::kErrorOnMean );

      m_negZ_rphires_v_phi[ladder-1] = iHistoManager.profile(
	 2000 + ladder, ( string( "negZ rphi residual vs phi. ladder " ) + string( num ) ).c_str(),
	 1000, 0., 2 * 3.1415926,
	 -m_rphiCut.value(), m_rphiCut.value(), HIHistProf::kErrorOnMean );

      m_posZ_zres_v_phi[ladder-1] = iHistoManager.profile(
	 3000 + ladder, ( string( "posZ z residual vs phi. ladder " ) + string( num ) ).c_str(),
	 1000, 0., 2 * 3.1415926,
	 -m_zCut.value(), m_zCut.value(), HIHistProf::kErrorOnMean );

      m_negZ_zres_v_phi[ladder-1] = iHistoManager.profile(
	 4000 + ladder, ( string( "negZ z residual vs phi. ladder " ) + string( num ) ).c_str(),
	 1000, 0., 2 * 3.1415926,
	 -m_zCut.value(), m_zCut.value(), HIHistProf::kErrorOnMean );
   }

   m_posZ_rphires_v_phi_all = iHistoManager.profile(
      1099, "posZ rphi residual vs phi",
      1000, 0., 2 * 3.1415926,
      -m_rphiCut.value(), m_rphiCut.value(), HIHistProf::kErrorOnMean );

   m_negZ_rphires_v_phi_all = iHistoManager.profile(
      2099, "negZ rphi residual vs phi",
      1000, 0., 2 * 3.1415926,
      -m_rphiCut.value(), m_rphiCut.value(), HIHistProf::kErrorOnMean );

   m_posZ_zres_v_phi_all = iHistoManager.profile(
      3099, "posZ z residual vs phi",
      1000, 0., 2 * 3.1415926,
      -m_zCut.value(), m_zCut.value(), HIHistProf::kErrorOnMean );

   m_negZ_zres_v_phi_all = iHistoManager.profile(
      4099, "negZ z residual vs phi",
      1000, 0., 2 * 3.1415926,
      -m_zCut.value(), m_zCut.value(), HIHistProf::kErrorOnMean );
}

void DiagnoseHits2::doladder( int ladder, Frame& iFrame )
{
   FAItem< ASiStore > store;
   extract( iFrame.record( Stream::kSVAlignment ), store );

   HISiSurfaceFactory siSurfaceFactory( iFrame, NULL, NULL, false );

   report( NOTICE, kFacilityString )
      << "Plotting hits for ladder " << ladder << endl;

   HSAM_Store* hsam_iter;

   int count( 0 );

   for ( hsam_iter = m_hsam_store;
	 hsam_iter != NULL;
	 hsam_iter = hsam_iter->next() ) {

      vector< const CalibratedSVRphiHit* > rphi_hits;
      vector< const CalibratedSVZHit* > z_hits;

      // This helix is mine. I can swim it, but I need to remember to delete it.
      HIHelix* helix( hsam_iter->unsafe_helix() );

      // put things in a form that HISiSurfaceFactory likes
      // (this is all just a little silly since there is only ever one hit)

      switch ( hsam_iter->kind() ) {
	 case HSAM_Store::kNone:
	    assert( 0 );
	    break;

	 case HSAM_Store::kSVRonly:
	    rphi_hits.push_back( ((HSAM_StoreSVRonly*)(hsam_iter))->rphi_hit() );
	    break;

	 case HSAM_Store::kSVZonly:
	    z_hits.push_back( ((HSAM_StoreSVZonly*)(hsam_iter))->z_hit() );
	    break;

	 case HSAM_Store::kBoth:
	    rphi_hits.push_back( ((HSAM_StoreBoth*)(hsam_iter))->rphi_hit() );
	    z_hits.push_back( ((HSAM_StoreBoth*)(hsam_iter))->z_hit() );
	    break;
      }	    

      siSurfaceFactory.generateCenterSurfacesWithHits( rphi_hits, z_hits, NULL );

      vector< HIIntersectionSurface* > surfaces;
      vector< HIIntersectionSurface* >::const_iterator surfaces_iter;
      siSurfaceFactory.transferSurfacesTo( surfaces, false );
  
      vector< HIIntersectionSurface* > composite_surface;
      composite_surface.push_back( (* surfaces.begin()) );
      for ( surfaces_iter = surfaces.begin(), surfaces_iter++;
 	    surfaces_iter != surfaces.end();
 	    surfaces_iter++ ) {
 	 HICompositeSurface* tmp_surface
 	    = ( (HICompositeSurface*) (* surfaces_iter) );
 	 ( (HICompositeSurface*) (* composite_surface.begin()) )
 	    ->addChildren( *tmp_surface );
      } // end loop over surfaces

      HIHelixIntersector intersector( composite_surface,
 				      HIHelixIntersector::kIncreasingRadius,
 				      helix );

      for ( HIHelixIntersector::IntersectionStatus status =
	       intersector.swimToCurrentSurface(
		  KTMoveControl::kDirectionForward );
	    status == HIHelixIntersector::kIntersectionOK;
	    status = intersector.swimToNextIntersection(
	       KTMoveControl::kDirectionForward ) )
      {
	 HIIntersectionSurface* surface = intersector.currentSurface();
	 do { 
//  	    report( INFO, kFacilityString ) << "foreach hit" << endl;

	    double phi( helix->phi0() );
	    double trackz( helix->position().z() );
	    HepSymMatrix errorMatrix( helix->errorMatrix() );

	    const CalibratedHit* hit = surface->calibratedHit();
	    int sensor( ( (CalibratedSVHit*) (hit) )->sensor() );
	    int ladder( store->ladderForSensor( sensor ) );
	    int layer( store->layerForSensor( sensor ) );

	    double residual( surface->dca( *helix ) );

// DON'T DO THIS.	    
//  	    // handle weird sign conventions preemptively
//  	    double middleish( -0.005 );
//  	    if ( ( layer == 1  ||  layer == 3 ) && ( ladder % 2 == 0 ) )
//  	       middleish *= 1.;
//  	    if ( store->sensor( sensor ).coorInSi().z() < middleish )
//  	       residual *= 1.;
// end DON'T DO THIS.	    

//  	    // The rest of this is needed to calculate the residual derivative
//  	    const ASiSensorInfo& sensorInfo( store->sensor( sensor ) );
//  	    HepVector3D measurement_direction;
//  	    if ( hit->deviceType() == CalibratedHit::SVR )
//  	       measurement_direction = -sensorInfo.unitVectorXInSi();
//  	    else if ( hit->deviceType() == CalibratedHit::SVZ )
//  	       measurement_direction = sensorInfo.unitVectorZInSi();
//  	    else assert( 0 );

//  	    HepVector3D X( helix->position() - helix->referencePoint() );
//  	    HepVector3D P( helix->momentumUnit() );
//  	    HepVector3D N( sensorInfo.unitVectorYInSi() );
//  	    double nDotPInv = 1. / N.dot( P );
//  	    double pt = P.perp();
//  	    HepVector3D zCrossPUnit( -P.y() / pt, P.x() / pt, 0. );

//  	    HepVector derivs( HIHelix::kNTrackParameters, 0 );

//  	    derivs( HIHelix::kD0 ) =
//  	       measurement_direction.dot(
//  		  zCrossPUnit - ( N.x() * zCrossPUnit.x() +
//  				  N.y() * zCrossPUnit.y() ) * nDotPInv * P );

//  	    derivs( HIHelix::kZ0 ) =
//  	       measurement_direction.z() - N.z() * nDotPInv *
//  	       measurement_direction.dot( P );

//  // Do residual_errs by hand for speed (like Kalman does)
//  //    	    double residual_err2( errorMatrix.similarity( derivs ) );
//  	    double residual_err2( 0. );

//  	    HepVector VD( HIHelix::kNTrackParameters, 0 );
//  	    for ( int m = 1;  m <= HIHelix::kNTrackParameters;  m++ )
//  	       for ( int n = 1;  n <= HIHelix::kNTrackParameters;  n++ )
//  		  VD( m ) += errorMatrix( m, n ) * derivs( n );
//  	    for ( int i = 1;  i <= HIHelix::kNTrackParameters;  i++ ) {
//  	       double derivi = derivs( i );
//  	       residual_err2 += sqr( derivi ) * errorMatrix.fast( i, i );

//  	       for ( int j = i + 1;  j <= HIHelix::kNTrackParameters;  j++ )
//  		  residual_err2 += 2. * derivi * derivs( j ) * errorMatrix.fast( j, i );
//  	    }
//  // end Do residual_errs by hand
	    
	    ////////////////////////// if this is an RPHI hit
	    if ( hit->deviceType() == CalibratedHit::SVR ) {
	       const CalibratedSVRphiHit* rphi_hit = ((CalibratedSVRphiHit*) hit);

	       count++;

//  	       report( INFO, kFacilityString )
//  		  << "rphi residual = " << residual
//  		  << " +/- " << sqrt( residual_err2 )
//  		  << " = " << residual / sqrt( residual_err2 )
//  		  << " sigmas" << endl;

	       if ( trackz > m_pthresh.value() ) {
		  m_posZ_rphires_v_phi[ladder-1]->fill( phi, residual );
		  m_posZ_rphires_v_phi_all->fill( phi, residual );
	       }
	       if ( trackz < m_nthresh.value() ) {
		  m_negZ_rphires_v_phi[ladder-1]->fill( phi, residual );
		  m_negZ_rphires_v_phi_all->fill( phi, residual );
	       }

	    ////////////////////////// if this is a Z hit
	    } else if ( hit->deviceType() == CalibratedHit::SVZ ) {

	       if ( trackz > m_pthresh.value() ) {
		  m_posZ_zres_v_phi[ladder-1]->fill( phi, residual );
		  m_posZ_zres_v_phi_all->fill( phi, residual );
	       }
	       if ( trackz < m_nthresh.value() ) {
		  m_negZ_zres_v_phi[ladder-1]->fill( phi, residual );
		  m_negZ_zres_v_phi_all->fill( phi, residual );
	       }

	    } else { assert( 0 ); }

	 } while ( surface->advanceToNextCalibratedHit( *helix ) );
      } // end (for) loop over surfaces

      vector< HIIntersectionSurface* >::const_iterator surfaces_end = surfaces.end();
      for ( surfaces_iter = surfaces.begin();
	    surfaces_iter != surfaces_end;
	    surfaces_iter++ ) {
	 (* surfaces_iter)->deleteCondition();
	 delete (* surfaces_iter );
      }
      delete helix;

   } // end loop over tracks/hits

   report( NOTICE, kFacilityString ) << "Done filling histograms (" << count << " hits)." << endl;
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
DiagnoseHits2::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   if ( ! m_done ) {
      for ( int ladder = int( floor( m_diagLadderStart.value() ) );
	    ladder <= int( floor( m_diagLadderStop.value() ) );
	    ladder++ ) {

	 switchToContext( ladder );
	 doladder( ladder, iFrame );
      }

      m_done = true;
   }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
DiagnoseHits2::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
DiagnoseHits2::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

//
// const member functions
//

//
// static member functions
//
