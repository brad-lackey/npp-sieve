#!/usr/bin/perl

use POSIX;

@n = (20,30,40,50);
$trials = 10;

$precision = 0;
$N = $trials;
for $n (@n){
  $precision += $n + int(log($n)/log(2));
  $N *= $n;
}

print $precision;
$outputfile = "./Data/numbers.0";
$command = sprintf "./bin/create_uniform_numbers.exe %d %d > $outputfile", $N, $precision;
print "$command\n";
system($command);

for ($n=0; $n<scalar(@n); $n++){
  $inputfile = $outputfile;
  $outputfile = sprintf "./Data/numbers.%d", $n+1;
  $command = "./bin/npp-experiment.exe $inputfile $n[$n] > $outputfile";
  print "$command\n";
  system($command);
}

__END__

local $trials = 1000;
local $distribution = "exponential";

#open($lfh, ">", "Data/output.log." . "$distribution");
#print $lfh "N S\n";

for ($p=40; $p<=40; $p+=10){
  my @array;
  
  $precision = 4*$p;
  $inputfile = sprintf "./Data/numbers.%02d.%s", $p, $distribution;
  $outputfile = sprintf "./Data/output.%02d.%s", $p, $distribution;
  $command = sprintf "./bin/create_%s_numbers.exe %d %d > $inputfile", $distribution, $trials*$p, $precision;
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
  printf "%02d %.3f\n", $p, log($invmean)/log(2.0);

}

#close($lfh);

__END__
