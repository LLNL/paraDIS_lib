

./stitch.sh $numtasks || ( echo stitch failed; exit 1 )

set -v; set -x

mv final.png ${result}.png

set +v; set +x
image=${dir}/${result}/${result}.png
if [ -e $image ] ; then 
    echo image is at ${image}
    display $image ; 
fi&
