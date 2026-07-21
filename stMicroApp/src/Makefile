# SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#
# SPDX-License-Identifier: EPICS

TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += stMicroMotor

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install devihm02a1.dbd into <top>/dbd
DBD += devihm02a1.dbd

INC += ihm02a1_Driver.h

# specify all source files to be compiled and added to the library
#fastech_SRCS += xxx
stMicroMotor_SRCS += ihm02a1_Driver.cpp

stMicroMotor_LIBS += motor asyn
stMicroMotor_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
