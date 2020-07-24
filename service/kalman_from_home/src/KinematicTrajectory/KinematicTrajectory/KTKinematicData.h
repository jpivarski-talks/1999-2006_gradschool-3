#if !defined(KINEMATICTRAJECTORY_KTKINEMATICDATA_H)
#define KINEMATICTRAJECTORY_KTKINEMATICDATA_H
// -*- C++ -*-
//
// Package:     KinematicTrajectory
// Module:      KTKinematicData
//
/**\class KTKinematicData KTKinematicData.h KinematicTrajectory/KTKinematicData.h
   Description: Kinematic particle description.

   Usage:

   Describes kinematic properties of charged tracks, photons, and
   virtual particles such as pi0, Ks, Lambda, D0, etc., so that a user
   can carry out standard operations such as calculating masses, adding
   4-momenta together, etc. The basic information consists of the
   3-momentum, 3-position, mass and charge. A 7x7 error matrix is stored
   for the quantities \c (Px,Py,Pz,E,x,y,z).

   KTKinematicData objects cannot be moved. Movement is performed by
   the KTKinematicMoveable class, which inherits from KTKinematicData.

   \par Building a simple KTKinematicData object
   In addition to the usual copy constructor, KTKinematicData objects can
   be built from basic momentum and position information, e.g.,
\code
   HepVector3D momentum(1.2, -0.5, 0.6);
   HepPoint3D  position(0.002, 0.003, 0.);
   double mass = 0.1396;
   double charge = -1.0;
   HepSymMatrix errMatrix(7,1);  // Create a 7x7 unit matrix for now
   KTKinematicData pion(momentum, position, mass, charge, errorMatrix);
\endcode
   The error matrix argument is optional. If absent, a null error matrix
   is stored.
   <br><br>
   Most of the time you build a kinematic object from a helix read
   from the data access system. In that case you supply the helix
   parameters, magnetic field (to compute the momentum) and mass (needed
   for the energy). The kinematic parameters are evaluated at the point
   of closest approach to the reference point.
   <br><br>
   Note that the magnetic field is always specified in \e kilogauss.
\code
   KTHelix helix;
   double mass = 0.1396;
   HepVector3D bField(0., 0., -15.);
   KTKinematicData pion(helix, mass, bField);
\endcode
   By default, the helix error matrix is converted to the appropriate
   7x7 form and added to the object. You can prevent the error matrix
   from being formed by replacing the above declaration by
\code
   DABoolean noErrorMatrix = false;
   KTKinematic pion(helix, mass, bField, noErrorMatrix);
\endcode

   \par Setting and retrieving information from KTKinematicData objects.
   All kinds of information can be set or retrieved.
\code
   KTKinematicData pion(helix, mass, bField);            // Make a pion
\endcode
   Modify some of the pion components
\code
   pion.setMomentum(HepVector3D(0.2,0.5,-1.2));          // 3-momentum
   pion.setPosition(HepPoint3D(0.002,0.005,0.02));       // Position
   pion.setMass(0.4937);                                 // Mass
   pion.clearErrorMatrix();                              // Clear err matrix
\endcode
   Retrieve pion information
\code
   HepVector3D momentum = pion.momentum();               // 3-momentum
   HepLorentzVector fourMomentum = pion.lorenzMomentum(); // 4-momentum
   HepPoint3D  position = pion.position();               // Position
   double mass = pion.mass();                            // Mass
   double charge = pion.charge();                        // Charge
   HepSymMatrix errorMatrix = pion.errorMatrix();        // Error matrix
   double ptot = pion.pmag();                            // p
   double ptotsq = pion.pmag2();                         // p^2
   double pt = pion.pperp();                             // pt
   double ptsq = pion.pperp2();                          // pt^2
   double px = pion.px();                                // px
   double py = pion.py();                                // py
   double pz = pion.pz();                                // pz
   double E  = pion.energy();                            // E
   double x = pion.x();                                  // x
   double y = pion.y();                                  // y
   double z = pion.z();                                  // z
\endcode

   \par Fixed or floating mass
   Particles like pions, kaons and gammas have predetermined, fixed masses
   while those calculated from invariant masses or mass fits, such as
   D0's and B's, have masses which "float" because the energy is
   independent of the momentum. KTKinematicData objects have a flag
   that specifies whether or not the mass is floating or not. The flag
   can be accessed as follows
\code
   KTKinematicData pion(...);
   DABoolean massFixed = pion.hasFixedMass();  // Get the fixed mass flag
   pion.setMassFixed();                        // Set fixed mass
   pion.setMassFloat();                        // Set floating mass
\endcode
   One should rarely need to set the mass flag since the defaults used by
   the tracking system are expected to be adequate. For example, when
   building a KTKinematicData object from a helix or from the basic
   3-momentum, 3-position, etc., the flag is set to "true". As kinematic
   fitting becomes available, particles built by combining the 4-momentum
   of daughter particles will have the flag set to "false" because the
   energy will be truly independent of the 3-momentum.

*/
//
// Author:      Paul Avery
// Created:     Thu Oct 23 14:46:00 EDT 1997
// $Id: KTKinematicData.h,v 1.12 2002/03/06 14:41:26 cdj Exp $
//
// Revision history
//
// $Log: KTKinematicData.h,v $
// Revision 1.12  2002/03/06 14:41:26  cdj
// made KinematicData::errorMatrix() virtual to allow speed optimizations
//
// Revision 1.11  2001/03/29 21:22:54  wsun
// Added comparison operator.
//
// Revision 1.10  2001/01/06 20:48:53  cdj
// KTKinematicData now always returns a non null errorMatrix
//
// Revision 1.9  2000/06/15 20:16:53  pappas
// Double checked and committed Paul Avery's modifications eliminating
// references to Toolbox classes. Should be completely transparent since
// Toolbox is just typedefs.
//
// Revision 1.8  2000/04/03 20:49:40  marsh
// Doxygenated comments.
//
// Revision 1.7  1998/12/09 22:09:32  avery
// Added notion of a "sticky" distance when moving. Fixed some small errors
// in moving KTHelix and KTKinematicData objects over large arcs. Implemented
// some missing set functions.
//
// Revision 1.6  1998/07/09 21:33:36  avery
// Bug fixes, finished implementation of move routines, updated documentation.
//
// Revision 1.5  1998/06/01 18:24:28  avery
// Fixed problem with copy constructor
//
// Revision 1.4  1998/05/27 17:12:51  avery
// Improved documentation
//
// Revision 1.3  1998/05/24 21:30:45  avery
// Add move, PCA routines. Some cleanup
//
// Revision 1.2  1997/12/03 22:03:15  mkl
// improved documentation; fixed g++ problems (warning about unused parameters etc.)
//
// Revision 1.1.1.1  1997/11/04 23:38:47  cdj
// First submission
//
// Revision 1.1.1.1  1997/11/04 22:45:14  avery
// First submission
//
//
// system include files
// user include files
#include "CLHEP/Geometry/Vector3D.h"    // can not forward decl as is typedef
#include "CLHEP/Geometry/Point3D.h"     // can not forward decl as is typedef
#include "CLHEP/Vector/LorentzVector.h" // can not forward decl as is typedef
#include "CLHEP/Matrix/SymMatrix.h" 

// forward declarations
class KTHelix ;

class KTKinematicData
{
   public:
      // constants, enums and typedefs
      typedef double ValueType;

      enum { kDefaultMass = 0 ,
	     kDefaultCharge = 0
      } ;

      ///This is needed to associate the elements correctly in the error matrix
      enum ParameterOrder {kPx = 1,
			   kPy,
			   kPz,
			   kEnergy,
			   kX,
			   kY,
			   kZ};

      // Constructors and destructor
      KTKinematicData( void ) ;

      KTKinematicData( const KTKinematicData& aKinematicData );

      KTKinematicData( const KTKinematicData& aKinematicData,
		       const DABoolean aCopyErrorMatrix ) ;

      KTKinematicData( const HepVector3D& aMomentum ,
		       const HepPoint3D& aPosition,
		       const ValueType aMass,
		       const ValueType aCharge) ;

      KTKinematicData( const HepVector3D& aMomentum ,
		       const HepPoint3D& aPosition,
		       const ValueType aMass,
		       const ValueType aCharge,
		       const HepSymMatrix& aErrorMatrix ) ;

      KTKinematicData( const KTHelix& aHelix ,
		       const ValueType aMass,
		       const HepVector3D& aMagneticField ,
		       const DABoolean aCopyErrorMatrix = true );

      KTKinematicData( const KTHelix& aHelix ,
		       const ValueType aMass,
		       const ValueType aBFieldMagnitude,
		       const DABoolean aCopyErrorMatrix = true );

      virtual ~KTKinematicData( void ) ;

      // assignment operator(s)
      const KTKinematicData& operator=(
	                        const KTKinematicData& aOtherKinematicData ) ;

      DABoolean operator==( const KTKinematicData& rhs ) const ;
      DABoolean operator!=( const KTKinematicData& rhs ) const ;

      // member functions
      void setMass( const ValueType aMass ) ;
      void setMomentum( const HepVector3D& aMomentum ) ;
      void setPosition( const HepPoint3D& aPosition ) ;
      void setMassFixed( void ) ;
      void setMassFloat( void ) ;
      void clearErrorMatrix( void ) ;
      void setErrorMatrix( const HepSymMatrix& aMatrix ) ;

      // const member functions
      ValueType mass( void ) const ;
      ValueType charge( void ) const ;
      ValueType px( void ) const ;
      ValueType py( void ) const ;
      ValueType pz( void ) const ;
      ValueType energy( void ) const ;
      ValueType x( void ) const ;
      ValueType y( void ) const ;
      ValueType z( void ) const ;
      ValueType pperp( void ) const ;
      ValueType pperp2( void ) const ;
      ValueType pmag( void ) const ;
      ValueType pmag2( void ) const ;
      const HepVector3D& momentum( void ) const ;
      const HepPoint3D& position( void ) const ;
      const HepLorentzVector lorentzMomentum( void ) const ;
      DABoolean hasFixedMass( void ) const ;
      virtual const HepSymMatrix& errorMatrix( void ) const ;

      /// \return TRUE if errors are all zero
      DABoolean hasNullErrorMatrix() const {
	 return (&errorMatrix() == &s_nullMatrix);};

   protected:
      // protected member functions

      // These routines are used to optimize the performance of some
      // of the move routines
      HepSymMatrix* takeOwnershipOfPointer( void );
      void         restoreOwnershipOfPointer( HepSymMatrix* const aPointer);
      void calculate7x7ErrorMatrixFrom5x5ErrorMatrix( const KTHelix& aHelix);

   private:

      // data members
      DABoolean m_hasFixedMass ;
      ValueType m_mass ;
      ValueType m_charge ;
      HepVector3D m_momentum ;
      HepPoint3D m_position ;
      HepSymMatrix* m_errorMatrix ;   // Order is (px, py, pz, E, x, y, z)

      //All matricies without a set error matrix can share the same nullMatrix
      static  HepSymMatrix s_nullMatrix;
};

// inline function definitions

#endif /* KINEMATICTRAJECTORY_KTKINEMATICDATA_H */
