*
* $Id: tfpsum.F,v 1.1.1.1 1998/02/06 19:11:40 dpp Exp $
*
* $Log: tfpsum.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:45  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:58  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  13.51.39  by  Steve Schaffner
*CMZ :  5.53/06 30/07/90  18.15.26  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.37.13  by  Steve Schaffner
*CMZ :  5.50/00 11/05/89  16.55.12  by  Steve Schaffner
*-- Author :
      SUBROUTINE TFPSUM
C.......................................................................
C.
C. TFPSUM - Print summary of Duet track finding statistics
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : DUET
C. AUTHOR    : ? Converted to F77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : 08-Dec-86
C. LAST MOD  : 08-Dec-86
C.
C.
C. Modification Log.
C.
C.......................................................................
C
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfunitcd.inc"

      CHARACTER*4 NMSTAT(3,8)
      INTEGER KERG20,KERG22,NCDHMX,NCDFMX,NCDTMX,NCDSMX,NHITMX
      INTEGER NLNKMX,NLMBMX,NCHNMX,NRBLMX,NTRKMX,NLKTMX,NSTKMX,N,I
      INTEGER NCFOND,NLFOND,NTFOND,NTRUIN,NTRU3F,NTRUCD,NTRUDR,NDRHMX

      DATA NMSTAT/'KIND','R >=','  0',3*'    ',
     +            'KIND','R  =',' -1',3*'    ',
     +            'KIND','R  =',' -2',3*'    ',
     +            'DRED','GED ','   ',3*'    '/

      DATA NHITMX,NLNKMX,NLMBMX,NCHNMX,NRBLMX,NTRKMX,NLKTMX,NSTKMX/
     +     NHITDM,NLNKDM,NLMBDM,NCHNDM,NRBLDM,NTRKDM,NLKTDM,NSTKDM/

      DATA NCDHMX,NCDFMX,NCDTMX,NCDSMX/
     +     MHWCD ,MTRKCD,MTRKCD,MTRKDR/

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      KERG20 = - NERCTL(IERG20)
      KERG22 = - NERCTL(IERG22)

      WRITE (IUNLPT,100) NEVDU3,
     +                   NERCTL(IERG20),KERG20,
     +                   NEVDRH,
     +                   NDMUSE(IERIZH),       NERCTL(IERIZH),
     +                   NDMUSE(IERDRH),NDRHMX,NERCTL(IERDRH),
     +                   NDMUSE(IERCDH),NCDHMX,NERCTL(IERCDH),
     +                   NERCTL(IERG22),KERG22,
     +                   NEVGDR

      WRITE (IUNLPT,105) NDMUSE(IERCDF),NCDFMX,NERCTL(IERCDF),
     +                   NEVUSE,
     +                   NDMUSE(IERCDT),NCDTMX,NERCTL(IERCDT),
     +                   NDMUSE(IERCDS),NCDSMX,NERCTL(IERCDS)

      WRITE (IUNLPT,110)
     +  NDMUSE(IERHIT),NHITMX,NERCTL(IERHIT),
     +                        NCLOST(IERHIT),NLLOST(IERHIT),
     +  NDMUSE(IERLNK),NLNKMX,NERCTL(IERLNK),
     +                        NCLOST(IERLNK),NLLOST(IERLNK),
     +  NDMUSE(IERLMB),NLMBMX,NERCTL(IERLMB),
     +                        NCLOST(IERLMB),NLLOST(IERLMB),
     +  NDMUSE(IERCHN),NCHNMX,NERCTL(IERCHN),
     +                        NCLOST(IERCHN),NLLOST(IERCHN),
     +  NDMUSE(IERRBL),NRBLMX,NERCTL(IERRBL),
     +                        NCLOST(IERRBL),NLLOST(IERRBL)

      WRITE (IUNLPT,115)
     +  NDMUSE(IERTRK),NTRKMX,NERCTL(IERTRK),
     +                        NCLOST(IERTRK),NLLOST(IERTRK),
     +  NDMUSE(IERLKT),NLKTMX,NERCTL(IERLKT),
     +                        NCLOST(IERLKT),NLLOST(IERLKT),
     +  NDMUSE(IERSTK),NSTKMX,NERCTL(IERSTK),
     +                        NCLOST(IERSTK),NLLOST(IERSTK),
     +         NERCTL(IERFIT),NCLOST(IERFIT),NLLOST(IERFIT),
     +         NERCTL(IER3FT),NCLOST(IER3FT),NLLOST(IER3FT),
     +         NERCTL(IER3LP),NCLOST(IER3LP),NLLOST(IER3LP)

      WRITE (IUNLPT,120)
     +      (NTTRYD(N),NTTRYD(N+1),(NMSTAT(I,N),I=1,3),
     +       NTBADL(N),NTBADL(N+1),(NMSTAT(I,N),I=1,3),
     +       NTLOST(N),NTLOST(N+1),(NMSTAT(I,N),I=1,3),
     +       NTFIND(N),NTFIND(N+1),(NMSTAT(I,N),I=1,3),
     +       N=1,NFSTAT,2)

      NCFOND = 0
      NLFOND = 0

      DO 10 N = 1, NFSTAT, 2
          NCFOND = NCFOND + NTFIND(N)
          NLFOND = NLFOND + NTFIND(N+1)
10    CONTINUE

      NTFOND = NLFOND

      NTRUIN = 0
      NTRU3F = 0
      NTRUCD = 0
      NTRUDR = 0

      DO 20 I = -2, 2
          NTRUIN = NTRUIN + NTRTIN(I)
          NTRU3F = NTRU3F + NTRT3F(I)
          NTRUCD = NTRUCD + NTRTCD(I)
          NTRUDR = NTRUDR + NTRTDR(I)
20    CONTINUE

      WRITE (IUNLPT,125)
     +       NTFOND,NCFOND,NLFOND,(I,I=-2,2),
     +       NTRTIN,NTRUIN,NTRT3F,NTRU3F,NTRTCD,NTRUCD,NTRTDR,NTRUDR

      RETURN

  100 FORMAT( '2TFPSUM : PRINTS A SUMMARY OF EVENTS GIVEN TO DUET :'/
     + /7X,'           ','    USED     DIM   # ERR  EVENTS :'/
     + /7X,' : NEVDU3 :',8X,8X,8X,I8,' : # OF EVENTS GIVEN TO DUET3'/
     + /7X,' : LERG20 :',8X,8X,I8,I8,' : # OF EVENTS LOST WITH',
     +                                 ' ERRORS IN GETTING DIN 20',
     +                                 ' (DR HITS)'/
     + /7X,' : NEVDRH :',8X,8X,8X,I8,' : # OF EVENTS WITH GOOD',
     +                                 ' DR HITS'/
     + /7X,' : LERIZH :',I8,8X,I8,8X,' : # OF EVENTS WITH TOO MANY',
     +                                 ' IZ HITS (NOT LOST)'/
     + /7X,' : LERCDH :',I8,I8,I8,8X,' : # OF EVENTS WITH TOO MANY',
     +                                 ' CD HITS (NOT LOST)'/
     + /7X,' : LERG22 :',8X,8X,I8,I8,' : # OF EVENTS LOST WITH',
     +                                 ' ERRORS IN GETTING DIN 22',
     +                                 ' (SOLO TRACKS)'/
     + /7X,' : NEVGDR :',8X,8X,8X,I8,' : # OF EVENTS WITH GOOD',
     +                                 ' SOLO TRACKS')
  105 FORMAT(
     + /7X,' : LERCDF :',I8,I8,I8,8X,' : # OF EVENTS WITH TOO MANY',
     +                                 '  SEED TRACKS FOR /CDTRAK/',
     +                                 ' (NOT LOST IN CDFTCH)'/
     + /7X,' : NEVUSE :',8X,8X,8X,I8,' : # OF EVENTS USED'/
     + /7X,' : LERCDT :',I8,I8,I8,8X,' : # OF EVENTS WITH TOO MANY',
     +                                 '  DUET    TRACKS FOR /CDTRAK/',
     +                                 ' (NOT LOST IN CDNEWT)'/
     + /7X,' : LERCDS :',I8,I8,I8,8X,' : # OF EVENTS WITH TOO MANY',
     +                                 ' /CDTRAK/ TRACKS FOR  OUTPUT ',
     +                                 ' (NOT LOST IN CDSTOR)')
  110 FORMAT( '1TFPSUM : PRINTS A SUMMARY OF THE ARRAY USE AND',
     +                 ' OVERFLOWS  :'/
     +       /'        : ERROR  :',
     +        '    USED     DIM   # ERR  CFLOST  LFLOST :'/
     + /7X,' : LERHIT :',I8,I8,I8,2I8,' : # OF TRACKS THAT OVERFLOWED',
     +                       ' /TFHITS/'/
     + /7X,' : LERLNK :',I8,I8,I8,2I8,' : # OF TRACKS THAT OVERFLOWED',
     +                       ' /TFLINK/'/
     + /7X,' : LERLMB :',I8,I8,I8,2I8,' : # OF TRACKS THAT OVERFLOWED',
     +                       ' THE LIMB LIST IN /TFTREE/'/
     + /7X,' : LERCHN :',I8,I8,I8,2I8,' : # OF TRACKS WITH TOO MANY',
     +                       ' ROOT OR BRANCH CHAINS IN TFCLMB'/
     + /7X,' : LERRBL :',I8,I8,I8,2I8,' : # OF TRACKS WITH TOO MANY',
     +                       ' LINKS IN ROOT OR BRANCH CHAINS',
     +                       ' IN TFCLMB')
  115 FORMAT(
     + /7X,' : LERTRK :',I8,I8,I8,2I8,' : # OF TRACKS THAT OVERFLOWED',
     +                       ' /TFTRKS/'/
     + /7X,' : LERLKT :',I8,I8,I8,2I8,' : # OF TRACKS THAT OVERFLOWED',
     +                       ' /TFTRKS/ LINKS'/
     + /7X,' : LERSTK :',I8,I8,I8,2I8,' : # OF TRACKS THAT OVERFLOWED',
     +                       ' /TFSTAK/'/
     + /7X,' : LERFIT :',8X,8X,I8,2I8,' : # OF TRACK CANDIDATES',
     +                       ' WITH CHISQ < 0.0'/
     + /7X,' : LER3FT :',8X,8X,I8,2I8,' : # OF TRACK CANDIDATES',
     +                       ' WITH AN ERROR IN TF3FIT'/
     + /7X,' : LER3LP :',8X,8X,I8,2I8,' : # OF TRACK CANDIDATES',
     +                       ' WITH TF3FIT LOOPING IN CALCULATING Z')
  120 FORMAT(/' TFPSUM : PRINTS ',
     +        'TRACK STATISTICS    TFIND   CFIND   LFIND :'/
     +(/7X,' : TRIED  :',8X,8X,8X,2I8,' : # OF TRACKS TRIED IN',
     +                       ' CFIND OR LFIND : ',3A4/
     +  7X,' : BADLYR :',8X,8X,8X,2I8,' : # OF TRACKS WITH NOT',
     +                       ' ENOUGH LAYERS  : ',3A4/
     +  7X,' : LOST   :',8X,8X,8X,2I8,' : # OF TRACKS LOST  IN',
     +                       ' CFIND OR LFIND : ',3A4/
     +  7X,' : FOUND  :',8X,8X,8X,2I8,' : # OF TRACKS FOUND IN',
     +                       ' CFIND OR LFIND : ',3A4))
  125 FORMAT(
     + /7X,' : FOUND  :',8X,8X,I8,2I8,' : # OF TRACKS FOUND IN',
     +                       ' CFIND OR LFIND : TOTAL'/
     + /7X,' : KINDR  :',I8,I8,I8,2I8,' :    TOTAL :'/
     + /7X,' : FOUND  :',I8,I8,I8,2I8,' : ',I8,
     +                                ' : # OF TRACKS FROM SOLO   '/
     +  7X,' : FOUND  :',I8,I8,I8,2I8,' : ',I8,
     +                                ' : # OF TRACKS FOR  TF3FIT '/
     +  7X,' : FOUND  :',I8,I8,I8,2I8,' : ',I8,
     +                                ' : # OF TRACKS FOR /CDTRAK/'/
     +  7X,' : FOUND  :',I8,I8,I8,2I8,' : ',I8,
     +                                ' : # OF TRACKS FOR  ANLCDR ')

      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
