#!/usr/local/bin/perl

%causes = ();
%dates = ();

open(UN, "unformatted3.txt");
while($line = <UN>) {
    chop($line);
    next if ($line eq "");
    die "\"$line\"" if ($line !~ /"(.*)" (.*)$/);
    $name = $1;
#    print "$name\n";
    @unform = split(" ", $2);
    @set = @{ $causes{$name} };
    foreach $s ( @unform ) {
	die if ($s !~ /^(.*),(.*)$/);
	$amount = $1;
	$date = $2;
	$dates{$date} = 1;
	if (&check($amount, $date, @set)) {
	    push(@set, [$amount, $date]);
	}
    }
    sort bydate @set;
    $causes{$name} = [ @set ];
}

open(FOR, "> formatted.txt");

# print "printing dates\n";
@dateorder = ();
@datenames = ("Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
	      "Sep", "Oct", "Nov", "Dec");
print FOR "Organization\t";
# print "Organization\n";
foreach $d (sort bydate2 keys %dates) {
    die if ($d !~ /^(.*)\/(.*)$/);
    my ($month, $year) = ($1, $2);
    print FOR $datenames[$month-1]."-$year\t";
#    print $datenames[$month-1]."-$year\n";
    push(@dateorder, $d);
}
# print "done with dates\n";
print FOR "\n";

foreach $n (sort byname keys %causes) {
    print FOR "$n\t";
#    print "$n\n";
    @set = @{ $causes{$n} };
    foreach $d (@dateorder) {
#	print "$d? ";
	$fillblank = "\t";
	foreach $s (@set) {
	    ($amount, $date) = @{ $s };
	    if (&dateeq($d, $date)) {
		die "$n on $d has both $fillblank and $amount"
		    if ($fillblank ne "\t");
#		print "$amount";
		$fillblank = "$amount\t";
	    }
	}
	print FOR $fillblank;
#	print "\n";
    }
    print FOR "\n";
}

sub check {
    my ($amount, $date, @set) = @_;
    foreach my $s ( @set ) {
	if ($date eq $s->[1]) {
	    if ($amount eq $s->[0]) {
		return 0;
	    }
	    else {
		die "$date (\$$amount) VS $s->[1] (\$$s->[0])";
	    }
	}
    }
    return 1;
}

sub bydate {
    die "bydate: \"$a->[1]\" VS \"$b->[1]\"" if ($a->[1] !~ /^(.*)\/(.*)$/);
    my ($amonth, $ayear) = ($1, $2);
    die if ($b->[1] !~ /^(.*)\/(.*)$/);
    my ($bmonth, $byear) = ($1, $2);
    $amonth = 0 if ($amonth eq "?");
    $bmonth = 0 if ($bmonth eq "?");
    $ayear += 100 if ($ayear < 50);
    $byear += 100 if ($byear < 50);
    ($ayear + $amonth/13.0) <=> ($byear + $bmonth/13.0);
}

sub bydate2 {
    die "bydate2: \"$a\" VS \"$b\"" if ($a !~ /^(.*)\/(.*)$/);
    my ($amonth, $ayear) = ($1, $2);
    die if ($b !~ /^(.*)\/(.*)$/);
    my ($bmonth, $byear) = ($1, $2);
    $amonth = 0 if ($amonth eq "?");
    $bmonth = 0 if ($bmonth eq "?");
    $ayear += 100 if ($ayear < 50);
    $byear += 100 if ($byear < 50);
    ($byear + $bmonth/13.0) <=> ($ayear + $amonth/13.0);
}

sub dateeq {
    my ($a, $b) = @_;
    die if ($a !~ /^(.*)\/(.*)$/);
    my ($amonth, $ayear) = ($1, $2);
    die if ($b !~ /^(.*)\/(.*)$/);
    my ($bmonth, $byear) = ($1, $2);
    $amonth = 0 if ($amonth eq "?");
    $bmonth = 0 if ($bmonth eq "?");
    return ($amonth == $bmonth  &&  $ayear == $byear);
}

sub byname {
    return $a cmp $b if ($a =~ /^Indiv:/  &&  $b =~ /^Indiv:/);
    return 1 if ($a =~ /^Indiv:/);
    return -1 if ($b =~ /^Indiv:/);
    return $a cmp $b;
}

