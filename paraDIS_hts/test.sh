#!/bin/env bash
# exercises restartConvert and import-restart.sh pipeline for creating registry from paraDIS restart data 
set -v; set -x
version="$Id: test.sh,v 1.6 2005/11/09 17:59:57 rcook Exp $"
name=${name:-converted_restart}
logfile=${logfile:-${name}.log}
statsfile=${statsfile:-$name.stats}
reg=${reg:-${name}.reg}
template=${template:-'misc-datafiles/rs%04d.data'}
filenum=${filenum:-5300}
keep=${keep:-'-keep no'}
nth=${nth:-1}
if [ x$nth == x1 ] ; then 
    nth=
else
    nth="-drawonly $nth"
fi
#box='-xmin -1000 -xmax 1200 -ymin -2000 -ymax 000 -zmin -10000 -zmax -6000'
#template=special-%04d.data
#filenum=0
echo see log in logfile $logfile
exec >$logfile 2>&1
echo test.sh results run on `date`
rm -rf $reg *.hts

restartConvert -pov -spheres -step 1 -first ${filenum} -last ${filenum} -template ${template} -name $name -v 3 $nth $box

echo
echo NEXT TEST
echo

import-restart.sh -step 1 -first ${filenum} -last ${filenum} -inputtemplate ${template} $keep -neighbors 4 -spheres yes -monsters yes -armstats $statsfile -registry $reg -v 3 $nth -htsname $name

# totalview restartConvert -a -step 1 -first ${filenum} -last ${filenum} -template ${template}  -neighbors -4  -monsters -armstats -statsfile test.stats  -v 5 

set +x; set +v
