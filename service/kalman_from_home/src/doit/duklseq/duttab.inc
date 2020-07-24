*
* $Id: duttab.inc,v 1.1 1998/07/09 02:36:24 lyon Exp $
*
* $Log: duttab.inc,v $
* Revision 1.1  1998/07/09 02:36:24  lyon
* Adding duklseq from DPP
*
* Revision 1.2  1995/09/07 23:41:07  zfiles
* Added Id and Log. Added SAVE statements.
*
*
C
C Data structures for truth table Pattern recognition.
C
C     Maximum number of tracks.
      INTEGER NTRKMX
      PARAMETER ( NTRKMX = 150 )

C     Maximum number of hits on each track.
C     This is large enough to allow for both arcs of a curler and
C     a single cell efficiency of 100 percent.
      INTEGER NHITMX
      PARAMETER ( NHITMX = 300 )

C NTKTTA            Number of entries in this table for this event.
C IPART (  NTRKMX)  Particle ID code
C NHIT  (  NTRKMX)  Number of hits; two sides of an SVD wafer count as 1
C NHITST(  NTRKMX)  Number of stereo hits
C NHITC (  NTRKMX)  Number of cathode hits 
C NHTSVP(  NTRKMX)  Number of r-phi side SVD hits
C NHTSVZ(  NTRKMX)  Number of z side SVD hits
C VERT  (3,NTRKMX)  Position of the production vertex
C PTRA  (4,NTRKMX)  4 momentum at production vertex
C PMASS (  NTRKMX)  Mass 
C THETA (5,NTRKMX)  Track parameters at the origin computed using the
C                   position and momentum at the production vertex.
C THEND (5,NTRKMX)  Track parameters at the origin computed using the
C                   position and momentum at the last measurement.
C XC    ( ,NTRKMX)  Center of curvature
C YC    ( ,NTRKMX)  
C
C INEXT(  NHITMX,NTRKMX) Specifies order of increasing arc length.
C ILHIT(  NHITMX,NTRKMX) Layer number of hit
C IWHIT(  NHITMX,NTRKMX) Wire/Wafer number of hit
C IFLAG(  NHITMX,NTRKMX) 1 = wire
C                        2 = cathode
C                        3 = SVD
C                        4 = QDIV
C XHIT (3,NHITMX,NTRKMX) Position of a point on the track:
C                        a) at point of closest approach to wire
C                        b) in the midplane of an SVD detector
C                        c) at the effective radius of the cathode
C PHIT (3,NHITMX,NTRKMX) Momentum of track at XHIT.
C DHIT (2,NHITMX,NTRKMX) Measurement:
C                        wire:    1 = drift distance,       0 = unused
C                        cathode: 1 = z at effective radius, 2= unused
C                        svd:     1 = r-phi,               , 2= z
C                        cathode: 1 = z at effective radius, 2= unused
C WHIT (2,NHITMX,NTRKMX) Weight of the measurement.
C XWIR (  NHITMX,NTRKMX) Effective x position of wire at z=0
C YWIR (  NHITMX,NTRKMX) Effective y position of wire at z=0
C SHIT (  NHITMX,NTRKMX) Arc length from PCAO to XHIT.
C
C DIFMAX(  NTRKMX)  When PERFECT=.TRUE. ( see duklmncd.inc ) the FETCHG
C                   computes each drift distance using THETA and compares
C                   it with the drift distance as computed by CLEOG. This
C                   is the largest difference.
      REAL*4  VERT, PTRA, PMASS, THETA, THEND, DIFMAX
      REAL*4  XHIT, PHIT, DHIT, WHIT, XWIR, YWIR, SHIT

      INTEGER NTKTTA, IPART,  NHIT, NHITST, NHITC, NHTSVP, NHTSVZ
      INTEGER ILHIT, IWHIT, IFLAG, INEXT

      REAL*8  XC, YC

      COMMON /CRTTAB/ VERT  (3,NTRKMX),
     +                PTRA  (4,NTRKMX), 
     +                PMASS (  NTRKMX),
     +                THETA (5,NTRKMX),
     +                THEND (5,NTRKMX), 
     +                DIFMAX(  NTRKMX),
     +                XHIT(3,NHITMX,NTRKMX), 
     +                PHIT(3,NHITMX,NTRKMX),
     +                DHIT(2,NHITMX,NTRKMX), 
     +                WHIT(2,NHITMX,NTRKMX),
     +                XWIR(  NHITMX,NTRKMX),
     +                YWIR(  NHITMX,NTRKMX),
     +                SHIT(  NHITMX,NTRKMX)

      COMMON /CITTAB/ NTKTTA,
     +                IPART(NTRKMX),  NHIT(NTRKMX), 
     +                NHITST(NTRKMX), NHITC(NTRKMX),
     +                NHTSVP(NTRKMX), NHTSVZ(NTRKMX),
     +                ILHIT(NHITMX,NTRKMX), 
     +                IWHIT(NHITMX,NTRKMX),
     +                IFLAG(NHITMX,NTRKMX),
     +                INEXT(NHITMX,NTRKMX)
      SAVE /CITTAB/

      COMMON /CDTTAB/ XC(NTRKMX), YC(NTRKMX)
      SAVE /CDTTAB/
