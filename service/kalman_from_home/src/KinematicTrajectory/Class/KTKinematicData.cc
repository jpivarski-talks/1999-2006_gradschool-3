// -*- C++ -*-
//
// Package:     KinematicTrajectory
// Module:      Product
// 
// Description: Methods to handle a particle's kinematic information
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Paul Avery
// Created:     Wed Oct 29 14:46:03 EST 1997
// $Id: KTKinematicData.cc,v 1.16 2002/03/06 14:41:23 cdj Exp $
//
// Revision history
//
// $Log: KTKinematicData.cc,v $
// Revision 1.16  2002/03/06 14:41:23  cdj
// made KinematicData::errorMatrix() virtual to allow speed optimizations
//
// Revision 1.14  2001/03/29 21:22:50  wsun
// Added comparison operator.
//
// Revision 1.13  2001/01/06 20:48:49  cdj
// KTKinematicData now always returns a non null errorMatrix
//
// Revision 1.12  2000/08/04 00:21:07  wsun
// Added check for zero curvature in calculate7x7ErrorMatrixFrom5x5ErrorMatrix()
//
// Revision 1.11  2000/06/15 20:16:47  pappas
// Double checked and committed Paul Avery's modifications eliminating
// references to Toolbox classes. Should be completely transparent since
// Toolbox is just typedefs.
//
// Revision 1.10  2000/02/26 14:01:12  cdj
// report is now always called using two arguments
//
// Revision 1.9  1999/10/08 22:26:14  marsh
// Changed TBThreeVector to TBThreePoint.
//
// Revision 1.8  1999/10/08 22:18:43  marsh
// Changed TBThreeVector to TBThreePoint.
//
// Revision 1.7  1998/10/13 17:25:30  avery
// - Added operator= to class
// - Fixed bug in constructor causing object not to be initialized
// - Fixed bug in copy constructor causing memory stomping
// - Break out of loop sooner in setErrorMatrix method
//
// Revision 1.6  1998/07/09 21:32:46  avery
// Bug fixes, finished implementation of move routines, updated documentation.
//
// Revision 1.5  1998/06/01 18:24:08  avery
// Fixed problem with copy constructor
//
// Revision 1.4  1998/05/24 21:30:30  avery
// Add move, PCA routines. Some cleanup
//
// Revision 1.3  1997/12/19 19:42:19  avery
// Changed B field constant to reflect use of KGauss instead of Tesla.
//
// Revision 1.2  1997/12/03 22:03:13  mkl
// improved documentation; fixed g++ problems (warning about unused parameters etc.)
//
// Revision 1.1.1.1  1997/11/04 23:38:46  cdj
// First submission
//
// Revision 1.1.1.1  1997/11/04 22:45:14  avery
// First submission
//
//

#include "Experiment/Experiment.h"
// system include files
#include <stdlib.h>  // For 'exit'

#if defined(EXCEPTION_CLASH_WITH_MATH_H_BUG)
#define exception _math_bug_
#include <math.h>  // required for trigonometry and fabs
#undef exception
#else
#include <math.h>  // For 'sqrt'
#endif /* EXCEPTION_CLASH_WITH_MATH_H_BUG */

#include <assert.h>

// user include files
#include "Experiment/report.h" // For error report
#include "KinematicTrajectory/KTHelix.h"

#include "KinematicTrajectory/KTKinematicData.h"
#include "CLHEP/Matrix/Matrix.h"

//
// constants, enums and typedefs
//

const double kLarge = 1.e20;

// B field constant to convert curvature to momentum
const double kBfieldConstant = -0.0299792458;

static const char* const kFacilityString = "KinematicTrajectory.KTKinematicData" ;


//
// static data member definitions
//
HepSymMatrix
KTKinematicData::s_nullMatrix(7,0);

//
// constructors and destructor
//
KTKinematicData::KTKinematicData() :
   m_hasFixedMass( !false ) ,
   m_mass( kDefaultMass ) ,
   m_charge( kDefaultCharge ) ,
   m_momentum( HepVector3D( 0.0 , 0.0 , 0.0 ) ) ,
   m_position( HepPoint3D(  0.0 , 0.0 , 0.0 ) ) ,
   m_errorMatrix( &s_nullMatrix )
{
   return ;
}

// Copy constructor with optional argument for copying over error matrix
KTKinematicData::KTKinematicData( const KTKinematicData& aKinematicData) :
   m_hasFixedMass( aKinematicData.m_hasFixedMass ) ,
   m_mass( aKinematicData.m_mass ) ,
   m_charge( aKinematicData.m_charge ) ,
   m_momentum( aKinematicData.m_momentum ) ,
   m_position( aKinematicData.m_position ) ,
   m_errorMatrix( &s_nullMatrix )
{
   // Copy error matrix if it exists
   if(! aKinematicData.hasNullErrorMatrix() ){
      setErrorMatrix( *aKinematicData.m_errorMatrix ) ;
   }
   return ;
}


// Similar to copy constructor with optional argument for copying
// over error matrix
KTKinematicData::KTKinematicData( const KTKinematicData& aKinematicData,
				  const DABoolean aCopyErrorMatrix) :
   m_hasFixedMass( aKinematicData.m_hasFixedMass ) ,
   m_mass( aKinematicData.m_mass ) ,
   m_charge( aKinematicData.m_charge ) ,
   m_momentum( aKinematicData.m_momentum ) ,
   m_position( aKinematicData.m_position ) ,
   m_errorMatrix( &s_nullMatrix )
{
   // Copy error matrix if is requested and if it exists
   if(aCopyErrorMatrix && ! aKinematicData.hasNullErrorMatrix() ){
      setErrorMatrix( *aKinematicData.m_errorMatrix ) ;
   }
   return ;
}


KTKinematicData::KTKinematicData( const KTHelix& aHelix ,
				  const ValueType aMass,
				  const ValueType aBFieldMagnitude ,
				  const DABoolean aCopyErrorMatrix) :
   m_hasFixedMass( !false ),
   m_mass ( aMass ),
   m_errorMatrix(&s_nullMatrix)
{
   *this = KTKinematicData(aHelix, aMass,
			   HepVector3D(0.,0.,aBFieldMagnitude),
			   aCopyErrorMatrix);
}


KTKinematicData::KTKinematicData( const KTHelix& aHelix ,
				  const ValueType aMass,
				  const HepVector3D& aMagneticField ,
				  const DABoolean aCopyErrorMatrix) :
   m_hasFixedMass( !false ),
   m_mass ( aMass ),
   m_errorMatrix(&s_nullMatrix)
{

   double temp = kBfieldConstant
                  * aHelix.curvature() * aMagneticField.z();
   if(temp > 0.) {
      m_charge = +1.;
   }
   else if(temp < 0) {
      m_charge = -1.;
   }
   else {
      m_charge = 0.;
   }

   double pperp;
   if(aHelix.curvature() == 0.) {
      pperp = kLarge;
   }
   else {
      pperp = 0.5 * kBfieldConstant * aMagneticField.z() * m_charge
	                / aHelix.curvature();
   }
   const double phi0 = aHelix.phi0();
   const double u0 = cos(phi0);
   const double v0 = sin(phi0);
   const double cotTheta = aHelix.cotTheta();
   const double z0 = aHelix.z0();

   m_momentum = HepVector3D( pperp * u0,
			     pperp * v0,
			     pperp * cotTheta);

   const double d0 = aHelix.d0();
   m_position = aHelix.referencePoint() + HepPoint3D( -d0 * v0,
						      +d0 * u0,
						      +z0);
   if(aCopyErrorMatrix) {
      calculate7x7ErrorMatrixFrom5x5ErrorMatrix(aHelix);
   }
}

KTKinematicData::KTKinematicData(const HepVector3D& aMomentum ,
				 const HepPoint3D&  aPosition ,
				 const ValueType aMass ,
				 const ValueType aCharge):
   m_hasFixedMass( !false ) ,
   m_mass( aMass ) ,
   m_charge( aCharge ) ,
   m_momentum( aMomentum ) ,
   m_position( aPosition ) ,
   m_errorMatrix( &s_nullMatrix )

{
   return ;
}

KTKinematicData::KTKinematicData(const HepVector3D& aMomentum ,
				 const HepPoint3D&  aPosition ,
				 const ValueType aMass ,
				 const ValueType aCharge,
				 const HepSymMatrix& aErrorMatrix):
   m_hasFixedMass( !false ) ,
   m_mass( aMass ) ,
   m_charge( aCharge ) ,
   m_momentum( aMomentum ) ,
   m_position( aPosition ) ,
   m_errorMatrix( &s_nullMatrix )
{
   setErrorMatrix( aErrorMatrix);
   return ;
}


KTKinematicData::~KTKinematicData()
{
   clearErrorMatrix();
   return ;
}

//
// assignment operators
//
const KTKinematicData& 
KTKinematicData::operator=( const KTKinematicData& aOtherKinematicData )
{
   if(this != &aOtherKinematicData) {
      m_hasFixedMass = aOtherKinematicData.m_hasFixedMass ;
      m_mass = aOtherKinematicData.m_mass ;
      m_charge = aOtherKinematicData.m_charge ;
      m_momentum = aOtherKinematicData.m_momentum ;
      m_position = aOtherKinematicData.m_position ;
      if ( ! aOtherKinematicData.hasNullErrorMatrix() ) {
	 setErrorMatrix( *aOtherKinematicData.m_errorMatrix ) ;
      }
      else {
	 clearErrorMatrix();
      }
   }
   return ( *this ) ;
}

DABoolean
KTKinematicData::operator==( const KTKinematicData& rhs ) const
{
   if( !( m_hasFixedMass == rhs.m_hasFixedMass &&
	  m_mass == rhs.m_mass &&
	  m_charge == rhs.m_charge &&
	  m_momentum == rhs.m_momentum &&
	  m_position == rhs.m_position &&
	  hasNullErrorMatrix() == rhs.hasNullErrorMatrix() ) )
   {
      return false ;
   }
   else if( hasNullErrorMatrix() )
   {
      return true ;
   }
   else
   {
      return *m_errorMatrix == *( rhs.m_errorMatrix ) ;
   }
}

DABoolean
KTKinematicData::operator!=( const KTKinematicData& rhs ) const
{
   return !( *this == rhs ) ;
}

//
// member functions
//
void 
KTKinematicData::setMass( const ValueType aMass )
{
   m_mass = aMass ;
}

void 
KTKinematicData::setMomentum( const HepVector3D& aMomentum )
{
   m_momentum = aMomentum ;
}

void 
KTKinematicData::setPosition( const HepPoint3D& aPosition )
{
   m_position = aPosition ;
}

void 
KTKinematicData::setMassFixed( void )
{
   m_hasFixedMass = !false ;
}

void 
KTKinematicData::setMassFloat( void )
{
   m_hasFixedMass = false ;
}

void 
KTKinematicData::clearErrorMatrix( void )
{
   if( !hasNullErrorMatrix() ) {
      delete m_errorMatrix ;
      m_errorMatrix = &s_nullMatrix ;
   }
}

void 
KTKinematicData::setErrorMatrix( const HepSymMatrix& aMatrix )
{
   const unsigned int kMatrixSize = 7;
   assert( kMatrixSize == aMatrix.num_row() );
  
   //Check to see if new Matrix is null
   DABoolean isNullErrorMatrix = true;
   if( &aMatrix != &s_nullMatrix ) {
      for(unsigned int column = 1; column <= kMatrixSize; ++column) {
	 for(unsigned int row = column; row <= kMatrixSize; ++row) {
	    if(0 != aMatrix.fast(row,column)) {
	       isNullErrorMatrix = false;
	       goto endOfLoop;
	    }
	 }
      }
   }

  endOfLoop: clearErrorMatrix();
   if( !isNullErrorMatrix ) {
      m_errorMatrix = new HepSymMatrix(aMatrix);
   }
}

HepSymMatrix*
KTKinematicData::takeOwnershipOfPointer( void )
{
   // Remove the error matrix pointer from control of the object.
   // The matrix contents are not touched
   if( hasNullErrorMatrix() ) {
      return 0;
   }
   else {
      HepSymMatrix* tempPointer = m_errorMatrix;
      m_errorMatrix = &s_nullMatrix;
      return tempPointer;
   }
}



void
KTKinematicData::restoreOwnershipOfPointer( HepSymMatrix* const aPointer)
{
   // Restore the error matrix pointer to the object.
   // The matrix contents are not touched
   if(aPointer) {
      m_errorMatrix = aPointer;
   }
}

template <class T, unsigned int IRow, unsigned int IColumn>
class KTMatrixArray
{
   public:
      KTMatrixArray(DABoolean iClearMemory = true ) { 
	 if( iClearMemory) {clearMemory();} }
      //NOTE: indicies begin with 1
      T& operator()(unsigned int iRow, unsigned int iColumn) {
	 return m_array[ (iRow-1) + IRow*(iColumn-1) ];
      }
   private:
      void clearMemory() {
	 memset(m_array, 0, sizeof(m_array));
      }
      T m_array[IRow*IColumn];
};


void
KTKinematicData::calculate7x7ErrorMatrixFrom5x5ErrorMatrix(
                                          const KTHelix& aHelix)
{
   // This code converted from the Fortran code in kwfit.

// Build the 7x7 covariance matrix, V_w, in the Kinematic representation
// using the 5x5 Helix covariance matrix, V_c.
//
// Let W be the 7 Kinematic parameters
// Let C be the 5 Helix (CLEO) parameters
// Let A be the matrix of derivatives relating the 7 Kinematic
//      parameters to the 5 Helix parameters. The elements of
//      A are given by A_ij = d(W_i) / d(C_j).
//
// The 7x7 covariance matrix is then given by
//
//     V_w = A * V_c * A(t)
//
// The equations relating the W parameters to the C parameters are shown below.
//
// W = (px, py, pz, E, x, y, z)
//
// C = (cu, phi0, d0, ct, z0)
// cu = curvature
// phi0 = phi angle at point of closest approach to reference point
// d0 = distance of closest approach to reference point
// ct = cot(theta)
// z0 = z position at point of closest approach to reference point
//
// BfieldConstant = -0.0299792458
//
// pt = BfieldConstant * Bfield * charge / cu
//
// px = pt * cos(phi0)
// py = pt * sin(phi0)
// pz = pt * ct
// E  = sqrt(px^2 + py^2 + pz^2 + mass^2)
//  x = xref - d0 * sin(phi0)
//  y = yref + d0 * cos(phi0)
//  z = zref + z0
//
// The derivatives are obtained from the differential expressions
//       dpx = -px/cu*dcu - py*dphi0             = A11*dcu + A12*dphi0
//       dpy = -py/cu*dcu + px*dphi0             = A21*dcu + A22*dphi0
//       dpz = -pz/cu*dcu + pt*dct               = A31*dcu + A34*dct
//       de  = -p^2/E/cu*dcu + ct*pt^2/E*dct     = A41*dcu + A44*dct
//       dx  = -py/pt*dd0 - y*dphi0              = A52*dphi0+ A53*dd0
//       dy  =  px/pt*dd0 + x*dphi0              = A62*dphi0+ A63*dd0
//       dz  =  dz0                              = A75*dz0 = dz0

   if(aHelix.hasNullErrorMatrix()) {return;}

   // wsun 8-3-00: added check for zero curvature.
   double cuinv = aHelix.curvature() == 0. ? kLarge : 1. / aHelix.curvature();

   double ct = aHelix.cotTheta();
   double px = momentum().x();
   double py = momentum().y();
   double pz = momentum().z();
   double pt = sqrt(px*px + py*py);
   double p = momentum().mag();
   double Einv = 1. / energy();
   double x = position().x();
   double y = position().y();

   // Create 7x5 transformation matrix filled with zeros
   //HepMatrix A(7,5,0);
   KTMatrixArray<double, 7,5> A;

   // This makes the matrix more understandable
   const unsigned int kCurvature = KTHelix::kCurvature;
   const unsigned int kPhi0      = KTHelix::kPhi0;
   const unsigned int kD0        = KTHelix::kD0;
   const unsigned int kCotTheta  = KTHelix::kCotTheta;
   const unsigned int kZ0        = KTHelix::kZ0;

   A(kPx, kCurvature) = -px * cuinv;
   A(kPy, kCurvature) = -py * cuinv;
   A(kPz, kCurvature) = -pz * cuinv;
   A(kEnergy, kCurvature) = -p*p * Einv * cuinv;
// A(kX, kCurvature) = 0.;
// A(kY, kCurvature) = 0.;
// A(kZ, kCurvature) = 0.;


   A(kPx,kPhi0) = -py;
   A(kPy,kPhi0) =  px;
// A(kPz,kPhi0) = 0.;
// A(kEnergy,kPhi0) = 0.;
   A(kX,kPhi0) = -y;
   A(kY,kPhi0) =  x;
// A(kZ,kPhi0) = 0.;
     
// A(kPx, kD0) = 0.;
// A(kPy, kD0) = 0.;
// A(kPz, kD0) = 0.;
// A(kEnergy, kD0) = 0.;
   A(kX, kD0) = -py / pt;
   A(kY, kD0) =  px / pt;
// A(kZ, kD0) = 0.;
     
// A(kPx, kCotTheta) = 0.;
// A(kPy, kCotTheta) = 0.;
   A(kPz, kCotTheta) = pt;
   A(kEnergy, kCotTheta) = ct * pt*pt * Einv;
// A(kX, kCotTheta) = 0.;
// A(kY, kCotTheta) = 0.;
// A(kZ, kCotTheta) = 0.;

// A(kPx, kZ0) = 0.;
// A(kPy, kZ0) = 0.;
// A(kPz, kZ0) = 0.;
// A(kEnergy, kZ0) = 0.;
// A(kX, kZ0) = 0.;
// A(kY, kZ0) = 0.;
   A(kZ, kZ0) = 1.;

// Calculate V_w = A * V_c * A(t).
//   setErrorMatrix ( aHelix.errorMatrix().similarity( A ) );

// The following code is fast because it only multiplies the non-zero
// elements of A.
/*
  do i=1,5
   AVtkc(1,i) = Vtkc(1,i)*A(1,1) + Vtkc(2,i)*A(1,2)
   AVtkc(2,i) = Vtkc(1,i)*A(2,1) + Vtkc(2,i)*A(2,2)
   AVtkc(3,i) = Vtkc(1,i)*A(3,1) + Vtkc(4,i)*A(3,4)
   AVtkc(4,i) = Vtkc(1,i)*A(4,1) + Vtkc(4,i)*A(4,4)
   AVtkc(5,i) = Vtkc(2,i)*A(5,2) + Vtkc(3,i)*A(5,3)
   AVtkc(6,i) = Vtkc(2,i)*A(6,2) + Vtkc(3,i)*A(6,3)
   AVtkc(7,i) = Vtkc(5,i)
  enddo

  do i=1,7
   Vtkw(1,i) = A(1,1)*AVtkc(i,1) + A(1,2)*AVtkc(i,2)
  enddo
  do i=2,7
   Vtkw(2,i) = A(2,1)*AVtkc(i,1) + A(2,2)*AVtkc(i,2)
  enddo
  do i=3,7
   Vtkw(3,i) = A(3,1)*AVtkc(i,1) + A(3,4)*AVtkc(i,4)
  enddo
  do i=4,7
   Vtkw(4,i) = A(4,1)*AVtkc(i,1) + A(4,4)*AVtkc(i,4)
  enddo

  Vtkw(5,5) = A(5,2)*AVtkc(5,2) + A(5,3)*AVtkc(5,3)
  Vtkw(5,6) = A(5,2)*AVtkc(6,2) + A(5,3)*AVtkc(6,3)
  Vtkw(5,7) = A(5,2)*AVtkc(7,2) + A(5,3)*AVtkc(7,3)
  Vtkw(6,6) = A(6,2)*AVtkc(6,2) + A(6,3)*AVtkc(6,3)
  Vtkw(6,7) = A(6,2)*AVtkc(7,2) + A(6,3)*AVtkc(7,3)
  Vtkw(7,7) = AVtkc(7,5)

  */

// The following code is fast because it only multiplies the non-zero
// elements of A.
   const HepSymMatrix& Vtkc = aHelix.errorMatrix();
   KTMatrixArray<double, 7,5> AVtkc(false);
   unsigned int i;
   for (i=1; i < 6; ++i) {
      double Vtkc_1_i = Vtkc.fast(i,1);
      double Vtkc_2_i;
      if( i == 1) {
        Vtkc_2_i = Vtkc.fast(2,1);
      } else {
        Vtkc_2_i = Vtkc.fast(i,2);
      }
      double Vtkc_3_i = Vtkc(3,i);
      double Vtkc_4_i = Vtkc(4,i);
      AVtkc(1,i) = Vtkc_1_i*A(1,1) + Vtkc_2_i*A(1,2);
      AVtkc(2,i) = Vtkc_1_i*A(2,1) + Vtkc_2_i*A(2,2);
      AVtkc(3,i) = Vtkc_1_i*A(3,1) + Vtkc_4_i*A(3,4);
      AVtkc(4,i) = Vtkc_1_i*A(4,1) + Vtkc_4_i*A(4,4);
      AVtkc(5,i) = Vtkc_2_i*A(5,2) + Vtkc_3_i*A(5,3);
      AVtkc(6,i) = Vtkc_2_i*A(6,2) + Vtkc_3_i*A(6,3);
      AVtkc(7,i) = Vtkc.fast(5,i);
   }

   if(m_errorMatrix == &s_nullMatrix){
      m_errorMatrix = new HepSymMatrix(7,0);
   }
   HepSymMatrix& Vtkw = *m_errorMatrix;

  //when  using the 'fast' method the row index must be >= column index
   const unsigned int kMatrixSize = 7;
   for (i=1; i<kMatrixSize+1; ++i) {
      Vtkw.fast(i,1) = A(1,1)*AVtkc(i,1) + A(1,2)*AVtkc(i,2);
   }
   for (i=2; i<kMatrixSize+1; ++i) {
      Vtkw.fast(i,2) = A(2,1)*AVtkc(i,1) + A(2,2)*AVtkc(i,2);
   }
   for (i=3; i<kMatrixSize+1; ++i) {
      Vtkw.fast(i,3) = A(3,1)*AVtkc(i,1) + A(3,4)*AVtkc(i,4);
   }
   for (i=4; i<kMatrixSize+1; ++i) {
      Vtkw.fast(i,4) = A(4,1)*AVtkc(i,1) + A(4,4)*AVtkc(i,4);
   }
  Vtkw.fast(5,5) = A(5,2)*AVtkc(5,2) + A(5,3)*AVtkc(5,3);
  Vtkw.fast(6,5) = A(5,2)*AVtkc(6,2) + A(5,3)*AVtkc(6,3);
  Vtkw.fast(7,5) = A(5,2)*AVtkc(7,2) + A(5,3)*AVtkc(7,3);
  Vtkw.fast(6,6) = A(6,2)*AVtkc(6,2) + A(6,3)*AVtkc(6,3);
  Vtkw.fast(7,6) = A(6,2)*AVtkc(7,2) + A(6,3)*AVtkc(7,3);
  Vtkw.fast(7,7) = AVtkc(7,5);

}

//
// const member functions
//

KTKinematicData::ValueType 
KTKinematicData::mass( void ) const
{
   return ( m_mass ) ;
}

KTKinematicData::ValueType 
KTKinematicData::charge( void ) const
{
   return ( m_charge ) ;
}

KTKinematicData::ValueType 
KTKinematicData::px( void ) const
{
   return ( momentum().x() ) ;
}

KTKinematicData::ValueType 
KTKinematicData::py( void ) const
{
   return ( momentum().y() ) ;
}

KTKinematicData::ValueType 
KTKinematicData::pz( void ) const
{
   return ( momentum().z() ) ;
}

KTKinematicData::ValueType 
KTKinematicData::energy( void ) const
{
   return ( sqrt( ( mass() * mass() ) +
		  ( momentum().mag2() ) ) ) ;
}

KTKinematicData::ValueType
KTKinematicData::x( void ) const
{
   return ( position().x() ) ;
}

KTKinematicData::ValueType
KTKinematicData::y( void ) const
{
   return ( position().y() ) ;
}

KTKinematicData::ValueType
KTKinematicData::z( void ) const
{
   return ( position().z() ) ;
}

KTKinematicData::ValueType 
KTKinematicData::pperp( void ) const
{
   return ( sqrt( momentum().x()*momentum().x()
		+ momentum().y()*momentum().y() ) );
}

KTKinematicData::ValueType 
KTKinematicData::pperp2( void ) const
{
   return ( momentum().x()*momentum().x()
	  + momentum().y()*momentum().y() );
}

KTKinematicData::ValueType 
KTKinematicData::pmag( void ) const
{
   return ( momentum().mag() );
}

KTKinematicData::ValueType 
KTKinematicData::pmag2( void ) const
{
   return ( momentum().mag2() );
}

const HepVector3D& 
KTKinematicData::momentum( void ) const
{
   return ( m_momentum ) ;
}

const HepPoint3D& 
KTKinematicData::position( void ) const
{
   return ( m_position ) ;
}

const HepLorentzVector 
KTKinematicData::lorentzMomentum( void ) const
{
   return ( HepLorentzVector( momentum() , energy() ) ) ;
}

DABoolean 
KTKinematicData::hasFixedMass( void ) const
{
   return ( m_hasFixedMass ) ;
}

const HepSymMatrix& 
KTKinematicData::errorMatrix( void ) const
{
   return ( *m_errorMatrix ) ;
}

//
// static member functions
//







