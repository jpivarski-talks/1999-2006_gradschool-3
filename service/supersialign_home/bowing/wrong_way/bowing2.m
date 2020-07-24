Solve[ (R-s)^2 + (l/2)^2 == R^2, R ]

                2      2
               l  + 4 s
Out[2]= {{R -> ---------}}
                  8 s

Solve[ ( zprime - z ) / yprime == ( z0 - z ) / y0, z ]

                 yprime z0 - y0 zprime
Out[4]= {{z -> -(---------------------)}}
                      y0 - yprime

Solve[ ( ( (zprime-z)/(z0-z) )*y0 + (l^2-4*s^2)/(8*s) )^2 + zprime^2 ==
       ( (l^2+4*s^2)/(8*s) )^2, zprime ]

Out[6]= {{zprime -> 
 
         2           2              2      2            2
>      (l  y0 z - 4 s  y0 z + 8 s y0  z - l  y0 z0 + 4 s  y0 z0 - 
 
                         4   2      2  2   2       4   2       2  2  2
>         (z - z0) Sqrt[l  y0  + 8 l  s  y0  + 16 s  y0  + 16 l  s  z  - 
 
                 2       2       3     2       2   2  2       2  2
>            16 l  s y0 z  + 64 s  y0 z  - 64 s  y0  z  - 32 l  s  z z0 + 
 
                 2                 3               2  2   2
>            16 l  s y0 z z0 - 64 s  y0 z z0 + 16 l  s  z0 ]) / 
 
                  2        2                    2
>       (2 (4 s y0  + 4 s z  - 8 s z z0 + 4 s z0 ))}, 
 
>    {zprime -> 
 
         2           2              2      2            2
>      (l  y0 z - 4 s  y0 z + 8 s y0  z - l  y0 z0 + 4 s  y0 z0 + 
 
                         4   2      2  2   2       4   2       2  2  2
>         (z - z0) Sqrt[l  y0  + 8 l  s  y0  + 16 s  y0  + 16 l  s  z  - 
 
                 2       2       3     2       2   2  2       2  2
>            16 l  s y0 z  + 64 s  y0 z  - 64 s  y0  z  - 32 l  s  z z0 + 
 
                 2                 3               2  2   2
>            16 l  s y0 z z0 - 64 s  y0 z z0 + 16 l  s  z0 ]) / 
 
                  2        2                    2
>       (2 (4 s y0  + 4 s z  - 8 s z z0 + 4 s z0 ))}}

( ( (zprime-z)/(z0-z)*y0 ) /. Out[6][[1]] )
    /. { s -> 0.000100, l -> 0.15, y0 -> 1., z0 -> 0., z -> 0 }

{ zprime - z /. Out[6][[1]], zprime /. Out[6][[2]] }
    /. { s -> 0.000100, l -> 0.15, y0 -> 1., z0 -> -0.14, z -> 0. }

Out[43]= {-0.000014, 7.72362}

Out[42]= {-0.000014, 7.72362}

Out[36]= {0.000014, -7.72362}

Out[35]= {0.14, 0.14}

Out[34]= {0.1, 0.1}

Out[33]= {0.00001, -5.56931}

Out[32]= {0.000999822, -5.5703}

Out[31]= {0.000499978, -10.8178}

Out[30]= {0.000199999, -22.5002}

Out[29]= {0.0000999998, -28.125}

Out[28]= {0.00002, -10.8173}

Out[27]= {0.00001, -5.56931}

Out[6][[1]] // InputForm

Out[37]//InputForm= 
{zprime ->


(l^2*y0*z - 4*s^2*y0*z + 8*s*y0^2*z - l^2*y0*z0 + 4*s^2*y0*z0 - 

    (z - z0)*Sqrt[l^4*y0^2 + 8*l^2*s^2*y0^2 + 16*s^4*y0^2 + 16*l^2*s^2*z^2 - 

       16*l^2*s*y0*z^2 + 64*s^3*y0*z^2 - 64*s^2*y0^2*z^2 - 32*l^2*s^2*z*z0 + 

       16*l^2*s*y0*z*z0 - 64*s^3*y0*z*z0 + 16*l^2*s^2*z0^2])/

   (2*(4*s*y0^2 + 4*s*z^2 - 8*s*z*z0 + 4*s*z0^2))

Solve[( xprime-x ) / yprime == ( x0 - x ) / y0, xprime ]

General::spell: 
   Possible spelling error: new symbol name "xprime"
     is similar to existing symbols {yprime, zprime}.

                       -(x y0) + x yprime - x0 yprime
Out[38]= {{xprime -> -(------------------------------)}}
                                     y0


( xprime /. Out[38] ) /. yprime -> ( zprime - z ) / ( z0 - z ) * y0
 // FullSimplify

          x0 z - x z0 + x zprime - x0 zprime
Out[41]= {----------------------------------}
                        z - z0

Exit
[Mathematica finished.]
