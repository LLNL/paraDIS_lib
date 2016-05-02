#!/usr/bin/env bash
cd $(dirname $0)
version=$(./setversion.sh -p)
farm_dir=/g/g0/rcook/current_projects/eris/lclocal/public/paradis-lib/branches/paradis-lib-${version}
export_tarball=${farm_dir}/paradis-lib-v${version}.tar

errexit() {
    echo "ERROR: $1"
    exit ${2:-1}
}

[ -d ${farm_dir} ] || errexit "$farm_dir does not exist -- please create it"

if [ -f $export_tarball ]; then
    echo "$export_tarball already exists.  Overwrite? (y/n)"
    read answer
    [ "$answer" == "y" ] ||  errexit "OK.  I'll exit then." 0
    echo "I will overwrite $export_tarball"
fi


git archive  master --prefix paradis-lib-v${version}/ | gzip > ${farm_dir}/paradis-lib-v${version}.tgz || errexit "Could not compress ${farm_dir}/paradis-lib-v${version}.tar into ${farm_dir}/paradis-lib-v${version}.tar.gz"
echo "${farm_dir}/paradis-lib-v${version}.tgz created."
