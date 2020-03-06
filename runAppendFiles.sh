#!/bin/sh

length=40
lib='BIC'
nbFiles=25
total=1000
det='_64mmDetCol_Z200mm_Y400mm_org_'
deg="45deg"
prim="pGamma"

for radius in 5
do
    for itr in {1..40}
    do
	cat 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'.txt' >> 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$total'_1e6p'$det$deg'.txt'
	rm -rf 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'.txt'
	
	cat 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Gamma_PerfectDetData.dat' >> 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$total$det'1e9p_'$deg'_'$prim'_Gamma_PerfectDetData.dat'
	rm -rf 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Gamma_PerfectDetData.dat'
	
	cat 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Neutron_PerfectDetData.dat' >> 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$total$det'1e9p_'$deg'_'$prim'_Neutron_PerfectDetData.dat'
	rm -rf 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Neutron_PerfectDetData.dat'
	
	
	cat 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Gamma_DetectorData.dat' >> 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$total$det'1e9p_'$deg'_'$prim'_Gamma_DetectorData.dat'
	rm -rf 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Gamma_DetectorData.dat'
	
	cat 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Neutron_DetectorData.dat' >> 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$total$det'1e9p_'$deg'_'$prim'_Neutron_DetectorData.dat'
	rm -rf 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Neutron_DetectorData.dat'
	
	
	cat 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Gamma_EnergyData.dat' >> 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$total$det'1e9p_'$deg'_'$prim'_Gamma_EnergyData.dat'
	rm -rf 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Gamma_EnergyData.dat'
	
	cat 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Neutron_EnergyData.dat' >> 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$total$det'1e9p_'$deg'_'$prim'_Neutron_EnergyData.dat'
	rm -rf 'QGSP_'$lib'_HP_EMZ_240MeV_PMMA_'$length'x'$radius'cm_'$nbFiles'_1e6p'$det$itr'_'$deg'_'$prim'_Neutron_EnergyData.dat'
	

	done
done

