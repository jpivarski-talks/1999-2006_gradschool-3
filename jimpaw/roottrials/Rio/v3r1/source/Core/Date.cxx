// this :
#include <Rio/Core/Date.h>

#ifdef WIN32
#include <windows.h>
#endif

//////////////////////////////////////////////////////////////////////////////
Rio::Date::Date(
)
:fDate(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  set();
}
//////////////////////////////////////////////////////////////////////////////
Rio::Date::Date(
 const Date& aDate
)
:fDate(aDate.fDate)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Date::~Date(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
unsigned int Rio::Date::date(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDate;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Date::recordSize(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return sizeof(unsigned int);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Date::setDate(
 unsigned int aDate
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fDate = aDate;
}
#include <time.h>
//////////////////////////////////////////////////////////////////////////////
void Rio::Date::set(
)
//////////////////////////////////////////////////////////////////////////////
// Set Date/Time to current time as reported by the system.
// Date and Time are encoded into one single unsigned 32 bit word.
// Date is stored with the origin being the 1st january 1995.
// Time has 1 second precision.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef WIN32
  SYSTEMTIME tp;
  ::GetLocalTime(&tp);
  unsigned int year   = tp.wYear-1900;
  unsigned int month  = tp.wMonth;
  unsigned int day    = tp.wDay;
  unsigned int hour   = tp.wHour;
  unsigned int min    = tp.wMinute;
  unsigned int sec    = tp.wSecond;
#else
  time_t tloc = ::time(0);
  struct tm *tp = (tm*)::localtime(&tloc);
  unsigned int year   = tp->tm_year;
  unsigned int month  = tp->tm_mon + 1;
  unsigned int day    = tp->tm_mday;
  unsigned int hour   = tp->tm_hour;
  unsigned int min    = tp->tm_min;
  unsigned int sec    = tp->tm_sec;
#endif
  fDate = (year-95)<<26 | month<<22 | day<<17 | hour<<12 | min<<6 | sec;
}

