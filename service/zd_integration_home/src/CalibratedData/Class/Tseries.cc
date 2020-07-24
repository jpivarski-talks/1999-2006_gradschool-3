// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      Tseries
// 
// Description: Taylor Series Expansions about Point On A DriftFunction
//
// Implementation:
//
// Author:      Kenneth McLean 
// Created:     Wed Aug 12 13:00:00 EST 1998
//
// Revision history
//
#include "Experiment/Experiment.h"

// system include files
#include "C++Std/fstream.h"
#include "C++Std/iostream.h"
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
// user include files
#include "Experiment/report.h"
#include "CalibratedData/Tseries.h"
// STL include files
#include <vector>

// Constructor, Destructor

Tseries::Tseries( double D , double V , double A ) :
  m_D(D),
  m_V(V),
  m_A(A)
{}

Tseries::~Tseries(){}

// Functions to set private member data

void Tseries::setD(double D){m_D=D;}

void Tseries::setV(double V){m_V=V;}

void Tseries::setA(double A){m_A=A;}

// Functions to retrieve private member data

double Tseries::distance() const 
     {return m_D;}

double Tseries::velocity() const 
     {return m_V;}

double Tseries::acceleration() const 
     {return m_A;}

// Function to do quadratic extrapolation

double Tseries::extrapolated_value(double deltaT) const
     {
      double extrapolated_distance;
      extrapolated_distance = m_D + 
                            ( m_V * deltaT ) + 
                            ( m_A * deltaT * deltaT)/2.;
      if ( extrapolated_distance < 0 ) 
	{
          cout << "Tseries-W-(1) Bad Drift Distance " << extrapolated_distance << endl;
          extrapolated_distance = 0 ;
	}
      //  
      // Would be nice to access maximum drift distance here which would
      // be easy if this was still in CDOffCaL .... (KM)
      //
      if ( extrapolated_distance > 0.020 ) 
	{
          cout << "Tseries-W-(2) Bad Drift Distance " << extrapolated_distance << endl;
	}
      return extrapolated_distance;}

// Function to guesstimate error
// RMS Value of last term in Taylor series
//
//    SQRT
//         { integral (-deltaT,deltaT) [   (at^2/2)^2 ] /
//           integral (-deltaT,deltaT) [   1          ] }
//     = a deltaT^2 / sqrt(20)
double Tseries::extrapolated_error(double deltaT) const
     {return ( m_A * deltaT * deltaT)/4.47;}

DABoolean 
Tseries::operator==( const Tseries& rhs ) const 
{
   return ( m_D == rhs.m_D && m_V == rhs.m_V && m_A == rhs.m_A ) ;
}

DABoolean 
Tseries::operator!=( const Tseries& rhs ) const 
{
   return !operator==( rhs ) ;
}







