C 
C  $Id: fill_geant_drhits.F,v 1.12 2002/08/07 21:26:39 bkh Exp $
C 
C  $Log: fill_geant_drhits.F,v $
C  Revision 1.12  2002/08/07 21:26:39  bkh
C  Move to using guser_suez and Cleog3_suez in place of guser and Cleog3
C
C  Revision 1.11  2000/08/11 19:54:47  dubrovin
C  Change in check of arrays dimension kMaxHit -> kMaxHitTotal
C
C  Revision 1.10  2000/08/10 17:02:39  dubrovin
C  Insert protection from NGeantHits overflow
C
C  Revision 1.9  2000/04/18 14:23:40  pg
C  Added protection against GFHITS overflow
C
C  Revision 1.8  2000/02/15 00:31:02  jjo
C  Remove dummy hits
C
C  Revision 1.7  1999/12/13 01:45:54  jjo
C  Use suezCleog version of cddspa.inc & use report for
C  debugging messages.
C
C  Revision 1.6  1999/12/08 00:17:42  jjo
C  Updated dummy dr hits used for development
C
C  Revision 1.5  1999/11/22 01:16:17  jjo
C  Update to get hits for each det as we have 1 Geant 'DET' per layer in
C  cleo3
C
C  Revision 1.4  1999/10/27 22:39:37  jjo
C  Fix CPP flag for DUMMY_DR_HITS
C
C  Revision 1.3  1999/10/26 18:35:32  jjo
C  Improve docs.  Still waiting for DRWireLayer to build
C  Geant volumes - then uncomment code.
C
C  Revision 1.2  1999/10/21 19:56:04  jjo
C  Comment out dummy dr hits in official code
C
C  Revision 1.1  1999/10/20 21:50:31  jjo
C  Import routines for DR MC response simulation
C
C 
      SUBROUTINE FILL_GEANT_DRHITS( NGeantHits,
     &                              GeantTracks,
     &                              GeantLayers, 
     &                              GeantHits )
C 
C  begin_doc
C 
C   Documentation for subroutine fill_geant_drhits
C 
C   Purpose: Fill Geant dr hits by calling Geant routine GFHITS
C   -------- and return arrays with this info.
C 
C   Input Parameters:  None
C   ----------------
C
C   Output Parameters: 
C   -----------------
C   NGeantHits  - Number of Geant hits read
C   GeantTracks - Array of MC tracks that correspond to Geant hit
C   GeantLayers - Array of layer number of Geant hit
C   GeantHits   - 2D Array of Geant hit
C
C   Other routines: cqisid, GFHITS
C   ---------------
C 
C   Called By: MCResponseProd/Fortran/mc_dr_responses.F 
C   ----------
C
C   Notes: 
C   ------ 
C
C   Author:   John J O'Neill      Created:  Wed Sep 29 13:02:36 EDT 1999
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

C   Offsets into Geant hit bank
#include "DRMCUtil/Fortran/geant_dr_hit_def.inc"

C   DRMC parameters
#include "DRMCUtil/Fortran/drmc_param.inc"

C   Cleog DR info
C#include "Cleog3_suez/cg3cdes/cddsco.inc"
C#include "Cleog3_suez/cg3cdes/cgspec.inc"

C Need suezCleog version of this
#include "Cleog3_suez/cg3cdes/cddspa.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
C     kNWordVol - # of words specifying volume # used by DR for layer number
      integer kNWordVol
      parameter  (kNWordVol = 1)

      integer NGeantHits
      integer GeantTracks(*), GeantLayers(*)
      real GeantHits(MXCDNH,*)
C 
C   Local User defined variables:
C   -----------------------------
 
C     cuset - GFHITS: Name of DR Set
C     TODO: update DRGeant3SetNames.h so the set name is only defined in 1 place
      character*4 cuset
      parameter ( cuset = 'DR3 ' )

C     cudet - GFHITS: Name of Det
      character*4 cudet

C kAllTracks - Tell Geant to return hits from all of the tracks
C kAllValidVol - Tell Geant to use all valid numbers of volumes
      integer kAllTracks, kAllValidVol
      parameter(kAllTracks=0, kAllValidVol=0)


C     iGeantHit  - loop index for Geant hits
c     integer iGeantHit

C     iset, idet - geant set,det names
      integer iset, idet

C     NHitsInLayer - number of hits in given layer
C     layer - loop over DR layers
      integer NHitsInLayer, layer

C     Variables used to call gfhits - called for each DR wire layer
C     Numbv - GEANT volume number
C     Tracks - MC track number # that caused this hit
      integer Tracks(kMaxHit), Numbv( kNWordVol, kMaxHit)
C     Hits - Array that holds the Geant hits for this layer
      real Hits(MXCDNH,kMaxHit)

c     ihit - loop over hits in given layer
C     j - loop over elements that make up a hit
      integer ihit, j

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: fill_geant_drhits.F,v $')
      parameter(chrcsr='$Revision: 1.12 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2002/08/07 21:26:39 $')
      parameter(chrcsa='$Author: bkh $')
 
      character*(*) crname
      parameter(    crname = 'fill_geant_drhits' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine fill_geant_drhits:
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
        chmess='Filling DR Geant hits'
        call report(MS_INFO, crname, chmess)
        lfirst=.false.
      endif

C     initialize
      NGeantHits = 0

c     Loop over DR Wire layers & fetch hits for each layer
      DO layer = 1, kNDRLayers

C       Set Det name Need to have cudet = 'WL01' for layer #1, not 'WL1'
        if ( layer .le. 9 ) then
          write(cudet,9998) layer
 9998     format('WL0',I1)
        else
          write(cudet,9999) layer
 9999     format('WL',I2)
        endif

c       Initialize number of hits found in each layer
        NHitsInLayer = 0

C       Find iset, idet based on cuset & cudet
C       iset & idet aren't used but if they're .le.0 somethings wrong
        call cqisid( cuset, cudet, iset, idet )
        if ( (iset .le. 0) .or. (idet .le. 0) ) then
          write(chmess,*) ' cqisid error.  iset=', iset, ' idet=',idet
          call report(MS_EMERGENCY, crname, chmess)          
        endif
        
C       Get Geant hits - track#, vol#, hits
C       MXCDNH - defined in cddspa.inc.  Needs to agree with # of hit
C       elements defined in DRWireLayerEVSProxy.cc
C       TODO: rewrite to avoid 6 character variables such as MXCDNH
        call gfhits( cuset, cudet,
     &               kNWordVol, MXCDNH,
     &               kMaxHit, kAllTracks, kAllValidVol, 
     &               Tracks, Numbv, Hits, NHitsInLayer )

        if ( NHitsInLayer .gt. kMaxHit ) then
c  In case of overflow gfhits in its infinite wisdom returns Nhits=Maxhit+1
c  Reset it to Maxhit to avoid overstepping array bounds
          NHitsInLayer = kMaxHit
          write(chmess,*) 'Too many hits, using = ', kMaxHit
          call report(MS_EMERGENCY, crname, chmess)          
        endif          

C       If there are hits in this layer add them to output arrays
        if ( NHitsInLayer .ge. 1 ) then
          do ihit = 1, NHitsInLayer
C           Increment # of total Geant hits 
            NGeantHits = NGeantHits + 1
C           Copy MC track # that caused this hit
            GeantTracks(NGeantHits) = Tracks(ihit)
C           Numbv is zero, for some unknown reason, so use layer # which is Ok 
            GeantLayers(NGeantHits) = layer
C           Hits array
            do j = 1, MXCDNH
              GeantHits(j,NGeantHits) = Hits(j,ihit)
            enddo

c D.M. 08/10/00 Obviously, sometimes NGeantHits exceeds kMaxHit=2500,
c      which is used as an arrays dimension limit.
c      Probably, the best sollution is to increase kMaxHit, but as an 
c      urgent solution I skip the rest part oh DR3 hits with
c      CRITICAL message in order to look at statistics of these messages.
c D.M. 08/11/00 Changed arrays dimension from kMaxHit to kMaxHitTotal

             if( NGeantHits .ge. kMaxHitTotal ) then
              write(chmess,*) 'NGeantHits = ',NGeantHits,
     &      ' the maximal size of dimensions kMaxHitTotal has reached'
              call report( MS_CRITICAL, crname, chmess)                
              write(chmess,*) 
     &         'The rest part of DR3 hits since layer = ',layer, 
     &         ' will be losed for this event'
              call report( MS_CRITICAL, crname, chmess)                

              return 

             endif

          enddo
        endif

      enddo




#if defined(DEBUG_RESPONSE)
      write(chmess,*) 'Number of Geant Hits = ', NGeantHits
      call report(MS_DEBUG, crname, chmess)          

      do ihit = 1, NGeantHits
        write(chmess,*) 'DR Geant hit #',ihit
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' GeantTracks=',GeantTracks(ihit)
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' GeantLayers=',GeantLayers(ihit)
        call report(MS_DEBUG, crname, chmess)       
        do j = 1, MXCDNH
          write(chmess,*) ' GeantHits(',j, ')=',GeantHits(j,ihit)
          call report(MS_DEBUG, crname, chmess)       
        enddo
        chmess = '---------------------'
        call report(MS_DEBUG, crname, chmess)       
      enddo
#endif
      
      return
      end



