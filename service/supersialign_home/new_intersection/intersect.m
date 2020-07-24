foursols = Solve[ s^2 == R^2 - Sqrt[ R^2 - w^2/4 ], R ] // InputForm

Out[19]//InputForm= 
{{R -> -Sqrt[1/2 + s^2 - Sqrt[1 + 4*s^2 - w^2]/2]}, 
 {R -> Sqrt[1/2 + s^2 - Sqrt[1 + 4*s^2 - w^2]/2]}, 
 {R -> -Sqrt[1/2 + s^2 + Sqrt[1 + 4*s^2 - w^2]/2]}, 
 {R -> Sqrt[1/2 + s^2 + Sqrt[1 + 4*s^2 - w^2]/2]}}

R is positive.

twosols = { #[[2]], #[[4]] }& /@ foursols

Out[20]//InputForm= 
{{R -> Sqrt[1/2 + s^2 - Sqrt[1 + 4*s^2 - w^2]/2]}, 
 {R -> Sqrt[1/2 + s^2 + Sqrt[1 + 4*s^2 - w^2]/2]}}

It should work on both sides.

Simplify[ ( R - Sqrt[ R^2 - x^2 ] /. # )& /@ twosols ]

Out[26]//InputForm= 
{Sqrt[1/2 + s^2 - Sqrt[1 + 4*s^2 - w^2]/2] - 
  Sqrt[1/2 + s^2 - Sqrt[1 + 4*s^2 - w^2]/2 - x^2], 
 Sqrt[1/2 + s^2 + Sqrt[1 + 4*s^2 - w^2]/2] - 
  Sqrt[1/2 + s^2 + Sqrt[1 + 4*s^2 - w^2]/2 - x^2]}

Out[26][[1]]

                                   2    2
              1    2   Sqrt[1 + 4 s  - w ]
Out[27]= Sqrt[- + s  - -------------------] - 
              2                 2
 
                               2    2
          1    2   Sqrt[1 + 4 s  - w ]    2
>    Sqrt[- + s  - ------------------- - x ]
          2                 2

Simplify[ ( ( w^2 + 4*s^2 ) / ( 8 * s ) )^2 ]

             2    2 2
         (4 s  + w )
Out[29]= ------------
                2
            64 s

Expand[ ( ( w^2 + 4 s^2 ) / (8 s) )^2 - x^2 ]

          2    2     4
         s    w     w       2
Out[31]= -- + -- + ----- - x
         4    8        2
                   64 s

Exit
[Mathematica finished.]
