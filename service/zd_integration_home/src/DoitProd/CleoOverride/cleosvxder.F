      subroutine cleosvxder(k,d0,phi,z,t,x0,v0,u0,derv,dpmin)
 
      implicit none

      integer IERR

      real k,phi,d0,t,z,x0(3),v0(3),u0(3)
      real derv(5),dpmin     
      real eta(5),x_int(3)
      real*8 dummy(5,5)
      real dummy3(3,3),dummy2

      PRINT*, '!!!!!! DANGER - DUMMY ROUTINE OF CLEOSVXDER !!!!!'

      RETURN
      END
