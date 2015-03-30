# 
# 
# 
# 
root -l -q finalPlot.C+\(13,1,\"MVA\",\"\",\"data-hwidth/top/1jet_TopControlRegion_HwidthMVAbkg.root\",\"mva_1jet\",0,125,19.4,0,1,0,0,0,800,600\)
#
root -l -q finalPlot.C+\(13,1,\"#Delta#phi_{ll}\",\"\",\"data-hwidth/top/1jet_TopControlRegion_dphill.root\",\"dphill_1jet_top\",0,125,19.4,0,1,0,0,0,800,600\)
root -l -q finalPlot.C+\(13,1,\"#Delta#phi_{ll}\",\"\",\"data-hwidth/top/0jet_TopControlRegion_dphill.root\",\"dphill_0jet_top\",0,125,19.4,0,1,0,0,0,800,600\)
# 
# 
# 
# 
# finalPlot (int nsel             = 0, 
#            int ReBin            = 1, 
#            char XTitle[300]     = "N_{jets}", 
#            char units[300]      = "", 
#            char plotName[300]   = "histo_nice.root", 
#            char outputName[300] = "njets",
#            bool isLogY          = false, 
#            int MassH            = 160, 
#            double lumi          = 4.6, 
#            bool doDataMCRatio   = true, 
#            int signalZoom       = 1,
#            int labelType        = 0,
#            int blindSX          = 0,
#            int blindDX          = 0,
#            int XCanvas          = 500,
#            int YCanvas          = 500
#           )
# 
# 
