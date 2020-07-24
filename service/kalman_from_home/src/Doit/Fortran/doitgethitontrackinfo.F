*
* $Id: doitgethitontrackinfo.F,v 1.7 2001/11/20 17:57:51 dpp Exp $
*
* $Log: doitgethitontrackinfo.F,v $
* Revision 1.7  2001/11/20 17:57:51  dpp
*      -> delete unsed nonexistant include file
*
* Revision 1.6  2000/11/04 17:04:25  lyon
* comment out diagnonstic print out
*
* Revision 1.5  2000/10/25 15:37:19  lyon
* Handle translation of BUCKCD from doit to fitFlag in SeedDRHitLink link data
*
* Revision 1.4  2000/05/30 20:28:24  pg
* Aded code creating Si lattices to seed tracks
*
* Revision 1.3  2000/05/27 05:22:31  lyon
* Handle cathode clusters
*
* Revision 1.2  2000/05/19 19:58:58  cdj
* C++ now knows what device the doit hit comes from
*
* Revision 1.1  1999/05/20 13:03:54  lyon
* Needed to pass hit information through fortran functions
*
* Revision 1.1  1999/05/03 21:17:02  lyon
* Fortran/C++ communication
*
* Revision 1.1  1999/04/16 18:57:47  lyon
* for Fortran communication
*
*
      SUBROUTINE DOITGETHITONTRACKINFO( trknum, hitnum, iadr,
     &     g_device, g_layer, g_wire, g_pull, g_meas, g_buckcd, ok)
*
* begin_doc
*
*  Documentation for subroutine doitgethitontrackinfo
*
*  Purpose: Get the information for this hit on this track
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
*  Author:   Adam Lyon      Created:  Thu Apr 15 15:29:57 EDT 1999
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
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfhistpa.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/svscratch.inc"

#include "Doit/device_constants.inc"

#include "CathodeFortranAccess/cathclus.inc"

* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
      INTEGER trknum, hitnum, iadr, ok, g_device, g_layer, g_wire,
     &     g_buckcd
      REAL g_pull, g_meas
      INTEGER device_trans(4)
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: doitgethitontrackinfo.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.7 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2001/11/20 17:57:51 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DOITGETHITONTRACKINFO' )
*
*  Local User defined variables:
*  -----------------------------
*
      INTEGER NWRITE, i, j
      LOGICAL LFIRST
*
* end_var
*
*  Data statements:
*  ----------------
*
      SAVE RCSINI
      SAVE NWRITE
      SAVE LFIRST
      SAVE device_trans
*
      DATA RCSINI/.TRUE./
      DATA NWRITE/0/
      DATA LFIRST/.TRUE./
*
*  executable code for routine doitgethitontrackinfo:
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

      if ( LFIRST ) then
        LFIRST = .FALSE.
        device_trans(IDRFT) = kDoitDeviceDR
        device_trans(IDRCT) = kDoitDeviceCathode
        device_trans(ISVX1) = kDoitDeviceSVRphi
        device_trans(ISVZ1) = kDoitDeviceSVZ
      endif

      ok = 0

      if ( trknum .lt. 1 .or. trknum .gt. NTRKCD ) then
        ok = 1
        return
      endif
       
      i = trknum
      j = hitnum
      
      iadr = abs(IADFCD(j, i))

      g_device = device_trans( IDVCTF( IPLRCD(j,i) ) )      
      g_layer = ICYLCD(iadr)
      g_wire =  IWIRCD(iadr)

      g_pull = PULLCD(j, i)
      g_meas = MEASCD(j, i)
      g_buckcd = BUCKCD(j, i)

C*****Let's dump information
C      print*, 'DUMP: ', trknum, hitnum, iplrcd(j,i),
C     &     idvctf(iplrcd(j,i)), g_device, iadr,
C     &     g_layer, g_wire

      if ( g_device .eq. kDoitDeviceCathode ) then

C*****  Set the wire number to be the C++ identifier number of the cluster
C        print*, 'Got a cathode hit!', i, j, iadr, ICYLCD(iadr),
C     &       IWIRCD(iadr),
C     &       CLUSCPP(iadr)

        g_wire = CLUSCPP(iadr)
        
C        print*, 'Cathode hit ', hitnum, ' on track ', trknum,
C     &       ' iadr = ', iadr, ' Cluster# = ', g_wire
        

      endif
      if ( g_device .eq. kDoitDeviceSVRphi .or. 
     +     g_device .eq. kDoitDeviceSVZ ) then
c iadr is an index in the combined array of Si hits
c Need to get identifier in the SVR table
         g_wire = SV_INDEX(iadr)

      endif
      
      return
      END




