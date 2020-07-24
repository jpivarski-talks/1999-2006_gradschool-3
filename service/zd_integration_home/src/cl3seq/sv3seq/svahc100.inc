*
* $Id: svahc100.inc,v 1.1 1998/01/07 20:49:27 pg Exp $
*
* $Log: svahc100.inc,v $
* Revision 1.1  1998/01/07 20:49:27  pg
*   Added files needed for SVX hit storage. Rearranged and modified
* svgeompa.inc and svlaseq.inc to make hit-related things work.
*
* Revision 1.4  1997/03/21 01:39:03  palmer
* Mods to clinc (lite/extended roar):  svahc100.inc, svtrc100.inc,
*         clinc (roar field cleanup):  svcrr100.inc, svghr100.inc, svmhr100.inc,
*                                      svrhr100.inc, svtrr100.inc
*
* Revision 1.3  1996/03/27 08:10:20  sidev
* add new variable BZGASV to svahc100.inc
*
* Revision 1.2  1995/05/16 14:09:08  zfiles
* New version of svtr. Added another variable to svah
*
* Revision 1.1.1.1  1994/10/04  20:59:00  zfiles
* first version of seq in CVS
*
*
*
* svahc100.inc
*
C     +---------------------------------------------------------+
C     | This is svahc100.inc                                    |
C     | SVD ANALYZED HIT INFORMATION                            |
C     |                                                         |
C     | Modifications:                                          |
C     | Cleanup and comments.                M. Palmer 02/27/97 |
C     | Add entry for cluster significance.  M. Palmer 03/10/97 |
C     +---------------------------------------------------------+

C     +-----------------------------------------------------------+
C     | MXSVAH - Maximum number of SVX analyzed clusters.         |
C     |                                                           |
C     | NSVAH  - Number of analyzed clusters in the SVD detector. |
C     +-----------------------------------------------------------+
      INTEGER MXSVAH
      PARAMETER ( MXSVAH = 3000 )

      INTEGER NSVAH

C     +---------------------------------------------------------------+
C     | The following variables are saved in FULL ROAR.  They do NOT  |
C     | represent complete analyzed hit information but, in           |
C     | combination with the SVRH raw hit list and SVD constants,     |
C     | can be used to re-create all of the useful SVAH quantities.   |
C     |                                                               |
C     | IFSASV - First strip number of the cluster (RLA number).      |
C     | NHCASV - Number of hits in the cluster.                       |
C     | TKNASV - Pointer to the DUET track to which the hit was       |
C     |          assigned (=0 if none).                               |
C     +---------------------------------------------------------------+
      INTEGER IFSASV, NHCASV, TKNASV

      COMMON /SVAH00/ NSVAH, 
     +                IFSASV(MXSVAH), NHCASV(MXSVAH), TKNASV(MXSVAH)
      SAVE   /SVAH00/

C     +---------------------------------------------------------------+
C     | The following variables are NOT saved in FULL ROAR. They can  |
C     | can be re-created from the information in SVAH00 along with   |
C     | the SVRH hit list and the SVD constants.  They can also be    |
C     | saved in an EXPANDED roar output at the end user's            |
C     | discretion.                                                   |
C     |                                                               |
C     | WNOASV - Wafer number.                                        |
C     | ORIASV - R-Phi(0) or Z(1).                                    |
C     | XCEASV - center coordinate of the hit in WLCS (wafer local    |
C     |          coordinate system - meters).                         |
C     | DXCASV - error on the center coordinate of the hit in WLCS    |
C     |          (meters).                                            |
C     | QDPASV - Energy deposit (units of 1000 electron-hole pairs).  |
C     | DQDASV - Error on energy deposit (units of 1000 electron-hole |
C     |          pairs).                                              |
C     | SGNASV - Cluster significance in units of -ln(Prob)           |
C     | IQLASV - Quality of cluster:  0    - pretty good              |
C     |                               1-10 - somewhat bad             |
C     |                               >10  - pretty bad               |
C     | BZGASV - Flag for bad z-gang:  0     -  all gangs possible    |
C     |                                bit 0 -  gang 0 is bad         |
C     |                                bit 1 -  gang 1 is bad         |
C     |                                bit 2 -  gang 2 is bad         |
C     +---------------------------------------------------------------+
      INTEGER WNOASV, ORIASV 
      INTEGER IQLASV, BZGASV
      REAL XCEASV, DXCASV
      REAL QDPASV, DQDASV
      REAL SGNASV

      COMMON /SVAH01/ WNOASV( MXSVAH ), ORIASV( MXSVAH ),
     +                XCEASV( MXSVAH ), DXCASV( MXSVAH ),
     +                QDPASV( MXSVAH ), DQDASV( MXSVAH ),
     +                SGNASV( MXSVAH ),
     +                IQLASV( MXSVAH ), BZGASV( MXSVAH )
      SAVE   /SVAH01/

C     +--------------------------------------------------------------+
C     | The following variables are not stored in ANY ROAR field.    |
C     | They can be derived from the ones stored.                    |
C     |                                                              |
C     | XWWASV  Weighted centor position of the hit in the world CS. |
C     | ERWASV  Error matrix.                                        |
C     | DCNASV  Direction of the constraint (wafer, perpendicular to |
C     |         the strips).                                         |
C     | IP3ASV  Pointer to a cluster in another wafer from same      |
C     |         on layer 3 (0=layer 1, or 2).                        |
C     +--------------------------------------------------------------+
      INTEGER IP3ASV
      REAL XWWASV, ERWASV, DCNASV

      COMMON /SVAH02/ XWWASV(3, MXSVAH), ERWASV(3, 3, MXSVAH),
     +                DCNASV(3, MXSVAH), IP3ASV( MXSVAH )
      SAVE   /SVAH02/





