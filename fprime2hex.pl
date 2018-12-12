#!/usr/bin/perl

use Math::Prime::Util ':all';  # import all functions
use Math::BigInt::GMP;
use bigint;

my $filename = $ARGV[0];
open my $fh, '<', $filename or die $!;

print Math::BigInt->new(<$fh>)->as_hex; # print file contents

close $fh;
