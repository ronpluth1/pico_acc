#!/usr/bin/perl

my $infilename = '/dev/ttyACM0';
my $outfilename = 'log.txt';

while (TRUE) {
    do {
        sleep(1);
    } while (!(-e $infilename));

    open(my $ifh, '<', $infilename) or die "Cannot open $infilename: $!";
    open my $ofh, '>>', $outfilename or die "Cannot open $outfilename: $!";

    while (<$ifh>) {
        print;
        print $ofh "$_";
        $ofh->flush;
    }

    close ($ifh);
    close ($ofh);
}
