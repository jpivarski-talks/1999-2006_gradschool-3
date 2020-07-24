// -*- C++ -*-
//
// Package:     <HandSiAlignmentMod>
// Module:      HandSiAlignmentMod
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
#include "Experiment/report.h"
#include "HandSiAlignmentMod/HandSiAlignmentMod.h"
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

#include <fstream.h>
#include <iostream.h>
#include <stdio.h>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "HandSiAlignmentMod.HandSiAlignmentMod" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule.cc,v 1.2 2000/12/04 19:11:05 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

const double fakePrecision = 0.000001;

//
// static data member definitions
//

//
// constructors and destructor
//
HandSiAlignmentMod::HandSiAlignmentMod()
   : FrameIteratorModuleBase( "HandSiAlignmentMod", "This is my module" )
   , m_const_global( new DBSVAlignmentGlobal[1], 1 )
   , m_const_layer(  new DBSVAlignmentLayer[4], 4 )
   , m_const_ladder( new DBSVAlignmentLadder[61], 61 )
   , m_const_sensor( new DBSVAlignmentSensor[447], 447 )
   , m_holder_global( &m_const_global )
   , m_holder_layer(  &m_const_layer )
   , m_holder_ladder( &m_const_ladder )
   , m_holder_sensor( &m_const_sensor )
   , m_doinits( "doinits", this )
   , m_replot( "replot", this )
   , m_pickLadder( "ladder", this )
   , m_fakeLadder( "fake", this )
   , m_moveX( "x", this, kDeltaX )
   , m_moveY( "y", this, kDeltaY )
   , m_moveZ( "z", this, kDeltaZ )
   , m_movePhiX( "phix", this, kPhiX )
   , m_movePhiY( "phiy", this, kPhiY )
   , m_movePhiZ( "phiz", this, kPhiZ )
   , m_moveSagitta( "sagitta", this, kSagitta )
   , m_pthresh( "pthresh", this, 0., ThreshCommand::kPosThresh )
   , m_nthresh( "nthresh", this, 0., ThreshCommand::kNegThresh )
   , m_restrictSensor( "sensor", this )
   , m_baseDir( "base", this, "../enderskim/" )
   , m_maxHits( "maxHits", this, -1. )
   , m_globalFile( "globalFile", this, "state.svalignmentglobal" )
   , m_layerFile( "layerFile", this, "state.svalignmentlayer" )
   , m_ladderFile( "ladderFile", this, "state.svalignmentladder" )
   , m_sensorFile( "sensorFile", this, "state.svalignmentsensor" )
   , m_sagittaFile( "sagittaFile", this, "state.sagitta" )
   , m_rphiCut( "rphiCut", this, 0.0002 )
   , m_zCut( "zCut", this, 0.005 )
   , m_fakeNumPhi0( "fakeNumPhi0", this, 80. )
   , m_fakeNumCotTheta( "fakeNumCotTheta", this, 20. )
   , m_ladder( 0 )
   , m_faking( false )
   , m_hsam_store( NULL )
   , m_jc( NULL )
   , m_hm( NULL )
   , m_app( NULL )
   , m_window( NULL )
{
   //register the Proxy Factory created by the FIHolder

   registerProxyFactory(
      Stream::kSVAlignment, m_holder_global.makeFactory(), UsageTag() );

   registerProxyFactory(
      Stream::kSVAlignment, m_holder_layer.makeFactory(),  UsageTag() );

   registerProxyFactory(
      Stream::kSVAlignment, m_holder_ladder.makeFactory(), UsageTag() );

   registerProxyFactory(
      Stream::kSVAlignment, m_holder_sensor.makeFactory(), UsageTag() );

   //You must set what streams you which to iterate over
   //  that is, what events should the Frames be stopped on
   iterateOver( Stream::kEvent );
}

// HandSiAlignmentMod::HandSiAlignmentMod( const HandSiAlignmentMod& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HandSiAlignmentMod::~HandSiAlignmentMod()
{
}

//
// assignment operators
//
// const HandSiAlignmentMod& HandSiAlignmentMod::operator=( const HandSiAlignmentMod& rhs )
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

void HandSiAlignmentMod::deleteTracksHits()
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

static int argc=1;
static char* argv[] ={"HistogramViewer"};

DABoolean
HandSiAlignmentMod::doinits()
{
   if ( m_hm != NULL ) return false;

   report( NOTICE, kFacilityString )
      << "Reading initial constants from files..." << endl;

   ifstream global_file( m_globalFile.value().c_str() );
   if ( ! m_const_global.readFromStream( global_file ) ) {
      report( ERROR, kFacilityString )
	 << "Couldn't read from file " << m_globalFile.value().c_str()
	 << "! Reset and try again." << endl;
      return false;
   }

   ifstream layer_file( m_layerFile.value().c_str() );
   if ( ! m_const_layer.readFromStream( layer_file ) ) {
      report( ERROR, kFacilityString )
	 << "Couldn't read from file " << m_layerFile.value().c_str()
	 << "! Reset and try again." << endl;
      return false;
   }

   ifstream ladder_file( m_ladderFile.value().c_str() );
   if ( ! m_const_ladder.readFromStream( ladder_file ) ) {
      report( ERROR, kFacilityString )
	 << "Couldn't read from file " << m_ladderFile.value().c_str()
	 << "! Reset and try again." << endl;
      return false;
   }

   ifstream sensor_file( m_sensorFile.value().c_str() );
   if ( ! m_const_sensor.readFromStream( sensor_file ) ) {
      report( ERROR, kFacilityString )
	 << "Couldn't read from file " << m_sensorFile.value().c_str()
	 << "! Reset and try again." << endl;
      return false;
   }

   ifstream sagitta_file( m_sagittaFile.value().c_str() );
   double value;
   for ( int i = 0;  i < 61;  i++ )
      if ( sagitta_file >> value )
	 m_sagitta[i] = m_original_sagitta[i] = value;
      else {
	 report( ERROR, kFacilityString )
	    << "Couldn't read from file " << m_sagittaFile.value().c_str()
	    << "! Reset and try again." << endl;
	 return false;
      }

   int which;
   for ( which = 0;  which < 61;  which++ ) {
      m_original_ladder_x[which] = m_const_ladder[which].get_deltaX();
      m_original_ladder_y[which] = m_const_ladder[which].get_deltaY();
      m_original_ladder_z[which] = m_const_ladder[which].get_deltaZ();
      m_original_ladder_phix[which] = m_const_ladder[which].get_phiX();
      m_original_ladder_phiy[which] = m_const_ladder[which].get_phiY();
      m_original_ladder_phiz[which] = m_const_ladder[which].get_phiZ();
   }
   for ( which = 0;  which < 447;  which++ ) {
      m_original_sensor_x[which] = m_const_sensor[which].get_deltaX();
      m_original_sensor_y[which] = m_const_sensor[which].get_deltaY();
      m_original_sensor_z[which] = m_const_sensor[which].get_deltaZ();
      m_original_sensor_phix[which] = m_const_sensor[which].get_phiX();
      m_original_sensor_phiy[which] = m_const_sensor[which].get_phiY();
      m_original_sensor_phiz[which] = m_const_sensor[which].get_phiZ();
   }

   report( NOTICE, kFacilityString )
      << "Setting up histograms and HistogramViewer..." << endl;

   m_jc = JobControl::instance();
   m_hm = m_jc->histogramManagerP();

   m_align = m_hm->mkdir( "align" );
   m_smiles = m_hm->mkdir( "smiles" );
   m_other = m_hm->mkdir( "other" );
   
//     m_rphiview = m_hm->histogram(
//        1, "rphi view", 500, -0.12, 0.12, 500, -0.12, 0.12 );
//     m_zview = m_hm->histogram(
//        2, "z view", 100, -0.25, 0.25 );

   m_posZ_rphiVx = m_align->profile(
      100, "posZ rphi V x", 50, -0.0124, 0.0124, -m_rphiCut.value(), m_rphiCut.value(), HIHistProf::kSpread );
   m_negZ_rphiVx = m_align->profile(
      200, "negZ rphi V x", 50, -0.0124, 0.0124, -m_rphiCut.value(), m_rphiCut.value(), HIHistProf::kSpread );
   m_rphi_trackz = m_align->histogram(
      300, "rphi trackZ", 200, -0.2, 0.2 );
   m_rphi_matchedz = m_other->histogram(
      301, "rphi matchedZ", 200, -0.2, 0.2 );

   m_w_posZ_rphiVx = m_align->profile(
      150, "w posZ rphi V x", 50, -0.0124, 0.0124, -m_rphiCut.value(), m_rphiCut.value(),
      HIHistProf::kWeightedError );
   m_w_negZ_rphiVx = m_align->profile(
      250, "w negZ rphi V x", 50, -0.0124, 0.0124, -m_rphiCut.value(), m_rphiCut.value(),
      HIHistProf::kWeightedError );

   m_posZ_rphiVx_scat = m_other->histogram(
      101, "posZ rphi V x (scat)", 25, -0.0125, 0.0125, 25, -m_rphiCut.value(), m_rphiCut.value() );
   m_negZ_rphiVx_scat = m_other->histogram(
      201, "negZ rphi V x (scat)", 25, -0.0125, 0.0125, 25, -m_rphiCut.value(), m_rphiCut.value() );
   m_rphi_hitx = m_other->histogram(
      302, "rphi hitX", 200, -0.0125, 0.0125 );
   m_rphiVmatchedz = m_other->profile(
      400, "rphi V matchedz", 250, -0.2, 0.2, -m_rphiCut.value(), m_rphiCut.value(), HIHistProf::kSpread );
   m_rphiVmatchedz_scat = m_other->histogram(
      401, "rphi V matchedz (scat)", 25, -0.2, 0.2, 25, -m_rphiCut.value(), m_rphiCut.value() );
   m_rphiVtrackz = m_align->profile(
      402, "rphi V trackz", 250, -0.2, 0.2, -m_rphiCut.value(), m_rphiCut.value(), HIHistProf::kSpread );

   m_rphiVtrackz_scat = m_other->histogram(
      403, "rphi V trackz (scat)", 25, -0.2, 0.2, 25, -m_rphiCut.value(), m_rphiCut.value() );
   m_w_rphiVtrackz = m_align->profile(
      452, "w rphi V trackz", 250, -0.2, 0.2, -m_rphiCut.value(), m_rphiCut.value(), HIHistProf::kWeightedError );
   m_zresVz = m_align->profile(
      500, "zres V z", 250, -0.2, 0.2, -m_zCut.value(), m_zCut.value(), HIHistProf::kSpread );
   m_zresVz_scat = m_other->histogram(
      501, "zres V z (scat)", 25, -0.2, 0.2, 25, -m_zCut.value(), m_zCut.value() );
   m_zresVtrackz = m_other->profile(
      502, "zres V trackz", 250, -0.2, 0.2, -m_zCut.value(), m_zCut.value(), HIHistProf::kSpread );
   m_zresVtrackz_scat = m_other->histogram(
      503, "zres V trackz (scat)", 25, -0.2, 0.2, 25, -m_zCut.value(), m_zCut.value() );
   m_w_zresVz = m_align->profile(
      550, "w zres V z", 250, -0.2, 0.2, -m_zCut.value(), m_zCut.value(), HIHistProf::kWeightedError );

   m_zresVphi_layer1 = m_smiles->profile(
      601, "zres V phi (L1)", 20, -0.55, 0.35, -m_zCut.value(), m_zCut.value(), HIHistProf::kSpread );
   m_zresVphi_layer2 = m_smiles->profile(
      602, "zres V phi (L2)", 20, -0.55, 0.15, -m_zCut.value(), m_zCut.value(), HIHistProf::kSpread );
   m_zresVphi_layer3 = m_smiles->profile(
      603, "zres V phi (L3)", 20, -0.35, 0.05, -m_zCut.value(), m_zCut.value(), HIHistProf::kSpread );
   m_zresVphi_layer4 = m_smiles->profile(
      604, "zres V phi (L4)", 20, -0.21, -0.03, -m_zCut.value(), m_zCut.value(), HIHistProf::kSpread );
   m_zresVphi_layer1_scat = m_smiles->histogram(
      611, "zres V phi (L1) (scat)", 25, -0.55, 0.35, 25, -m_zCut.value(), m_zCut.value() );
   m_zresVphi_layer2_scat = m_smiles->histogram(
      612, "zres V phi (L2) (scat)", 25, -0.55, 0.15, 25, -m_zCut.value(), m_zCut.value() );
   m_zresVphi_layer3_scat = m_smiles->histogram(
      613, "zres V phi (L3) (scat)", 25, -0.35, 0.05, 25, -m_zCut.value(), m_zCut.value() );
   m_zresVphi_layer4_scat = m_smiles->histogram(
      614, "zres V phi (L4) (scat)", 25, -0.21, -0.03, 25, -m_zCut.value(), m_zCut.value() );
   m_w_zresVphi_layer1 = m_smiles->profile(
      651, "w zres V phi (L1)", 20, -0.55, 0.35, -m_zCut.value(), m_zCut.value(), HIHistProf::kWeightedError );
   m_w_zresVphi_layer2 = m_smiles->profile(
      652, "w zres V phi (L2)", 20, -0.55, 0.15, -m_zCut.value(), m_zCut.value(), HIHistProf::kWeightedError );
   m_w_zresVphi_layer3 = m_smiles->profile(
      653, "w zres V phi (L3)", 20, -0.35, 0.05, -m_zCut.value(), m_zCut.value(), HIHistProf::kWeightedError );
   m_w_zresVphi_layer4 = m_smiles->profile(
      654, "w zres V phi (L4)", 20, -0.21, -0.03, -m_zCut.value(), m_zCut.value(), HIHistProf::kWeightedError );

   m_app = new QApplication( argc, &(argv[0]) );
   m_window = new HVPWindow( m_hm );

   return true;
}

DABoolean HandSiAlignmentMod::replot()
{
   if ( m_window == NULL  ||  m_app == NULL ) {
      report( ERROR, kFacilityString )
	 << "You need to doinits first." << endl;
      return false;
   }

   if ( m_ladder < 1  ||  m_ladder > 61 ) {
      report( ERROR, kFacilityString )
	 << "You need to choose a ladder first." << endl;
      return false;
   }

   m_window->updateInfo();
   m_app->exec();

   return true;
}

DABoolean HandSiAlignmentMod::switchToContext( int ladder )
{
   deleteTracksHits();

   m_ladder = ladder;
   m_faking = false;
   report( NOTICE, kFacilityString )
      << "Loading hits for ladder " << ladder << "..." << endl;

   char num[3];
   sprintf( num, "%02d", ladder );

   // this is where to change the input directories
   const int numDirs = 139;
   const char* dir[numDirs] = { "116368", "116369", "116371", "116372",
				"116374", "116375", "116376", "116380",
				"116381", "116383", "116384", "116385",
				"116387", "116393", "116394", "116395",
				"116396", "116398", "116399", "116407",
				"116409", "116410", "116412", "116413",
				"116414", "116416", "116420", "116421",
				"116422", "116423", "116424", "116425",
				"116429", "116430", "116432", "116433",
				"116436", "116439", "116440", "116441",
				"116443", "116450", "116451", "116453",
				"116454", "116458", "116460", "116462",
				"116463", "116464", "116465", "116468",
				"116470", "116475", "116477", "116478",
				"116480", "116482", "116483", "116484",
				"116485", "116495", "116496", "116498",
				"116499", "116500", "116501", "116502",
				"116503", "116504", "116505", "116509",
				"116510", "116512", "116514", "116515",
				"116516", "116517", "116518", "116519",
				"116520", "116521", "116568", "116569",
				"116570", "116573", "116574", "116577",
				"116579", "116580", "116581", "116582",
				"116584", "116585", "116588", "116589",
				"116590", "116591", "116593", "116594",
				"116595", "116596", "116597", "116600",
				"116601", "116602", "116603", "116604",
				"116609", "116611", "116612", "116614",
				"116615", "116616", "116617", "116621",
				"116624", "116626", "116628", "116629",
				"116630", "116634", "116635", "116636",
				"116637", "116639", "116640", "116641",
				"116646", "116647", "116650", "116651",
				"116652", "116656", "116658", "116659",
				"116660", "116662", "116663" };


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
      filename += "/stereo_only/ladder";
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

void HandSiAlignmentMod::fakeContext1( int ladder )
{
   deleteTracksHits();

   m_ladder = ladder;
   m_faking = true;

   report( NOTICE, kFacilityString )
      << "Now run `iterator go' to fake the tracks and hits." << endl;
}

void HandSiAlignmentMod::fakeContext2( FAItem< ASiStore > store,
				       HISiSurfaceFactory& siSurfaceFactory )
{
   if ( m_hsam_store != NULL ) return;

   report( NOTICE, kFacilityString )
      << "Faking tracks and hits for ladder " << m_ladder << "..." << endl;

   double phimin[61];
   double phimax[61];
   double themin[61];
   double themax[61];

   phimin[0] = 1.58944;      phimax[0] = 2.06814;   themin[0] = -1.11777;    themax[0] = 1.25736;
   phimin[1] = 2.20556;      phimax[1] = 3.04003;   themin[1] = -1.25803;    themax[1] = 1.20519;
   phimin[2] = 2.94167;      phimax[2] = 3.97475;   themin[2] = -1.26097;    themax[2] = 1.25952;
   phimin[3] = 3.97475;      phimax[3] = 4.9317;    themin[3] = -1.24199;    themax[3] = 1.23686;
   phimin[4] = 4.9317;       phimax[4] = 5.79349;   themin[4] = -1.24199;    themax[4] = 1.23686;
   phimin[5] = 0.0553184;    phimax[5] = 6.26189;   themin[5] = -1.22139;    themax[5] = 1.25079;
   phimin[6] = 0.342767;     phimax[6] = 1.19522;   themin[6] = -1.11243;    themax[6] = 1.26058;
   phimin[7] = 1.43471;      phimax[7] = 1.97466;   themin[7] = -1.23606;    themax[7] = 1.15937;
   phimin[8] = 1.97466;      phimax[8] = 2.62496;   themin[8] = -1.25516;    themax[8] = 1.25736;
   phimin[9] = 2.60689;      phimax[9] = 3.27107;   themin[9] = -1.245;      themax[9] = 1.25435;
   phimin[10] = 3.25468;     phimax[10] = 3.92271;  themin[10] = -1.25545;   themax[10] = 1.25945;
   phimin[11] = 3.93046;     phimax[11] = 4.18504;  themin[11] = -1.20733;   themax[11] = 1.17097;
   phimin[12] = 4.56971;     phimax[12] = 5.16627;  themin[12] = -1.25416;   themax[12] = 1.25452;
   phimin[13] = 5.24788;     phimax[13] = 5.7954;   themin[13] = -1.24377;   themax[13] = 1.017;
   phimin[14] = 0.00106746;  phimax[14] = 6.28251;  themin[14] = -1.26411;   themax[14] = 1.26453;
   phimin[15] = 0.116371;    phimax[15] = 0.738464; themin[15] = -1.26411;   themax[15] = 1.26312;
   phimin[16] = 0.653307;    phimax[16] = 1.28297;  themin[16] = -1.21553;   themax[16] = 1.26278;
   phimin[17] = 1.43269;     phimax[17] = 1.72411;  themin[17] = -1.23606;   themax[17] = 1.18035;
   phimin[18] = 1.73656;     phimax[18] = 2.07238;  themin[18] = -1.23548;   themax[18] = -0.434935;
   phimin[19] = 2.07506;     phimax[19] = 2.44487;  themin[19] = -1.25516;   themax[19] = 1.17936;
   phimin[20] = 2.43587;     phimax[20] = 2.78089;  themin[20] = -1.25998;   themax[20] = -0.331779;
   phimin[21] = 2.7748;      phimax[21] = 3.13996;  themin[21] = -1.24321;   themax[21] = 1.25964;
   phimin[22] = 3.13044;     phimax[22] = 3.51145;  themin[22] = -1.24993;   themax[22] = 1.25746;
   phimin[23] = 3.4907;      phimax[23] = 3.85628;  themin[23] = -1.25545;   themax[23] = 1.26649;
   phimin[24] = 3.85565;     phimax[24] = 4.2032;   themin[24] = -1.25224;   themax[24] = 1.25945;
   phimin[25] = 4.23052;     phimax[25] = 4.57247;  themin[25] = -1.24149;   themax[25] = 1.14946;
   phimin[26] = 4.56886;     phimax[26] = 4.8999;   themin[26] = -1.19705;   themax[26] = 1.25452;
   phimin[27] = 4.90911;     phimax[27] = 5.25791;  themin[27] = -1.25416;   themax[27] = 1.23426;
   phimin[28] = 5.25916;     phimax[28] = 5.61551;  themin[28] = -1.25278;   themax[28] = 1.24636;
   phimin[29] = 5.61418;     phimax[29] = 5.96148;  themin[29] = -1.26128;   themax[29] = 0.30211;
   phimin[30] = 0.00262151;  phimax[30] = 6.28271;  themin[30] = -1.26411;   themax[30] = 1.26453;
   phimin[31] = 0.00106746;  phimax[31] = 6.28014;  themin[31] = -1.26411;   themax[31] = 1.26696;
   phimin[32] = 0.350637;    phimax[32] = 0.69552;  themin[32] = -1.26619;   themax[32] = 1.26481;
   phimin[33] = 0.685642;    phimax[33] = 1.06197;  themin[33] = -1.2516;    themax[33] = 1.26278;
   phimin[34] = 1.05129;     phimax[34] = 1.34629;  themin[34] = -1.23561;   themax[34] = 0.224585;
   phimin[35] = 1.43727;     phimax[35] = 1.60063;  themin[35] = 0.0881149;  themax[35] = 1.18035;
   phimin[36] = 1.72411;     phimax[36] = 1.92165;  themin[36] = -1.23548;   themax[36] = 1.12179;
   phimin[37] = 1.93189;     phimax[37] = 2.15668;  themin[37] = -1.19228;   themax[37] = 1.25736;
   phimin[38] = 2.17668;     phimax[38] = 2.41033;  themin[38] = -1.25516;   themax[38] = 1.23865;
   phimin[39] = 2.40794;     phimax[39] = 2.65413;  themin[39] = -1.25998;   themax[39] = 1.24229;
   phimin[40] = 2.64548;     phimax[40] = 2.89971;  themin[40] = -1.2527;    themax[40] = 1.13254;
   phimin[41] = 2.88409;     phimax[41] = 3.081;    themin[41] = -0.0875836; themax[41] = 1.25862;
   phimin[42] = 3.13456;     phimax[42] = 3.39375;  themin[42] = -1.25673;   themax[42] = 1.25486;
   phimin[43] = 3.37751;     phimax[43] = 3.63841;  themin[43] = -1.26829;   themax[43] = 1.26649;
   phimin[44] = 3.62528;     phimax[44] = 3.88192;  themin[44] = -1.26284;   themax[44] = 1.26108;
   phimin[45] = 3.86452;     phimax[45] = 4.12399;  themin[45] = -1.25224;   themax[45] = 1.24508;
   phimin[46] = 4.12399;     phimax[46] = 4.363;    themin[46] = -1.23312;   themax[46] = 1.24212;
   phimin[47] = 4.35031;     phimax[47] = 4.60581;  themin[47] = -1.24149;   themax[47] = 1.25452;
   phimin[48] = 4.60132;     phimax[48] = 4.82653;  themin[48] = -1.19705;   themax[48] = 1.03632;
   phimin[49] = 4.85506;     phimax[49] = 5.06549;  themin[49] = -1.12844;   themax[49] = 1.23426;
   phimin[50] = 5.09237;     phimax[50] = 5.32004;  themin[50] = -1.25416;   themax[50] = 1.23387;
   phimin[51] = 5.32525;     phimax[51] = 5.57323;  themin[51] = -1.25278;   themax[51] = 1.25079;
   phimin[52] = 5.56288;     phimax[52] = 5.81116;  themin[52] = -1.24858;   themax[52] = 1.26686;
   phimin[53] = 5.8009;      phimax[53] = 6.05056;  themin[53] = -1.26128;   themax[53] = 1.25164;
   phimin[54] = 6.03481;     phimax[54] = 6.28193;  themin[54] = -0.0120484; themax[54] = 1.26453;
   phimin[55] = 0.000846894; phimax[55] = 6.28271;  themin[55] = -1.26406;   themax[55] = 1.26427;
   phimin[56] = 0.230408;    phimax[56] = 0.47251;  themin[56] = -1.22539;   themax[56] = 1.26696;
   phimin[57] = 0.465146;    phimax[57] = 0.72254;  themin[57] = -1.26619;   themax[57] = 1.26724;
   phimin[58] = 0.711975;    phimax[58] = 0.958277; themin[58] = -1.24517;   themax[58] = 0.115727;
   phimin[59] = 0.948349;    phimax[59] = 1.18672;  themin[59] = -0.0346173; themax[59] = 1.21173;
   phimin[60] = 1.19522;     phimax[60] = 1.4338;   themin[60] = -1.23561;   themax[60] = 1.24327;

   int sensormin[61];
   int sensormax[61];

   sensormin[ 0] =   1; sensormax[ 0] =   3;
   sensormin[ 1] =   4; sensormax[ 1] =   6;
   sensormin[ 2] =   7; sensormax[ 2] =   9;
   sensormin[ 3] =  10; sensormax[ 3] =  12;
   sensormin[ 4] =  13; sensormax[ 4] =  15;
   sensormin[ 5] =  16; sensormax[ 5] =  18;
   sensormin[ 6] =  19; sensormax[ 6] =  21;
   sensormin[ 7] =  22; sensormax[ 7] =  25;
   sensormin[ 8] =  26; sensormax[ 8] =  29;
   sensormin[ 9] =  30; sensormax[ 9] =  33;
   sensormin[10] =  34; sensormax[10] =  37;
   sensormin[11] =  38; sensormax[11] =  41;
   sensormin[12] =  42; sensormax[12] =  45;
   sensormin[13] =  46; sensormax[13] =  49;
   sensormin[14] =  50; sensormax[14] =  53;
   sensormin[15] =  54; sensormax[15] =  57;
   sensormin[16] =  58; sensormax[16] =  61;
   sensormin[17] =  62; sensormax[17] =  68;
   sensormin[18] =  69; sensormax[18] =  75;
   sensormin[19] =  76; sensormax[19] =  82;
   sensormin[20] =  83; sensormax[20] =  89;
   sensormin[21] =  90; sensormax[21] =  96;
   sensormin[22] =  97; sensormax[22] = 103;
   sensormin[23] = 104; sensormax[23] = 110;
   sensormin[24] = 111; sensormax[24] = 117;
   sensormin[25] = 118; sensormax[25] = 124;
   sensormin[26] = 125; sensormax[26] = 131;
   sensormin[27] = 132; sensormax[27] = 138;
   sensormin[28] = 139; sensormax[28] = 145;
   sensormin[29] = 146; sensormax[29] = 152;
   sensormin[30] = 153; sensormax[30] = 159;
   sensormin[31] = 160; sensormax[31] = 166;
   sensormin[32] = 167; sensormax[32] = 173;
   sensormin[33] = 174; sensormax[33] = 180;
   sensormin[34] = 181; sensormax[34] = 187;
   sensormin[35] = 188; sensormax[35] = 197;
   sensormin[36] = 198; sensormax[36] = 207;
   sensormin[37] = 208; sensormax[37] = 217;
   sensormin[38] = 218; sensormax[38] = 227;
   sensormin[39] = 228; sensormax[39] = 237;
   sensormin[40] = 238; sensormax[40] = 247;
   sensormin[41] = 248; sensormax[41] = 257;
   sensormin[42] = 258; sensormax[42] = 267;
   sensormin[43] = 268; sensormax[43] = 277;
   sensormin[44] = 278; sensormax[44] = 287;
   sensormin[45] = 288; sensormax[45] = 297;
   sensormin[46] = 298; sensormax[46] = 307;
   sensormin[47] = 308; sensormax[47] = 317;
   sensormin[48] = 318; sensormax[48] = 327;
   sensormin[49] = 328; sensormax[49] = 337;
   sensormin[50] = 338; sensormax[50] = 347;
   sensormin[51] = 348; sensormax[51] = 357;
   sensormin[52] = 358; sensormax[52] = 367;
   sensormin[53] = 368; sensormax[53] = 377;
   sensormin[54] = 378; sensormax[54] = 387;
   sensormin[55] = 388; sensormax[55] = 397;
   sensormin[56] = 398; sensormax[56] = 407;
   sensormin[57] = 408; sensormax[57] = 417;
   sensormin[58] = 418; sensormax[58] = 427;
   sensormin[59] = 428; sensormax[59] = 437;
   sensormin[60] = 438; sensormax[60] = 447;

   int layer;
   if ( 1 <= m_ladder && m_ladder <= 7 ) layer = 1;
   else if ( 8 <= m_ladder && m_ladder <= 17 ) layer = 2;
   else if ( 18 <= m_ladder && m_ladder <= 35 ) layer = 3;
   else if ( 36 <= m_ladder && m_ladder <= 61 ) layer = 4;
   else assert( 0 );

   double phi0step( ( phimax[m_ladder-1] - phimin[m_ladder-1] ) / m_fakeNumPhi0.value() );
   double cotThetaStep( ( themax[m_ladder-1] - themin[m_ladder-1] ) / m_fakeNumCotTheta.value() );

   for ( double phi0 = phimin[m_ladder-1];
	 phi0 < phimax[m_ladder-1];
	 phi0 += phi0step )
      for ( double cotTheta = themin[m_ladder-1];
	    cotTheta < themax[m_ladder-1];
	    cotTheta += cotThetaStep ) {

//  	 report( INFO, kFacilityString )
//  	    << "Making a new helix at phi0 = " << phi0
//  	    << " and cotTheta = " << cotTheta << endl;

	 HepSymMatrix tmpMat(5);
	 tmpMat(1,1) = sqr( fakePrecision );
	 tmpMat(1,2) = 0.;
	 tmpMat(1,3) = 0.;
	 tmpMat(1,4) = 0.;
	 tmpMat(1,5) = 0.;
	 tmpMat(2,2) = sqr( fakePrecision );
	 tmpMat(2,3) = 0.;
	 tmpMat(2,4) = 0.;
	 tmpMat(2,5) = 0.;
	 tmpMat(3,3) = sqr( fakePrecision );
	 tmpMat(3,4) = 0.;
	 tmpMat(3,5) = 0.;
	 tmpMat(4,4) = sqr( fakePrecision );
	 tmpMat(4,5) = 0.;
	 tmpMat(5,5) = sqr( fakePrecision );

	 HIHelix helix( 0., phi0, 0., cotTheta, 0., tmpMat );

	 // find the approximate z intercept of the track with this
	 // ladder
	 HIHelix temp_helix( helix );
	 double radius( store->ladder( m_ladder ).coorInSi().perp() );
	 temp_helix.moveToRadius( radius );
	 double z( temp_helix.position().z() );
	 int sensor;
	 for ( sensor = sensormin[m_ladder-1];
	       sensor <= sensormax[m_ladder-1];
	       sensor++ ) {
	    double zcenter( store->sensor( sensor ).coorInSi().z() );
	    double zhalfwidth( store->sensor( sensor ).halfLengthZ() );
	    if ( z > zcenter - zhalfwidth  &&  z < zcenter + zhalfwidth )
	       break;
	 }

	 int hybrid( store->hybridInLadderForSensor( sensor ) );
	 if ( hybrid == ASiStore::FirstHybrid ) hybrid = m_ladder;
	 else if ( hybrid == ASiStore::SecondHybrid ) hybrid = 61 + m_ladder;
	 else assert( 0 );

	 double rphi_one( -0.00625 );
	 double rphi_two( 0.00625 );
	 double rphi_residual_one( 2000000. );
	 double rphi_residual_two( 2000000. );
	 do {
//  	    report( INFO, kFacilityString )
//  	       << "  rphi_one = " << rphi_one << ", rphi_two = " << rphi_two << endl;

	    CalibratedSVRphiHit rphi_hit_one(
	       1, 1, hybrid, layer, sensor, 1, 0., 0., rphi_one, 1., 0, false );
	    CalibratedSVRphiHit rphi_hit_two(
	       2, 1, hybrid, layer, sensor, 1, 0., 0., rphi_two, 1., 0, false );

	    if ( rphi_residual_one == 2000000. )
	       rphi_residual_one = residualizer( store, siSurfaceFactory, &helix, &rphi_hit_one, NULL );
	    rphi_residual_two = residualizer( store, siSurfaceFactory, &helix, &rphi_hit_two, NULL );

//  	    report( INFO, kFacilityString )
//  	       << "  rphi_residual_one = " << rphi_residual_one
//  	       << ", rphi_residual_two = " << rphi_residual_two << endl;

	    // linear interpolation/extrapolation to the coordinate which zeros residual
	    double rphi_next( ( rphi_one * rphi_residual_two - rphi_two * rphi_residual_one )
			      / ( rphi_residual_two - rphi_residual_one ) );

	    // but I want to iterate to account for non-linearity introduced by bowing
	    rphi_one = rphi_two;
	    rphi_two = rphi_next;

	    rphi_residual_one = rphi_residual_two;
	    rphi_residual_two = 2000000.;

	 } while ( abs( rphi_residual_one ) > fakePrecision  &&  rphi_residual_one != 1000000. );

//  	 report( INFO, kFacilityString ) << "  I'm happy with this rphi_residual_two." << endl;

	 double z_one( -0.0125 );
	 double z_two( 0.0125 );
	 double z_residual_one( 2000000. );
	 double z_residual_two( 2000000. );
	 do {
//  	    report( INFO, kFacilityString )
//  	       << "  z_one = " << z_one << ", z_two = " << z_two << endl;

	    CalibratedSVZHit z_hit_one(
	       1, 1, hybrid, layer, sensor, 1, 0., 0., z_one, 1., 0, false );
	    CalibratedSVZHit z_hit_two(
	       2, 1, hybrid, layer, sensor, 1, 0., 0., z_two, 1., 0, false );

	    if ( z_residual_one == 2000000. )
	       z_residual_one = residualizer( store, siSurfaceFactory, &helix, NULL, &z_hit_one );
	    z_residual_two = residualizer( store, siSurfaceFactory, &helix, NULL, &z_hit_two );

//  	    report( INFO, kFacilityString )
//  	       << "  z_residual_one = " << z_residual_one
//  	       << ", z_residual_two = " << z_residual_two << endl;

	    // linear interpolation/extrapolation to the coordinate which zeros residual
	    double z_next( ( z_one * z_residual_two - z_two * z_residual_one )
			   / ( z_residual_two - z_residual_one ) );

	    // but I want to iterate to account for non-linearity introduced by bowing
	    z_one = z_two;
	    z_two = z_next;

	    z_residual_one = z_residual_two;
	    z_residual_two = 2000000.;

	 } while ( abs( z_residual_one ) > fakePrecision  &&  z_residual_one != 1000000. );

//  	 report( INFO, kFacilityString ) << "  I'm happy with this z_residual_two." << endl;

	 if ( rphi_residual_one != 1000000.  &&  z_residual_one != 1000000. ) {
//  	    report( INFO, kFacilityString )
//  	       << "I'm keeping the new helix." << endl;

	    m_hsam_store = new HSAM_StoreBoth(
	       m_hsam_store,
	       0., phi0, 0., cotTheta, 0.,
	       sqr( fakePrecision ), 0., 0., 0., 0.,
	       sqr( fakePrecision ), 0., 0., 0.,
	       sqr( fakePrecision ), 0., 0.,
	       sqr( fakePrecision ), 0.,
	       sqr( fakePrecision ),
	       
	       1, 1, hybrid, layer, sensor, 1, 0., 0., rphi_one, 1., 0, false,
	       2, 1, hybrid, layer, sensor, 1, 0., 0., z_one, 1., 0, false
	       );
	    
	 } // end both residuals are okay
//  	 else {
//  	    report( INFO, kFacilityString )
//  	       << "A residual didn't converge. Better luck next time." << endl;
//  	 }

      } // end loop over tracks

}

DABoolean
HandSiAlignmentMod::move( Direction dir, double val, HandSiAlignmentMod::Action act )
{
   if ( m_ladder < 1  ||  m_ladder > 61 ) {
      report( ERROR, kFacilityString )
	 << "You need to choose a ladder first." << endl;
      return false;
   }

   report( NOTICE, kFacilityString )
      << "ladder " << m_ladder << " ";

   switch ( dir ) {
      case kDeltaX:
	 if ( act == kMove )
	    m_const_ladder[m_ladder-1].set_deltaX(
	       m_const_ladder[m_ladder-1].get_deltaX() + val/1e6 );
	 else if ( act == kReset )
	    m_const_ladder[m_ladder-1].set_deltaX(
	       m_original_ladder_x[m_ladder-1] );
	 else if ( act == kZero )
	    m_const_ladder[m_ladder-1].set_deltaX( 0. );
	 else if ( act == kRound )
	    m_const_ladder[m_ladder-1].set_deltaX(
	       rint( m_const_ladder[m_ladder-1].get_deltaX()*1e6 )/1e6 );

	 report( NOTICE, kFacilityString )
	    << "x is now " << m_const_ladder[m_ladder-1].get_deltaX()*1e6 << " microns." << endl;
	 break;
      case kDeltaY:
	 if ( act == kMove )
	    m_const_ladder[m_ladder-1].set_deltaY(
	       m_const_ladder[m_ladder-1].get_deltaY() + val/1e6 );
	 else if ( act == kReset )
	    m_const_ladder[m_ladder-1].set_deltaY(
	       m_original_ladder_y[m_ladder-1] );
	 else if ( act == kZero )
	    m_const_ladder[m_ladder-1].set_deltaY( 0. );
	 else if ( act == kRound )
	    m_const_ladder[m_ladder-1].set_deltaY(
	       rint( m_const_ladder[m_ladder-1].get_deltaY()*1e6 )/1e6 );

	 report( NOTICE, kFacilityString )
	    << "y is now " << m_const_ladder[m_ladder-1].get_deltaY()*1e6 << " microns." << endl;
	 break;
      case kDeltaZ:
	 if ( act == kMove )
	    m_const_ladder[m_ladder-1].set_deltaZ(
	       m_const_ladder[m_ladder-1].get_deltaZ() + val/1e6 );
	 else if ( act == kReset )
	    m_const_ladder[m_ladder-1].set_deltaZ(
	       m_original_ladder_z[m_ladder-1] );
	 else if ( act == kZero )
	    m_const_ladder[m_ladder-1].set_deltaZ( 0. );
	 else if ( act == kRound )
	    m_const_ladder[m_ladder-1].set_deltaZ(
	       rint( m_const_ladder[m_ladder-1].get_deltaZ()*1e6 )/1e6 );

	 report( NOTICE, kFacilityString )
	    << "z is now " << m_const_ladder[m_ladder-1].get_deltaZ()*1e6 << " microns." << endl;
	 break;
      case kPhiX:
	 if ( act == kMove )
	    m_const_ladder[m_ladder-1].set_phiX(
	       m_const_ladder[m_ladder-1].get_phiX() + val/1e6 );
	 else if ( act == kReset )
	    m_const_ladder[m_ladder-1].set_phiX(
	       m_original_ladder_phix[m_ladder-1] );
	 else if ( act == kZero )
	    m_const_ladder[m_ladder-1].set_phiX( 0. );
	 else if ( act == kRound )
	    m_const_ladder[m_ladder-1].set_phiX(
	       rint( m_const_ladder[m_ladder-1].get_phiX()*1e6 )/1e6 );

	 report( NOTICE, kFacilityString )
	    << "phix is now " << m_const_ladder[m_ladder-1].get_phiX()*1e6 << " microradians." << endl;
	 break;
      case kPhiY:
	 if ( act == kMove )
	    m_const_ladder[m_ladder-1].set_phiY(
	       m_const_ladder[m_ladder-1].get_phiY() + val/1e6 );
	 else if ( act == kReset )
	    m_const_ladder[m_ladder-1].set_phiY(
	       m_original_ladder_phiy[m_ladder-1] );
	 else if ( act == kZero )
	    m_const_ladder[m_ladder-1].set_phiY( 0. );
	 else if ( act == kRound )
	    m_const_ladder[m_ladder-1].set_phiY(
	       rint( m_const_ladder[m_ladder-1].get_phiY()*1e6 )/1e6 );

	 report( NOTICE, kFacilityString )
	    << "phiy is now " << m_const_ladder[m_ladder-1].get_phiY()*1e6 << " microradians." << endl;
	 break;
      case kPhiZ:
	 if ( act == kMove )
	    m_const_ladder[m_ladder-1].set_phiZ(
	       m_const_ladder[m_ladder-1].get_phiZ() + val/1e6 );
	 else if ( act == kReset )
	    m_const_ladder[m_ladder-1].set_phiZ(
	       m_original_ladder_phiz[m_ladder-1] );
	 else if ( act == kZero )
	    m_const_ladder[m_ladder-1].set_phiZ( 0. );
	 else if ( act == kRound )
	    m_const_ladder[m_ladder-1].set_phiZ(
	       rint( m_const_ladder[m_ladder-1].get_phiZ()*1e6 )/1e6 );

	 report( NOTICE, kFacilityString )
	    << "phiz is now " << m_const_ladder[m_ladder-1].get_phiZ()*1e6 << " microradians." << endl;
	 break;
      case kSagitta:
	 if ( act == kMove )
	    m_sagitta[m_ladder-1] += val/1e6;
	 else if ( act == kReset )
	    m_sagitta[m_ladder-1] = m_original_sagitta[m_ladder-1];
	 else if ( act == kZero )
	    m_sagitta[m_ladder-1] = 0.;
	 else if ( act == kRound )
	    m_sagitta[m_ladder-1] = rint( m_sagitta[m_ladder-1]*1e6 )/1e6;

	 report( NOTICE, kFacilityString )
	    << "sagitta is now " << m_sagitta[m_ladder-1]*1e6 << " microns." << endl;
	 break;
   }

   print_out();
   return true;
}

double HandSiAlignmentMod::naturalThresh( ThreshCommand::WhichThresh which )
{
   int nChannels( m_rphi_trackz->nChannels() );
   double total( 0. );
   double partial( 0. );

   int chan;
   for ( chan = 1;  chan <= nChannels;  chan++ )
      total += m_rphi_trackz->channelValue( chan );

   if ( which == ThreshCommand::kPosThresh )
      for ( chan = nChannels;  chan >= 1;  chan-- ) {
	 partial += m_rphi_trackz->channelValue( chan );
	 if ( partial >= 0.4 * total ) return m_rphi_trackz->channelCenter( chan );
      }
   else if ( which == ThreshCommand::kPosThresh )
      for ( chan = 1;  chan <= nChannels;  chan++ ) {
	 partial += m_rphi_trackz->channelValue( chan );
	 if ( partial >= 0.4 * total ) return m_rphi_trackz->channelCenter( chan );
      }
   
   return 0.;
}

double HandSiAlignmentMod::residualizer( FAItem< ASiStore > store,
					 HISiSurfaceFactory& siSurfaceFactory,
					 HIHelix* ihelix,
					 CalibratedSVRphiHit* rphi_hit,
					 CalibratedSVZHit* z_hit )
{
   // apply the verbose HIIntersection residual calculating procedure
   HIHelix helix( *ihelix );

   assert( ( rphi_hit != NULL  &&  z_hit == NULL )  ||
	   ( rphi_hit == NULL  &&  z_hit != NULL )     );

   double residual( 1000000. );

   vector< const CalibratedSVRphiHit* > rphi_hits;
   vector< const CalibratedSVZHit* > z_hits;
   if ( rphi_hit != NULL )
      rphi_hits.push_back( rphi_hit );
   if ( z_hit != NULL )
      z_hits.push_back( z_hit );

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
				   &helix );

   for ( HIHelixIntersector::IntersectionStatus status =
	    intersector.swimToCurrentSurface(
	       KTMoveControl::kDirectionForward );
	 status == HIHelixIntersector::kIntersectionOK;
	 status = intersector.swimToNextIntersection(
	    KTMoveControl::kDirectionForward ) )
   {
      HIIntersectionSurface* surface = intersector.currentSurface();
      do {
	 residual = surface->dca( helix );
      } while ( surface->advanceToNextCalibratedHit( helix ) );
   }

   vector< HIIntersectionSurface* >::const_iterator surfaces_end = surfaces.end();
   for ( surfaces_iter = surfaces.begin();
	 surfaces_iter != surfaces_end;
	 surfaces_iter++ ) {
      (* surfaces_iter)->deleteCondition();
      delete (* surfaces_iter );
   }

   return residual;
}

//
// const member functions
//
void
HandSiAlignmentMod::iterate( const FIFrameIterator& iBegin,
			     const FIFrameIterator& iEnd )
{
   if ( ! m_faking  &&  ( m_ladder == 0  ||  m_hsam_store == NULL ) ) {
      report( ERROR, kFacilityString )
	 << "You must use the ladder command first to load tracks." << endl;
      return;
   }
   
   if ( m_hm == NULL ) doinits();
   if ( m_app->mainWidget() == NULL ) {
      m_app->setMainWidget( m_window );
      m_window->show();
   }

   report( NOTICE, kFacilityString )
      << "Reseting histograms..." << endl;

//     m_rphiview->reset();
//     m_zview->reset();

   m_posZ_rphiVx->reset();
   m_negZ_rphiVx->reset();
   m_rphi_trackz->reset();
   m_rphi_matchedz->reset();

   m_w_posZ_rphiVx->reset();
   m_w_negZ_rphiVx->reset();

   m_posZ_rphiVx_scat->reset();
   m_negZ_rphiVx_scat->reset();
   m_rphi_hitx->reset();
   m_rphiVmatchedz->reset();
   m_rphiVmatchedz_scat->reset();
   m_rphiVtrackz->reset();
   m_rphiVtrackz_scat->reset();
   m_w_rphiVtrackz->reset();
   m_zresVz->reset();
   m_zresVz_scat->reset();
   m_zresVtrackz->reset();
   m_zresVtrackz_scat->reset();
   m_w_zresVz->reset();

   m_zresVphi_layer1->reset();
   m_zresVphi_layer1->reset();
   m_zresVphi_layer2->reset();
   m_zresVphi_layer3->reset();
   m_zresVphi_layer4->reset();
   m_zresVphi_layer1_scat->reset();
   m_zresVphi_layer2_scat->reset();
   m_zresVphi_layer3_scat->reset();
   m_zresVphi_layer4_scat->reset();
   m_w_zresVphi_layer1->reset();
   m_w_zresVphi_layer2->reset();
   m_w_zresVphi_layer3->reset();
   m_w_zresVphi_layer4->reset();

   report( NOTICE, kFacilityString )
      << "Extracting constants and loading ASiStore..." << endl;

   FIFrameIterator itFrame = iBegin;
   FAItem< ASiStore > store;
   extract( itFrame->record( Stream::kSVAlignment ), store );
   assert( store.valid() );
   HISiSurfaceFactory siSurfaceFactory( *itFrame, NULL, NULL, false, m_sagitta );

   // if you're faking hits, you need to make them
   if ( m_faking ) fakeContext2( store, siSurfaceFactory );

   report( NOTICE, kFacilityString )
      << "Plotting hits for ladder " << m_ladder << " at "
      << "( " << m_const_ladder[m_ladder-1].get_deltaX()
      << ", " << m_const_ladder[m_ladder-1].get_deltaY()
      << ", " << m_const_ladder[m_ladder-1].get_deltaZ() << " ), "
      << "( " << m_const_ladder[m_ladder-1].get_phiX()
      << ", " << m_const_ladder[m_ladder-1].get_phiY()
      << ", " << m_const_ladder[m_ladder-1].get_phiZ() << " )" << endl;
   cout << m_ladder << " "
	<< m_const_ladder[m_ladder-1].get_deltaX() << " "
	<< m_const_ladder[m_ladder-1].get_deltaY() << " "
	<< m_const_ladder[m_ladder-1].get_deltaZ() << " "
	<< m_const_ladder[m_ladder-1].get_phiX() << " "
	<< m_const_ladder[m_ladder-1].get_phiY() << " "
	<< m_const_ladder[m_ladder-1].get_phiZ() << endl;

   HSAM_Store* hsam_iter;

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

	    // the sensor can be (very) wrong if this is an rphi hit. Correct it!
	    if ( hit->deviceType() == CalibratedHit::SVR )
	       for ( sensor = m_restrictSensor.sensormin( ladder );
		     sensor <= m_restrictSensor.sensormax( ladder );
		     sensor++ ) {
		  double zcenter( store->sensor( sensor ).coorInSi().z() );
		  double zhalfwidth( store->sensor( sensor ).halfLengthZ() );
		  if ( trackz > zcenter - zhalfwidth  &&  trackz < zcenter + zhalfwidth )
		     break;
	       }
	    // edge effect: some z positions are not quite on a sensor
	    // due to imperfect trackz resolution. Just skip these.
	    if ( sensor > m_restrictSensor.sensormax( ladder ) ) break;

	    // skip out if restrictSensor tells you to
	    if ( m_restrictSensor.restrict( sensor ) ) break;

	    double residual( surface->dca( *helix ) );

// DON'T DO THIS.	    
//  	    // handle weird sign conventions preemptively
//  	    double middleish( -0.005 );
//  	    if ( ( layer == 1  ||  layer == 3 ) && ( ladder % 2 == 0 ) )
//  	       middleish *= 1.;
//  	    if ( store->sensor( sensor ).coorInSi().z() < middleish )
//  	       residual *= 1.;
// end DON'T DO THIS.	    

	    // The rest of this is needed to calculate the residual derivative
	    const ASiSensorInfo& sensorInfo( store->sensor( sensor ) );
	    HepVector3D measurement_direction;
	    if ( hit->deviceType() == CalibratedHit::SVR )
	       measurement_direction = -sensorInfo.unitVectorXInSi();
	    else if ( hit->deviceType() == CalibratedHit::SVZ )
	       measurement_direction = sensorInfo.unitVectorZInSi();
	    else assert( 0 );

	    HepVector3D X( helix->position() - helix->referencePoint() );
	    HepVector3D P( helix->momentumUnit() );
	    HepVector3D N( sensorInfo.unitVectorYInSi() );
	    double nDotPInv = 1. / N.dot( P );
	    double pt = P.perp();
	    HepVector3D zCrossPUnit( -P.y() / pt, P.x() / pt, 0. );

	    HepVector derivs( HIHelix::kNTrackParameters, 0 );

	    derivs( HIHelix::kD0 ) =
	       measurement_direction.dot(
		  zCrossPUnit - ( N.x() * zCrossPUnit.x() +
				  N.y() * zCrossPUnit.y() ) * nDotPInv * P );

	    derivs( HIHelix::kZ0 ) =
	       measurement_direction.z() - N.z() * nDotPInv *
	       measurement_direction.dot( P );

// Do residual_errs by hand for speed (like Kalman does)
//    	    double residual_err2( errorMatrix.similarity( derivs ) );
	    double residual_err2( 0. );

	    HepVector VD( HIHelix::kNTrackParameters, 0 );
	    for ( int m = 1;  m <= HIHelix::kNTrackParameters;  m++ )
	       for ( int n = 1;  n <= HIHelix::kNTrackParameters;  n++ )
		  VD( m ) += errorMatrix( m, n ) * derivs( n );
	    for ( int i = 1;  i <= HIHelix::kNTrackParameters;  i++ ) {
	       double derivi = derivs( i );
	       residual_err2 += sqr( derivi ) * errorMatrix.fast( i, i );

	       for ( int j = i + 1;  j <= HIHelix::kNTrackParameters;  j++ )
		  residual_err2 += 2. * derivi * derivs( j ) * errorMatrix.fast( j, i );
	    }
// end Do residual_errs by hand
	    
	    ////////////////////////// if this is an RPHI hit
	    if ( hit->deviceType() == CalibratedHit::SVR ) {
	       const CalibratedSVRphiHit* rphi_hit = ((CalibratedSVRphiHit*) hit);

//  	       report( INFO, kFacilityString )
//  		  << "rphi residual = " << residual
//  		  << " +/- " << sqrt( residual_err2 )
//  		  << " = " << residual / sqrt( residual_err2 )
//  		  << " sigmas" << endl;

	       HepPoint3D location( rphi_hit->worldcoord( *store ) );
//  	       m_rphiview->fill( location.x(), location.y() );

	       double ladderphi( store->ladder( ladder ).phiInSi() + 3.1415926 / 2 );
	       if ( ladderphi < 0. ) ladderphi += 2 * 3.1415926;
	       double cell( floor( ladderphi * 64. / 2/3.1415926 ) * 2.*3.1415926 / 64. );
	       double phicell( phi - cell );

	       if ( trackz > m_pthresh.value() ) {
		  m_posZ_rphiVx->fill( rphi_hit->coord(), residual, 1./residual_err2 );
		  m_w_posZ_rphiVx->fill( rphi_hit->coord(), residual, 1./residual_err2 );
		  m_posZ_rphiVx_scat->fill( rphi_hit->coord(), residual );

	       }
	       if ( trackz < m_nthresh.value() ) {
		  m_negZ_rphiVx->fill( rphi_hit->coord(), residual, 1./residual_err2 );
		  m_w_negZ_rphiVx->fill( rphi_hit->coord(), residual, 1./residual_err2 );
		  m_negZ_rphiVx_scat->fill( rphi_hit->coord(), residual );

	       }
	       m_rphi_hitx->fill( rphi_hit->coord() );

	       if ( ! z_hits.empty() ) {
		  double zloc( (* z_hits.begin())->worldcoord( *store ).z() );

		  m_rphi_matchedz->fill( zloc );
		  m_rphiVmatchedz->fill( zloc, rphi_hit->coord() );
		  m_rphiVmatchedz_scat->fill( zloc, rphi_hit->coord() );
	       }
	       m_rphi_trackz->fill( trackz );
	       m_rphiVtrackz->fill( trackz, residual, 1./residual_err2 );
	       m_rphiVtrackz_scat->fill( trackz, residual );
	       m_w_rphiVtrackz->fill( trackz, residual, 1./residual_err2 );

	    ////////////////////////// if this is a Z hit
	    } else if ( hit->deviceType() == CalibratedHit::SVZ ) {
	       const CalibratedSVZHit* z_hit = ((CalibratedSVZHit*) hit);

//  	       report( INFO, kFacilityString )
//  		  << "z residual = " << residual
//  		  << " +/- " << sqrt( residual_err2 )
//  		  << " = " << residual / sqrt( residual_err2 )
//  		  << " sigmas" << endl;

	       HepPoint3D location( z_hit->worldcoord( *store ) );
//  	       m_zview->fill( location.z() );

	       m_zresVz->fill( location.z(), residual, 1./residual_err2 );
	       m_zresVz_scat->fill( location.z(), residual );
	       m_w_zresVz->fill( location.z(), residual, 1./residual_err2 );

	       m_zresVtrackz->fill( trackz, residual, 1./residual_err2 );
	       m_zresVtrackz_scat->fill( trackz, residual );

	       const double l1phidiv( 2 * 3.1415926 / 7 );
	       const double l2phidiv( 2 * 3.1415926 / 10 );
	       const double l3phidiv( 2 * 3.1415926 / 18 );
	       const double l4phidiv( 2 * 3.1415926 / 26 );

	       double smilephi( phi );
	       switch ( ladder ) {
		  case 6: case 15: case 31: case 32: case 55: case 56:
		     smilephi = ( smilephi > 3.1415926 ? smilephi - 2. * 3.1415926 : smilephi );
		     break;
	       } // end switch sensor for smilephi
	       
	       switch ( ladder ) {
		  case 1: smilephi -= 2 * l1phidiv; break;
		  case 2: smilephi -= 3 * l1phidiv; break;
		  case 3: smilephi -= 4 * l1phidiv; break;
		  case 4: smilephi -= 5 * l1phidiv; break;
		  case 5: smilephi -= 6 * l1phidiv; break;
		  case 6: smilephi -= 0 * l1phidiv; break;
		  case 7: smilephi -= 1 * l1phidiv; break;

		  case 8: smilephi -= 3 * l2phidiv; break;
		  case 9: smilephi -= 4 * l2phidiv; break;
		  case 10: smilephi -= 5 * l2phidiv; break;
		  case 11: smilephi -= 6 * l2phidiv; break;
		  case 12: smilephi -= 7 * l2phidiv; break;
		  case 13: smilephi -= 8 * l2phidiv; break;
		  case 14: smilephi -= 9 * l2phidiv; break;
		  case 15: smilephi -= 0 * l2phidiv; break;
		  case 16: smilephi -= 1 * l2phidiv; break;
		  case 17: smilephi -= 2 * l2phidiv; break;

		  case 18: smilephi -= 5 * l3phidiv; break;
		  case 19: smilephi -= 6 * l3phidiv; break;
		  case 20: smilephi -= 7 * l3phidiv; break;
		  case 21: smilephi -= 8 * l3phidiv; break;
		  case 22: smilephi -= 9 * l3phidiv; break;
		  case 23: smilephi -= 10 * l3phidiv; break;
		  case 24: smilephi -= 11 * l3phidiv; break;
		  case 25: smilephi -= 12 * l3phidiv; break;
		  case 26: smilephi -= 13 * l3phidiv; break;
		  case 27: smilephi -= 14 * l3phidiv; break;
		  case 28: smilephi -= 15 * l3phidiv; break;
		  case 29: smilephi -= 16 * l3phidiv; break;
		  case 30: smilephi -= 17 * l3phidiv; break;
		  case 31: smilephi -= 0 * l3phidiv; break;
		  case 32: smilephi -= 1 * l3phidiv; break;
		  case 33: smilephi -= 2 * l3phidiv; break;
		  case 34: smilephi -= 3 * l3phidiv; break;
		  case 35: smilephi -= 4 * l3phidiv; break;

		  case 36: smilephi -= 7 * l4phidiv; break;
		  case 37: smilephi -= 8 * l4phidiv; break;
		  case 38: smilephi -= 9 * l4phidiv; break;
		  case 39: smilephi -= 10 * l4phidiv; break;
		  case 40: smilephi -= 11 * l4phidiv; break;
		  case 41: smilephi -= 12 * l4phidiv; break;
		  case 42: smilephi -= 13 * l4phidiv; break;
		  case 43: smilephi -= 14 * l4phidiv; break;
		  case 44: smilephi -= 15 * l4phidiv; break;
		  case 45: smilephi -= 16 * l4phidiv; break;
		  case 46: smilephi -= 17 * l4phidiv; break;
		  case 47: smilephi -= 18 * l4phidiv; break;
		  case 48: smilephi -= 19 * l4phidiv; break;
		  case 49: smilephi -= 20 * l4phidiv; break;
		  case 50: smilephi -= 21 * l4phidiv; break;
		  case 51: smilephi -= 22 * l4phidiv; break;
		  case 52: smilephi -= 23 * l4phidiv; break;
		  case 53: smilephi -= 24 * l4phidiv; break;
		  case 54: smilephi -= 25 * l4phidiv; break;
		  case 55: smilephi -= 0 * l4phidiv; break;
		  case 56: smilephi -= 1 * l4phidiv; break;
		  case 57: smilephi -= 2 * l4phidiv; break;
		  case 58: smilephi -= 3 * l4phidiv; break;
		  case 59: smilephi -= 4 * l4phidiv; break;
		  case 60: smilephi -= 5 * l4phidiv; break;
		  case 61: smilephi -= 6 * l4phidiv; break;
	       } // end switch ladder for overlapping phi

	       double smileresid( residual );
	       if ( trackz < 0. ) smileresid *= -1.;

		  switch ( layer ) {
		     case 1:
			m_zresVphi_layer1->fill( smilephi, smileresid, 1./residual_err2 );
			m_zresVphi_layer1_scat->fill( smilephi, smileresid );
			m_w_zresVphi_layer1->fill( smilephi, smileresid, 1./residual_err2 );
			break;
		     case 2:
			m_zresVphi_layer2->fill( smilephi, smileresid, 1./residual_err2 );
			m_zresVphi_layer2_scat->fill( smilephi, smileresid );
			m_w_zresVphi_layer2->fill( smilephi, smileresid, 1./residual_err2 );
			break;
		     case 3:
			m_zresVphi_layer3->fill( smilephi, smileresid, 1./residual_err2 );
			m_zresVphi_layer3_scat->fill( smilephi, smileresid );
			m_w_zresVphi_layer3->fill( smilephi, smileresid, 1./residual_err2 );
			break;
		     case 4:
			m_zresVphi_layer4->fill( smilephi, smileresid, 1./residual_err2 );
			m_zresVphi_layer4_scat->fill( smilephi, smileresid );
			m_w_zresVphi_layer4->fill( smilephi, smileresid, 1./residual_err2 );
			break;
		  } // end switch layer

	    // I don't have any other kinds of hits
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

   report( NOTICE, kFacilityString ) << "Done filling histograms." << endl;

   m_window->updateInfo();
   m_app->exec();
   print_out();
}

void
HandSiAlignmentMod::print_out()
{
   uint which;

   ofstream glob( "state.svalignmentglobal" );
   glob << "1 0 0 0 0 0 1 10 DEFAULT" << endl
	<< "1" << endl
	<< "1 " << m_const_global[0].get_deltaX() << " "
	<< m_const_global[0].get_deltaY() << " "
	<< m_const_global[0].get_deltaZ() << " "
	<< m_const_global[0].get_phiX() << " "
	<< m_const_global[0].get_phiY() << " "
	<< m_const_global[0].get_phiZ() << endl;

   ofstream lay( "state.svalignmentlayer" );
   lay << "1 0 0 0 0 0 1 11 DEFAULT" << endl
       << "4" << endl;
   for ( which = 1;  which <= 4;  which++ )
      lay << which << " " << m_const_layer[ which-1 ].get_deltaX() << " "
	  << m_const_layer[ which-1 ].get_deltaY() << " "
	  << m_const_layer[ which-1 ].get_deltaZ() << " "
	  << m_const_layer[ which-1 ].get_phiX() << " "
	  << m_const_layer[ which-1 ].get_phiY() << " "
	  << m_const_layer[ which-1 ].get_phiZ() << endl;

   ofstream lad( "state.svalignmentladder" );
   lad << "1 0 0 0 0 0 1 12 DEFAULT" << endl
       << "61" << endl;
   for ( which = 1;  which <= 61;  which++ )
      lad << which << " " << m_const_ladder[ which-1 ].get_deltaX() << " "
	  << m_const_ladder[ which-1 ].get_deltaY() << " "
	  << m_const_ladder[ which-1 ].get_deltaZ() << " "
	  << m_const_ladder[ which-1 ].get_phiX() << " "
	  << m_const_ladder[ which-1 ].get_phiY() << " "
	  << m_const_ladder[ which-1 ].get_phiZ() << endl;

   ofstream sen( "state.svalignmentsensor" );
   sen << "1 0 0 0 0 0 1 4 DEFAULT" << endl
       << "447" << endl;
   for ( which = 1;  which <= 447;  which++ )
      sen << which << " " << m_const_sensor[ which-1 ].get_deltaX() << " "
	  << m_const_sensor[ which-1 ].get_deltaY() << " "
	  << m_const_sensor[ which-1 ].get_deltaZ() << " "
	  << m_const_sensor[ which-1 ].get_phiX() << " "
	  << m_const_sensor[ which-1 ].get_phiY() << " "
	  << m_const_sensor[ which-1 ].get_phiZ() << endl;

   ofstream sag( "state.sagitta" );
   for ( which = 1;  which <= 61;  which++ )
      sag << m_sagitta[ which-1 ] << endl;
}

//
// static member functions
//
