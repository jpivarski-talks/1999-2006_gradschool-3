      SUBROUTINE DUMP_ZD_RESPONSE( NResponse,
     &                             CellID,
     &                             TrackGeantHit,
     &                             LR_ambiguity,
     &                             DriftDistGeantHit,
     &                             DriftTime,
     &                             sinXangleGeantHit,
     &                             cosZGeantHit,
     &                             pathLengthGeantHit,
     &                             XGeantHit,
     &                             YGeantHit,
     &                             ZGeantHit,
     &                             ChargeDep,
     &                             ElossGeantHit,
     &                             HitIsEff )
C 
C  begin_doc
C 
C   Documentation for subroutine dump_zd_response
C 
C   Purpose: dump a ZD Response via the cleo3 report system
C   --------
C 
C   Input Parameters:  variables defined in MCResponseProd/Fortran/mc_zd_responses.F
C   ----------------
C 
C   Output Parameters: None
C   -----------------
C 
C   Other routines: report
C   ---------------
C 
C   Called By: MCResponseProd/Fortran/mc_zd_responses.F
C   ----------
C
C   Notes:
C   ------
C 
C   Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
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
      integer NResponse, CellId, TrackGeantHit, LR_ambiguity
      
      real DriftDistGeantHit, DriftTime, sinXangleGeantHit,
     &     cosZGeantHit,pathLengthGeantHit, ZGeantHit, ChargeDep,
     &     ElossGeantHit, XGeantHit, YGeantHit

      logical HitIsEff
C 
C   Local User defined variables:
C   -----------------------------
 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: dump_zd_response.F,v $')
      parameter(chrcsr='$Revision: 1.5 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:40 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'dump_zd_response' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine dump_zd_response:
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
        chmess='dump a ZD Response via the cleo3 report system'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif

      
      write(chmess,*) '---- begining dump of zd responses ---' 
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'NResponse= ', NResponse
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'CellID= ', CellID
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'TrackGeantHit= ', TrackGeantHit
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'LR_ambiguity= ', LR_ambiguity
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'DriftDistGeantHit= ', DriftDistGeantHit
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'DriftTime= ', DriftTime
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'sinXangleGeantHit= ', sinXangleGeantHit
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'cosZGeantHit= ', cosZGeantHit
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'pathLengthGeantHit= ', pathLengthGeantHit
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'XGeantHit= ', XGeantHit
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'YGeantHit= ', YGeantHit
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'ZGeantHit= ', ZGeantHit
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'ChargeDep= ', ChargeDep
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'ElossGeantHit= ', ElossGeantHit
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) 'HitIsEff= ', HitIsEff
      call report(MS_DEBUG, crname, chmess)

      write(chmess,*) '---- End dump of zd responses ---' 
      call report(MS_DEBUG, crname, chmess)
      
      return
      end
