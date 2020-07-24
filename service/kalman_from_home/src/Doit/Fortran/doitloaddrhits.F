*
* $Id: doitloaddrhits.F,v 1.5 2000/03/03 15:19:30 lyon Exp $
*
* $Log: doitloaddrhits.F,v $
* Revision 1.5  2000/03/03 15:19:30  lyon
* Get the TDC right and add quality and window information
*
* Revision 1.4  1999/12/01 22:15:00  lyon
* allow for gaps in identifier numbers
*
* Revision 1.3  1999/11/19 15:15:22  lyon
* Put tdc into TDCWCD instead of TCWCD -- oops
*
* Revision 1.2  1999/11/18 22:33:16  lyon
* tdc is a Integer, not a Real (duh)
*
* Revision 1.1  1999/05/20 13:03:58  lyon
* Needed to pass hit information through fortran functions
*
* Revision 1.1  1999/05/03 21:17:04  lyon
* Fortran/C++ communication
*
* Revision 1.1  1999/04/16 18:57:48  lyon
* for Fortran communication
*
*
      SUBROUTINE DOITLOADDRHITS(layer, wire, tdc, time,
     &     charge, phi, distance, xpos, ypos, cdla, qual,
     &     early, late, ntdc, o_idx)
*
* begin_doc
*
*  Documentation for subroutine doitloaddrhits
*
*  Purpose:
*  --------
*
*  Input Parameters:  (Name - Type - Meaning)
*  ----------------
*
*  Output Parameters:  (Name - Type - Meaning)
*  -----------------
*
*  Other routines:
*  ---------------
*
*  Notes:
*  ------
*
*  Author:   Adam Lyon      Created:  Thu Apr 15 14:08:16 EDT 1999
*  -------
*
*  Major revisions:
*  ----------------
*     
*
* end_doc
*
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
*
* begin_inc
*  Include files :
*  ---------------------
#include "seq/clutil/mesrep.inc"
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfextra.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
      INTEGER layer, wire, hitnum, cdla, fh, tdc, o_idx, qual, ntdc
      REAL time, charge, phi, distance, xpos, ypos, early, late
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: doitloaddrhits.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.5 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2000/03/03 15:19:30 $')
      PARAMETER(CHRCSA='$Author: lyon $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DOITLOADDRHITS' )
*
*  Local User defined variables:
*  -----------------------------
      INTEGER i
*
*
* end_var
*
*  Data statements:
*  ----------------
*
      SAVE RCSINI
*
      DATA RCSINI/.TRUE./

*
*  executable code for routine doitloaddrhits:
#if WRITE_RCS_INFO
      IF(RCSINI)THEN
        RCSINI = .FALSE.
        WRITE(6,*)'-------- CVSinfo --------'
        WRITE(6,*)CHRCSS
        WRITE(6,*)CHRCSR//' '//CHRCST
        WRITE(6,*)CHRCSD//' '//CHRCSA
      ENDIF
#endif
*----6----------------------------------------------------------------72
*
      NHTWCD = NHTWCD + 1

      if ( NHTWCD .gt. MHWCD ) then
        CHMESS='Doit -- Number of hits exceeded MHWCD!!'
        CALL MESLOG(CRNAME, 1, MSEMER)
        STOP
      endif
        
      i = NHTWCD

C*****Copy hit information into the common block
      ICYLCD(i) = layer
      IWIRCD(i) = wire
      TDCWCD(i) = tdc
      TCWCD(i)  = time
      TDWCD(i)  = time
      QWCD(i)   = charge
      DWCD(i)   = distance
      PHIWCD(i) = phi
      XWCD(i)   = xpos
      YWCD(i)   = ypos
      QUALCD(i) = qual
      EARLYCD(i) = early
      LATECD(i) = late
      NTDCCD(i) = ntdc

      DVHTCD(i) = i
      MAPCD(CDLA) = NHTWCD

      o_idx = i

C*****Fill some SF stuff

C*****Is this the first hit in the layer?
      fh = exfhcd(layer)
      if ( fh .eq. 0 ) then

        exfhcd(layer) = i

      else
        
C*****  Already a first hit in the layer, move it out of the way
        exfhcd(layer) = i
        exnhcd(i) = fh

      endif
      
      
      RETURN
      END



