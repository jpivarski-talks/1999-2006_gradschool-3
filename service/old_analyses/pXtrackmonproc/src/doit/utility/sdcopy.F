      SUBROUTINE SDCOPY ( A, B, N )
C
C Copy N elements of a  single precision array to a 
C double precision array.
C 
      INTEGER N, I
      REAL*4 A(N)
      REAL*8 B(N)

      DO 100 I = 1, N
         B(I) = A(I)
 100  CONTINUE
      RETURN
      END
