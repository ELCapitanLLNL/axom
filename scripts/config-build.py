#!/usr/bin/env python
# Python wrapper script for generating the correct cmake line with the options specified by the user.
#
# Please keep parser option names as close to possible as the names of the cmake options they are wrapping.

import sys
import os
import subprocess
import argparse
import platform
import shutil

parser = argparse.ArgumentParser(description="Configure cmake build.")

parser.add_argument("-bp",
                    "--buildpath",
                    type=str,
                    default="",
                    help="specify path for build directory.  If not specified, will create one under current directory.")

parser.add_argument("-ip",
                    "--installpath", 
                    type=str, default="",
                    help="specify path for installation directory.  If not specified, will create one under current directory.")

parser.add_argument("-c",
                    "--compiler",
                    type=str,
                    default="gnu",
                    help="compiler to use.")

parser.add_argument("-bt",
                    "--buildtype",
                    type=str,
                    choices=["Release", "Debug", "RelWithDebInfo", "MinSizeRel"],
                    default="Debug",
                    help="build type.")

parser.add_argument("-e",
                    "--eclipse",
                    action='store_true',
                    help="create an eclipse project file.")

parser.add_argument("-x",
                    "--xcode",
                    action='store_true',
                    help="create an xcode project.")

parser.add_argument("-ecc",
                    "--exportcompilercommands",
                    action='store_true',
	                help="generate a compilation database.  Can be used by the clang tools such as clang-modernize.  Will create a file called 'compile_commands.json' in your build directory.")

parser.add_argument("-co",
                    "--cmakeoption",
                    type=str,
                    help="specify additional cmake option to add to cmake line.  A '-D' will automatically be prepended.  Use caution, if you are doing something non-trivial, use ccmake or cmake-gui.")

parser.add_argument("-hc",
                    "--hostconfig",
                    default="",
                    type=str,
                    help="select a specific host-config file to initalize CMake's cache")

args = parser.parse_args()




########################
# Find CMake Cache File
########################
platform_info = ""
scriptsdir = os.path.dirname( os.path.abspath(sys.argv[0]) )

if args.hostconfig != "":
    cachefile = os.path.abspath(args.hostconfig)
    platform_info = os.path.split(cachefile)[1]
    if platform_info.endswith(".cmake"):
        platform_info = platform_info[:-6]
    print "Using user specified host config file: '%s'." % cachefile
else:
    # Check if 'SYS_TYPE' exists, and look for cache file there.
    cachefile = scriptsdir.replace("scripts","host-configs")
    if "SYS_TYPE" in os.environ:
        systype = os.environ["SYS_TYPE"]
        platform_info = systype.split("_")[0]
        cachefile = os.path.join( cachefile, platform_info, "%s.cmake" % args.compiler ) 
        print "Detected LC SYS_TYPE '%s'.  Using host config file: '%s'." % ( os.environ["SYS_TYPE"], cachefile )
    else:
        platform_info = platform.node()
        cachefile = os.path.join(cachefile, "other", "%s.cmake" % platform_info )
        print "No SYS_TYPE in environment, using hostname config file: '%s'" % ( cachefile )

print "Environment:"
print "------------"
print os.environ
print "------------"

assert os.path.exists( cachefile ), "Could not find cmake cache file '%s'." % cachefile


#####################
# Setup Build Dir
#####################
if args.buildpath != "":
    # use explicit build path
    buildpath = args.buildpath
elif args.hostconfig != "":
    # use host config name (via platform_info) as build dir base
    buildpath = "-".join(["build",platform_info,args.buildtype.lower()])
elif args.buildpath == "":
    # Generate build directory name based on platform, buildtype, compiler
    buildpath = "-".join(["build",platform_info, args.compiler, args.buildtype.lower()])

buildpath = os.path.abspath(buildpath)

if os.path.exists(buildpath):
    print "Build directory '%s' already exists.  Deleting..." % buildpath
    shutil.rmtree(buildpath)

print "Creating build directory '%s'..." % buildpath
os.makedirs(buildpath)

#####################
# Setup Install Dir
#####################
# For install directory, we will clean up old ones, but we don't need to create it, cmake will do that.
if args.installpath != "":
    installpath = os.path.abspath(args.installpath)
elif args.hostconfig != "":
    # use host config name (via platform_info) as install dir base
    installpath = "-".join(["install",platform_info,args.buildtype.lower()])
else:
    # Generate install directory name based on platform, buildtype, compiler
    installpath = "-".join(["install",platform_info, args.compiler, args.buildtype.lower()])

installpath = os.path.abspath(installpath)

if os.path.exists(installpath):
    print "Install directory '%s' already exists, deleting..." % installpath
    import shutil
    shutil.rmtree(installpath)

print "Creating install path '%s'..." % installpath
os.makedirs(installpath)

############################
# Build CMake command line
############################

cmakeline = "cmake"
# Add cache file option
cmakeline += " -C %s" % cachefile
# Add build type (opt or debug)
cmakeline += " -DCMAKE_BUILD_TYPE=" + args.buildtype
# Set install dir
cmakeline += " -DCMAKE_INSTALL_PREFIX=%s" % installpath

if args.exportcompilercommands:
    cmakeline += " -DCMAKE_EXPORT_COMPILE_COMMANDS=on"

if args.eclipse:
    cmakeline += ' -G "Eclipse CDT4 - Unix Makefiles"'

if args.xcode:
    cmakeline += ' -G "XCode"'

if args.cmakeoption:
    cmakeline += " -D" + args.cmakeoption

cmakeline += " %s/../src " % scriptsdir

############################
# Run CMake
############################
print "Changing to build directory..."
os.chdir(buildpath)
print "Executing cmake line: '%s'" % cmakeline
print 
subprocess.call(cmakeline,shell=True)

