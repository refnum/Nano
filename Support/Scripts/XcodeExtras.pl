#!/usr/bin/perl -w
#============================================================================
#	NAME:
#		XcodeExtras.pl
#
#	DESCRIPTION:
#		Install additional support for Nano into Xcode.
#	
#	COPYRIGHT:
#		Copyright (c) 2006, refNum Software
#		<http://www.refnum.com/>
#
#		All rights reserved.
#============================================================================
#		Module header
#----------------------------------------------------------------------------
use strict;
use FindBin;





#============================================================================
#		.gdbinit content
#----------------------------------------------------------------------------
my $kNanoDelim = "# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="; 
my $kNanoGDB   = <<'END';
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Nano Macros
#
# This section is auto-generated - do not edit.
# ------------------------------------------------
define pn
    po $arg0.mObject
end
document pn
	Ask an NCFObject object (e.g., NString) to print itself.
end


define pv
    call (void) DebugPrintControl($arg0)
end
document pv
	Print debugging information about a view.
end


define pw
    call (void) DebugPrintWindow($arg0)
end
document pw
	Print debugging information about a window.
end
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
END





#============================================================================
#		getFileData : Get a file's data.
#----------------------------------------------------------------------------
sub getFileData
{


	# Retrieve our parameters
	my ($theFile) = @_;



	# Load the file
	my $theData = "";
	
	if (-e $theFile)
		{
		local($/) = undef;

		open(INPUT, $theFile) or die "Can't open $theFile for reading: $!\n";
		my $theData = <INPUT>;
		close(INPUT);
		}



	# Get the file data
	return($theData);
}





#============================================================================
#		setFileData : Set a file's data.
#----------------------------------------------------------------------------
sub setFileData
{


	# Retrieve our parameters
	my ($theFile, $theData) = @_;



	# Set the file data
	open( OUTPUT, ">$theFile") or die "Can't open $theFile for writing: $!\n";
	print OUTPUT $theData;
	close(OUTPUT);
}





#============================================================================
#		installExtras : Install the extras.
#----------------------------------------------------------------------------
sub installExtras
{


	# Get the state we need
	my $nanoRoot = $FindBin::Bin;
	   $nanoRoot =~ s/(.*)\/.*\/.*/$1/;



	# Check our state
	my $isRunning = `ps -xw | grep Xcode.app | grep -v grep`;
	chomp($isRunning);

	if ($isRunning ne "")
		{
		print "Please quit Xcode before running this script.\n";
		exit(0);
		}



	# Print some help
	print "This script installs additional support for Nano within Xcode:\n";
	print "\n";
	print "    o Xcode templates for creating new projects\n";
	print "    o Xcode source tree for Nano development\n";
	print "    o Additional gdb macros\n";
	print "\n";



	# Prompt the user
 	print "Continue installation [Y]: ";

	my $theReply = readline(STDIN);
	chomp($theReply);

	$theReply = "Y" if (!defined($theReply) || $theReply eq "");
	exit(0) if (uc($theReply) ne "Y");



	# Install the templates
	#
	# The templates must be installed as an administrator, but since we need to write to
	# Xcode's preferences for the current user we can't require the script be sudo'd.
	my $templatesNano  = "$nanoRoot/Support/Xcode/Templates";
	my $templatesXcode = "/Library/Application Support/Apple/Developer Tools/Project Templates/Application/";
	my $pathScript     = "/tmp/nano.script";

	my $theScript  = "#!/bin/sh\n";
	   $theScript .= "ln -fsv \"$templatesNano/Nano Application\"                \"$templatesXcode\"\n";
	   $theScript .= "ln -fsv \"$templatesNano/Nano Document-based Application\" \"$templatesXcode\"\n";

	setFileData($pathScript, $theScript);

	`osascript -e "do shell script \\\"sh $pathScript\\\" with administrator privileges"`;
	unlink($pathScript);



	# Install the source tree
	`defaults write com.apple.xcode PBXApplicationwideBuildSettings -dict-add Nano "$nanoRoot"`;
	`defaults write com.apple.xcode PBXSourceTreeDisplayNames       -dict-add Nano "Nano Installation"`;



	# Install the gdb macros
	my $gdbInit = $ENV{'HOME'} . "/.gdbinit";
	my $theData = getFileData($gdbInit);

	$theData =~ s/$kNanoDelim.*$kNanoDelim//s;
	$theData .= "\n$kNanoGDB";

	setFileData($gdbInit, $theData);



	# Print some status
	print "    done\n\n";
}





#============================================================================
#		Script entry point
#----------------------------------------------------------------------------
installExtras();
