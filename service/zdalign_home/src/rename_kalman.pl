#!/usr/local/bin/perl

$newname = $ARGV[0];

die if (! -e "KalmanFilter"  ||  ! -e "KalmanProd");

&ren("KalmanFilter");
&ren("KalmanProd");

sub ren {
    my $file = @_[0];
    if ( -d $file ) {
	opendir(DIR, $file);
	foreach my $underling (readdir(DIR)) {
	    next if ($underling eq "."  ||  $underling eq "..");
	    &ren("$file/$underling");
	}
	closedir(DIR);
    }
    else {
	print "Edit   $file\n";
	my $contents = "";
	open(READFILE, $file);
	while (my $line = <READFILE>) {
	    $line =~ s/Kalman/$newname/g;
	    $contents .= $line;
	}
	close(READFILE);
	open(WRITEFILE, "> $file");
	print WRITEFILE $contents;
	close(WRITEFILE);
    }
    print "Rename $file\n";
    my @broken = split('/', $file);
    $broken[$#broken] =~ s/Kalman/$newname/g;
    rename($file, join('/', @broken));
}

print "done!\n";
