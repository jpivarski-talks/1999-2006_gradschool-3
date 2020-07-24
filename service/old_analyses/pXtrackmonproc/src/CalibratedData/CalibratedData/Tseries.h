#ifndef TSERIES_H
#define TSERIES_H
// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      Tseries
// 
// Description: Taylor Expansion of Drift Functions about a time point
//
// Usage:
//   
// Author:      Kenneth McLean 
// Created:     Wed Aug 12 13:00:00 EST 1998
//
// Revision history
//

class Tseries {

public:

  // constructor, copy constructor destructor
  Tseries( double D=0, double V=0 , double A=0);
  //Tseries( const Tseries& );
  //Tseries & operator = (const Tseries & );
  virtual ~Tseries();

  // output functions
  double distance() const;
  double velocity() const;
  double acceleration() const;
  double extrapolated_value(double) const;
  double extrapolated_error(double) const;

  // input functions
  void setD(double);
  void setV(double);
  void setA(double);

      DABoolean operator==( const Tseries& rhs ) const ;
      DABoolean operator!=( const Tseries& rhs ) const ;

private: 
  double m_D, m_V, m_A;
};

#endif 




