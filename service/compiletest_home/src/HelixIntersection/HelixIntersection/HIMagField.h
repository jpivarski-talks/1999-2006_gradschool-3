#if !defined(HELIXINTERSECTION_HIMAGFIELD_H)
#define HELIXINTERSECTION_HIMAGFIELD_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIMagField
// 
// Description: Singleton to provide the magnetic field to functions that
//              do not have direct access to the Frame.
//
// Usage:
//
// To initialize the magnetic field by hand, use
//     HIMagField::instance()->setBfield( const Record& aBeginRunRecord )
// or
//     HIMagField::instance()->setBfield( double bfieldMagnitudeInKGauss )

// The first of these extracts the FAItem< MagneticField >, which will then
// be used to supply the field for a given position.  The second function
// initializes the field to a constant vector (independent of position)
// pointing in the -z direction with the given magnitude (in kGauss).

// IMPORTANT: After the second function is called, the constant field OVERRIDES
// the position-dependent field from MagneticField.
//
// Author:      Werner Sun
// Created:     Tue Jun 22 23:14:39 EDT 1999
// $Id: HIMagField.h,v 1.2 2002/01/23 22:17:46 llh14 Exp $
//
// Revision history
//
// $Log: HIMagField.h,v $
// Revision 1.2  2002/01/23 22:17:46  llh14
// Added HICorrectForBFieldOperation, and small modifications to HISingleWireCylinder to incorporate the quad fields
//
// Revision 1.1  1999/06/24 22:17:13  wsun
// First submission.
//

// system include files

// user include files
#include "FrameAccess/FAItem.h"
#include "MagField/MagneticField.h"
#include "CLHEP/Vector/ThreeVector.h"

// forward declarations
class HepPoint3D;
class KTHelix ;
class Record ;

class HIMagField
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------

      // ---------- member functions ---------------------------

      // This function extracts the MagneticField object from the Frame.
      void setBfield( const Record& aBeginRunRecord );

      // Using this member function causes bfield() not to use the
      // MagneticField object.  Instead, bfield() returns a vector pointing
      // in the -z direction with given magnitude.
      void setBfield( double aMagnitudeInKGauss ) ;

      // ---------- const member functions ---------------------

      // input position, output mag field at given point.
      Hep3Vector bfield( const HepPoint3D& position ) const;

      //  returns the additional b field produced by the quadrupoles
      Hep3Vector deltab( const HepPoint3D& position ) const;

//       // input helix, output mag field at current helix position.
//       Hep3Vector bfield( const KTHelix& aHelix ) const;

      // ---------- static member functions --------------------
      static HIMagField* instance();
      static void destroy();

   protected:
      // ---------- protected constructors and destructor ------
      HIMagField();
      virtual ~HIMagField();

      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIMagField( const HIMagField& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIMagField& operator=( const HIMagField& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_useFrameField ;
      FAItem< MagneticField > m_magneticField;
      Hep3Vector m_userSetField ;

      // ---------- static data members ------------------------
      static HIMagField* m_instance;
};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIMagField.cc"
//#endif

#endif /* HELIXINTERSECTION_HIMAGFIELD_H */
