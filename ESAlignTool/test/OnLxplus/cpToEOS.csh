#!/bin/tcsh
if ( $1 == "" ) then
    echo "ERROR: Please input work folder name."
    echo "Ex: ./cpToEOS.csh [name]"
    exit
endif
if ( ! ( -e $1 ) ) then
	echo "ERROR: Here is no work folder name $1 "
	exit
endif

cmsenv
set eosDir="eos/cms/store/caf/user/jtsai/ESAlignment/results/2015/CMSSW_7_4_15_patch1"
set subDirs=`\ls -l $1 | grep drwxr-xr-x | awk '{print $9}'` 
echo $subDirs
foreach subDir( $subDirs )

    set myDir="$1/$subDir"
    set toEOS="$eosDir/$1/$subDir"

    echo ">> In $myDir"
    eos mkdir -p $toEOS

    set roots=`\ls -l "$myDir/output" | grep root | awk '{print $9}'`
    set nroots=`\ls -l "$myDir/output" | grep root | awk '{print $9}' | wc -l `
    echo "   Copy $nroots roots to $toEOS..."
    foreach root($roots)
        xrdcp $myDir/output/$root xroot://eoscms.cern.ch//$toEOS/$root
        if ( $2 == 'deleteRoot' || $2 == 'deleteAll' ) then
            rm -f $myDir/output/$root
        endif
    end
    set logs=`\ls -l "$myDir/output" | grep  'job_' | grep '.log' | awk '{print $9}'`
    set nlogs=`\ls -l "$myDir/output" | grep 'job_' | grep '.log' | awk '{print $9}' | wc -l `
    echo "   Copy $nlogs logs to $toEOS..."
    foreach log($logs)
        xrdcp $myDir/output/$log xroot://eoscms.cern.ch//$toEOS/$log
        if ( $2 == 'deleteLog' || $2 == 'deleteAll' ) then
            rm -f $myDir/output/$log
        endif
    end
end
