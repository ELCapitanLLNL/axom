#!/bin/bash

##
## Copyright (c) 2017, Lawrence Livermore National Security, LLC.
##
## Produced at the Lawrence Livermore National Laboratory.
##
## LLNL-CODE-xxxxxx
##
## All rights reserved.
##
## This file is part of Axom.
##
## For details about use and distribution, please read axom/LICENSE.
##

#MSUB -l nodes=1
#MSUB -q pbatch
#MSUB -l walltime=8:00:00
#MSUB -A wbronze
#MSUB -j oe
#MSUB -o m.out.cz.uberenv.toss3.all.compilers.%j.%N.txt
#
# usage: 
#  cd {to directory with this script}
#  msub -d `pwd` msub_llnl_cz_toss3_all_compilers.sh

date
/usr/bin/python llnl_cz_uberenv_install_toss_3_x86_64_ib_all_compilers.py
date

