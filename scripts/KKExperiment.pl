#!/usr/bin/perl

use POSIX;

local $trials = 1000;


for (my $trial=0; $trial<$trials; $trial+=1){
  $out = -log(rand())*(2**-26.850);
  push @array, 1.0*$out;
}

print "T N L\n";
$i=0;
for $m (sort { $a <=> $b } @array){
  print $i, " ", $m, " ", log($m)/log(2.0), "\n";
  $i+=1;
}

__END__

open($lfh, ">", "Data/output.tree.log.uniform");
print $lfh "N S\n";

for ($p=6; $p<=40; $p+=2){
  my @array;
  
  $precision = 200;
  for (my $trial=0; $trial<$trials; $trial+=1){
    $command = sprintf "./bin/npp-tree2.exe %d %d %d", $p, $precision, $trial, "\n";
    my $out = `$command`;
    push @array, 1.0*$out;
  }

  $filename = sprintf "Data/output.tree.%02d.uniform", $p;
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
  printf $lfh "%02d %.3f\n", $p, log(($trials-2)/$sum)/log(2.0);
}

close($lfh);

__END__


