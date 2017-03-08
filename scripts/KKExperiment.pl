#!/usr/bin/perl

use POSIX;

local $trials = 100;

for (my $trial=0; $trial<$trials; $trial+=1){
  $out = -log(rand())*(2**-183.129);
  push @array, 1.0*$out;
}

print "T N L\n";
$i=0;
for $m (sort { $a <=> $b } @array){
  print $i, " ", $m, " ", log($m)/log(2.0), "\n";
  $i+=1;
}

__END__

open($lfh, ">", "Data/output.log.uniform");

for ($p=160; $p<=250; $p+=2){
  my @array;
  
  $log2n = 0.1*$p;
  $precision = ceil(0.6*$log2n*$log2n);
  for (my $trial=0; $trial<$trials; $trial+=1){
    $command = sprintf "./bin/karmarkar-karp.exe %d %d %d", floor(2**$log2n), $precision, $trial, "\n";
    my $out = `$command`;
    push @array, 1.0*$out;
  }

  $filename = sprintf "Data/output.%d.uniform", $p;
  open(my $fh, ">", $filename);
  print $fh "T N L\n";
  $i=0;
  for $m (sort { $a <=> $b } @array){
    printf $fh "%d %e %f\n", $i, $m, log($m)/log(2.0);
    $i+=1;
  }
  close($fh);

  my $sum = 0.0;
  map { $sum += $_ } @array;
  printf $lfh "%4.1f %.3f\n", $log2n, log(($trials-2)/$sum)/log(2.0);

}

close($lfh);

__END__

