#!/usr/bin/perl

use POSIX;

$distribution = "uniform";

local $trials = 10;
local $n_min =  1800000;
local $n_max =  2100000;
local $n_step =   20000;

for ($n = $n_min; $n <= $n_max; $n += $n_step){
	my @a = ();
	print $n, "\n";
	for ($i=0; $i<$trials; $i++){
		my $x = `./bin/kk-mpfr.exe $n 300 $i`;
		chomp $x;
		push @a, $x
	}
	open($fh,">output.kk.$distribution.$n");
	for $m (sort { $a <=> $b } @a){
		printf $fh "%e %f\n", $m, log($m)/log(2);
	}
	close($fh);
}
