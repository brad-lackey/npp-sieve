#!/usr/bin/perl

$outputfile = $ARGV[0];
$ideal = $ARGV[1];
$datafile = $outputfile . "-plot";

local @a = ();
local $sum = 0.0;
local $invmean;

open($fh, "<", $outputfile);
while( <$fh> ){
  if( /^n\s/ ){
    chomp;
    my @b = split;
    shift @b;
    push @a, $b[0];
    $sum += $b[0];
  }
}
close($fh);

if (scalar(@ARGV) < 2 ){
  $invmean = (scalar(@a)-2)/$sum;
} else {
  $invmean = 2**$ideal;
}

$loglambda = log($invmean)/log(2);
@exper = sort { $a <=> $b } @a;

open($fh, ">", $datafile);
print $fh "x e m\n";
for ($j=0; $j<scalar(@exper); $j+=1){
  printf $fh "%f %f %f %f\n", ($j+0.5)/scalar(@exper), log($exper[$j])/log(2), 1.0-exp(-$invmean*$exper[$j]),  log(-log(1.0 - ($j+0.5)/scalar(@exper)))/log(2) - $loglambda;
}
close($fh);
printf "%.2f\n", $loglambda;

__END__
