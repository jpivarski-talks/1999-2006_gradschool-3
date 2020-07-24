*
* $Id: trkpar.inc,v 1.1 1999/11/29 21:43:40 lyon Exp $
*
* $Log: trkpar.inc,v $
* Revision 1.1  1999/11/29 21:43:40  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.6  1996/05/10 13:12:14  zfiles
* Add QDIVA1.
*
* Revision 1.5  1996/04/23 23:50:49  zfiles
* Comments in previous version are wrong!
* True comment:
*      Moved QDIVAD into here from duet/duseq/tfindpar.inc
*
* Revision 1.4  1996/04/23 07:07:24  zfiles
* Mods from Rob.
*
* Revision 1.3  1995/08/24 13:27:02  zfiles
* Changed MXTRK1 to 25 (from 40)
*
*
C 
C MTRAK1 - Maximum number of tracks.
C
C NTKCOP - Number of different copies of each track which are kept.
C          Currently 6, 5 different mass hypotheses reported at the
C          origin and the pion hypothesis reported at the last hit
C          on the track.
C
C MXTRAK - Dimension of arrays holding tracking information.
C
      INTEGER NTKCOP
      PARAMETER ( NTKCOP = 6 )
      INTEGER MTRAK1 
      PARAMETER ( MTRAK1 = 25 )
      INTEGER MXTRAK
      PARAMETER ( MXTRAK = MTRAK1*NTKCOP)

C Flag for charge division hits:
C Layer number is set to the real layer number + QDIVAD.

      INTEGER QDIVAD
      PARAMETER ( QDIVAD = 100 )

C Similar flag but for use in the THID zebra bank.
C For QDIV hits, the CDLA is encoded as CDLA+QDIVA1.

      INTEGER QDIVA1
      PARAMETER ( QDIVA1 = 100000 )
