*
* $Id: bnchdoc.inc,v 1.1 1999/11/29 21:44:16 lyon Exp $
*
* $Log: bnchdoc.inc,v $
* Revision 1.1  1999/11/29 21:44:16  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.5  1995/10/26 02:55:16  zfiles
* Individual hits info stored.
*
* Revision 1.4  1995/10/10  14:54:11  zfiles
* Reorganized completely by D. Kim.
*
* Revision 1.3  1995/09/19  10:56:37  zfiles
* New version from D. kim
*
* Revision 1.2  1995/09/09  20:30:57  zfiles
* Added a variable on bnchcom.inc
*
* Revision 1.1  1994/11/07  17:55:18  zfiles
* New common blocks for the CESR bunch train.
*
*
* BNCHCOM DOCUMENTATION
*
* bnchdoc.inc
*
C...... BUNCH FINDING COMMON BLOCK DOCUMENTATION
C	
C
C....... CESR STUFF
C
C MTRNCS - Parameter. Maximum possible number of trains. (INTEGER)
C MBUNCS - Parameter. Maximum possible number of bunches. (INTEGER) 
C NTRNCS - Number of trains.  Should be nine for the moment.  (INTEGER)
C NBUNCS - Number of bunches within each train.  (INTEGER)
C DELTCS - Separation of bunches in time (picoseconds) (REAL)
C
C
C....... TOF BUNCH-FINDER STUFF
C
C TFSTTF - Peak location of the first bunch(ramping down time, in ns. real)
C
C IBUNTF - Bunch determined by ToF.  Zero if finding fails. (INTEGER)
C          Variable value is determined from P1OP2:
C             P1OP2 < 0 <==> IBUNTF = 1
C             P1OP2 > 0 <==> IBUNTF = 2
C             P1OP2 = 0 <==> IBUNTF = 0
C NHTTFB - Number of BARREL ToF counters firing. (INTEGER)
C NHTTFE - Number of END CAP ToF counters firing. (INTEGER)
C
C P1OP2  - Log (antiprob(bunch 1)/antiprob(bunch 2)).  "Net bunch evidence".
C CONFTF - Confidence of the P1OP2 result.  "Evidence consistency".
C                         points scored by "losing" hypothesis
C          Equal to:  1 - -------------------------------------
C                         points scored by "winning" hypothesis
C
C TFBNBP(I) - Points (P1OP2) scored by the Ith barrel hit.  
C TFBNEP(I) - Points (P1OP2) scored by the Ith endcap hit.
C TFBNBC(I) - Counter number (1-64) of the Ith barrel hit.
C TFBNEC(I) - Counter number (1-56) of the Ith endcap hit.
C
C
C....... DR BUNCH-FINDER STUFF
C
C LPSSHF - Logical flag that determines whether GTCDRW needs to offset
C	 - drift times.
C TFSTDR - Absolute time of the first bunch crossing.
C IBUNDR - Bunch determined by DR tracking.   Zero if finding failed. (INTEGER)
C IMODTF - Flag indicating method of determination... (INTEGER)
C           1 - Polling individual chains for bunch yeild minimum space resid.
C	    2 - Averaging over chains and picking bunch yeild minimum.
C	   -1 - Polling individual chains.  Flage set negative if method
C		2 does not agree with method one.
C
C MCHNBF - Maximum number of chains to be used. (INTEGER)
C NCHNBF - Number of chains used. Maximum of MCHNBF. (INTEGER)
C NJNKBF - Number of chains discarded. (INTEGER)
C LENCBF(I)   - Lengths of I-th good chain.  I ranges from 1
C               to MCHNBF (INTEGER)
C MFITBF(I)   - No. of hits fit for I-th chain. (INTEGER)
C IBNCBF(I)   - Bunch yielding smallest RMS residual for I-th chain. 
C RMSCBF(I,J) - RMS spatial residual for I-th chain assuming tracks
C	        originate in bunch J.  I ranges from 1 to MCHNBF, and
C	        J ranges from 1 to MBUNCS. (REAL)
C ARMSBF(J) -   average over chains of RMS spatial residual assuming tracks
C               originate in bunch J.  J ranges from 0 to MBUNCS.
C SRMSBF(J) -   sigma for previous average.
C RRMSBF(J) -   ratio of average RMS spatial residual for bunch I to
C               average RMS spatial residual for bunch giving minimum, IBUNDR.
C
C....... SUMMARY STUFF (END RESULTS)
C
C IBUNTD - Final determination of bunch number.
C IDRTF  - Flag indicating agreement or disagreement with ToF...
C           -3  Both ToF and DR bunch finders failed.
C	    -2  DR succeeded, but ToF failed.
C	    -1  ToF succeeded, but DR failed.
C	     0  Both bunch finders succeeded, but they disagree.
C	     1  Both bunch finders succeeded, and they agree.
* ------------------------------------------








