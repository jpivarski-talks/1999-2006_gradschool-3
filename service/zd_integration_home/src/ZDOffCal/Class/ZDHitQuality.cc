// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      ZDDriftFunction
// 
// Description: This is the root class for DriftFunction classes
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDHitQuality.cc,v 1.1 2003/04/24 18:08:11 huanggs Exp $
//
// Revision history
//
// $Log: ZDHitQuality.cc,v $
// Revision 1.1  2003/04/24 18:08:11  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:51  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:57  huanggs
// imported source
//
//

// compiler bug flags
#include "Experiment/Experiment.h"

// compiler bug fixes 
#include "ZDOffCal/ZDHitQuality.h"

static const char* const kFacilityString   = "ZDOffCal.ZDHitQuality" ;

const int   ZDHitQuality::Flag_Bad          =   1;
const int   ZDHitQuality::Flag_SingleTDC    =   2;
const int   ZDHitQuality::Flag_TestChannel  =   4;
const int   ZDHitQuality::Flag_NoCharge     =   8;
const int   ZDHitQuality::Flag_NoTime       =  16;
const int   ZDHitQuality::Flag_WindowNarrow =  32;
const int   ZDHitQuality::Flag_WindowWide   =  64;
const int   ZDHitQuality::Flag_WindowHigh   = 128;
const int   ZDHitQuality::Flag_WindowLow    = 256;

ZDHitQuality::ZDHitQuality(){m_quality = Flag_Bad;}
ZDHitQuality::ZDHitQuality(int value): m_quality(value){}
ZDHitQuality::~ZDHitQuality(){}

DABoolean ZDHitQuality::test_all() const
{
  if ( m_quality != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean ZDHitQuality::test_good() const
{
  if ( ( m_quality & Flag_Bad ) == 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean ZDHitQuality::test_bad() const
{
  if ( ( m_quality & Flag_Bad ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean ZDHitQuality::test_singletdc() const
{
  if ( ( m_quality & Flag_SingleTDC ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean ZDHitQuality::test_testchannel() const
{
  if ( ( m_quality & Flag_TestChannel ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean ZDHitQuality::test_nocharge() const
{
  if ( ( m_quality & Flag_NoCharge ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean ZDHitQuality::test_notime() const
{
  if ( ( m_quality & Flag_NoTime ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean ZDHitQuality::test_windowNarrow() const
{
  if ( ( m_quality & Flag_WindowNarrow ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean ZDHitQuality::test_windowWide() const
{
  if ( ( m_quality & Flag_WindowWide ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean ZDHitQuality::test_windowHigh() const
{
  if ( ( m_quality & Flag_WindowHigh ) != 0 ) 
    { return true; }
  else
    { return false; }
}
DABoolean ZDHitQuality::test_windowLow() const
{
  if ( ( m_quality & Flag_WindowLow ) != 0 ) 
    { return true; }
  else
    { return false; }
}

void ZDHitQuality::set_good()
{
  // clear 1st bit "and (compliment 0 exclusive or 1)"
  m_quality = m_quality & ( (~0) ^ 1 ) ;
}
void ZDHitQuality::set_bad()
{
  m_quality = m_quality | Flag_Bad ;
}
void ZDHitQuality::set_singletdc()
{
  m_quality = m_quality | Flag_SingleTDC ;
}
void ZDHitQuality::set_testchannel()
{
  m_quality = m_quality | Flag_TestChannel ;
}
void ZDHitQuality::set_nocharge()
{
  m_quality = m_quality | Flag_NoCharge ;
}
void ZDHitQuality::set_notime()
{
  m_quality = m_quality | Flag_NoTime ;
}
void ZDHitQuality::set_windowNarrow()
{
  m_quality = m_quality | Flag_WindowNarrow ;
}
void ZDHitQuality::set_windowWide()
{
  m_quality = m_quality | Flag_WindowWide ;
}
void ZDHitQuality::set_windowHigh()
{
  m_quality = m_quality | Flag_WindowHigh ;
}
void ZDHitQuality::set_windowLow()
{
  m_quality = m_quality | Flag_WindowLow ;
}
int ZDHitQuality::value()
{
  return m_quality ;
}
