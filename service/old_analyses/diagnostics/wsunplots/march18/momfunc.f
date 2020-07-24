      function momfunc(x)
      common/pawpar/par(7)
      real gauss, bif

c      par(1) = gauss normalization
c      par(2) = gauss mean
c      par(3) = gauss sigma
c      par(4) = bif normalization
c      par(5) = bif mean
c      par(6) = bif sigmaleft
c      par(7) = bif sigmaright

      gauss = par(1)*exp(-(x-par(2))**2/2/par(3))

      if ( x .lt. par(5) ) then
         bif = par(4)*exp(-(x-par(5))**2/2/par(6))
      else
         bif = par(4)*exp(-(x-par(5))**2/2/par(7))
      endif

      momfunc = gauss + bif
      end
