*
* $Id: ibitcd.F,v 1.3 2001/11/19 22:46:05 dpp Exp $
*
* $Log: ibitcd.F,v $
* Revision 1.3  2001/11/19 22:46:05  dpp
*      -> delete unused include file
*
* Revision 1.2  1998/07/09 00:24:44  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:21  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:41  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/16 07/03/94  16.58.06  by  Rob Kutschke
*CMZ :  5.54/07 21/05/91  14.40.53  by  Steve Schaffner
*CMZ :  5.54/00 30/11/90  15.57.22  by  Steve Schaffner
*CMZ :  5.53/04 18/06/90  16.18.13  by  Steve Schaffner
*CMZ :  5.52/00 08/02/90  12.51.05  by  Steve Schaffner
*CMZ :  5.51/03 31/10/89  15.41.37  by  Steve Schaffner
*CMZ :          26/10/89  12.38.23  by  Steve Schaffner
*New version to rationalize the initialization (eliminating ABITCD)
*CMZ :          09/08/89  13.17.05  by  Steve Schaffner
*CMZ :  5.50/00 16/05/89  14.18.43  by  Steve Schaffner
*-- Author :
      SUBROUTINE IBITCD
C.......................................................................
C.
C. IBITCD - Initialize arrays for bit handling routines
C.
C. COMMON    : ?
C. CALLS     : VZERO
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 01-Apr-87
C.
C. Modification Log.
C. 26-OCT-89  This version will load addresses for all devices present in the
C. geometry in radial order (with cathodes stuck at the end); will only
C. initialize once per run.  Devices that are not in the geometry are loaded
C. with address -1.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
C        IERROR   error code: 0 = ok
C                 1 = more layers than number of KBIT words allows
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfbitcom.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "seq/clutil/mesrep.inc"

C     NTOTAL -- total number of layers added to address list so far
C     IDEV -- position of the current device in the address array
C     JDEV -- number (dev. definition) of current device
C     IDVRAD -- temp. cross-ref of order of devices
      INTEGER IERROR,IDEV,NTOTAL,LRUN,INEXT(MXDVCD),I,JDEV,IFIRST
      INTEGER IDVRAD(MXDVCD)
      REAL RADIUS(MXDVCD)
      DATA LRUN/-1/

* ----------Executable code starts here---------------------

C     Only execute once per run
      IF (LRUN.EQ.NRUN) RETURN
      LRUN = NRUN

C->   Zero arrays
      IERROR = 0
      NDEVDR = 0
      IDEV = 0
      CALL VZERO( IADR,   MXDVCD*2 )
      CALL VZERO( NLYRDR, MXDVCD   )
      CALL VZERO( IDVRAD, MXDVCD   )

C     Sort devices in order of increasing radius (of first layer)
C     First initialize the pointer array
      DO 5,I=1,MXDVCD
         IF (I.LT.MXDVCD) INEXT(I) = I+1
         IF (I.EQ.MXDVCD) INEXT(I) = 0
         IF ( ILDVCD(1,I) .NE. 0 ) THEN
            RADIUS(I) = RCD(ILDVCD(1,I))
         ELSE
            RADIUS(I) = 999.999
         ENDIF
5     CONTINUE
      IFIRST = 1
C     Sort
      CALL INCSRR(IFIRST,INEXT,RADIUS)

      NTOTAL = 0

C   >>Fill array with base addresses for outward track branch
      JDEV = IFIRST
      IDEV = 0
C     First anodes (if present in geometry):
10    IF ( (ILDVCD(1,JDEV).NE.0) ) THEN
       IF ( .NOT.LCTLCD(ILDVCD(1,JDEV)) ) THEN
C                             Device is .not.cathode and is present
         IDEV = IDEV + 1
         NDEVDR = NDEVDR + 1
C        Add entry to cross-reference table
         IDVRAD(IDEV) = JDEV
         IADR(JDEV,1)   = NTOTAL
         NLYRDR(JDEV)   = NLDVCD(JDEV)
         NTOTAL         = NTOTAL + NLYRDR(JDEV)
       ENDIF
      END IF
      JDEV = INEXT(JDEV)
      IF (JDEV.NE.0) GOTO 10

C     Now cathodes
      JDEV = IFIRST
20    IF ( ILDVCD(1,JDEV).NE.0 ) THEN
       IF ( LCTLCD(ILDVCD(1,JDEV)) ) THEN
C                             Device is cathode and is present
         IDEV = IDEV + 1
         IDVRAD(IDEV) = JDEV
         IADR(JDEV,1)   = NTOTAL
         NLYRDR(JDEV)   = NLDVCD(JDEV)
         NTOTAL         = NTOTAL + NLYRDR(JDEV)
       ENDIF
      END IF

C     If device not present, load with -1
      IF (ILDVCD(1,JDEV).EQ.0) THEN
         IADR(JDEV,1) = -1
         IDEV = IDEV + 1
         IDVRAD(IDEV) = JDEV
      END IF

      JDEV = INEXT(JDEV)
      IF (JDEV.NE.0) GOTO 20


C   >>Fill array with base addresses for return track branch
C     (return track addresses continue where outward tracks left off)
      DO 30, IDEV = 1,MXDVCD
         JDEV = IDVRAD(IDEV)
         IADR(JDEV,2) = NTOTAL
         IF (IADR(JDEV,1).EQ.-1) IADR(JDEV,2) = -1
         NTOTAL       = NTOTAL + NLYRDR(JDEV)
30    CONTINUE

C   >>Check if too many layers specified
      IF((NTOTAL-1)/32.GE.MBITDR) THEN
         IERROR = 1
         CHMESS = 'Number of layers exceeds capacity of KBITDR; code needs to
     +      be rewritten'
         CALL MESLOG('IBITCD',IERROR,MSERRO)
      END IF

      RETURN
      END
