C 
C  $Id: hit_intime.F,v 1.3 1999/12/14 00:00:43 jjo Exp $
C 
C  $Log: hit_intime.F,v $
C  Revision 1.3  1999/12/14 00:00:43  jjo
C  Dummy routine for now.  This probably should be
C  in digitization phase of simulation.
C
C  Revision 1.2  1999/10/26 18:45:03  jjo
C  Rename DaqId to CellId
C
C  Revision 1.1  1999/10/20 21:50:34  jjo
C  Import routines for DR MC response simulation
C
C 
      LOGICAL FUNCTION HIT_INTIME( CellID, DRIFT_TIME, TOF )
C 
C  begin_doc
C 
C   Documentation for function hit_intime
C 
C   Purpose:
C   --------
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   CellID - Logical address of wire in DR3
C   DRIFT_TIME  - drift time in picoseconds
C   TOF - time of flight in picoseconds
C
C   Output Parameters: (Name - Meaning)
C   -----------------
C   Function returns .true. if  (DRIFT_TIME + TOF) is 
C   less than what is needed to integrate charge.
C
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes:
C   ------
C 
C   Author:   John J O'Neill      Created:  Tue Sep 28 14:45:37 EDT 1999
C   -------
C 
C   Detailed description:
C   ---------------------
C 
C   Major revisions:
C   ----------------
C      
C 
C  end_doc
C 
      IMPLICIT NONE
 
C  begin_inc
C   Include files :
C   ---------------------
C   Cleo3 Message Reporter
#include "ToolBox/report.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      integer CellID
      real DRIFT_TIME, TOF
C 
C   Local User defined variables:
C   -----------------------------
C     PED_TIME - time pedestal
C     TIME_MAX - Maximum time for hit to be integrated
C     TOTAL_TIME - Total time of hit - drift time + TOF
      real PED_TIME, TIME_MAX, TOTAL_TIME

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: hit_intime.F,v $')
      parameter(chrcsr='$Revision: 1.3 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/12/14 00:00:43 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'hit_intime' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine hit_intime:
#if WRITE_RCS_INFO
      if(rcsini)then
        rcsini = .false.
        write(6,*)'-------- CVSinfo --------'
        write(6,*)chrcss
        write(6,*)chrcsr//' '//chrcst
        write(6,*)chrcsd//' '//chrcsa
      endif
#endif
C ----6----------------------------------------------------------------72
 
      if (lfirst) then
        chmess='Determine if hit will make charge integration cutoff'
        call report(MS_DEBUG, crname, chmess)

        chmess='Need to implement for Cleo3.  Move to digitization ?'
        call report(MS_WARNING, crname, chmess)
        chmess='Will hardwire all hits to be in time for now'
        call report(MS_WARNING, crname, chmess)

        lfirst=.false.
      endif

      HIT_INTIME = .TRUE.

CC     Initialize output
C      HIT_INTIME = .FALSE.
C
CC      This was commented out in cleo2 but would be a good idea
CC      instead of hardwiring numbers       
Cc      CALL GET_PEDESTALS(IDEV, IDLA, TDC_PED, TDC_WID, 
Cc     +     ADC_PED, ADC_WID)
C
C
CC     What time (in ps) is the TDC pedestal?
CC     Cleo2 hardwired this.
CC      PED_TIME = PSOFKT(IDEV, IDLA, TDC_PED)
C      PED_TIME = 550000.
C
CC     Add in the device's time offset from common stop for charge integration.
CC     Just hardwired numbers for now, taken from 4s6 running of DR2
C      TIME_MAX = PED_TIME + 278000.      
C
CC     Add
C      TOTAL_TIME = DRIFT_TIME + TOF
C
CC     If TOTAL_TIME is bigger than TIME_MAX, then no charge integration occurs.
CC     Moreover, since this is after an L0, no hit is even recorded.
C      IF ( TOTAL_TIME .LE. TIME_MAX ) HIT_INTIME = .TRUE.

      return
      end



