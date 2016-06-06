#!/usr/bin/env python

import sys, os, shutil, re, subprocess, time, glob
from optparse import OptionParser

## bsub Error description
errorMsg = { 
"Fail open root"  : ["Fatal Root Error", "Failed to open the file"],
"EOS error"       : ["Error while doing the asyn writing", "error: target file was not created!"],
"Segmentation"    : ["Segmentation"],
"Over CPU/RAM" : ["CPU time limit exceeded", "Aborted", "Killed", "bad_alloc"],
}

## usage description
usage = """
>> [INFO] ./checkJobs.py [options]
>>        Example: ./checkJobs.py -w [workDir]
>>        Detial: . /checkJobs.py --help 
"""

## Assistant functions
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

def checkRoots( outputDir, nJobs, rootName, noDoneRootList ):
    originDir = os.getcwd()
    os.chdir(outputDir)
    roots = glob.glob( rootName+'_*.root' )
    jobn = 0
    while ( jobn < nJobs ):
        if rootName+'_'+str(jobn)+'.root' not in roots:
            noDoneRootList.append(jobn) 
        jobn+=1
    os.chdir(originDir)

def main():

    # input parameters
    parser = OptionParser(usage=usage)
    parser.add_option("-w", "--workDir",     dest="workDir",     action='store',       help="Main working directory",       metavar="myWorkDir"                            )
    parser.add_option("-d", "--dataset",     dest="dataset",     action='store',       help="Dataset in working directory", metavar="dataName",     default=''             )
    parser.add_option("-o", "--outputRoot",  dest="outRoot",     action='store',       help="Output root file name",        metavar="rootName",     default='AlignmentFile')
    parser.add_option("-q", "--queue",       dest="queue",       action='store',       help="LXBatch queue",                metavar="queueName",    default='1nh'          )
    parser.add_option("-r", "--resubmit",    dest="resubmit",    action='store',       help="re-submit percific job",       metavar="resubmitJob",  default=None           )
    parser.add_option("-R", "--resubmitAll", dest="resubmitAll", action='store_true',  help="re-submit all failed job",                             default=False          )
    (options, args) = parser.parse_args()

    # make sure all necessary input parameters are provided
    if not (options.workDir):
        print usage
        sys.exit()

    if options.resubmit and options.dataset='':
        print '>> [INFO] Please add dataset for resubmitting'
        print '>>        ./checkJob.py -w [workDir] -d [datasetName] -r [resubmitJobNum] (-q [queue])'
        sys.exit()

    # Store name of datasets
    datasetDir={}
    noDoneRoot={}
    failedInfo={}
    for fname in os.listdir(options.workDir):
        f=options.workDir+'/'+fname
        if os.path.isdir(f):
            if 'output' in os.listdir(f) and 'input' in os.listdir(f):
                noDoneRoot[fname] = []
                failedInfo[fname] = {}
                datasetDir[fname] = [ len(glob.glob(f+'/input/job_*.sh')),
                                      len(glob.glob(f+'/output/job_*.log')),
                                      len(glob.glob(f+'/output/'+options.outRoot+'_*.root'))
                                    ]
                checkRoots(f+'/output', datasetDir[fname][0], options.outRoot,  noDoneRoot[fname] )
                checkError(f+'/output', failedInfo[fname] )
    
    # Print all information
    nData=len(datasetDir)
    nDone=0

    print '>> ------------------------------------------------------- '
    print '>> [INFO] Check jot status:' 
    print '>>        Workspace : '+options.workDir
    print '>>        Root name : '+options.outRoot
    print '>> ------------------------------------------------------- '

    for name in datasetDir:
        noYet=len(noDoneRoot[name])
        sumErrJobs=[]
        failedJobs={}
        for errType in failedInfo[name]:
            failedJobs[errType]=[]
            for err in failedInfo[name][errType]:
                failedJobs[errType]=list(set(failedJobs[errType]+failedInfo[name][errType][err]))
                for fjob in failedInfo[name][errType][err]:
                    if fjob not in sumErrJobs:
                        sumErrJobs.append(int(fjob))
                        if fjob not in noDoneRoot[name]:
                            noYet+=1
                    

        nJobs = int(datasetDir[name][0])
        nLogs = int(datasetDir[name][1])
        nRoot = int(datasetDir[name][2])
        nFail = len(sumErrJobs)
        nFinish = nJobs-noYet
        if nFinish == nJobs :
            print '> [DONE] '+name
            nDone+=1
        else:
            print '> '+name

        print '> Output log  : '+str(nLogs)+'/'+str(nJobs)
        print '> Output root : '+str(nRoot)+'/'+str(nJobs)
        print '> Finshed job : '+str(nFinish)+'/'+str(nJobs)

        if nFail != 0:
            print '> Failed jots : '+str(nFail)+' '+str(sorted(sumErrJobs))
            for errType in failedJobs:
                if len(failedJobs[errType]) > 0:
                    print '  --- '+errType+" : "+str(sorted(failedJobs[errType]))

        if len(noDoneRoot[name]) != 0:
            print '> No see root : '+str(len(noDoneRoot[name]))+' '+str(sorted(noDoneRoot[name]))

        if options.resubmitAll and not options.resubmit:
            print '> ReSubmiting all failed jobs: '+str(sorted(sumErrJobs))
            originDir = os.getcwd()
            #os.chdir(options.workDir)
            for job in sumErrJobs:
                path = originDir+'/'+options.workDir+'/'+name
                if os.path.isfile(path+'/output/job_'+str(job)+'.log'):
                    cmd = 'mv '+path+'/output/job_'+str(job)+'.log '+path
                    os.system(cmd1)
                cmd = 'bsub -q '+options.queue + ' -o ' +path+'/output/job_'+str(job)+'.log'+' source '+path+'/input/job_'+str(job)+'.sh' 
                os.system(cmd)
                print '   %d resubmitted!'%(job)
            #os.chdir(originDir)
            

        print '>> ------------------------------------------------------- '
    
    print '>> [INFO] Workspace  : '+options.workDir
    print '>>        Root name  : '+options.outRoot
    print '>>        Done/Total : '+str(nDone)+'/'+str(nData)

if __name__ == "__main__":
  main()


    #cmd = 'ls -ltr '+options.workDir
    #os.system(cmd)
