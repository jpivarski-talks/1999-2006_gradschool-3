      SUBROUTINE FILL_GEANT_ZDHITS( NGeantHits,
     &                              GeantTracks,
     &                              GeantLayers, 
     &                              GeantHits )
C 
C  begin_doc
C 
C   Documentation for subroutine fill_geant_zdhits
C 
C   Purpose: Fill Geant zd hits by calling Geant routine GFHITS
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
C   Called By: MCResponseProd/Fortran/mc_zd_responses.F 
C   ----------
C
C   Notes: 
C   ------ 
C
C  Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
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
#include "ZDMCUtil/Fortran/geant_zd_hit_def.inc"

C   ZDMC parameters
#include "ZDMCUtil/Fortran/zdmc_param.inc"

C   Cleog ZD info

C Need suezCleog version of this
#include "Cleog3_suez/cg3cdes/zddspa.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
C     kNWordVol - # of words specifying volume # used by ZD for layer number
      integer kNWordVol
      parameter  (kNWordVol = 1)

      integer NGeantHits
      integer GeantTracks(*), GeantLayers(*)
      real GeantHits(MXZDNH,*)
C 
C   Local User defined variables:
C   -----------------------------
 
C     cuset - GFHITS: Name of ZD Set
C     TODO: update ZDGeant3SetNames.h so the set name is only defined in 1 place
      character*4 cuset
      parameter ( cuset = 'ZD  ' )

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
C     layer - loop over ZD layers
      integer NHitsInLayer, layer

C     Variables used to call gfhits - called for each ZD wire layer
C     Numbv - GEANT volume number
C     Tracks - MC track number # that caused this hit
      integer Tracks(kMaxHit), Numbv( kNWordVol, kMaxHit)
C     Hits - Array that holds the Geant hits for this layer
      real Hits(MXZDNH,kMaxHit)

c     ihit - loop over hits in given layer
C     j - loop over elements that make up a hit
      integer ihit, j

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: fill_geant_zdhits.F,v $')
      parameter(chrcsr='$Revision: 1.6 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:40 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'fill_geant_zdhits' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine fill_geant_zdhits:
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
        chmess='Filling ZD Geant hits'
        call report(MS_INFO, crname, chmess)
        lfirst=.false.
      endif

C     initialize
      NGeantHits = 0

c     Loop over ZD Wire layers & fetch hits for each layer
      DO layer = 1, kNZDLayers

C       Set Det name Need to have cudet = 'WL01' for layer #1, not 'WL1'
        write(cudet,9998) layer
 9998   format('ZL0',I1)

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
C       MXZDNH - defined in zddspa.inc.  Needs to agree with # of hit
C       elements defined in ZDWireLayerEVSProxy.cc
C       TODO: rewrite to avoid 6 character variables such as MXZDNH
        call gfhits( cuset, cudet,
     &               kNWordVol, MXZDNH,
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
            do j = 1, MXZDNH
              GeantHits(j,NGeantHits) = Hits(j,ihit)
            enddo

c D.M. 08/10/00 Obviously, sometimes NGeantHits exceeds kMaxHit=2500,
c      which is used as an arrays dimension limit.
c      Probably, the best sollution is to increase kMaxHit, but as an 
c      urgent solution I skip the rest part oh ZD hits with
c      CRITICAL message in order to look at statistics of these messages.
c D.M. 08/11/00 Changed arrays dimension from kMaxHit to kMaxHitTotal

             if( NGeantHits .ge. kMaxHitTotal ) then
              write(chmess,*) 'NGeantHits = ',NGeantHits,
     &      ' the maximal size of dimensions kMaxHitTotal has reached'
              call report( MS_CRITICAL, crname, chmess)                
              write(chmess,*) 
     &         'The rest part of ZD hits since layer = ',layer, 
     &         ' will be losed for this event'
              call report( MS_CRITICAL, crname, chmess)                

              return 

             endif

          enddo
        endif

      enddo




#if defined(DEBUG_RESPONSE)
      write(chmess,*) 'Number of Geant Hits = ', NGeantHits
      call report(MS_INFO, crname, chmess)          

      do ihit = 1, NGeantHits
        write(chmess,*) 'ZD Geant hit #',ihit
        call report(MS_INFO, crname, chmess)       
        write(chmess,*) ' GeantTracks=',GeantTracks(ihit)
        call report(MS_INFO, crname, chmess)       
        write(chmess,*) ' GeantLayers=',GeantLayers(ihit)
        call report(MS_INFO, crname, chmess)       
        do j = 1, MXZDNH
          write(chmess,*) ' GeantHits(',j, ')=',GeantHits(j,ihit)
          call report(MS_INFO, crname, chmess)       
        enddo
        chmess = '---------------------'
        call report(MS_INFO, crname, chmess)       
      enddo
#endif
      
      return
      end
