WW style plots
====

See https://github.com/govoni/HWWPlots for HIG-13-023 style.


Tool to make the plots for the CMS generic WW papers
---------------------------------------------------

Usage example:

    .x finalPlot.C+(5, 1, "#Delta#eta_{jj}", "", "test.root", "delta_eta_jj", 0, 125, 24.4, 1, 20)
    r00t finalPlot.C+\(5,1,\"#Delta#eta_{jj}\",\"\",\"test.root\",\"delta_eta_jj\",0,125,24.4,1,20\)

    r00t finalPlot.C+\(11,1,\"mll\ vs\ m_{T}^{H}\/mva\",\"\",\"/home/amassiro/Cern/Code/VBF/qqHWW/AnalysisPackage_qqHWWlnulnu/test/HWidth/Plot/final_6.root\",\"mllmthmva_1jet\",0,125,19.4,0,1,0,20,10,800,600\)
    r00t finalPlot.C+\(11,1,\"mll\ vs\ m_{T}^{H}\/mva\",\"\",\"/home/amassiro/Cern/Code/VBF/qqHWW/AnalysisPackage_qqHWWlnulnu/test/HWidth/Plot/final_4.root\",\"mllmthmva_0jet\",0,125,19.4,0,1,0,41,22,800,600\)
    r00t finalPlot.C+\(11,1,\"mll\ vs\ m_{T}^{H}\/mva\",\"\",\"/home/amassiro/Cern/Code/VBF/qqHWW/AnalysisPackage_qqHWWlnulnu/test/HWidth/Plot/final_0.root\",\"mllmth_0jet\",0,125,19.4,0,1,0,33,22,800,600\)
    r00t finalPlot.C+\(11,1,\"mll\ vs\ m_{T}^{H}\/mva\",\"\",\"/home/amassiro/Cern/Code/VBF/qqHWW/AnalysisPackage_qqHWWlnulnu/test/HWidth/Plot/final_2.root\",\"mllmth_1jet\",0,125,19.4,0,1,0,26,26,800,600\)
    r00t finalPlot.C+\(11,1,\"mll\ vs\ m_{T}^{H}\/mva\",\"\",\"/home/amassiro/Cern/Code/VBF/qqHWW/AnalysisPackage_qqHWWlnulnu/test/HWidth/Plot/final_3.root\",\"mllmth_2jet\",0,125,19.4,0,1,0,3,5,500,500\)
    
    

with the following arguments:

    - nsel : what type of plots
    - ReBin : rebinning
    - XTitle[300] : x axis title
    - units[300] : x axis units
    - plotName[300] : name of the plot
    - outputName[300] : name of the output file
    - isLogY : log y option
    - MassH : higgs mass
    - lumi : integrated lumi
    - doMCRatio : add the data/mc ratio plot 
    - singnalZoom : value by which the signal has to be multiplied


The input root file is expected to contain histograms with the following names:

    - WW       : WW bkg
    - DY+jets  : DY + jets bkg
    - top      : top and ttbar bkg
    - VV       : VV excluding WW
    - VVV      : VV (excluding WW) and VVV (read for VH H > lvjj only)
    - W+jets   : W + jets
    - Wg       : Vgamma
    - Wgs      : Vgamma*
    - Data     : data
    - ggH      : gluon fusion signal
    - qqH      : vbf signal (if missing is not used)
    - VH       : VH signal (if missing is not used)

The available analysis categories for the plotting are the following:

    - nsel == 0 : HWW main analysis
    - nsel == 1 :
    - nsel == 2 : VH 3 leptons analysis
    - nsel == 3 :
    - nsel == 4 :
    - nsel == 5 : VBF/VH @ WW level
    - nsel == 6 : VBF @ Higgs level
    - nsel == 7 : VH V > jj
    - nsel == 8 : ZH H > lvjj
    - nsel == 9 : HWW main analysis, but with input error band 
    - nsel == 10 : HWW main analysis, but with input error band, no signal thus no signal legend
    - nsel == 11 : Higgs width analysis
    

    
Produce H-width plots
---------------------------------------------------
    
    sh scripts/doHWidth_1.sh
    sh scripts/doHWidth_2.sh
    sh scripts/doHWidth_Top_control.sh
    sh scripts/doHWidth_2j.sh
    

    
    

Produce WW unfolded cross section plots
---------------------------------------------------
    
    r99t  Unfolded.C 
    