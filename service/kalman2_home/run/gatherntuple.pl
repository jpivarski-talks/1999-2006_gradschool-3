#!/usr/local/bin/perl

open(LOG, "/cdat/tem/mccann/findbug3osf.log");
open(NTUPLE, "> ../rzn/findbug3osf.ntuple");

$iteration = "undef";
$track = "undef";
$c_sign = "undef";
$p_sign = "undef";
$d_sign = "undef";
$curvature = "undef";
$xdotp = "undef";
$d0 = "undef";
$zat2cm = "undef";
$z_sign = "undef";
$sig2z = "undef";

while($line = <LOG>) {
    next if ( $line !~ /NTUPLE/ );

    if ( $line =~ /Starting iteration ([0-9]+) of track ([0-9]+)/ ) {
	if ( $iteration != 0  &&  $1 == 1 ) {
	    print NTUPLE "$c_sign $p_sign $d_sign $zat2cm $sig2z $xdotp\n";
	}

	$iteration = $1;
	$track = $2;
	$c_sign = "undef";
	$p_sign = "undef";
	$d_sign = "undef";
	$curvature = "undef";
	$xdotp = "undef";
	$d0 = "undef";
	$zat2cm = "undef";
	$z_sign = "undef";
	$sig2z = "undef";
    }

    if ( $line =~ /c_sign ([01\-]+) p_sign ([01\-]+) d_sign ([01\-]+) curvature ([0-9\.\-\+eE]+) xdotp ([0-9\.\-\+eE]+) d0 ([0-9\.\-\+eE]+) zat2cm ([0-9\.\-\+eE]+) z_sign ([01\-]+)/ ) {
	$c_sign = $1;
	$p_sign = $2;
	$d_sign = $3;
	$curvature = $4;
	$xdotp = $5;
	$d0 = $6;
	$zat2cm = $7;
	$z_sign = $8;
    }

    if ( $line =~ /sigz2 ([0-9\.\-\+eE]+)/ ) {
	$sig2z = $1;
    }

}
