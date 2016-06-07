#!/usr/bin/env python

import sys, os, shutil, re, subprocess, time, glob
from optparse import OptionParser

### * bsub Error description
errorMsg = { 
"Fail open root" : ["Fatal Root Error", "Failed to open the file"],
"EOS error"      : ["Error while doing the asyn writing", "error: target file was not created!"],
"Segmentation"   : ["Segmentation"],
"Over CPU/RAM"   : ["CPU time limit exceeded", "Aborted", "Killed", "bad_alloc"],
}

### * usage description
usage = """
>> [INFO] ./checkJobs.py [options]
>>        Example: ./checkJobs.py -w [workDir]
>>        Detial: . /checkJobs.py --help 
"""

### * Assistant functions
#
# Check log and store the error according to 'errorMsg'
def checkError( logsDir, failJobDetail ):
    for errType in errorMsg:
        failJobDetail[errType] = {} 
        for err in errorMsg[errType]:
            failJobDetail[errType][err] = [] 
            cmd = 'grep -r \''+err+'\' '+logsDir
            errInJobs = os.popen(cmd).read()
            if errInJobs != '':
                for errJob in errInJobs.split('\n'):
                    if errJob == '':
                        continue
                    job = re.search('\d', re.search('job_\d',errJob).group(0)).group(0)
                    if int(job) not in failJobDetail[errType][err]:
                        failJobDetail[errType][err].append(int(job))

# Check not done roots
def checkRoots( outputDir, nJobs, rootName, noDoneRootList ):
    originDir = os.getcwd()
    os.chdir(outputDir)
    roots = glob.glob( rootName+'_*.root' )
    i = 0
    while ( i < nJobs ):
        if rootName+'_'+str(i)+'.root' not in roots:
            noDoneRootList.append(i) 
        i+=1
    os.chdir(originDir)
    return len(noDoneRootList)

# Check not done logs
def checkLogs( outputDir, nJobs, noDoneLogList ):
    originDir = os.getcwd()
    os.chdir(outputDir)
    logs = glob.glob( 'job_*.log' )
    i = 0
    while ( i < nJobs ):
        if 'job_'+str(i)+'.log' not in logs:
            noDoneLogList.append(i) 
        i+=1
    os.chdir(originDir)
    return len(noDoneLogList)

### * Main working func.
def main():

    ## * input parameters
    parser = OptionParser(usage=usage)
    parser.add_option("-w", "--workDir",     dest="workDir",     action='store',       help="Main working directory",       metavar="myWorkDir"                            )
    parser.add_option("-d", "--dataset",     dest="dataset",     action='store',       help="Dataset in working directory", metavar="dataName",     default=''             )
    parser.add_option("-o", "--outputRoot",  dest="outRoot",     action='store',       help="Output root file name",        metavar="rootName",     default='AlignmentFile')
    parser.add_option("-q", "--queue",       dest="queue",       action='store',       help="LXBatch queue",                metavar="queueName",    default='1nh'          )
    parser.add_option("-r", "--resubmit",    dest="resubmit",    action='store',       help="re-submit percific job",       metavar="resubmitJob",  default=None           )
    parser.add_option("-R", "--resubmitAll", dest="resubmitAll", action='store_true',  help="re-submit all failed job",                             default=False          )
    (options, args) = parser.parse_args()

    ## * make sure all necessary input parameters are provided
    if not (options.workDir):
        print usage
        sys.exit()

    datasets=os.listdir(options.workDir)

    doOneResubmit=False
    if options.resubmit and options.dataset=='':
        print '>> [INFO] Please add dataset for resubmitting'
        print '>>        ./checkJob.py -w [workDir] -d [datasetName] -r [1 or 1,2,5...] (-q [queue])'
        sys.exit()
    elif options.resubmit and options.dataset != '':
        if options.dataset in datasets:
            doOneResubmit=True
        else:
            print '>> [INFO] Not found %s '%( options.dataset )
            print '>>        ./checkJob.py -w [workDir] -d [datasetName] -r [1 or 1,2,5...] (-q [queue])'
            sys.exit()

    ## * Enter infomation
    resubmitJobs=[]
    if doOneResubmit :
        print '>> ------------------------------------------------------- '
        print '>> [INFO] Resubmitting...' 
        print '>>        Workspace : %s '%( options.workDir )
        print '>>        Dataset   : %s '%( options.dataset )
        print '>>        Job list  : %s '%( str(options.resubmit) )
        print '>> ------------------------------------------------------- '
        for i in options.resubmit.split(','):
            if i not in resubmitJobs:
                resubmitJobs.append(int(i))
        print resubmitJobs
        sys.exit()
    else:
        print '>> ------------------------------------------------------- '
        print '>> [INFO] Checking jot status...' 
        print '>>        Workspace : %s '%( options.workDir )
        print '>>        Root name : %s '%( options.outRoot )
        print '>> ------------------------------------------------------- '

    ## * Store the status of each datasets by log
    datasetDir={}
    noDoneRoot={}
    noDoneLogs={}
    failedInfo={}
    for data in datasets:
        f=options.workDir+'/'+data
        if os.path.isdir(f):
            if 'output' in os.listdir(f) and 'input' in os.listdir(f):
                nJobs = len(glob.glob(f+'/input/job_*.sh'))
                noDoneRoot[data] = []
                noDoneLogs[data] = []
                failedInfo[data] = {}
                datasetDir[data] = [ 
                                      len(glob.glob(f+'/input/job_*.sh')),
                                      len(glob.glob(f+'/output/job_*.log')),
                                      len(glob.glob(f+'/output/'+options.outRoot+'_*.root'))
                                    ]
                checkRoots(f+'/output', datasetDir[data][0], options.outRoot,  noDoneRoot[data] )
                checkLogs( f+'/output', datasetDir[data][0], noDoneLogs[data] )
                checkError(f+'/output', failedInfo[data] )
    
    ## * Print and summerize all information
    nData=len(datasets)
    nDone=0

    # Go through each dataset
    for name in datasetDir:

        # 1. Count no done job ( including 'not found root', 'error in log' and 'not found log' )
        # 2. Simplize the info of error jobs
        noLogOrRoot=list(set(noDoneLogs[name]+noDoneRoot[name]))
        noYet=len(noLogOrRoot)
        sumErrJobs=[]
        failedJobs={}
        for errType in failedInfo[name]: # loop error type
            failedJobs[errType]=[]
            for err in failedInfo[name][errType]: # loop detail error msg
                # Simplize and store error jobs
                failedJobs[errType]=list(set(failedJobs[errType]+failedInfo[name][errType][err]))
                for fjob in failedInfo[name][errType][err]:
                    if fjob not in sumErrJobs:
                        sumErrJobs.append(int(fjob)) 
                        if fjob not in noLogOrRoot:  # Sum not done jobs
                            noYet+=1
                    
        # Print simple summary
        nJobs = int(datasetDir[name][0])
        nLogs = int(datasetDir[name][1])
        nRoot = int(datasetDir[name][2])
        nFail = len(sumErrJobs)
        nFinish = nJobs-noYet
        if nFinish == nJobs :
            print '> [DONE] %s '%( name )
            nDone+=1
        else:
            print '> %s '%( name )

        print '> N(Log)  : %d/%d '%( nLogs,   nJobs )
        print '> N(Root) : %d/%d '%( nRoot,   nJobs )
        print '> N(Done) : %d/%d '%( nFinish, nJobs )

        # Print error massage
        if nFail != 0:
            print '> N(Error): %d '%( nFail )
            print '  %-15s %s  '%( 'Sum fail jobs', str(sorted(sumErrJobs)).replace(" ", ""))
            for errType in failedJobs:
                if len(failedJobs[errType]) > 0:
                    print '  %-15s %s'%( errType,   str(sorted(failedJobs[errType])).replace(" ", ""))

        # Print not found logs 
        if len(noDoneLogs[name]) != 0:
            print '> %-15s %s'%( 'Not found logs',  str(sorted(noDoneLogs[name])).replace(" ", ""))

        # Print not found root 
        if len(noDoneRoot[name]) != 0:
            print '> %-15s %s'%( 'Not found roots', str(sorted(noDoneRoot[name])).replace(" ", ""))

        # Resubmit all 
        if options.resubmitAll and not options.resubmit:
            print '> ReSubmiting all failed jobs %s '%( str(sorted(sumErrJobs)) )
            originDir = os.getcwd()
            for job in sumErrJobs:
                path = originDir+'/'+options.workDir+'/'+name
                if os.path.isfile(path+'/output/job_'+str(job)+'.log'):
                    cmd = 'mv '+path+'/output/job_'+str(job)+'.log '+path
                    os.system(cmd1)
                cmd = 'bsub -q '+options.queue + ' -o ' +path+'/output/job_'+str(job)+'.log'+' source '+path+'/input/job_'+str(job)+'.sh' 
                os.system(cmd)
                print ' --- %d resubmitted!'%(job)
            
        print '>> ------------------------------------------------------- '
    
    # Simple summary
    print '>> [INFO] Workspace  : %s   '%( options.workDir )
    print '>>        Root name  : %s   '%( options.outRoot )
    print '>>        Done/Total : %d/%d'%( nDone, nData )

if __name__ == "__main__":
  main()

