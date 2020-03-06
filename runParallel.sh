#!/bin/bash
#
#PBS -l nodes=2:ppn=20,walltime=40:00:00
#
#PBS -q guest
#
#PBS -M a.gutierrez@tudelft.nl
cd $PBS_O_WORKDIR
cp gammaSim $TMPDIR

cd $TMPDIR

chmod u+x gammaSim

radius=5
length=40
lib='BIC'
nbFiles=25
isGamma='true'
batch='batch01'

for itr in {1..40}
do
    echo '#macro file ' > gammaSim$itr$radius.mac
    echo '/detector/TargetRadius     '$radius' cm' >> gammaSim$itr$radius.mac
    echo '/detector/TargetLength     '$length' cm' >> gammaSim$itr$radius.mac
    echo '/testhadr/PrintModulo       1000000' >> gammaSim$itr$radius.mac
    echo '/run/setCut          0.01 mm' >> gammaSim$itr$radius.mac
    echo '/testhadr/Physics          QGSP_'$lib'_HP_EMZ' >> gammaSim$itr$radius.mac
    echo '/phsp/isAbsorber false' >> gammaSim$itr$radius.mac
    echo '/phsp/detAng 0 deg' >> gammaSim$itr$radius.mac
    echo '/phsp/NbFiles '$nbFiles >> gammaSim$itr$radius.mac
    echo '/phsp/NbAvailableFiles 2000' >> gammaSim$itr$radius.mac
    echo '/phsp/outputfilename QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p_'$itr >> gammaSim$itr$radius.mac
    echo '/phsp/isGamma '$isGamma >> gammaSim$itr$radius.mac
    echo '/phsp/filename /home/andreagutierre/simulations/protonSim-final-root/'$batch'/data/240MeV_QGSP_'$lib'_HP_EMZ_PMMA_40x2cm_1e6p_' >> gammaSim$itr$radius.mac

    ./gammaSim gammaSim$itr$radius.mac &  
    sleep 30
done

wait

cp *.dat $PBS_O_WORKDIR/data
cp *.txt $PBS_O_WORKDIR/data
cp *.mac $PBS_O_WORKDIR/data
