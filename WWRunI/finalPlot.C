// .x finalPlot.C+(0,5,"E_{T}^{miss}","GeV","histo.root","histo_zhpresel_met",1,125,24.4, doDataMCRatio, signalZoom)

#include "TROOT.h"
#include "TSystem.h"
#include "TInterpreter.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TPad.h"
#include "Math/QuantFuncMathCore.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "StandardPlot.C"
#include <iostream> 

void 
finalPlot (int nsel             = 0, 
           int ReBin            = 1, 
           char XTitle[300]     = (char*) "N_{jets}", 
           char units[300]      = (char*) "", 
           char plotName[300]   = (char*) "histo_nice.root", 
           char outputName[300] = (char*) "njets",
           bool isLogY          = false, 
           int MassH            = 160, 
           double lumi          = 4.6, 
           bool doDataMCRatio   = true, 
           int signalZoom       = 1,
           int labelType        = 0,
           int blindSX          = 0,
           int blindDX          = 0,
           int XCanvas          = 500,
           int YCanvas          = 500,
           int breakdown        = -1
          )
{
 
 std::cout << " nsel = " << nsel << std::endl;
 
 gInterpreter->ExecuteMacro("GoodStyle.C");
//  gInterpreter->ExecuteMacro("tdrStyle.C");
 //  gROOT->LoadMacro("StandardPlot.C");
 
 std::cout << "reading " << plotName << std::endl ;
 TFile* file = new TFile(plotName, "read");
 
 //---- prepare the object that is making the plots
 //---- ---- ---- ---- ---- ---- ---- ---- ---- ----
 
 std::cout << "setting up the plot object " << std::endl ;
 
 StandardPlot myPlot;
 myPlot.setLumi(lumi);
 myPlot.setLabel(XTitle);
 myPlot.setSignalZoom (signalZoom) ;
 if(MassH != 999) myPlot.setMass (MassH); // not used for WG*
 
 if      (lumi ==    4.9) myPlot.setLumiLabel ("4.9 fb^{-1} (7 TeV)");
 else if (lumi ==   19.4) myPlot.setLumiLabel ("19.4 fb^{-1} (8 TeV)");
 else if (lumi ==   24.4) myPlot.setLumiLabel ("4.9 fb^{-1} (7 TeV) + 19.4 fb^{-1} (8 TeV)"); 
 else                     myPlot.setLumiLabel (""); 
 
 if      (labelType ==  1) myPlot.addLabel ("3l3#nu OSSF");
 else if (labelType ==  2) myPlot.addLabel ("3l3#nu SSSF");
 else if (labelType ==  3) myPlot.addLabel ("e#mu 0-jet");
 else if (labelType ==  4) myPlot.addLabel ("e#mu 1-jet");
 else if (labelType ==  5) myPlot.addLabel ("dilepton 2-jets");
 else if (labelType ==  6) myPlot.addLabel ("e#mu 0/1-jet");
 else if (labelType ==  7) myPlot.addLabel ("e#mu 2-jets, VBF tag");
 else if (labelType ==  8) myPlot.addLabel ("e#mu 2-jets, VH tag");
 else if (labelType ==  9) myPlot.addLabel ("top-tagged, e#mu 0-jet");
 else if (labelType == 10) myPlot.addLabel ("top-tagged, e#mu 1-jet");
 else if (labelType == 11) myPlot.addLabel ("W^{+}#gamma^{*} #rightarrow l^{+/-}#mu^{+/-}#mu^{-/+} selection");
 else if (labelType == 12) myPlot.addLabel ("3l#nu + 2-jets");
 else if (labelType == 13) myPlot.addLabel ("CR1, e#mu 0-jet");
 else if (labelType == 14) myPlot.addLabel ("CR2, e#mu 0-jet");
 else if (labelType == 15) myPlot.addLabel ("same-sign e#mu 0-jet");
 else if (labelType == 16) myPlot.addLabel ("top-tagged, e#mu 1-jet");
 else if (labelType == 17) myPlot.addLabel ("#splitline{top-tagged}{dilepton + 2-jets}");
 //---- H width paper
 else if (labelType == 18) myPlot.addLabel ("e#mu 0-jet");
 else if (labelType == 19) myPlot.addLabel ("e#mu 1-jet");
 else if (labelType == 20) myPlot.addLabel ("e#mu 2-jet");
 
 myPlot.setUnits(units);
 myPlot.SetColorsAndLabels () ;
 
 //---- get the bkg histograms from the file
 //---- ---- ---- ---- ---- ---- ---- ---- ---- ----
 
 std::cout << "getting histograms " << std::endl ;
 
 TH1F* hWW       = (TH1F*) file->Get ("WW");
 TH1F* hZZ       = (TH1F*) file->Get ("ZZ");
 TH1F* hFakes    = (TH1F*) file->Get ("fakes");
 TH1F* hZJets    = (TH1F*) file->Get ("DY+jets");
 TH1F* hTop      = (TH1F*) file->Get ("top");
 TH1F* hVV       = (TH1F*) file->Get ("VV"); 
 TH1F* hVVV      = (TH1F*) file->Get ("VVV"); 
 TH1F* hWZ       = (TH1F*) file->Get ("WZ"); 
 TH1F* hWJets    = (TH1F*) file->Get ("W+jets");
 TH1F* hWg       = (TH1F*) file->Get ("Wg");
 TH1F* hWgs      = (TH1F*) file->Get ("Wgs");
 TH1F* hVg       = (TH1F*) file->Get ("Vg");
 TH1F* hggWW     = (TH1F*) file->Get ("ggWW");
 TH1F* hVVandVVV = (TH1F*) file->Get ("VVandVVV"); 

 TH1F* hDYJets   = (TH1F*) file->Get ("DYll");
 TH1F* hZpluJets = (TH1F*) file->Get ("Z+jets");
 TH1F* hWjets    = (TH1F*) file->Get ("Wjets");
 
 //---- fix of names
 if (hDYJets   && !hZJets) hZJets = hDYJets;
 if (hZpluJets && !hZJets) hZJets = hZpluJets;
 if (hWjets    && !hWJets) hWJets = hWjets;
 
 
 
 double scale = 1;
 if (hWW)    hWW   ->Scale(scale);
 if (hZZ)    hZZ   ->Scale(scale);
 if (hFakes) hFakes->Scale(scale);
 if (hZJets) hZJets->Scale(scale);
 if (hTop)   hTop  ->Scale(scale);
 if (hVV)    hVV   ->Scale(scale);
 if (hVVV)   hVVV  ->Scale(scale);
 if (hWJets) hWJets->Scale(scale);
 if (hWg)    hWg   ->Scale(scale);
 if (hWgs)   hWgs  ->Scale(scale);
 if (hVg)    hVg   ->Scale(scale);
 if (hggWW)  hggWW ->Scale(scale);
 if (hVVandVVV)  hVVandVVV ->Scale(scale);
 
 //---- get the signal histograms from the file
 //---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 
 TH1F* hggH   = (TH1F*) file->Get("ggH");
 TH1F* hqqH   = (TH1F*) file->Get("qqH");
 TH1F* hVH    = (TH1F*) file->Get("VH");
 
 if (hggH) hggH->Scale (scale * signalZoom);
 if (hqqH) hqqH->Scale (scale * signalZoom);
 if (hVH)  hVH->Scale  (scale * signalZoom);


 TH1F* hggHoff = (TH1F*) file->Get("ggHoff");
 TH1F* hqqHoff = (TH1F*) file->Get("qqHoff");
 
 if (hggHoff)  hggHoff ->Scale(scale);
 if (hqqHoff)  hqqHoff ->Scale(scale);
 
 //---- assing the plots to the object making the plots,
 //---- according to the channel
 //---- ---- ---- ---- ---- ---- ---- ---- ---- ----
 
 std::cout << "assigning histos to the plotting object" << std::endl ;
 
 // nsel == 1 means HWW analysis
 if(nsel == 0 || nsel == 1) {
  std::cout << "nsel = " << nsel << ", main analysis plots" << std::endl ;
  if(hWW->GetSumOfWeights(   ) > 0) myPlot.setMCHist(iWW,      (TH1F*)hWW   ->Clone("hWW"));
  if(hZJets->GetSumOfWeights() > 0) myPlot.setMCHist(iZJets,   (TH1F*)hZJets->Clone("hZJets"));
  if(hTop->GetSumOfWeights()   > 0) myPlot.setMCHist(iTop,     (TH1F*)hTop  ->Clone("hTop"));
  if(hVV->GetSumOfWeights()    > 0) myPlot.setMCHist(iVV,      (TH1F*)hVV   ->Clone("hVV")); 
  if(hWJets->GetSumOfWeights() > 0) myPlot.setMCHist(iWJets,   (TH1F*)hWJets->Clone("hWJets"));
  //if(hWg->GetSumOfWeights()    > 0) myPlot.setMCHist(iWgamma,  (TH1F*)hWg->Clone("hWg"));
  //if(hWgs->GetSumOfWeights()   > 0) myPlot.setMCHist(iWgammaS, (TH1F*)hWgs->Clone("hWgs"));
  TH1F* hHWW     = (TH1F*) hggH->Clone ("hggH");
  if (hqqH != 0) hHWW->Add (hqqH) ;
  if (hVH != 0)  hHWW->Add (hVH) ;
  myPlot.setMCHist (iHWW, (TH1F*) hHWW->Clone ("hHWW")) ;
  myPlot.setIsHWWOverlaid(true);
  
 }
 // nsel == 2 means VH > 3 leptons
 else if (nsel == 2 || nsel == 3) {
  
  TH1F* hZGamma = (TH1F*)file->Get("histo6");
  if(hZGamma->GetSumOfWeights() > 0) myPlot.setMCHist(iZGamma, (TH1F*)hZGamma->Clone("hZGamma"));
  
  if (hWgs)    myPlot.setMCHist(iWgammaS, (TH1F*)hWgs->Clone("hWgs"));
  
  //myPlot.setMCHist(iWW,    (TH1F*)hWW   ->Clone("hWW"));
  //myPlot.setMCHist(iZJets, (TH1F*)hZJets->Clone("hZJets"));
  myPlot.setMCHist(iZZ,    (TH1F*)hTop  ->Clone("hTop"));
  myPlot.setMCHist(iWZ,    (TH1F*)hVV   ->Clone("hVV")); 
  myPlot.setMCHist(iFakes, (TH1F*)hWJets->Clone("hWJets"));
  TH1F* hHWW     = (TH1F*) hggH->Clone ("hWW");
  if (hqqH != 0) hHWW->Add (hqqH) ;
  if (hVH != 0)  hHWW->Add (hVH) ;
  if (hHWW ->GetSumOfWeights() > 0)  myPlot.setMCHist (iVH, (TH1F*) hHWW->Clone("hVH")) ;
  myPlot.setIsHWWOverlaid(true);
 }
 else if (nsel == 4) {
  myPlot.setMCHist(iZJets, (TH1F*)hWW   ->Clone("hWW"));
  myPlot.setMCHist(iVVV,   (TH1F*)hZJets->Clone("hZJets"));
  myPlot.setMCHist(iWZ,    (TH1F*)hTop  ->Clone("hTop"));
  myPlot.setMCHist(iZZ,    (TH1F*)hVV   ->Clone("hVV")); 
  myPlot.setMCHist(iEM,    (TH1F*)hWJets->Clone("hWJets"));
 }
 else if (nsel == 5) {
  std::cout << "nsel = " << nsel << ", VBF/VH analysis plots (at WW level) (Fig. 2)" << std::endl ;
  myPlot.setMCHist (iWW,    (TH1F*) hWW   ->Clone ("hWW"));    // x
  myPlot.setMCHist (iVV,    (TH1F*) hVV   ->Clone ("hVV"));    // x
  myPlot.setMCHist (iWJets, (TH1F*) hWJets->Clone ("hWJets")); // x
  myPlot.setMCHist (iZJets, (TH1F*) hZJets->Clone("hZJets"));  // x
  myPlot.setMCHist (iWgamma,(TH1F*) hVg   ->Clone ("hVg"));    // x
  myPlot.setMCHist (iTop,   (TH1F*) hTop  ->Clone("hTop"));    // x
  myPlot.setMCHist (iggH,   (TH1F*) hggH  ->Clone ("hggH"));
  myPlot.setMCHist (iVBF,   (TH1F*) hqqH  ->Clone ("hVBF"));
  myPlot.setMCHist (iVH,    (TH1F*) hVH   ->Clone ("hVH"));
  myPlot._sampleLabel[iWgamma] = " W#gamma^{(*)}";
  myPlot._sampleLabel[iVV] = " WZ+ZZ+VVV";
  //     myPlot.setIsHWWOverlaid(true);
  
  myPlot.set_Position(0,   iVV);
  myPlot.set_Position(1,   iWJets);
  myPlot.set_Position(2,   iWZ);
  myPlot.set_Position(3,   iZZ);
  myPlot.set_Position(4,   iFakes);
  myPlot.set_Position(5,   iZGamma);
  myPlot.set_Position(6,   iVVV);
  myPlot.set_Position(7,   iEM);
  myPlot.set_Position(8,   iWgamma);
  myPlot.set_Position(9,   iWgammaS);
  myPlot.set_Position(10,  iTop);
  myPlot.set_Position(11,  iZJets);
  myPlot.set_Position(12,  iWW);
  myPlot.set_Position(13,  iHWW);
  myPlot.set_Position(14,  iggH);
  myPlot.set_Position(15,  iVBF);
  myPlot.set_Position(16,  iVH);
  myPlot.set_Position(17,  nSamples);
  
 }
 else if (nsel == 6) {
  std::cout << "nsel = " << nsel << ", VBF analysis plots (at Higgs level) (Fig. 10)" << std::endl ;
  myPlot.setMCHist (iWW,    (TH1F*) hWW   ->Clone ("hWW"));
  myPlot.setMCHist (iVV,    (TH1F*) hVV   ->Clone ("hVV"));
  myPlot.setMCHist (iWJets, (TH1F*) hWJets->Clone ("hWJets"));
  myPlot.setMCHist (iZJets, (TH1F*) hZJets->Clone("hZJets"));
  myPlot.setMCHist (iWgamma,(TH1F*) hVg   ->Clone ("hVg"));
  myPlot.setMCHist (iTop,   (TH1F*) hTop  ->Clone("hTop"));
  myPlot.setMCHist (iggH,   (TH1F*) hggH  ->Clone ("hggH")); //---- ggH + VBF fused into "ggH" --> "HWW"
  myPlot._sampleLabel[iWgamma] = " W#gamma^{(*)}";
  myPlot._sampleLabel[iVV] = " WZ+ZZ+VVV";
  myPlot._sampleLabel[iggH] = " H #rightarrow WW";
  myPlot.setBreakdown(1);
  myPlot.setIsHWWOverlaid(true);
  
  myPlot.set_Position(0,   iVV);
  myPlot.set_Position(1,   iWJets);
  myPlot.set_Position(2,   iWZ);
  myPlot.set_Position(3,   iZZ);
  myPlot.set_Position(4,   iFakes);
  myPlot.set_Position(5,   iZGamma);
  myPlot.set_Position(6,   iVVV);
  myPlot.set_Position(7,   iEM);
  myPlot.set_Position(8,   iWgamma);
  myPlot.set_Position(9,   iWgammaS);
  myPlot.set_Position(10,  iTop);
  myPlot.set_Position(11,  iZJets);
  myPlot.set_Position(12,  iWW);
  myPlot.set_Position(13,  iHWW);
  myPlot.set_Position(14,  iggH);
  myPlot.set_Position(15,  iVBF);
  myPlot.set_Position(16,  iVH);
  myPlot.set_Position(17,  nSamples);
  
  myPlot.set_ErrorBand(*((TGraphAsymmErrors*) file->Get("errorBand")));
 }
 else if (nsel == 7) {
  std::cout << "nsel = " << nsel << ", VH analysis plots (at Higgs level) (Fig. 12)" << std::endl ;
  myPlot.setMCHist (iWW,    (TH1F*) hWW   ->Clone ("hWW"));
  myPlot.setMCHist (iVV,    (TH1F*) hVV   ->Clone ("hVV"));
  myPlot.setMCHist (iWgamma,(TH1F*) hVg   ->Clone ("hVg"));
  myPlot.setMCHist (iWJets, (TH1F*) hWJets->Clone ("hWJets"));
  myPlot.setMCHist (iZJets, (TH1F*) hZJets->Clone("hZJets"));
  myPlot.setMCHist (iTop,   (TH1F*) hTop  ->Clone("hTop"));
  myPlot.setMCHist (iggH,   (TH1F*) hggH  ->Clone ("hggH")); //---- ggH + VBF fused into "ggH" --> "HWW"
  //     myPlot.setMCHist (iVBF,   (TH1F*) hqqH  ->Clone ("hVBF"));
  //     myPlot.setMCHist (iVH,    (TH1F*) hVH   ->Clone ("hVH"));
  myPlot._sampleLabel[iWgamma] = " W#gamma^{(*)}";
  myPlot._sampleLabel[iVV] = " WZ+ZZ+VVV";
  myPlot._sampleLabel[iggH] = " H #rightarrow WW";
  myPlot.setBreakdown(1);
  myPlot.setIsHWWOverlaid(true);
  
  myPlot.set_Position(0,   iVV);
  myPlot.set_Position(1,   iWJets);
  myPlot.set_Position(2,   iWZ);
  myPlot.set_Position(3,   iZZ);
  myPlot.set_Position(4,   iFakes);
  myPlot.set_Position(5,   iZGamma);
  myPlot.set_Position(6,   iVVV);
  myPlot.set_Position(7,   iEM);
  myPlot.set_Position(8,   iWgamma);
  myPlot.set_Position(9,   iWgammaS);
  myPlot.set_Position(10,  iTop);
  myPlot.set_Position(11,  iZJets);
  myPlot.set_Position(12,  iWW);
  myPlot.set_Position(13,  iHWW);
  myPlot.set_Position(14,  iggH);
  myPlot.set_Position(15,  iVBF);
  myPlot.set_Position(16,  iVH);
  myPlot.set_Position(17,  nSamples);
  
  myPlot.set_ErrorBand(*((TGraphAsymmErrors*) file->Get("errorBand")));
 }
 else if (nsel == 8) {
  std::cout << "nsel = " << nsel << ", ZH analysis plots" << std::endl ;
  myPlot.setMCHist (iZZ,    (TH1F*) hZZ->Clone ("hZZ"));
  myPlot.setMCHist (iWZ,   (TH1F*) hWZ->Clone("hWZ"));
  myPlot.setMCHist (iFakes, (TH1F*) hFakes  ->Clone("hFakes"));
  myPlot.setMCHist (iVH,    (TH1F*) hVH  ->Clone ("hVH"));
  myPlot._sampleLabel[iWZ    ] = " WZ+VVV";
 }
 else if(nsel == 9) { // main analysis with input error band(error)
  std::cout << "nsel = " << nsel << ", main analysis stacked plots for data - bkg" << std::endl ;
  if(hWW->GetSumOfWeights(   ) > 0) myPlot.setMCHist(iWW,      (TH1F*)hWW   ->Clone("hWW"));
  if(hZJets->GetSumOfWeights() > 0) myPlot.setMCHist(iZJets,   (TH1F*)hZJets->Clone("hZJets"));
  if(hTop->GetSumOfWeights()   > 0) myPlot.setMCHist(iTop,     (TH1F*)hTop  ->Clone("hTop"));
  if(hVV->GetSumOfWeights()    > 0) myPlot.setMCHist(iVV,      (TH1F*)hVV   ->Clone("hVV")); 
  if(hWJets->GetSumOfWeights() > 0) myPlot.setMCHist(iWJets,   (TH1F*)hWJets->Clone("hWJets"));
  if(hWg->GetSumOfWeights()    > 0) myPlot.setMCHist(iWgamma,  (TH1F*)hWg->Clone("hWg")); 
  // --> Wg means Wgamma + Wgamma*
  if(hWgs->GetSumOfWeights()   > 0) myPlot.setMCHist(iWgammaS, (TH1F*)hWgs->Clone("hWgs"));
  TH1F* hHWW     = (TH1F*) hggH->Clone ("hggH");
  if (hqqH != 0) hHWW->Add (hqqH) ;
  if (hVH != 0)  hHWW->Add (hVH) ;
  myPlot.setMCHist (iHWW, (TH1F*) hHWW->Clone ("hHWW")) ;
  myPlot.setIsHWWOverlaid(true);
  myPlot.setBreakdown(1);
  
  myPlot.set_ErrorBand(*((TGraphAsymmErrors*) file->Get("error")));
  myPlot._sampleLabel[iWgamma] = " W#gamma^{(*)}";
  myPlot._sampleLabel[iHWW] = " H #rightarrow WW";
  
 }
 else if(nsel == 10) { // main analysis with input error band(error), no signal component
  std::cout << "nsel = " << nsel << ", main analysis stacked plots for data - bkg" << std::endl ;
  if(hWW->GetSumOfWeights(   ) > 0) myPlot.setMCHist(iWW,      (TH1F*)hWW   ->Clone("hWW"));
  if(hZJets->GetSumOfWeights() > 0) myPlot.setMCHist(iZJets,   (TH1F*)hZJets->Clone("hZJets"));
  if(hTop->GetSumOfWeights()   > 0) myPlot.setMCHist(iTop,     (TH1F*)hTop  ->Clone("hTop"));
  if(hVV->GetSumOfWeights()    > 0) myPlot.setMCHist(iVV,      (TH1F*)hVV   ->Clone("hVV")); 
  if(hWJets->GetSumOfWeights() > 0) myPlot.setMCHist(iWJets,   (TH1F*)hWJets->Clone("hWJets"));
  if(hWg->GetSumOfWeights()    > 0) myPlot.setMCHist(iWgamma,  (TH1F*)hWg->Clone("hWg")); 
  // --> Wg means Wgamma + Wgamma*
  if(hWgs->GetSumOfWeights()   > 0) myPlot.setMCHist(iWgammaS, (TH1F*)hWgs->Clone("hWgs"));
  TH1F* hHWW     = (TH1F*) hggH->Clone ("hggH");
  if (hqqH != 0) hHWW->Add (hqqH) ;
  if (hVH != 0)  hHWW->Add (hVH) ;
  myPlot.setMCHist (iHWW, (TH1F*) hHWW->Clone ("hHWW")) ;
  myPlot.setIsHWWOverlaid(true);
  myPlot.setBreakdown(2);
  
  myPlot.set_ErrorBand(*((TGraphAsymmErrors*) file->Get("error")));
  myPlot._sampleLabel[iWgamma] = " W#gamma^{(*)}";
  myPlot._sampleLabel[iHWW] = " H #rightarrow WW";
  
 }
 else if (nsel == 11) {
  std::cout << "nsel = " << nsel << ", Higgs width analysis plots" << std::endl ;
  if(hWW->GetSumOfWeights()       > 0) myPlot.setMCHist(iWW,      (TH1F*)hWW   ->Clone("hWW"));
  if(hZJets->GetSumOfWeights()    > 0) myPlot.setMCHist(iZJets,   (TH1F*)hZJets->Clone("hZJets"));
  if(hTop->GetSumOfWeights()      > 0) myPlot.setMCHist(iTop,     (TH1F*)hTop  ->Clone("hTop"));
  if(hVVandVVV->GetSumOfWeights() > 0) myPlot.setMCHist(iVV,      (TH1F*)hVVandVVV  ->Clone("hVVandVVV")); 
  if(hWJets->GetSumOfWeights()    > 0) myPlot.setMCHist(iWJets,   (TH1F*)hWJets->Clone("hWJets"));
  if(hVg->GetSumOfWeights()       > 0) myPlot.setMCHist(iWgamma,  (TH1F*)hVg    ->Clone("hVg")); 
  // --> Vg means Zgamma + Wgamma + Wgamma*
  if(hggWW->GetSumOfWeights()     > 0) myPlot.setMCHist(iggWW,    (TH1F*)hggWW  ->Clone("hggWW")); 
  
  TH1F* hHWWoff  = (TH1F*) hggHoff->Clone ("hggHoff");
  if (hqqHoff != 0) hHWWoff->Add (hqqHoff) ;
  myPlot.setMCHist (iHWW, (TH1F*) hHWWoff->Clone ("hHWWoff")) ;
  myPlot.setIsHWWOverlaid(true);
  if (breakdown == -1) myPlot.setBreakdown(2); //---> weighted S/(S+B) plots
  else                 myPlot.setBreakdown(breakdown);
  
  TH1F* hHWW     = (TH1F*) hggH->Clone ("hggH");
  if (hqqH != 0) hHWW->Add (hqqH) ;
  if (hVH != 0)  hHWW->Add (hVH) ;
  myPlot.setMCHist (iWgammaS, (TH1F*) hHWW->Clone ("hHWW")) ;
  
  myPlot.set_ErrorBand(*((TGraphAsymmErrors*) file->Get("errorBand")));
  myPlot._sampleLabel[iWgamma] = " V#gamma^{(*)}";
  myPlot._sampleLabel[iHWW] = " off-shell 30 x #Gamma_{SM}";
  myPlot._sampleLabel[iWgammaS] = " on-shell";
  myPlot._sampleLabel[iVV] = " WZ+ZZ+VVV";
  myPlot._sampleLabel[iggWW] = " ggWW";
 }
 else if (nsel == 12) {
  std::cout << "nsel = " << nsel << ", Higgs width analysis plots: control plots" << std::endl ;
  
//   if(hWW->GetSumOfWeights()       > 0) myPlot.setMCHist(iWW,      (TH1F*)hWW   ->Clone("hWW"));
  if(hWW->GetSumOfWeights()       > 0) myPlot.setMCHist(iWW,      AddNuisance(0.10,(TH1F*)hWW   ->Clone("hWW")));

//   if(hZJets->GetSumOfWeights()    > 0) myPlot.setMCHist(iZJets,   (TH1F*)hZJets->Clone("hZJets"));
  if(hZJets->GetSumOfWeights()    > 0) myPlot.setMCHist(iZJets,   AddNuisance(0.30,(TH1F*)hZJets->Clone("hZJets")));

//   if(hTop->GetSumOfWeights()      > 0) myPlot.setMCHist(iTop,     (TH1F*)hTop  ->Clone("hTop"));
  if(hTop->GetSumOfWeights()      > 0) myPlot.setMCHist(iTop,     AddNuisance(0.10, (TH1F*)hTop  ->Clone("hTop")));
  
  
// //   if(hVVandVVV->GetSumOfWeights() > 0) myPlot.setMCHist(iVV,      (TH1F*)hVV   ->Clone("hVV")); 
  if(hVVandVVV->GetSumOfWeights() > 0) myPlot.setMCHist(iVV,      (TH1F*)hVVandVVV  ->Clone("hVVandVVV")); 

//   if(hWJets->GetSumOfWeights()    > 0) myPlot.setMCHist(iWJets,   (TH1F*)hWJets->Clone("hWJets"));
  if(hWJets->GetSumOfWeights()    > 0) myPlot.setMCHist(iWJets,   AddNuisance(0.36, (TH1F*)hWJets->Clone("hWJets")));

//   if(hVg->GetSumOfWeights()       > 0) myPlot.setMCHist(iWgamma,  (TH1F*)hVg    ->Clone("hVg")); 
  if(hVg->GetSumOfWeights()       > 0) myPlot.setMCHist(iWgamma,  AddNuisance(0.50,(TH1F*)hVg    ->Clone("hVg"))); 
  // --> Vg means Zgamma + Wgamma + Wgamma* 
//   if(hggWW->GetSumOfWeights()     > 0) myPlot.setMCHist(iggWW,    (TH1F*)hggWW  ->Clone("hggWW")); 
  if(hggWW->GetSumOfWeights()     > 0) myPlot.setMCHist(iggWW,    AddNuisance(0.15,(TH1F*)hggWW  ->Clone("hggWW"))); 
  
  TH1F* hHWWoff  = (TH1F*) hggHoff->Clone ("hggHoff");
  if (hqqHoff != 0) hHWWoff->Add (hqqHoff) ;
  myPlot.setMCHist (iHWW, (TH1F*) hHWWoff->Clone ("hHWWoff")) ;
//   myPlot.setIsHWWOverlaid(true);
  myPlot.setIsHWWOverlaid(false);
  //    myPlot.setBreakdown(2);
  
  TH1F* hHWW     = (TH1F*) hggH->Clone ("hggH");
  if (hqqH != 0) hHWW->Add (hqqH) ;
  if (hVH != 0)  hHWW->Add (hVH) ;
  myPlot.setMCHist (iWgammaS, (TH1F*) hHWW->Clone ("hHWW")) ;
  
//   myPlot.set_ErrorBand(*((TGraphAsymmErrors*) file->Get("errorBand")));
  myPlot._sampleLabel[iWgamma] = " V#gamma^{(*)}";
  myPlot._sampleLabel[iHWW] = " off-shell 30 x #Gamma_{SM}";
  myPlot._sampleLabel[iWgammaS] = " on-shell";
  myPlot._sampleLabel[iVV] = " WZ+ZZ+VVV";
  myPlot._sampleLabel[iggWW] = " ggWW";
 }
 else if (nsel == 13) {
  std::cout << "nsel = " << nsel << ", Higgs width analysis plots: Top control plots" << std::endl ;
  
  
//   if(hWW->GetSumOfWeights()       > 0) myPlot.setMCHist(iWW,      AddNuisance(0.10, (TH1F*)hWW   ->Clone("hWW")));
  
  if(hWW->GetSumOfWeights()       > 0) myPlot.setMCHist(iWW,      (TH1F*)hWW   ->Clone("hWW"));
  
  
  if(hZJets->GetSumOfWeights()    > 0) myPlot.setMCHist(iZJets,   (TH1F*)hZJets->Clone("hZJets"));

    if(hTop->GetSumOfWeights()      > 0) myPlot.setMCHist(iTop,     (TH1F*)hTop  ->Clone("hTop"));
//   if(hTop->GetSumOfWeights()      > 0) myPlot.setMCHist(iTop,     AddNuisance(1.00, (TH1F*)hTop  ->Clone("hTop")));
  
  
  //   if(hVVandVVV->GetSumOfWeights() > 0) myPlot.setMCHist(iVV,      (TH1F*)hVV   ->Clone("hVV")); 
    if(hVVandVVV->GetSumOfWeights() > 0) myPlot.setMCHist(iVV,      (TH1F*)hVVandVVV  ->Clone("hVVandVVV")); 
  if(hWJets->GetSumOfWeights()    > 0) myPlot.setMCHist(iWJets,   (TH1F*)hWJets->Clone("hWJets"));
  //   if(hVg->GetSumOfWeights()       > 0) myPlot.setMCHist(iWgamma,  (TH1F*)hVg    ->Clone("hVg")); 
  // --> Vg means Zgamma + Wgamma + Wgamma* 
  //     if(hggWW->GetSumOfWeights()     > 0) myPlot.setMCHist(iggWW,    (TH1F*)hggWW  ->Clone("hggWW")); 
  
  //   TH1F* hHWWoff  = (TH1F*) hggHoff->Clone ("hggHoff");
  //   if (hqqHoff != 0) hHWWoff->Add (hqqHoff) ;
  //   myPlot.setMCHist (iHWW, (TH1F*) hHWWoff->Clone ("hHWWoff")) ;
  //   myPlot.setIsHWWOverlaid(true);
  //  myPlot.setBreakdown(2);
  
  //   TH1F* hHWW     = (TH1F*) hggH->Clone ("hggH");
  //   if (hqqH != 0) hHWW->Add (hqqH) ;
  //   if (hVH != 0)  hHWW->Add (hVH) ;
  //   myPlot.setMCHist (iWgammaS, (TH1F*) hHWW->Clone ("hHWW")) ;
  
  //   myPlot.set_ErrorBand(*((TGraphAsymmErrors*) file->Get("errorBand")));
  //   myPlot._sampleLabel[iWgamma] = " V#gamma^{(*)}";
  //   myPlot._sampleLabel[iHWW] = " H off 30#Gamma_{SM}";
  //   myPlot._sampleLabel[iWgammaS] = " H on";
  myPlot._sampleLabel[iVV] = " WZ+ZZ+VVV";
  //   myPlot._sampleLabel[iggWW] = " ggWW";
 }
 
 
 
 
 
 
 //---- get the data histogram
 //---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 
 std::cout << "getting data" << std::endl ;
 
 TH1F *hData = (TH1F*) file->Get("Data"); 
 
 if (!hData) { //---- in case "Data" is called "data"
  hData = (TH1F*)file->Get("data"); 
 }
 std::cout << "passing data to the plotting object" << std::endl ;
 myPlot.setDataHist( (TH1F*)hData->Clone("data"), blindSX, blindDX);
 
 
 std::cout << "get graph of data if available" << std::endl ;
 TGraphAsymmErrors *gr_data = (TGraphAsymmErrors*)file->Get("gr_data");  
 if (gr_data) {
  std::cout << " found gr_data " << std::endl;
  myPlot.setDataGraph( (TGraphAsymmErrors*)gr_data->Clone("gr_data"), blindSX, blindDX);  
 }
 std::cout << "printout" << std::endl ;
 
 
 
 
 
 //---- increase the Y length to get 20% size with the ratio plot
 if (doDataMCRatio) {
  YCanvas *= 1.2;
  
  gStyle->SetPadBottomMargin(0.20);
  gStyle->SetPadTopMargin   (0.08);
  gStyle->cd();
 }
 
 TCanvas* c1 = new TCanvas("c1", "c1", XCanvas, YCanvas);
 TPad* pad1;
 TPad* pad2;
 
 if (doDataMCRatio) {
  
//   pad1 = new TPad("pad1", "pad1", 0, 0.20, 1, 1);
  pad1 = new TPad("pad1","pad1", 0, 1-0.72, 1, 1);
  pad1->SetTopMargin(0.098);
  pad1->SetBottomMargin(0.000);
  pad1->Draw();

  pad2 = new TPad("pad2","pad2",0,0,1,1-0.72);
  pad2->SetTopMargin(0.000);
  pad2->SetBottomMargin(0.392);
  pad2->Draw();
  
  
  pad1->Modified();
  pad1->Update();
  pad2->Modified();
  pad2->Update();
  
  
  if(isLogY == true) pad1->SetLogy();
 }
 
 
 if (!doDataMCRatio){
  if(isLogY == true) c1->SetLogy();
 }
 
 //---- fold the underflow and overflow bin
 myPlot.FoldOverFlowUnderFlowBin();
 
 
 std::cout << "call the draw method of the plotting tool" << std::endl ;
 if (doDataMCRatio) { 
  pad1->cd();
 }
 
 myPlot.Draw(ReBin);  // Can pass a rebin 
 std::cout << "done" << std::endl ;
 c1->GetFrame()->DrawClone();
 
 
 //hggH->Rebin(ReBin);
 //hqqH->Rebin(ReBin);
 //hVH ->Rebin(ReBin);
 //hggH->Draw("same,hist");
 //hqqH->Draw("same,hist");
 //hVH ->Draw("same,hist");
 //std::cout << hggH->GetSumOfWeights() << " " << hqqH->GetSumOfWeights() << " " << hVH->GetSumOfWeights() << std::endl;
 
  
 
 if(doDataMCRatio == true){
  // Data / MC
  //--------------------------------------------------------------------------
//   TCanvas* c2 = new TCanvas("c2", "c2", 575, 10, 550, 600);
  
//   TH1F* dt = (TH1F*) hData->Clone();
  TH1F* dt = (TH1F*) (myPlot._data)->Clone();
  TH1F* mc = (TH1F*) (myPlot._bkgHist[0])->Clone();
  
  dt->SetNameTitle("dt", "dt");
  mc->SetNameTitle("mc", "mc");
  
  //---- sum up all the bkg histos saved in myPlot object
  for (int i = 1 ; i < nSamples ; ++i) { //---- 0 already added before!
   if (myPlot._bkgHist[i] != 0)
    mc->Add(myPlot._bkgHist[i]) ;    
  }
  
  dt->Rebin(ReBin);
  mc->Rebin(ReBin);
  
  
  TGraphAsymmErrors* gr_ratio_stat = new TGraphAsymmErrors();
  TGraphAsymmErrors* gr_ratio_all = new TGraphAsymmErrors();
  
  double sum[2] = {0., 0.};
  TH1F* diff = (TH1F*)dt->Clone();
  diff->SetNameTitle("", "");
  diff->Reset();
  
  for (Int_t i=0; i < dt->GetNbinsX(); i++) {
//    std::cout << " i = " << i << " :: " << dt->GetNbinsX() << std::endl;
   gr_ratio_stat->SetPoint (i,dt->GetBinCenter(i+1), 1.0);
   gr_ratio_all ->SetPoint (i,dt->GetBinCenter(i+1), 1.0);
   
   Float_t a = dt->GetBinContent(i+1);
   Float_t b = mc->GetBinContent(i+1);
   sum[0] = sum[0] + a;
   sum[1] = sum[1] + b;
   
   //---- data/mc with no error
   Float_t d  = 1.0;
//    if (a > 0 && b >0) {
   if (b >0) {
     d  = a / b;
   }    
   diff->SetBinContent(i+1, d);
   diff->SetBinError  (i+1, 0);
   //----
   
   double err_y_lo_stat;
   double err_y_hi_stat;
   double err_y_lo;
   double err_y_hi;
   
   if (myPlot._BandError != 0x0) {
//     std::cout << "     yes error bands ..." << std::endl;
    Double_t x;
    Double_t y;
    (myPlot._BandError)->GetPoint(i, x, y);
    
    err_y_lo = (myPlot._BandError)->GetErrorYlow(i);
    if (y != 0) err_y_lo /= y;
    err_y_hi = (myPlot._BandError)->GetErrorYhigh(i);
    if (y != 0) err_y_hi /= y;
 
    if (myPlot._data_correct_error_bars != 0x0) {
     err_y_lo_stat = (myPlot._data_correct_error_bars)->GetErrorYlow(i);
     if (y != 0) err_y_lo_stat /= y;
     err_y_hi_stat = (myPlot._data_correct_error_bars)->GetErrorYhigh(i);
     if (y != 0) err_y_hi_stat /= y;
    }
    else {
     err_y_lo_stat = dt->GetBinError(i+1);
     err_y_hi_stat = err_y_lo_stat;
     if (y != 0) err_y_lo_stat /= y;
     if (y != 0) err_y_hi_stat /= y;
    }
    
    err_y_hi = sqrt(err_y_hi*err_y_hi + err_y_hi_stat*err_y_hi_stat);
    err_y_lo = sqrt(err_y_lo*err_y_lo + err_y_lo_stat*err_y_lo_stat);
   }
   else {
       
    Float_t sa = dt->GetBinError(i+1);
    Float_t sb = mc->GetBinError(i+1);
    Double_t y = b;
    
    err_y_lo = sb;
    if (y != 0) err_y_lo /= y;
    err_y_hi = sb;
    if (y != 0) err_y_hi /= y;
    
    if (myPlot._data_correct_error_bars != 0x0) {
     err_y_lo_stat = (myPlot._data_correct_error_bars)->GetErrorYlow(i);
     if (y != 0) err_y_lo_stat /= y;
     err_y_hi_stat = (myPlot._data_correct_error_bars)->GetErrorYhigh(i);
     if (y != 0) err_y_hi_stat /= y;
    }
    else {
     err_y_lo_stat = sa;
     err_y_hi_stat = err_y_lo_stat;
     if (y != 0) err_y_lo_stat /= y;
     if (y != 0) err_y_hi_stat /= y;
    }
   }
   
   //---- set the uncertainties ----
   gr_ratio_stat->SetPointError (i,dt->GetBinWidth(i+1)/2., dt->GetBinWidth(i+1)/2., err_y_lo_stat, err_y_hi_stat);
   gr_ratio_all->SetPointError  (i,dt->GetBinWidth(i+1)/2., dt->GetBinWidth(i+1)/2., err_y_lo, err_y_hi);
   std::cout << " [" << i << "] = " << dt->GetBinCenter(i+1) << " :: " << d << " +/- (" <<  err_y_lo_stat << "," << err_y_lo << " :: " << err_y_hi_stat << "," << err_y_hi << std::endl;
  }
    
  
//   diff->SetMinimum(0.3);
//   diff->SetMaximum(1.7);
  diff->SetMinimum(0.0);
  diff->SetMaximum(1.8);
  
  gr_ratio_stat->SetFillColor(0);
  gr_ratio_all->SetFillColor(kGray+1);
//   gr_ratio_all->SetFillStyle(1001);
//   gr_ratio_all->SetFillStyle(1002);
  gr_ratio_all->SetFillStyle(3018);
  
  gStyle->SetOptStat(0);
  
  //TBox* tenPercentBox = new TBox(diff->GetXaxis()->GetXmin(), 0.90,
  //               diff->GetXaxis()->GetXmax(), 1.10);
  
  //tenPercentBox->SetFillColor(kAzure-9);  
  //tenPercentBox->Draw("same");
  
  AxisFonts((TAxis*)diff->GetXaxis(), "x", XTitle);
  AxisFonts((TAxis*)diff->GetYaxis(), "y", "data / MC");
  
//   TLine* oneLine = new TLine(diff->GetXaxis()->GetXmin(), 1, diff->GetXaxis()->GetXmax(), 1);  
//   oneLine->SetLineStyle(3);
//   oneLine->SetLineWidth(3);
    
  diff->SetMarkerStyle(kFullCircle);
  diff->SetLineWidth(0);
//   diff->SetFillColor(kGray+1);
  diff->SetFillStyle(0);
  
  
  
  pad2->cd()->SetGrid();
  TLine* oneLine2 = new TLine(diff->GetXaxis()->GetXmin(), 1,  diff->GetXaxis()->GetXmax(), 1);
  oneLine2->SetLineStyle(3);
  oneLine2->SetLineWidth(3);
//   Pad2TAxis(diff, XTitle, "data / MC");
  TString completeTitleX;
  if (TString(units).Sizeof () != 1) completeTitleX = Form("%s [%s]", XTitle, units);
  else  completeTitleX = Form("%s", XTitle);
  Pad2TAxis(diff, completeTitleX , "data / MC");
   
  
  diff->GetXaxis()->SetTickLength(0.10);
  diff->Draw("P");
 //   diff->Draw("AE2,same"); 
//   diff->Draw("sameaxis");
//   diff->Draw("same");
  gr_ratio_all ->Draw("2");
  diff->Draw("Psame");
  gr_ratio_stat->Draw("P");
  oneLine2->Draw("same");
  
  
  c1->cd();
  c1->Update();
  c1->Modified();
  
 
  pad1->Update();
  pad1->Modified();
  
  pad2->Update();
  pad2->Modified();
  
//   gPad->WaitPrimitive();
 }
 
 
 char CommandToExec[300];
 sprintf(CommandToExec,"mkdir -p plots");
 gSystem->Exec(CommandToExec);  
 
 char myOutputFile[300];
 sprintf(myOutputFile,"plots/%s.eps",outputName);
 c1->SaveAs(myOutputFile);
 sprintf(myOutputFile,"plots/%s.png",outputName);
 c1->SaveAs(myOutputFile);
 sprintf(myOutputFile,"plots/%s.pdf",outputName);
 c1->SaveAs(myOutputFile);
 
 
}
