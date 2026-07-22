#!../../bin/linux-x86_64/stMicroIOC

#- SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#-
#- SPDX-License-Identifier: EPICS

#- You may have to change stMicroIOC to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/stMicroIOC.dbd"
stMicroIOC_registerRecordDeviceDriver pdbbase

drvAsynSPIConfigure("L0", "/dev/spidev0.0", 0, 0, 1)
ihm02a1CreateController("uProbChopper", "L0", 2, 10, 1000, 0)

## Load record instances
#dbLoadRecords("db/stMicroIOC.db","user=rea")
dbLoadRecords("db/motor.db","SYS=UPROB, SUB=Chopper")

cd "${TOP}/iocBoot/${IOC}"

asynSetTraceIOMask("L0", 0, 2)
asynSetTraceMask("L0", 0, ERROR|WARNING)
asynSetTraceIOMask("uProbChopper", 0, 2)
asynSetTraceMask("uProbChopper", 0, ERROR|WARNING)

iocInit

## Start any sequence programs
#seq sncxxx,"user=rea"
