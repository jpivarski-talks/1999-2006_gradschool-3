#!/usr/local/bin/perl

($layer_version, $drift_version, $theta_version, $run_start, $run_stop) = @ARGV;

open(LAYER, "version$layer_version.drweightlayer");
open(DRIFT, "version$drift_version.drweightdrift");
open(THETA, "version$theta_version.drweighttheta");

$first_line = <LAYER>;
$second_line = <LAYER>;
%layeronly = ();
while ($line = <LAYER>) {
    chop $line;
    die $line if ($line !~ /^(.+)\s+(.+)$/);
    die $line if ($1 < 1  ||  $1 > 47);
    $layeronly{$1} = $2;
}

foreach $layer (1..47) {
    die if (!defined($layeronly{"$layer"}));
}

$first_line = <DRIFT>;
$second_line = <DRIFT>;
$third_line = <DRIFT>;
$fourth_line = <DRIFT>;
die if ($third_line !~ /1\s+48\s+0\s+(.+)$/);
$drift_low = $1;
die if ($fourth_line !~ /2\s+0\s+(.+)\s+(.+)$/);
$drift_nbins = $1;
$drift_high = $2;
%drift = ();
while ($line = <DRIFT>) {
    chop $line;
    die $line if ($line !~ /^(.+)\s+(.+)\s+(.+)\s+(.+)$/);
    die $line if ($2 < 1  ||  $2 > 47);
    die "$line \$3 = \"$3\" \$drift_nbins = $drift_nbins" if ($3 < 1  ||  $3 > $drift_nbins);
    die $line if (defined($drift{"$2 $3"}));
    $drift{"$2 $3"} = $4;
}

foreach $layer (1..47) {
    foreach $driftbin (1..$drift_nbins) {
	die if (!defined($drift{"$layer $driftbin"}));
    }
}

$first_line = <THETA>;
$second_line = <THETA>;
$third_line = <THETA>;
$fourth_line = <THETA>;
die if ($third_line !~ /1\s+48\s+0\s+(.+)$/);
$theta_low = $1;
die if ($fourth_line !~ /2\s+0\s+(.+)\s+(.+)$/);
$theta_nbins = $1;
$theta_high = $2;
%theta = ();
while ($line = <THETA>) {
    chop $line;
    die $line if ($line !~ /^(.+)\s+(.+)\s+(.+)\s+(.+)$/);
    die $line if ($2 < 1  ||  $2 > 47);
    die $line if ($3 < 1  ||  $3 > $theta_nbins);
    die $line if (defined($theta{"$2 $3"}));
    $theta{"$2 $3"} = $4;
}

foreach $layer (1..47) {
    foreach $thetabin (1..$theta_nbins) {
	die if (!defined($theta{"$layer $thetabin"}));
    }
}

# # normalize theta-dependent weights
# die if ($theta_nbins != 20);
# die if ($theta_low != -1);
# die if ($theta_high != 1);
# foreach $layer (1..47) {

#     # I want to normalize these such that cos(theta)==0 gives a fw correction of 1.0
#     $average = 0.;
#     foreach $thetabin (10..11) {
# 	$average += $theta{"$layer $thetabin"};
#     }
#     $average /= 2;

# #    print "Normalize theta layer $layer: $average\n";

#     foreach $thetabin (1..$theta_nbins) {
# 	$theta{"$layer $thetabin"} /= $average;
#     }
#     $layeronly{$layer} *= $average;
# }

# unnormalize drift-dependent weights
foreach $layer (1..47) {
    
    $factor = $layeronly{$layer};
#    print "Unnormalize drift layer $layer: $factor\n";
    
    foreach $driftbin (1..$drift_nbins) {
	$drift{"$layer $driftbin"} *= $factor;
    }
    $layeronly{$layer} /= $factor;
}

open(DRIFT2, "> emulate_$layer_version-$drift_version-$theta_version\_$run_start-$run_stop.drweight2layerdriftentang");

print DRIFT2 "$run_start $run_stop 0 0 0 0 0 0 DEFAULT\n";
print (DRIFT2 47 * $drift_nbins * 2 + 4, "\n");
print DRIFT2 "0 1 0 $drift_low\n";
print DRIFT2 "0 $drift_nbins 0 $drift_high\n";
print DRIFT2 "0 0 1 -1.\n";
print DRIFT2 "0 0 2 1.\n";

foreach $layer (1..47) {
    foreach $driftbin (1..$drift_nbins) {
	print DRIFT2 "$layer $driftbin 1 ".$drift{"$layer $driftbin"}."\n";
	print DRIFT2 "$layer $driftbin 2 ".$drift{"$layer $driftbin"}."\n";
    }
}

open(THETA2, "> emulate_$layer_version-$drift_version-$theta_version\_$run_start-$run_stop.drweight2layertheta");

print THETA2 "$run_start $run_stop 0 0 0 0 0 0 DEFAULT\n";
print (THETA2 47 * $theta_nbins + 2, "\n");
print THETA2 "0 1 $theta_low\n";
print THETA2 "0 $theta_nbins $theta_high\n";

foreach $layer (1..47) {
    foreach $thetabin (1..$theta_nbins) {
	print THETA2 "$layer $thetabin ".$theta{"$layer $thetabin"}."\n";
    }
}

# print "done!\n";
