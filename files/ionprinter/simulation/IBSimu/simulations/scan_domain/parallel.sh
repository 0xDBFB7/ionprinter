export PKG_CONFIG_PATH=/home/arthurdent/NAS/primary_a/Projects/ion_printer/files/ionprinter/simulation/IBSimu/simulations/magnetic_focusing_1
make clean
make || exit 1
parallel --progress --results data -j 16 ./einzel ::: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
./postprocess.sh
