#!/usr/bin/perl
use strict;
use warnings;
use Text::Template qw(fill_in_file);
use File::Slurp qw(write_file);

die "Usage: $0 KQSomeClassname ..." unless @ARGV;

for my $classname (@ARGV) {
	my @ttParameters = (
		DELIMITERS => [ '@{', '}@' ],
		HASH => { classname  => $classname }
	);

	write_file( 'classes/' . lc $classname . '.h',
		fill_in_file('classes/template.h', @ttParameters));
	write_file( 'classes/' . lc $classname . '.cpp',
		fill_in_file('classes/template.cpp', @ttParameters));
	write_file( 'registered.h', { append => 1 },
		"REGISTER($classname)\n");
	write_file( 'classes/module.mk', { append => 1 },
		" \\\n\tclasses/" . lc $classname . '.o');
}
