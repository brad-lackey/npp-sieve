#!/usr/bin/perl

use POSIX;

$distribution = "exponential";

local $trials = 1000;

while(<>){
  if ( /(\d+) (\d+.\d+)/ ){
    my @array;
    $file = "Data/output.tree.$1." . "$distribution" . "-model";
    open($fh, ">", $file);
    my $lambda = -1.0*$2;
    for (my $trial=0; $trial<$trials; $trial+=1){
      $out = -log(rand())*(2**$lambda);
      push @array, 1.0*$out;
    }
    print $fh "T N L\n";
    $i=0;
    for $m (sort { $a <=> $b } @array){
      printf $fh "%d %e %f\n", $i, $m, log($m)/log(2.0);
      $i+=1;
    }
    close($fh);
  }
}

 __END__

open($lfh, ">", "Data/output.tree.log." . "$distribution");
print $lfh "N S\n";

for ($p=6; $p<=40; $p+=2){
  my @array;
  
  $precision = 200;
  for (my $trial=0; $trial<$trials; $trial+=1){
    $command = sprintf "./bin/npp-tree.exe %d %d %d", $p, $precision, $trial, "\n";
    my $out = `$command`;
    push @array, 1.0*$out;
  }

  $filename = sprintf "Data/output.tree.%02d.%s", $p, $distribution;
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

    
