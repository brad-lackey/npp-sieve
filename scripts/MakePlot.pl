#!/usr/bin/perl

$outputfile = $ARGV[0];
$modelfile = $outputfile . "-model";
$datafile = $outputfile . "-plot";

open($fh, "<", $outputfile);
local @a = ();
while( <$fh> ){
  if( /^n\s/ ){
    chomp;
    my @b = split;
    shift @b;
    push @a, $b[0];
  }
}
close($fh);
@exper = sort { $a <=> $b } @a;

@a = ();
open($fh, "<", $modelfile);
local @model = ();
while( <$fh> ){
  if( /^n\s/ ){
    chomp;
    my @b = split;
    shift @b;
    push @a, $b[0];
  }
}
close($fh);
@model = sort { $a <=> $b } @a;

open($fh, ">", $datafile);
print $fh "x e m\n";
for ($j=0; $j<scalar(@exper); $j+=1){
  printf $fh "%.3f %f %f\n", $j/1000.0, log($exper[$j])/log(2), log($model[$j])/log(2);
}
close($fh);
