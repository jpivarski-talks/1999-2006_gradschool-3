#!/usr/local/bin/perl

$base = $ARGV[0];
die if $base eq "";

$oldgeom = $ARGV[1];
die if $oldgeom eq "";

$newgeom = $ARGV[2];
die if $newgeom eq "";

$paw = "/nfs/cleo/dlib/cern/bin/paw";

sub moveable { 1; } # { @_[0] < 5; }

$linethree = 0;
open( OLDGEOM, $oldgeom );
while( <OLDGEOM> ) {
    $linethree = 1 if ( $_ =~ /^\s*3/ );
    if ( $linethree == 0 ) {
	print $_;
	$header .= $_;
    } else {
	chop;
	( $index, $x, $y, $z, $rotx, $roty, $rotz ) = split( /\s+/ );
	$old{$index} = [ $x, $y, $z, $rotx, $roty, $rotz ];
	print "\$old\{$index\} = \[ $x, $y, $z, $rotx, $roty, $rotz \]\n";
    } # end if in bulk
} # end loop through old geometry file

open( TEMPLATE, "calc_differences.template2" );
open( KUMAC, "> calc_differences.kumac" );
while( <TEMPLATE> ) {
    $_ =~ s/TEMPLATE/$base/g;
    print KUMAC $_;
} # end create kumac
close( TEMPLATE );
close( KUMAC );

open( PAW, "$paw -l calc_differences.kumac -w 0 |" );
while( <PAW> ) {
    if ( $_ =~ /cake ring ([0-9])/ ) {
	$ring = $1;
	$n = 1;
    } # end ring line

    if ( $_ =~ /Mean\s+([0-9\.\-eE\+]+)\s+([0-9\.\-eE\+]+)/ ) {
	if ( $n == 1 ) { $which = "west rot" }
	elsif ( $n == 2 ) { $which = "east rot" }
	elsif ( $n == 3 ) { $which = "west cos" }
	elsif ( $n == 4 ) { $which = "east cos" }
	elsif ( $n == 5 ) { $which = "west sin" }
	elsif ( $n == 6 ) { $which = "east sin" }
	$n++;

	$mean{"$which $ring"} = [ $1, $2 ];

    } # end mean line
} # end loop through paw output
close( PAW );

sub sqr { @_[0] * @_[0]; }

foreach $ring ( 1..8 ) {
    $westrot = $mean{"west rot $ring"}->[0];
    $eastrot = $mean{"east rot $ring"}->[0];
    $westcos = $mean{"west cos $ring"}->[0];
    $eastcos = $mean{"east cos $ring"}->[0];
    $westsin = $mean{"west sin $ring"}->[0];
    $eastsin = $mean{"east sin $ring"}->[0];

    $westrot_err = $mean{"west rot $ring"}->[1];
    $eastrot_err = $mean{"east rot $ring"}->[1];
    $westcos_err = $mean{"west cos $ring"}->[1];
    $eastcos_err = $mean{"east cos $ring"}->[1];
    $westsin_err = $mean{"west sin $ring"}->[1];
    $eastsin_err = $mean{"east sin $ring"}->[1];

    $westrot_move = -( 1.7*$westrot - 0.7*$eastrot );
    $eastrot_move = -( 1.7*$eastrot - 0.7*$westrot );
    $westx_move = -( 1.7*$westsin - 0.7*$eastsin );
    $eastx_move = -( 1.7*$eastsin - 0.7*$westsin );
    $westy_move = -( 1.7*$westcos - 0.7*$eastcos );
    $easty_move = -( 1.7*$eastcos - 0.7*$westcos );

    $westrot_moveerr = sqrt( sqr( 1.7*$westrot_err ) + sqr( 0.7*$eastrot_err ) );
    $eastrot_moveerr = sqrt( sqr( 1.7*$eastrot_err ) + sqr( 0.7*$westrot_err ) );
    $westx_moveerr = sqrt( sqr( 1.7*$westsin_err ) + sqr( 0.7*$eastsin_err ) );
    $eastx_moveerr = sqrt( sqr( 1.7*$eastsin_err ) + sqr( 0.7*$westsin_err ) );
    $westy_moveerr = sqrt( sqr( 1.7*$westcos_err ) + sqr( 0.7*$eastcos_err ) );
    $easty_moveerr = sqrt( sqr( 1.7*$eastcos_err ) + sqr( 0.7*$westcos_err ) );

    print "\n";

    printf( "ring $ring west rot should be moved by %9.4f +/- %9.4f microns (%9.4f sigmas)\n",
	    $westrot_move*1e6, $westrot_moveerr*1e6, $westrot_move / $westrot_moveerr );
    printf( "ring $ring east rot should be moved by %9.4f +/- %9.4f microns (%9.4f sigmas)\n",
	    $eastrot_move*1e6, $eastrot_moveerr*1e6, $eastrot_move / $eastrot_moveerr );

    printf( "ring $ring west  x  should be moved by %9.4f +/- %9.4f microns (%9.4f sigmas)\n",
	    $westx_move*1e6, $westx_moveerr*1e6, $westx_move / $westx_moveerr );
    printf( "ring $ring east  x  should be moved by %9.4f +/- %9.4f microns (%9.4f sigmas)\n",
	    $eastx_move*1e6, $eastx_moveerr*1e6, $eastx_move / $eastx_moveerr );

    printf( "ring $ring west  y  should be moved by %9.4f +/- %9.4f microns (%9.4f sigmas)\n",
	    $westy_move*1e6, $westy_moveerr*1e6, $westy_move / $westy_moveerr );
    printf( "ring $ring east  y  should be moved by %9.4f +/- %9.4f microns (%9.4f sigmas)\n",
	    $easty_move*1e6, $easty_moveerr*1e6, $easty_move / $easty_moveerr );

    $westindex = 3 + ( $ring - 1 );
    $eastindex = 11 + ( $ring - 1 );

    ( $x, $y, $z, $rotx, $roty, $rotz ) = @{ $old{$westindex} };
    $x += $westx_move * 39.0625 if &moveable($ring);
    $y += $westy_move * 39.0625 if &moveable($ring);
    $z = 0 if ( $z eq "" );
    $rotx = 0 if ( $rotx eq "" );
    $roty = 0 if ( $roty eq "" );
    $rotz += $westrot_move * 39.0625 if &moveable($ring);
    $new{$westindex} = [ $x, $y, $z, $rotx, $roty, $rotz ];
    print "\$new\{$westindex\} = \[ $x, $y, $z, $rotx, $roty, $rotz \]\n";

    ( $x, $y, $z, $rotx, $roty, $rotz ) = @{ $old{$eastindex} };
    $x += $eastx_move * 39.0625 if &moveable($ring);
    $y += $easty_move * 39.0625 if &moveable($ring);
    $z = 0 if ( $z eq "" );
    $rotx = 0 if ( $rotx eq "" );
    $roty = 0 if ( $roty eq "" );
    $rotz += $eastrot_move * 39.0625 if &moveable($ring);
    $new{$eastindex} = [ $x, $y, $z, $rotx, $roty, $rotz ];
    print "\$new\{$eastindex\} = \[ $x, $y, $z, $rotx, $roty, $rotz \]\n";

} # end foreach ring

open( NEWGEOM, "> $newgeom" );
print NEWGEOM $header;
foreach $index ( 3..18 ) {
    print NEWGEOM $index." ".join( " ", @{ $new{$index} } )."\n";
}
