      function sin2cos2(x)
      common/pawpar/dpar(5)

      sin2cos2 = dpar(1) + dpar(2)*sin(x) + dpar(3)*cos(x)
      sin2cos2 = sin2cos2 + dpar(4)*sin(2*x) + dpar(5)*cos(2*x)

      return
      end
