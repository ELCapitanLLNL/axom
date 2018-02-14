#!/bin/sh
"exec" "python" "-u" "-B" "$0" "$@"

###############################################################################
# Copyright (c) 2017-2018, Lawrence Livermore National Security, LLC.
#
# Produced at the Lawrence Livermore National Laboratory
#
# LLNL-CODE-741217
#
# All rights reserved.
#
# This file is part of Axom.
#
# For details about use and distribution, please read axom/LICENSE.
###############################################################################

"""
 file: build_src.py

 description: 
  Builds all Axom with the host-configs for the current machine.

"""

from llnl_lc_uberenv_install_tools import *

from optparse import OptionParser


def parse_args():
    "Parses args from command line"
    parser = OptionParser()
    # Location of source directory to build
    parser.add_option("-d", "--directory",
                      dest="directory",
                      default="",
                      help="Directory of source to be built (Defaults to current)")
    # Whether to archive results
    parser.add_option("-a", "--archive",
                      dest="archive",
                      default="",
                      help="Archive build results under given name (Defaults to off)")

    ###############
    # parse args
    ###############
    opts, extras = parser.parse_args()
    # we want a dict b/c the values could 
    # be passed without using optparse
    opts = vars(opts)
    return opts


def main():
    opts = parse_args()

    # Determine source directory to be built
    if os.environ.get("UBERENV_PREFIX") != None:
        src_dir = os.environ["UBERENV_PREFIX"]
        if not os.path.isdir(src_dir):
            print "[ERROR: Given environment variable 'UBERENV_PREFIX' is not a valid directory]"
            print "[    'UBERENV_PREFIX' = %s]" % src_dir
            return 1
    if opts["directory"] != "":
        src_dir = opts["directory"]
        if not os.path.isdir(src_dir):
            print "[ERROR: Given command line variable '--directory' is not a valid directory]"
            print "[    '--directory' = %s]" % src_dir
            return 1
    else:
        script_dir = os.path.dirname(os.path.realpath(__file__))
        src_dir = os.path.abspath(os.path.join(script_dir, "../.."))

    if opts["archive"] != "":
        job_name = opts["archive"]
    else:
        job_name = get_username() + "/" + os.path.basename(__file__)

    timestamp = get_timestamp()
    res = build_and_test_host_configs(src_dir, job_name, timestamp)

    # Archive logs
    if opts["archive"] != "":
        archive_src_logs(src_dir, job_name, timestamp)

    return res

if __name__ == "__main__":
    sys.exit(main())
