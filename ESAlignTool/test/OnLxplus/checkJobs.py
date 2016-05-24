#!/usr/bin/env python

import sys, os, shutil, re, subprocess, time
from optparse import OptionParser

#def checkError( logDir, errorMsg ):
#
#    return output.splitlines()

# usage description
usage = """
>> [INFO] ./checkJobs.py [options]
>>        Example: ./checkJobs.py -w [workDir]
>>        Detial: . /checkJobs.py --help 
"""

def main():
    # input parameters
    parser = OptionParser(usage=usage)

    parser.add_option("-w", "--workDir",     dest="workDir",     action='store',       help="Main working directory",       metavar="myWorkDir"                        )
    parser.add_option("-d", "--dataset",     dest="dataset",     action='store',       help="Dataset in working directory", metavar="dataName",     default=''         )
    parser.add_option("-q", "--queue",       dest="queue",       action='store',       help="LXBatch queue",                metavar="queueName",    default='cmscaf1nd')
    parser.add_option("-r", "--resubmit",    dest="resubmit",    action='store',       help="re-submit percific job",       metavar="resubmitJob",  default=None       )
    parser.add_option("-R", "--resubmitAll", dest="resubmitAll", action='store_true',  help="re-submit all failed job",                             default=False      )

    (options, args) = parser.parse_args()

    # make sure all necessary input parameters are provided
    if not (options.workDir):
        print usage
        sys.exit()
  
    cmd = 'ls -ltr '+options.workDir
    os.system(cmd)


if __name__ == "__main__":
  main()
