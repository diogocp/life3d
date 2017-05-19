#/bin/bash

instances=( s50e5k.300 s500e300k.2000 s500e5M.10 s600e20M.5 )
procs=( 1 2 4 8 16 32 64 )

for instance in "${instances[@]}"; do
    for p in "${procs[@]}"; do
        workdir="${instance}-${p}"
        infile="${instance%.*}.in"
        generations="${instance##*.}"
       
        mkdir "$workdir"
        cd "$workdir"

        condor_mpi -n $p /mnt/cirrus/users/2/2/ist158122/life3d-mpi "/mnt/cirrus/users/2/2/ist158122/instances/$infile" $generations

        cd ..
    done
done
