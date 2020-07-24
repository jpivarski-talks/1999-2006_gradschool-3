#!/usr/local/bin/perl

@v4 = ();
@v5 = ();

open(V4, "version4.drgarfield_distancecorrection");
while (<V4>) {
    chop;
    @tmp = split(/\s+/);
    push(@v4, [ @tmp ] );
}
close(V4);

open(V5, "version5.drgarfield_distancecorrection");
while (<V5>) {
    chop;
    @tmp = split(/\s+/);
    push(@v5, [ @tmp ] );
}
close(V5);

die if ( $#v4 != $#v5 );

for ( $i = 0;  $i <= $#v4;  $i++ ) {
    @a = @{ $v4[$i] };
    @b = @{ $v5[$i] };
    die if ($a[0] != $b[0]);
    die if ($a[1] != $b[1]);
    die if ($a[2] != $b[2]);
    die if ($a[3] != $b[3]);
    if ( abs($a[4]) > 0.  &&  abs($a[4]) > 0. ) {
	print( $a[4]/$b[4], "\n" );
    }
    else {
	die if ( $a[4] != $b[4] );
    }
}
