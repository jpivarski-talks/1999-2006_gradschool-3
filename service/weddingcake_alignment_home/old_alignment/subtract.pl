#!/usr/local/bin/perl

open( UNALIGNED, $ARGV[0] );
open( NEW, $ARGV[1] );

printf( "%10s %10s %10s %10s\n",
	"index", "delta_x", "delta_y", "rot_z" );

while ( ( $unaligned=<UNALIGNED> ) && ( $new=<NEW> ) )
{
    @unaligned = split( /\s+/, $unaligned );
    @new = split( /\s+/, $new );

    printf( "%10d %10.5f %10.5f %10.5f\n",
	    $unaligned[0],
	    25600*( $unaligned[1] - $new[1] ),
	    25600*( $unaligned[2] - $new[2] ),
	    25600*( $unaligned[6] - $new[6] ) )
	if ( $unaligned[0] >= 3  &&  !( $unaligned[1] == 0 && $unaligned[2] == 0 && $unaligned[6] == 0 ) );

    print "\n" if ( $unaligned[0] == 10 );
}
