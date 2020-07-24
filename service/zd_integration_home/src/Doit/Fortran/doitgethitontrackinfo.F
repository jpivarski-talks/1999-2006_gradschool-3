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
      SUBROUTINE DOITGETHITONTRACKINFO( trknum, hitnum, g_iadr,
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
#include "cl3seq/cdgm3/cdgeomcd.inc"

#include "Doit/device_constants.inc"

#include "CathodeFortranAccess/cathclus.inc"

* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
      INTEGER trknum, hitnum, g_iadr, ok, g_device, g_layer, g_wire,
     &     g_buckcd
      REAL g_pull, g_meas
      INTEGER iadr_CD,layer_CD
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
*
* end_var
*
*  Data statements:
*  ----------------
*
      SAVE RCSINI
      SAVE NWRITE
*
      DATA RCSINI/.TRUE./
      DATA NWRITE/0/
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

      if(
     1     (trknum.lt.1).or.
     2     (trknum.gt.NTRKCD))then
        ok = 1

      else
        ok = 0
        i = trknum
        j = hitnum
        
C IADFCD is fill with IADFIT in CDNEWT
        iadr_CD = abs(IADFCD(j,i))
        
        g_device =IDVCTF(IPLRCD(j,i))       
        layer_CD = ICYLCD(iadr_CD)
        g_wire =  IWIRCD(iadr_CD)
        
        g_pull = PULLCD(j, i)
        g_meas = MEASCD(j, i)
        g_buckcd = BUCKCD(j, i)
        
C*****Let's dump information
c        print 9901, trknum, hitnum, iplrcd(j,i),
c     &       g_device, iadr_CD, layer_CD, g_wire
c 9901   FORMAT(' doitgethitontrackinfo:',
c     1       '  trk=',I3,'  hit',I3,'  tflyr',I3,'   devcode',I3,
c     2       '  adr',I5,'  lyr',I3,'  wir',I5)




        if (g_device.eq.kDoitDeviceDR ) then

C***** Set the layer and address  to the local device
          g_iadr=iadr_CD - (EvntHit_1stDev(g_device)-1)
          g_layer=layer_CD - (ILDVCD(1,g_device)-1)



        elseif(g_device.eq.kDoitDeviceZD ) then

C***** Set the layer and address  to the local device
          g_iadr=iadr_CD - (EvntHit_1stDev(g_device)-1)
          g_layer=layer_CD - (ILDVCD(1,g_device)-1)

c          print 9902
c 9902     format(' doitgethitontrackinfo:',' Got a ZD HIT !') 



        elseif(g_device.eq.kDoitDeviceCathode ) then
          
C*****  Set the wire number to be the C++ identifier number of the cluster
          g_wire = CLUSCPP(iadr_CD)
          g_iadr=iadr_CD
          g_layer=layer_CD
          
C        print*, 'Got a cathode hit!', i, j, iadr_CD, 
C     &       ICYLCD(iadr_CD),IWIRCD(iadr_CD),CLUSCPP(iadr_CD)
          
          
C        print*, 'Cathode hit ', hitnum, ' on track ', trknum,
C     &       ' iadr = ', iadr_CD, ' Cluster# = ', g_wire
          

          

        elseif(
     1         (g_device.eq.kDoitDeviceSVRphi).or. 
     2         (g_device.eq.kDoitDeviceSVZ ))then
c iadr_CD is an index in the combined array of Si hits
c Need to get identifier in the SVR table
          g_wire = SV_INDEX(iadr_CD)
          g_iadr=iadr_CD
          g_layer=layer_CD



        endif
      endif
      
      return
      END











