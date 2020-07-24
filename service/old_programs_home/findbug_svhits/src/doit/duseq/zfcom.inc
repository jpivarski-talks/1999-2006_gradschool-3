*
* $Id: zfcom.inc,v 1.2 1998/05/08 14:13:55 pg Exp $
*
* $Log: zfcom.inc,v $
* Revision 1.2  1998/05/08 14:13:55  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.5  1996/04/24 01:13:20  zfiles
* Add variables for THDD bank.
* Update comments.
*
* Revision 1.4  1996/04/23 21:50:35  zfiles
* Comments in previous version are wrong!
* True comment:
*      Variables to access the TRHT, THID, THPL, THS1, THS2 banks.
*
* Revision 1.3  1996/04/23 07:28:58  zfiles
* Add new stack variables for kalman output.
*
* Revision 1.2  1995/09/05  13:34:49  zfiles
*  Add IHKLMN.
*
* Revision 1.1.1.1  1994/10/08  01:01:10  zfiles
* first version of doit in CVS
*
*
*CMZ :  6.00/04 08/08/93  17.33.32  by  Rob Kutschke
*CMZ :  5.54/15 03/10/92  21.02.25  by  Rob Kutschke
*CMZ :  5.54/14 28/09/92  20.36.21  by  Rob Kutschke
*CMZ :  5.52/00 23/01/90  19.17.54  by  Steve Schaffner
*-- Author :
*  Common block of variables needed for ZFILES calls
#include "cl3seq/zfiles/quest.inc"
#include "cl3seq/zfiles/usbank.inc"
#if !defined(CLEO_NOCOMDOC)
*
*   Documentation for ZFCOM
*
*
*IDCDAN -- ZFILES id no. for bank CDAN (analyzed data header)
*IDTRKF -- ZFILES id no. for bank TRKF (track header bank)
*IDTRAK -- ZFILES id no. for bank TRAK (track bank)
*IDVPTX -- ZFILES id no. for bank VPTX (primary vertex bank)
*IDTRHT -- ZFILES id no. for bank TRHT (track hits)
*IDTHID -- ZFILES id no. for bank THID (hit id)
*IDTHPL -- ZFILES id no. for bank THPL (residuals)
*IDTHS1 -- ZFILES id no. for bank THS1 (device resolutions)
*IDTHS2 -- ZFILES id no. for bank THS2 (extrapolation error)
*IDTHDD -- ZFILES id no. for bank THDD (fully corrected drift distance)
*
*IPCDAN -- ZFILES pointer to bank CDAN (analyzed data header)
*IPTRKF -- ZFILES pointer to bank TRKF (track header bank)
*IPTRAK(MTRKDR) -- pointers to TRAK banks (IPTRAK(1) is top bank = 1st track)
*IPTRHT(MTRKDR) -- pointers to TRHT banks (track hits)
*IPTHID(MTRKDR) -- pointers to THID banks (hit id)
*IPTHDD(MTRKDR) -- pointers to THDD banks (fully corrected drift distance)
*IPTHPL(MTRKDR) -- pointers to THPL banks (residuals)
*IPTHS1(MTRKDR) -- pointers to THS1 banks (device resolutions)
*IPTHS2(MTRKDR) -- pointers to THS2 banks (extrapolation error)
*IPVPTX -- ZFILES pointer to bank VPTX (primary vertex bank)
*IPTRHT -- ZFILES pointer to bank TRHT (track hits)
*IPTHID -- ZFILES pointer to bank THID (hit id)
*IPTHPL -- ZFILES pointer to bank THPL (residuals)
*IPTHS1 -- ZFILES pointer to bank THS1 (device resolutions)
*IPTHS2 -- ZFILES pointer to bank THS2 (extrapolation error)
*
*IHCDAN -- 'CDAN' translated into Hollerith constant
*IHDUET -- 'DUET' translated into Hollerith constant
*IHTRAK -- 'TRAK' translated into Hollerith constant
*IHTRKF -- 'TRKF' translated into Hollerith constant
*IHVPTX -- 'VPTX' translated into Hollerith constant
*IHKLMN -- 'KLMN' translated into Hollerith constant
*IHTRHT -- 'TRHT' translated into Hollerith constant
*IHTHID -- 'THID' translated into Hollerith constant
*IHTHPL -- 'THPL' translated into Hollerith constant
*IHTHS1 -- 'THS1' translated into Hollerith constant
*IHTHS2 -- 'THS2' translated into Hollerith constant
*
*JBIAS(9) -- Bias in parent bank for banks (TRKF,TRAK,VPTX)
*
* ------------------------------------------
#include "doit/duseq/duseqd.inc"

#endif
*
*Pointers and such:

      INTEGER IDCDAN,IDTRKF,IDTRAK,IDVPTX,IPCDAN,IPTRKF,IPTRAK,
     + IPVPTX,IHDUET,IHCDAN,IHTRAK,IHTRKF,IHVPTX,JBIAS,IHKLMN,
     + IHTRHT,IHTHID,IHTHPL,IHTHS1,IHTHS2,IHTHDD,
     + IDTRHT,IDTHID,IDTHPL,IDTHS1,IDTHS2,IDTHDD,
     + IPTRHT,IPTHID,IPTHPL,IPTHS1,IPTHS2,IPTHDD

      COMMON/ZFCOM/IDCDAN,IDTRKF,IDVPTX,IDTRAK,IPCDAN,IPTRKF,IPVPTX,
     + IPTRAK(MTRKDR),IHDUET,IHCDAN,IHTRAK,IHTRKF,IHVPTX,JBIAS(9),
     + IHKLMN,
     + IHTRHT,IHTHID,IHTHPL,IHTHS1,IHTHS2,IHTHDD,
     + IDTRHT,IDTHID,IDTHPL,IDTHS1,IDTHS2,IDTHDD,
     + IPTRHT(MTRKDR),IPTHID(MTRKDR),IPTHPL(MTRKDR),
     + IPTHS1(MTRKDR),IPTHS2(MTRKDR),IPTHDD(MTRKDR)

C
C Offsets to variables in Zebra banks.
C
#include "cl3seq/zfiles/zfdupar.inc"
C
