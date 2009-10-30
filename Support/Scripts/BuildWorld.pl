#!/usr/bin/perl -w
#============================================================================
#	NAME:
#		BuildWorld.pl
#
#	DESCRIPTION:
#		Global build script.
#
#		This script builds every Nano project.
#	
#	COPYRIGHT:
#		Copyright (c) 2006-2007, refNum Software
#		<http://www.refnum.com/>
#
#		All rights reserved.
#============================================================================
#		Module header
#----------------------------------------------------------------------------
use strict;
use FindBin;





#============================================================================
#		buildProject : Build a project.
#----------------------------------------------------------------------------
sub buildProject
{


	# Retrieve our parameters
	my ($theProject) = @_;
	
	
	
	# Build the project
	my $projectDir = $theProject;
	   $projectDir =~ s/(.*)\/.*xcodeproj/$1/;

	system("cd \"$projectDir\"; xcodebuild -alltargets -configuration Debug   build");
	system("cd \"$projectDir\"; xcodebuild -alltargets -configuration Release build");
}





#============================================================================
#		buildWorld : Build the world.
#----------------------------------------------------------------------------
sub buildWorld
{


	# Get the state we need
	my $nanoRoot = "$FindBin::Bin/../..";



	# Build the projects
	buildProject("$nanoRoot/Library/Project/Nano.xcodeproj");

	buildProject("$nanoRoot/Demos/NanoDemo/Project/NanoDemo.xcodeproj");
	buildProject("$nanoRoot/Demos/NanoText/Project/NanoText.xcodeproj");
	buildProject("$nanoRoot/Demos/NanoViewer/Project/NanoViewer.xcodeproj");
	buildProject("$nanoRoot/Demos/NanoWeb/Project/NanoWeb.xcodeproj");
}





#============================================================================
#		Script entry point
#----------------------------------------------------------------------------
buildWorld();
