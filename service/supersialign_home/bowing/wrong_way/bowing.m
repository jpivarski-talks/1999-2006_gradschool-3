Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

<< Graphics`Shapes`

?Cone

Cone[(r:1, h:1, (n:20r))] is a list of n polygons approximating a cone
   centered around the z-axis with radius r and extending from -h to h.

?Cylinder

Cylinder[(r:1, h:1, (n:20r))] is a list of n polygons approximating an open
   cylinder centered around the z-axis with radius r and half height h.

Solve[ x^2 + ( s/l^2 * z2 + h - s )^2 == z2 / cotth, z2 ] // InputForm

Out[15]//InputForm= 
{{z2 -> (l^4 - 2*cotth*h*l^2*s + 2*cotth*l^2*s^2 - 
     l^2*Sqrt[l^4 - 4*cotth*h*l^2*s + 4*cotth*l^2*s^2 - 4*cotth^2*s^2*x^2])/
    (2*cotth*s^2)}, 
 {z2 -> (l^4 - 2*cotth*h*l^2*s + 2*cotth*l^2*s^2 + 
     l^2*Sqrt[l^4 - 4*cotth*h*l^2*s + 4*cotth*l^2*s^2 - 4*cotth^2*s^2*x^2])/
    (2*cotth*s^2)}}

Sqrt[l^4 - 4*cotth*h*l^2*s + 4*cotth*l^2*s^2 - 4*cotth^2*s^2*x^2]
    /. { s -> 0.000100, l -> 0.25 }

                                 -9
Out[16]= Sqrt[0.00390625 + 2.5 10   cotth - 0.000025 cotth h - 
 
           -8      2  2
>     4. 10   cotth  x ]

Out[14]= {{z2 -> 
 
         4              2              2  2
>      (l  - 2 cotth h l  s + 2 cotth l  s  - 
 
           2       4              2              2  2          2  2  2
>         l  Sqrt[l  - 4 cotth h l  s + 4 cotth l  s  - 4 cotth  s  x ]) / 
 
                  2
>       (2 cotth s )}, {z2 -> 
 
         4              2              2  2
>      (l  - 2 cotth h l  s + 2 cotth l  s  + 
 
           2       4              2              2  2          2  2  2
>         l  Sqrt[l  - 4 cotth h l  s + 4 cotth l  s  - 4 cotth  s  x ]) / 
 
                  2
>       (2 cotth s )}}

    /. { s -> 0.000100, l -> 0.25 }

                         2        2
         2            s z  2     z
Out[9]= x  + (h - s + ----)  == -----
                        2       cotth
                       l

