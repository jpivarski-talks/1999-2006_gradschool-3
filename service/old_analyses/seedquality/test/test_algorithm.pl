 #!/usr/local/bin/perl

foreach $layer ( 1..47 ) {

    $whichByte = int( ( $layer - 1 ) / 8 );
    $whichBit = ( $layer - 1 ) % 8;

    $mask = 1 << $whichBit;

    print "$layer, $whichByte, $mask\n";

}
