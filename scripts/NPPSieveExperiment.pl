#!/usr/bin/perl

use POSIX;

local $trials = 1000;
local $distribution = "uniform";

open($lfh, ">", "Data/output.log." . "$distribution");
print $lfh "N S\n";

for ($p=10; $p<=50; $p+=1){
  my @array;
  
  $precision = 4*$p;
  $inputfile = sprintf "./Data/numbers.%02d.%s", $p, $distribution;
  $outputfile = sprintf "./Data/output.%02d.%s", $p, $distribution;
  $command = sprintf "./bin/create_%s_numbers.exe %d %d %d > $inputfile", $distribution, $trials*$p, $precision, $p;
  system($command);

  $command = sprintf "./bin/npp-experiment.exe $inputfile %d > $outputfile", $p;
  system($command);

  my $sum = 0.0;
  open($outfh, "<", $outputfile);
  while(<$outfh>){
    if(/^n\s/){
      @a = split;
      $sum += $a[1];
    }
  }
  close($outfh);
  my $invmean = ($trials-2)/$sum;
  printf $lfh "%02d %.3f\n", $p, log($invmean)/log(2.0);

  $modelfile = sprintf "./Data/output.%02d.%s-model", $p, $distribution;
  open($modfh, ">", $modelfile);
  print $modfh "npp $trials 48\n";
  for (my $trial=0; $trial<$trials; $trial+=1){
    $out = -log(rand())/$invmean;
    printf $modfh "n %e\n", $out;
  }
  close($modfh);
}

close($lfh);

__END__

    




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


