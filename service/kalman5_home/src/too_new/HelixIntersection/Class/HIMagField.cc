// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIMagField
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Jun 22 23:19:36 EDT 1999
// $Id: HIMagField.cc,v 1.3 2002/01/23 22:17:39 llh14 Exp $
//
// Revision history
//
// $Log: HIMagField.cc,v $
// Revision 1.3  2002/01/23 22:17:39  llh14
// Added HICorrectForBFieldOperation, and small modifications to HISingleWireCylinder to incorporate the quad fields
//
// Revision 1.2  1999/06/30 22:19:05  wsun
// Modified behavior to exit if HIMagField is not initialized.
//
// Revision 1.1  1999/06/24 22:24:46  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
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
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"
#include "HelixIntersection/HIMagField.h"
#include "KinematicTrajectory/KTHelix.h"
#include "CLHEP/Geometry/Point3D.h"

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

static const char* const kFacilityString = "HelixIntersection.HIMagField" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIMagField.cc,v 1.3 2002/01/23 22:17:39 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//
HIMagField* HIMagField::m_instance = 0;

//
// constructors and destructor
//
HIMagField::HIMagField()
   : m_useFrameField( true ),
     m_userSetField( Hep3Vector( 0., 0., 0. ) )
{

}

// HIMagField::HIMagField( const HIMagField& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIMagField::~HIMagField()
{
}

//
// assignment operators
//
// const HIMagField& HIMagField::operator=( const HIMagField& rhs )
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
void
HIMagField::setBfield( const Record& aBeginRunRecord )
{
   extract( aBeginRunRecord, m_magneticField ) ;
   return;
}

void
HIMagField::setBfield( double aMagnitudeInKGauss )
{
   m_useFrameField = false ;
   m_userSetField = Hep3Vector( 0., 0., -aMagnitudeInKGauss ) ;
   return;
}

//
// const member functions
//
Hep3Vector
HIMagField::bfield( const HepPoint3D& position ) const
{
   if( m_useFrameField )
   {
      if( m_magneticField.valid() )
      {
	 //Change this to return the original constant B field in order
	 //to get the correct momentum - LLH 1/02
	 //MagField returns the total (quad plus sol) field 
	 //if you specify a position
	 return m_magneticField->BField( );
      }
      else
      {
	 report( EMERGENCY, kFacilityString )
	    << "HIMagField not initialized.  Exiting to system." << endl ;
// 	    << "HIMagField not initialized. Using default value "
// 	    << m_userSetField << endl ;
// 	 return m_userSetField ;
	 exit( 1 ) ;
	 return m_userSetField ;
      }
   }
   else
   {
      return m_userSetField ;
   }
}

// Hep3Vector
// HIMagField::bfield( const KTHelix& aHelix ) const
// {
//    return bfield( aHelix.position() ) ;
// }

//
// static member functions
//

Hep3Vector
HIMagField::deltab( const HepPoint3D& position ) const
{
   if( m_useFrameField )
   {
      if( m_magneticField.valid() )
      {
	 Hep3Vector totalField = m_magneticField->BField( position);
	 Hep3Vector solField = m_magneticField->BField( );
	 
	 //(B_tot - B_const) 
	 Hep3Vector difference = totalField - solField;

	 return difference;
      }
   }
   else
   {
      report (DEBUG, kFacilityString) 
	 <<"Not using the frame value!" << endl;
   }

   return Hep3Vector(0., 0., 0.);

}
HIMagField*
HIMagField::instance()
{
   if ( 0 == m_instance )
   {
      m_instance = new HIMagField;
   }
   return m_instance;
}

void
HIMagField::destroy()
{
   if ( 0 != m_instance )
   {
      delete m_instance;
      m_instance = 0;
   }
   return;
}
