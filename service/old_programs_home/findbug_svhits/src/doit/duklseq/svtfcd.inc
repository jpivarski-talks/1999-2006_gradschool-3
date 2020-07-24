*
* $Id: svtfcd.inc,v 1.1 1998/07/09 02:36:26 lyon Exp $
*
* $Log: svtfcd.inc,v $
* Revision 1.1  1998/07/09 02:36:26  lyon
* Adding duklseq from DPP
*
* Revision 1.1  1997/04/15 04:13:09  clib
* New release.
*
*
C     +---------------------------------------------------------+
C     | This file is svtfcd.inc                                 |
C     | Contains variables controlling the linking of SVTF      |
C     | standalone track segments to DUET drift chamber tracks. |
C     |                                                         |
C     | AddSVT    Logicial indicating whether to add SVTF track |
C     |           segments to DUET tracks.                      |
C     | Do5SVT    Logical indicating whether to add 5-hit SVTF  |
C     |           track segments to DUET tracks.                |
C     | Do4SVT    Logical indicating whether to add 4-hit SVTF  |
C     |           track segments to DUET tracks.                |
C     | CkAllSVT  Check all SVTF track segments and report.     |
C     | RankSVT   Use the hierarchical linking algorithm (see   |
C     |           svdpr.F).                                     | 
C     | LnkSVT(3) Chisquare cutoff for good links in the 3      |
C     |           linking passes performed by SVDPR.            |
C     |                                                         |
C     | Author:  M. Palmer                              2/16/97 |
C     +---------------------------------------------------------+
      Logical AddSVT
      Logical Do5SVT
      Logical Do4SVT
      Logical CkAllSVT
      Logical RankSVT
      Real LnkSVT

      Common /SVTFCD/ AddSVT, Do5SVT, Do4SVT, CkAllSVT, RankSVT, 
     +                LnkSVT(3)
      Save   /SVTFCD/
