// -*- C++ -*-
//
// Package:     <geometryForFortran>
// Module:      fillFortranDRGeometry
// 
// Description: Fill the DR geometry (in CDGEOMCD).
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Tue May  4 10:04:08 EDT 1999
// $Id: fillFortranCDGeometry.cc,v 1.9 2003/06/27 20:05:15 huanggs Exp $
//
// Revision history
//
// $Log: fillFortranCDGeometry.cc,v $
// Revision 1.9  2003/06/27 20:05:15  huanggs
// change zdStore argument type
//
// Revision 1.8  2003/04/24 14:20:44  huanggs
// add ZD part
//
// Revision 1.7  2000/10/03 18:06:46  lyon
// Chris wanted me to remove the large print out at the beginning of jobs. I relented.
//
// Revision 1.6  2000/03/29 20:49:52  pg
// Added fillFortranSVGeometry.cc to be less disruptive to other packages
//
// Revision 1.5  2000/03/28 16:02:50  pg
// Added routine gff_fillsigeom.F to make doit Si-aware
//
// Revision 1.4  2000/02/27 18:18:57  dsr
// Fixing single argument report()s
//
// Revision 1.3  1999/10/31 15:11:21  lyon
// improvements for cathodes
//
// Revision 1.2  1999/05/28 13:25:11  lyon
// removed i from inside of loop since stupid Solaris does not follow the standard
//
// Revision 1.1  1999/05/28 02:36:35  lyon
// Was fillFortranDRGeometry.cc
//
// Revision 1.1.1.1  1999/05/04 17:44:13  lyon
// Imported geometryForFortran sources
//

#include "Experiment/Experiment.h"
#include "Experiment/types.h"
#include "Experiment/fortran_types.h"


// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>
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
#include "Experiment/report.h"
#include "geometryForFortran/geometryForFortran.h"
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

static const char* const kFacilityString = 
                      "geometryForFortran.fillFortranCDGeometry" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fillFortranCDGeometry.cc,v 1.9 2003/06/27 20:05:15 huanggs Exp $";
static const char* const kTagString = "$Name:  $";

extern "C" {
   void gff_filldrgeomcd_(FInteger*, FInteger*, FInteger*, FInteger*, FInteger*,
			      FReal*, FReal*, FReal*, FReal*, FReal*, FReal*,
			      FInteger*, FReal*, FReal*, FReal*);

   void gff_fillzdgeomcd_(FInteger*, FInteger*, FInteger*, FInteger*, FInteger*,
			      FReal*, FReal*, FReal*, FReal*, FReal*, FReal*,
			      FInteger*, FReal*, FReal*, FReal*);

   void gff_fillcdgeomglobal_(FInteger*, FInteger*,
                              FInteger*, FInteger*, FInteger*);

   void gff_fillcathgeom_(FInteger*, FInteger*, FReal*, FReal*,
			  FInteger*, FInteger*, FReal*, FReal*,
			  FReal*, FReal*, FInteger*);

   void gff_fillsirphigeom_(FInteger*, FInteger*, FReal*, FInteger*, 
			    FReal*, FReal*, FReal*, FInteger*);

   void gff_fillsirzgeom_(FInteger*, FInteger*, FReal*, FInteger*, 
			  FReal*, FReal*, FReal*, FInteger*);
			
}

//////////////////////////////////////////////////////////////////////
// Fill the appropriate Fortran common blocks with DR geometry information
//
void fillFortranCDGeometry(const ADRSenseWireStore& g,
			   const ADRCathodeStore& gc,
                           const AZDSenseWireStore* gzd,
                           DABoolean fill_SI,
                           DABoolean fill_ZD )
{


   report(INFO, kFacilityString) 
      << "fillFortranCDGeometry: enter:"
      << " fill_SI="
      << fill_SI << " " 
      << " fill_ZD="
      << fill_ZD << " " 
      << endl;
   


   FInteger nlaydr3 = g.kNumberOfLayers;
   FInteger ncathlay = 1;
   FInteger nsvrphilay = 4;
   FInteger nsvrzlay = 4;
   FInteger nzdlay = 0;
   if ( fill_ZD ) {
       nsvrphilay = 0;
       nsvrzlay = 0;
       nzdlay = gzd->kNumberOfSenseLayers;
   }

   gff_fillcdgeomglobal_(&nlaydr3, &ncathlay,
                         &nsvrphilay, &nsvrzlay, &nzdlay);

   // The ordering is DR-Anodes, DR-Cathodes, ( SVX-RPHI, SVX-RZ | ZD )

   // FILL THE DR ANODES!
   // Set the layer-by-layer constants
   FInteger index = 1;
   int cdlayer = 0;
   unsigned int i;
   for ( i=g.kFirstLayer;
	 i <= (g.kFirstLayer+g.kNumberOfLayers-1); ++i)
   {
      
      ++cdlayer;
      
      // Number of wires in the layer
      FInteger nwires = g.numberOfWiresLyr(i);
      
      // Is this layer stereo?
      FInteger isLayerStereo = g.isStereo(i);
      
      // Radius, tan of the stereo angle,
      // phi of wire 0, cell width, z at the end (+ side),
      // radius at +z end
      FReal rcd = g.radiusNominal(i);

      FReal tanscd = g.tanStereoAngle(i);

      FReal phifcd = g.phi0Nominal(i);

      FReal cellcd = g.cellPhiWidth(i);

	

      double zend = g.zMax(i);
      FReal zendcd = (FReal)zend;

      DABoolean success;
      double rend = g.radiusAtz(success, i, zend);
      if ( !success )
      {
	 report(EMERGENCY, kFacilityString) << "radiusAtz failed for layer "
					    << i << endl;
	 assert(success);
	 ::exit(1);
      }

      FReal rendcd = (FReal) rend;

      FInteger layerToFill = i;

      // Now need to get more detailed information
      const AEWireLayerCylindrical& layer = g.layer(i);

      report(INFO, kFacilityString) 
	 << "DR3 i, phi0, cellcd, phimin, phimax, zmin, zmax, tanscd, r " 
	 << i << " " 
	 << g.phi0Nominal(i) << " " << g.cellPhiWidth(i) << " " 
	 << layer.phiOfFirstWireInVolume1() << " "
	 << layer.phiOfFirstWireInVolume2() << " "
	 << layer.zMin() << " " 
	 << layer.zMax() << " " 
	 << layer.tanStereoAngle() << " "
	 << layer.radiusNominal() << " "
	 << endl;
      
      // Determine the read out end for this layer (WENDCD)
      int readoutEnd = layer.readoutEnd();
      
      FInteger wendcd = readoutEnd;
      
      // Is the layer stereo?
      DABoolean isStereo = layer.isStereo();
      
      // Let's try to calculate the number of cells a stereo wire
      //  is offset as it travels down the length of the chamber (DIFSCD)...
      
      // phi is the change in azimuth of the wire as it goes from one end
      // to the center
      double sinphi = zend * g.tanStereoAngle(i) / rend;
      
      // So number (or fraction thereof) of cells crossed when going down
      // the WHOLE chamber length is
      double cellsOffset = asin(sinphi) / g.cellPhiWidth(i) * 2.0;

      FReal difscd;
      difscd = cellsOffset;

      // Now the actual stereo angle is...
      double stereoAngle = atan(g.tanStereoAngle(i));
      FReal angscd = stereoAngle;
      

      // Get the maximum sag of the wire in y
      double wireSagInY = layer.wireSagy();

      FReal bsagcd = wireSagInY;

      FInteger laydev = i;

      // Save these in the common block
      gff_filldrgeomcd_(&layerToFill, &laydev, &nwires, &index, 
			&isLayerStereo, &rcd,
			&tanscd, &phifcd, &cellcd, &zendcd, 
			&rendcd, &wendcd, &difscd, &bsagcd, &angscd);

      // Update the index (should point to wire # of the next layer)
      index += nwires;
      
   }

   // DR-Cathodes
   ++cdlayer;
   FInteger cathLayer = cdlayer;
   FReal rcd = 1.0;
   FInteger nTotalPads = ADRCathodeStore::kNumberOfPadsTotal;
   FInteger nPadsOnPanel = ADRCathodeStore::kNumberOfPadsInPanel;

   FReal padWidth = M_PI/4;    // Width in phi
   FReal padPitch = 0.01157;   // width in Z
   FReal zendcd = padPitch*nPadsOnPanel/2;
   FReal rendcd = 1.0;         // Get from inga (use physics - no fancy stuff)
   FInteger laydev = 1;        // Outer cathodes are the first (and only)
                               // cathode layer in DRCD
   FReal phi0 = M_PI/8;        // phi at center of first panel (0.5 * 2pi/8)
   
   gff_fillcathgeom_(&cathLayer, &laydev, &rcd, &phi0, 
		     &nTotalPads,&nPadsOnPanel, &padWidth, &padPitch, 
		     &zendcd, &rendcd, &index);

// old  index += nPadsOnPanel;
   index += nTotalPads;
   
   report(INFO, kFacilityString) 
      << "fillFortranCDGeometry: at silicon:"
      << " fill_SI="
      << fill_SI << " " 
      << " fill_ZD="
      << fill_ZD << " " 
      << endl;

   if ( fill_SI )
   {
      
      // Silicon R-PHI
      for ( i = 1; i <= 4; ++i)
      {
	 ++cdlayer;
	 
	 FInteger siLayer = cdlayer;
	 FReal radius = 1.0;
	 FInteger nwafers = 1000;
	 FReal wafWidth = 0.001;
	 FReal zend = 1.0;
	 FReal rend = 1.0;
	 FInteger laydev = i;
	 gff_fillsirphigeom_(&siLayer, &laydev, &radius, &nwafers, 
			     &wafWidth, &zend, &rend, &index);
	 index += nwafers;
      }
      
      // Silicon R-Z
      for ( i = 1; i <= 4; ++i)
      {
	 ++cdlayer;
	 
	 FInteger siLayer = cdlayer;
	 FReal radius = 1.0;
	 FInteger nwafers = 1000;
	 FReal wafWidth = 0.001;
	 FReal zend = 1.0;
	 FReal rend = 1.0;
	 FInteger laydev = i;
	 gff_fillsirzgeom_(&siLayer, &laydev, &radius, &nwafers, &wafWidth, &zend,
			   &rend, &index);
	 index += nwafers;
      }
   }
   
   report(INFO, kFacilityString) 
      << "fillFortranCDGeometry: at ZD:"
      << " fill_SI="
      << fill_SI << " " 
      << " fill_ZD="
      << fill_ZD << " " 
      << endl;
   
   if ( fill_ZD )
   {
      
      
      // Now the ZD
      for ( i=gzd->kFirstLayer;
	    i <= (gzd->kFirstLayer+gzd->kNumberOfSenseLayers-1); ++i)
      {
	 
	 ++cdlayer;
	 
	 // Number of wires in the layer
	 FInteger nwires = gzd->numberOfWiresLyr(i);
	 
	 // Is this layer stereo?
	 FInteger isLayerStereo = gzd->isStereo(i);
	 
	 // Radius, tan of the stereo angle,
	 // phi of wire 0, cell width, z at the end (+ side),
	 // radius at +z end
	 FReal rcd = gzd->radiusNominal(i);
	 
	 FReal tanscd = gzd->tanStereoAngle(i);
	 
	 FReal phifcd = gzd->phi0Nominal(i);
	 
	 FReal cellcd = gzd->cellPhiWidth(i);
	 
	 double zend = gzd->zMax(i);
	 FReal zendcd = (FReal)zend;
	 
	 DABoolean success;
	 double rend = gzd->radiusAtz(success, i, zend);
	 if ( !success )
	 {
	    report(EMERGENCY, kFacilityString) << "radiusAtz failed for layer "
					       << i << endl;
	    assert(success);
	    ::exit(1);
	 }
	 
	 FReal rendcd = (FReal) rend;
	 
	 FInteger layerToFill = cdlayer;
	 
	 // Now need to get more detailed information
	 const AEWireLayerCylindrical& layer = gzd->layer(i);
	 
	 report(INFO, kFacilityString) 
	    << "ZD i, phi0, cellcd, phimin, phimax, zmin, zmax, tanscd, r " 
	    << i << " " 
	    << gzd->phi0Nominal(i) << " " << gzd->cellPhiWidth(i) << " " 
	    << layer.phiOfFirstWireInVolume1() << " "
	    << layer.phiOfFirstWireInVolume2() << " "
	    << layer.zMin() << " " 
	    << layer.zMax() << " " 
	    << layer.tanStereoAngle() << " "
	    << layer.radiusNominal() << " "
	    << endl;
	 
	 // Determine the read out end for this layer (WENDCD)
	 int readoutEnd = layer.readoutEnd();
	 
	 FInteger wendcd = readoutEnd;
	 
	 // Is the layer stereo?
	 DABoolean isStereo = layer.isStereo();
	 
	 // Let's try to calculate the number of cells a stereo wire
	 //  is offset as it travels down the length of the chamber (DIFSCD)...
	 
	 // phi is the change in azimuth of the wire as it goes from one end
	 // to the center
	 double sinphi = zend * gzd->tanStereoAngle(i) / rend;
	 
	 // So number (or fraction thereof) of cells crossed when going down
	 // the WHOLE chamber length is
	 double cellsOffset = asin(sinphi) / gzd->cellPhiWidth(i) * 2.0;
	 
	 FReal difscd;
	 difscd = cellsOffset;
	 
	 // Now the actual stereo angle is...
	 double stereoAngle = atan(gzd->tanStereoAngle(i));
	 FReal angscd = stereoAngle;
	 
	 // Get the maximum sag of the wire in y
	 double wireSagInY = layer.wireSagy();
	 
	 FReal bsagcd = wireSagInY;
	 
	 FInteger laydev = i;
	 
	 // Save these in the common block
	 gff_fillzdgeomcd_(&layerToFill, &laydev, &nwires, &index, 
			   &isLayerStereo, &rcd,
			   &tanscd, &phifcd, &cellcd, &zendcd, 
			   &rendcd, &wendcd, &difscd, &bsagcd, &angscd);
	 
	 // Update the index (should point to wire # of the next layer)
	 index += nwires;
      }
   }
   
}
