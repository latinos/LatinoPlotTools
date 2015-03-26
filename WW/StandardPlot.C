#include<vector>

//#if !defined (__CINT__) || defined (__MAKECINT__)
#include "THStack.h"
#include "TGaxis.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TFrame.h"
#include "TExec.h"
#include <iostream>
#include "TPaveText.h"
//#endif

//const Bool_t _isHWWOverlaid = false;
//enum samp { iHWW, iWW, iZJets, iTop, iVV, iWJets, iWZ, iZZ, iFakes, iZGamma, nSamples };

// const Bool_t _isHWWOverlaid = false;
// const Bool_t _isHWWOverlaid = true;

//---- NB nSamples is the actual size of the enum
// 0/1 jet PAS order:
enum samp { iWW, iggWW, iZJets, iTop, iVV, iWJets, iWZ, iZZ, iFakes, iZGamma, iVVV, iEM, iWgamma, iWgammaS, iHWW, iggH, iVBF, iVH, nSamples };

// VH and VBF PAS order:
// enum samp { iVV, iWJets, iWZ, iZZ, iFakes, iZGamma, iVVV, iEM, iWgamma, iWgammaS, iTop, iZJets, iWW, iHWW, iggH, iVBF, iVH, nSamples };


//                         data
float xPos[nSamples+1] = {0.19      ,0.19,0.19,0.19,0.19,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43,0.43}; 
float yOff[nSamples+1] = {0         ,1   ,2   ,3   ,4   ,0   ,1   ,2   ,3   ,4   ,5   ,6   ,7   ,8   ,9   ,10};
// float xPos[nSamples+1] = {0.19      ,0.19,0.19,0.19,0.19,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41}; 
// float yOff[nSamples+1] = {0         ,1   ,2   ,3   ,4   ,0   ,1   ,2   ,3   ,4   ,5   ,6   ,7   ,8   ,9   ,10};

const Float_t _tsize   = 0.033;
const Float_t _xoffset = 0.20;
const Float_t _yoffset = 0.05;


//------------------------------------------------------------------------------
// AddNuisance
//------------------------------------------------------------------------------
//----              in %, e.g. 0.10 -> 10%
TH1F* AddNuisance (float uncertainty, TH1F* h) {
//  std::cout << " AddNuisance: " << h->GetTitle() << std::endl; 
 for (Int_t i=0; i<=h->GetNbinsX (); i++) { 
  Float_t binHeight = h->GetBinContent (i);
  Float_t binTempError = h->GetBinError (i);
  h->SetBinError(i, sqrt(binTempError*binTempError + binHeight*binHeight*uncertainty*uncertainty));
//   std::cout << " Error = " << sqrt(binTempError*binTempError + binHeight*binHeight*uncertainty*uncertainty) << std::endl;
 }
 return h;
}



//------------------------------------------------------------------------------
// GetMaximumIncludingErrors
//------------------------------------------------------------------------------
Float_t GetMaximumIncludingErrors (TH1F* h) {
 Float_t maxWithErrors = 0;
 
 for (Int_t i=1; i<=h->GetNbinsX (); i++) {
  Float_t binHeight = h->GetBinContent (i) + h->GetBinError (i);
  if (binHeight > maxWithErrors) maxWithErrors = binHeight;
 }
 return maxWithErrors;
}


//------------------------------------------------------------------------------
// AxisFonts
//------------------------------------------------------------------------------
void AxisFonts (TAxis*  axis,  TString coordinate, TString title) {
 axis->SetLabelFont (   42);
 axis->SetLabelOffset (0.015);
 axis->SetLabelSize (0.050);
 axis->SetNdivisions (  505);
 axis->SetTitleFont (   42);
 axis->SetTitleOffset (  1.3); // 1.5
 axis->SetTitleSize (0.050);
 
 if (coordinate == "y") {
  axis->SetTitleOffset (  1.3); //  1.8
  TString titolo = axis->GetTitle () ;
  titolo.ReplaceAll ("Entries", "Events") ;
  axis->SetTitle (titolo) ;
 }
 
 axis->SetTitle (title);
}

void Pad2TAxis(TH1* hist, TString xtitle, TString ytitle) {
 TAxis* xaxis = (TAxis*)hist->GetXaxis();
 TAxis* yaxis = (TAxis*)hist->GetYaxis();
 xaxis->SetLabelFont ( 42);
 xaxis->SetLabelOffset( 0.025);
 xaxis->SetLabelSize ( 0.1);
 xaxis->SetNdivisions ( 505);
 xaxis->SetTitle (xtitle);
 xaxis->SetTitleFont ( 42);
 xaxis->SetTitleOffset( 1.35);
 xaxis->SetTitleSize ( 0.11);
 yaxis->CenterTitle ( );
 yaxis->SetLabelFont ( 42);
 yaxis->SetLabelOffset( 0.02);
 yaxis->SetLabelSize ( 0.1);
 yaxis->SetNdivisions ( 505);
 yaxis->SetTitle (ytitle);
 yaxis->SetTitleFont ( 42);
 yaxis->SetTitleOffset( .6);
 yaxis->SetTitleSize ( 0.11);
}

//------------------------------------------------------------------------------
// THStackAxisFonts
//------------------------------------------------------------------------------
void THStackAxisFonts (THStack* h,  TString  coordinate,  TString  title) {
 TAxis* axis = NULL;
 
 if (coordinate.Contains ("x")) axis = h->GetHistogram ()->GetXaxis ();
 if (coordinate.Contains ("y")) axis = h->GetHistogram ()->GetYaxis ();
 AxisFonts (axis, coordinate, title);
}


//------------------------------------------------------------------------------
// DrawLegend
//------------------------------------------------------------------------------
void DrawLegend (Float_t x1,  Float_t y1,  TH1F*   hist,  TString label,  TString option) {
 TLegend* legend = new TLegend (x1,     y1,       x1 + _xoffset,             y1 + _yoffset);
 legend->SetBorderSize (0) ;
 legend->SetFillColor (0) ;
 legend->SetTextAlign (12) ;
 legend->SetTextFont (42) ;
 legend->SetTextSize (_tsize) ;
 
 legend->AddEntry (hist, label.Data (), option.Data ());
 
 legend->Draw ();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


class StandardPlot {
 
 public: 
  StandardPlot () { 
   _hist.resize (nSamples, 0); 
   _bkgHist.resize (nSamples, 0); 
   _sigHist.resize (nSamples, 0); 
   _data = 0; 
   _data_correct_error_bars = 0x0;
   _breakdown = 0; 
   _mass = 0; 
   _signalZoom = 1; 
   _isHWWOverlaid = false;
   
   for (int itemp = 0 ; itemp < nSamples ; itemp++) {
    _position.push_back(itemp);
   }
   
   _BandError = 0x0;
  }
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  
  void setMCHist  (const samp & s, TH1F * h) { 
   _hist[s] = h ; 
   std::cout << "READING SAMPLE " << s << " :: " << _sampleLabel[s] << std::endl ; 
   std::cout << "                  int = " << _hist[s]->Integral() << std::endl ; 
   if (s != iHWW && s != iggH && s != iVBF && s != iVH) {
    std::cout << "DEBUG reading background\n" ;
    _bkgHist[s] = h ;
    std::cout << " > DEBUG reading background (end)\n" ;
   }
   else {
    std::cout << "DEBUG reading signal\n" ;
    _sigHist[s] = h ;
   }
  } 
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  
  void SetColorsAndLabels () {
   _sampleColor = new Color_t [nSamples] ;
   _sampleColor[iHWW    ] = kRed + 1 ;
   _sampleColor[iggWW   ] = kAzure - 5 ;
   _sampleColor[iWW     ] = kAzure - 9 ;
   _sampleColor[iEM     ] = kYellow ;
   _sampleColor[iZJets  ] = kGreen + 2 ;
   _sampleColor[iTop    ] = kYellow ;
   _sampleColor[iVV     ] = kAzure - 2 ;
   _sampleColor[iWJets  ] = kGray + 1 ;
   _sampleColor[iWZ     ] = kAzure-2 ;
   _sampleColor[iZZ     ] = kAzure-9 ;
   _sampleColor[iVVV    ] = kGray + 1 ;
   _sampleColor[iFakes  ] = kGray + 1 ;
   _sampleColor[iZGamma ] = kViolet - 9 ;
   _sampleColor[iggH    ] = kRed + 1 ;
   _sampleColor[iVBF    ] = kBlue + 1 ;
   _sampleColor[iVH     ] = 635 ; // kRed + 3?
   _sampleColor[iWgamma ] = 616+1; // kViolet + 1 ;
   _sampleColor[iWgammaS] = 616+1; // kViolet + 1  ;
   
   _sampleLabel = new TString [nSamples] ;
   TString higgsLabel ;
   if (_signalZoom > 1) {
    higgsLabel.Form (" %d x H #rightarrow WW",_signalZoom);
    _sampleLabel[iHWW    ] = higgsLabel ;
    higgsLabel.Form (" %d x VBF",_signalZoom);
    _sampleLabel[iVBF    ] = higgsLabel ;
    higgsLabel.Form (" %d x ggH",_signalZoom);
    _sampleLabel[iggH    ] = higgsLabel ;
    if(_signalZoom != 10){ // ugly! 
     higgsLabel.Form (" %d x VH",_signalZoom);
     _sampleLabel[iVH     ] = higgsLabel ;
    } else {
     higgsLabel.Form (" %d x ZH",_signalZoom);
     _sampleLabel[iVH     ] = higgsLabel ;
    }
   }
   else {
    higgsLabel.Form (" H #rightarrow WW");
    _sampleLabel[iHWW  ] = higgsLabel ;
    higgsLabel.Form (" VBF");
    _sampleLabel[iVBF  ] = higgsLabel ;
    higgsLabel.Form (" ggH");
    _sampleLabel[iggH  ] = higgsLabel ;
    higgsLabel.Form (" VH");
    _sampleLabel[iVH   ] = higgsLabel ;
   }
   
   
   _sampleLabel[iggWW   ] = " ggWW"         ;
   _sampleLabel[iWW     ] = " WW"           ;
   _sampleLabel[iZJets  ] = " DY+jets"      ;
   _sampleLabel[iTop    ] = " top"          ;
   _sampleLabel[iVV     ] = " WZ+ZZ+VVV"    ;
   _sampleLabel[iWJets  ] = " W+jets"       ;
   _sampleLabel[iWZ     ] = " WZ"           ;
   _sampleLabel[iZZ     ] = " ZZ"           ;
   _sampleLabel[iVVV    ] = " VVV"          ;
   _sampleLabel[iEM     ] = " WW+top+W+jets";
   _sampleLabel[iZGamma ] = " Z#gamma"      ;
   _sampleLabel[iFakes  ] = " non-prompt"        ;
   _sampleLabel[iWgammaS] = " W#gamma^{(*)}";
   
   return ;
   
  }
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  
  void setDataHist (TH1F * h, int blindSX = 0, int blindDX = 0)  {
   _data = (TH1F*) h->Clone();  
   for (int i=0; i<blindSX; i++) {
    _data->SetBinContent(i+1, 0);
    _data->SetBinError(i+1, 0);
   }
   int nbins = _data->GetNbinsX();
   for (int i=0; i<blindDX; i++) {
    _data->SetBinContent(nbins-i, 0);
    _data->SetBinError(nbins-i, 0);
   }
  }
  
  
  
  void setDataGraph (TGraphAsymmErrors * gr, int blindSX = 0, int blindDX = 0)  {
   _data_correct_error_bars = (TGraphAsymmErrors*) gr->Clone();  
   for (int i=0; i<blindSX; i++) {
    double x, y;
    _data_correct_error_bars->GetPoint(i, x, y);
    double exl = _data_correct_error_bars->GetErrorXlow(i);
    double exh = _data_correct_error_bars->GetErrorXhigh(i);
    _data_correct_error_bars->SetPoint (i, x ,0);
    _data_correct_error_bars->SetPointError(i, exl, exh, 0, 0);
   }
   int nbins = _data_correct_error_bars->GetN();
   for (int i=0; i<blindDX; i++) {
    double x, y;
    _data_correct_error_bars->GetPoint(nbins-i-1, x, y);
    double exl = _data_correct_error_bars->GetErrorXlow(nbins-i-1);
    double exh = _data_correct_error_bars->GetErrorXhigh(nbins-i-1);
    _data_correct_error_bars->SetPoint (nbins-i-1, x ,0);
    _data_correct_error_bars->SetPointError(nbins-i-1, exl, exh, 0, 0);
   }
  }
  
  
  
  void setHWWHist  (TH1F * h)                 { setMCHist (iHWW  ,h); } 
  void setWWHist   (TH1F * h)                 { setMCHist (iWW   ,h); } 
  void setggWWHist (TH1F * h)                 { setMCHist (iggWW ,h); } 
  void setZJetsHist (TH1F * h)                { setMCHist (iZJets,h); } 
  void setTopHist  (TH1F * h)                 { setMCHist (iTop  ,h); } 
  void setVVHist   (TH1F * h)                 { setMCHist (iVV   ,h); } 
  void setWZHist   (TH1F * h)                 { setMCHist (iWZ   ,h); } 
  void setZZHist   (TH1F * h)                 { setMCHist (iZZ   ,h); } 
  void setFakesHist (TH1F * h)                { setMCHist (iFakes,h); } 
  void setWJetsHist (TH1F * h)                { setMCHist (iWJets,h); }
  void setZGammaHist (TH1F * h)               { setMCHist (iZGamma,h);}
  void setVVVHist  (TH1F * h)                 { setMCHist (iVVV  ,h); } 
  void setEMHist   (TH1F * h)                 { setMCHist (iEM   ,h); } 
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  
  TH1F* getDataHist () { return _data; }
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  
  void setMass (const int &m) { 
   _mass = m ; 
   TString dummy = "m#lower[0.3]{_{H}} = " ;
   dummy += _mass ;
   dummy += " GeV";
   _extraLabels.push_back (dummy) ;
  }
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  
  void setSignalZoom (const int zoom) { _signalZoom = zoom ; }
  
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // ---- fold underflow and overflow bin into bin in visible range ----
  
  void FoldOverFlowBin (TH1* histo) {
   UInt_t nx = histo->GetNbinsX();
   double cont = histo->GetBinContent(nx) + histo->GetBinContent(nx+1) ;
   double err = sqrt( histo->GetBinError(nx)*histo->GetBinError(nx) + histo->GetBinError(nx+1)*histo->GetBinError(nx+1) ) ;
   histo->SetBinContent(nx,cont);
   histo->SetBinError(nx,err);
   histo->SetBinContent(nx+1,0.);
   histo->SetBinError(nx+1,0.);
  }
  
  void FoldUnderFlowBin (TH1* histo) {
   double cont = histo->GetBinContent(0) + histo->GetBinContent(1) ;
   double err = sqrt( histo->GetBinError(0)*histo->GetBinError(0) + histo->GetBinError(1)*histo->GetBinError(1) ) ;
   histo->SetBinContent(1,cont);
   histo->SetBinError(1,err);
   histo->SetBinContent(0,0.);
   histo->SetBinError(0,0.);
  }
  
  void FoldOverFlowUnderFlowBin (TH1* histo) {
   std::cout << " Folding :: " << histo->GetTitle() << std::endl;
   FoldOverFlowBin(histo);
   FoldUnderFlowBin(histo);
  }
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  
  void FoldOverFlowUnderFlowBin () {
   for (int itemp = 0 ; itemp < nSamples ; itemp++) {
    int i = _position.at(itemp);
    if (_bkgHist[i] == 0) {
     if (_sigHist[i] == 0) continue ;
    }  
    
    if (_hist[i] != 0) {
     FoldOverFlowUnderFlowBin(_hist[i]);
    }
   }
   
   if (_data) FoldOverFlowUnderFlowBin(_data);
   
  }
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  
  TH1* DrawAndRebinTo (const int &rebinTo) {
   if (rebinTo == 0) return Draw ();
   int rebin = 0, nbins = 0;
   for (int i=0; i<nSamples; i++) {
    
    // in case the user doesn't set it
    if ( !_hist[i] ) continue;
    
    nbins = _hist[i]->GetNbinsX ();
   }
   if (nbins == 0) return Draw ();
   
   rebin = nbins / rebinTo;
   while (nbins % rebin != 0) rebin--;
   return Draw (rebin);
  }
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  
  TH1* Draw (const int &rebin=1) {
   std::cout << " Draw " << std::endl;
   
   //setUpStyle ();
   //if (!gPad) new TCanvas ();
   
   //---- prepare the THStack
   //---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
   
   
   THStack* hstack = new THStack ();
   TH1D* hSum = (TH1D*)_data->Clone ();
   hSum->Rebin (rebin);
   hSum->Scale (0.0);
   
   //---- fill the THStack
   for (int itemp = 0 ; itemp < nSamples ; itemp++) {
    
    int i = _position.at(itemp);
    
    if (_bkgHist[i] == 0) {
     if (!_isHWWOverlaid ) continue ;
     else if (_sigHist[i] == 0) continue ;
    }  
    
    if (_hist[i] == 0) {
     std::cerr << "error: something is messed up in reading samples, exiting" << std::endl ;
     exit (1) ;
    }
    
    bool modifyXAxis = false;
    if (modifyXAxis == true) {
     TAxis *xa =_hist[i]->GetXaxis ();
     for (Int_t k=1;k<=_hist[i]->GetNbinsX ();++k) {
      xa->SetBinLabel (1 ,"2#mu+#mu");
      xa->SetBinLabel (2 ,"2#mu+e");
      xa->SetBinLabel (3 ,"2e+#mu");
      xa->SetBinLabel (4 ,"2e+e");
      xa->SetRangeUser (1,4);
     }
    }
    
    _hist[i]->Rebin (rebin);
    _hist[i]->SetLineColor (_sampleColor[i]);
    
    _hist[i]->SetFillColor (_sampleColor[i]);
    _hist[i]->SetFillStyle (1001);
    
    //---- the signal
    if (i==13 || i==14 || i==15 || i==16) {
     _hist[i]->SetLineWidth (3);
     _hist[i]->SetFillStyle (0);
    }
    
    TH1F* temp_hist = (TH1F*) _hist[i]->Clone();
    hstack->Add (temp_hist);
    hSum->Add (temp_hist);
    //           hstack->Add (_hist[i]);
    //           hSum->Add (_hist[i]);
   } //---- fill the THStack
   
   //---- setup signal samples
   for (int i=0; i<nSamples; i++) {
    if (_sigHist[i] == 0 ) continue ;
    _hist[i]->SetLineWidth (3) ;
    _hist[i]->SetLineColor (_sampleColor[i]) ;
    _hist[i]->SetFillStyle (0) ;          
    if (i == iVBF) _hist[i]->SetLineStyle (2) ;
   } //---- setup signal samples
   
   if (_data) _data->Rebin (rebin);
   if (_data) _data->SetLineColor (kBlack);
   if (_data) _data->SetMarkerStyle (kFullCircle);
   
   hstack->Draw ("hist");
   
   bool addTenPerCentSyst = false;
   bool plotSystErrorBars = true;
   
   std::cout << " plotSystErrorBars " << std::endl;
   if (plotSystErrorBars == true) {
    TGraphAsymmErrors * gsyst = new TGraphAsymmErrors (hSum);
    for (int i = 0; i < gsyst->GetN (); ++i) {
     if (addTenPerCentSyst) {
      gsyst->SetPointEYlow (i, sqrt (hSum->GetBinError (i+1)*hSum->GetBinError (i+1)+hSum->GetBinContent (i+1)*hSum->GetBinContent (i+1)*0.10*0.10));
      gsyst->SetPointEYhigh (i, sqrt (hSum->GetBinError (i+1)*hSum->GetBinError (i+1)+hSum->GetBinContent (i+1)*hSum->GetBinContent (i+1)*0.10*0.10));
     }
     else {
      gsyst->SetPointEYlow (i, hSum->GetBinError (i+1));
      gsyst->SetPointEYhigh (i, hSum->GetBinError (i+1));
     }
    }
    gsyst->SetFillColor (12);
    gsyst->SetFillStyle (3345);
    gsyst->SetMarkerSize (0);
    gsyst->SetLineWidth (0);
    gsyst->SetLineColor (kWhite);
    if (_BandError != 0x0) {
     _BandError -> SetFillColor (12);
     _BandError -> SetFillStyle (3345);
     _BandError -> SetMarkerSize (0);
     _BandError -> SetLineWidth (0);
     _BandError -> SetLineColor (kWhite);
     _BandError -> Draw ("E2same");
    }
    else {
     gsyst->Draw ("E2same");
    }
    //TExec *setex1 = new TExec ("setex1","gStyle->SetErrorX (0)");
    //setex1->Draw ();
   }
   
   if (_hist[iHWW] && _isHWWOverlaid == false) _hist[iHWW]->Draw ("hist,same");
   //---- draw signal samples
   std::cout << " draw signal samples " << std::endl;
   for (int i = 0 ; i < nSamples; i++) {
    if (_sigHist[i]) _hist[i]->Rebin(rebin);
    if (_sigHist[i]) _hist[i]->Draw ("hist,same") ;
   } //---- draw signal samples
   if (_data) {
    bool plotCorrectErrorBars = true;
    if (plotCorrectErrorBars == true) {
     std::cout << " _data_correct_error_bars = " << _data_correct_error_bars << std::endl;
     if (_data_correct_error_bars == 0x0) {
      _data_correct_error_bars = new TGraphAsymmErrors (_data);
      for (int i = 0; i < _data_correct_error_bars->GetN (); ++i) {
       double N = _data_correct_error_bars->GetY ()[i];
       double alpha= (1-0.6827);
       double L = (N==0) ? 0 : (ROOT::Math::gamma_quantile (alpha/2,N,1.));
       double U = (N==0) ? ( ROOT::Math::gamma_quantile_c (alpha,N+1,1.) ) :
       ( ROOT::Math::gamma_quantile_c (alpha/2,N+1,1.) );
       _data_correct_error_bars->SetPointEYlow (i,double (N)-L);
       if (N > 0) _data_correct_error_bars->SetPointEYhigh (i, U-double (N));
       else _data_correct_error_bars->SetPointEYhigh (i, 1.14); // --> bayesian interval Poisson with 0 events observed
       // _data_correct_error_bars->SetPointEYhigh (i, 0.0);
      }
     }
     else {
      for (int i = 0; i < _data_correct_error_bars->GetN (); ++i) {
       double N = _data_correct_error_bars->GetY ()[i];
       if (N == 0) _data_correct_error_bars->SetPointEYlow (i, 0); //---- if negative error bar, move remove the negative part
      }
     }
     _data_correct_error_bars->Draw ("P0");
     //---- see     https://root.cern.ch/root/html/TGraphPainter.html
    }
    else {
     _data->Draw ("ep,same");
    }
   }
   hstack->SetTitle ("");
   Float_t theMax = hstack->GetMaximum ();
   Float_t theMin = hstack->GetMinimum ();
   
   if (_hist[iHWW]) {
    if (_hist[iHWW]->GetMaximum () > theMax) theMax = _hist[iHWW]->GetMaximum ();
    if (_hist[iHWW]->GetMinimum () < theMin) theMin = _hist[iHWW]->GetMinimum ();
   }
   
   if (_data) {
    Float_t dataMax = GetMaximumIncludingErrors (_data);
    if (dataMax > theMax) theMax = dataMax;
   }
   
   if (gPad->GetLogy ()) {
    hstack->SetMaximum (18 * theMax);
    hstack->SetMinimum (0.10);
   } else {
    hstack->SetMaximum (1.80 * theMax);
    hstack->SetMinimum (0.05);
//     hstack->GetYaxis()->SetRangeUser(0.1, 1.80*theMax);
//     hstack->SetMaximum (1.75 * theMax);
    //         hstack->SetMaximum (1.55 * theMax);
   }
   
   THStackAxisFonts (hstack, "x", _xLabel.Data ());
   
   if     (_breakdown == 1) {
    THStackAxisFonts (hstack, "y", "Events / bin");
    hstack->GetHistogram ()->LabelsOption ("v");
    if (_units.Sizeof () != 1) {
     THStackAxisFonts (hstack, "x", TString::Format ("%s [%s]",_xLabel.Data (),_units.Data ()));           
    }
   } 
   else if(_breakdown == 2) {
    THStackAxisFonts (hstack, "y", "S/(S+B) weighted events / bin");
    hstack->GetHistogram ()->LabelsOption ("v");
    if (_units.Sizeof () != 1) {
     THStackAxisFonts (hstack, "x", TString::Format ("%s [%s]",_xLabel.Data (),_units.Data ()));           
    }
   } 
   else {
    THStackAxisFonts (hstack, "x", TString::Format ("%s [%s]",_xLabel.Data (),_units.Data ()));
    if (_units.Sizeof () == 1) {
     //               THStackAxisFonts (hstack, "y", "Events / bin");
     THStackAxisFonts (hstack, "y", TString::Format ("Events / %.1f", _data->GetBinWidth (0)));
    } else {
     THStackAxisFonts (hstack, "x", TString::Format ("%s [%s]",_xLabel.Data (),_units.Data ()));
     if (_data->GetBinWidth (0) < 1) THStackAxisFonts (hstack, "y", TString::Format ("Events / %.1f %s", _data->GetBinWidth (0),_units.Data ()));
     else                            THStackAxisFonts (hstack, "y", TString::Format ("Events / %.0f %s", _data->GetBinWidth (0),_units.Data ()));
    }
   }
   
   //---- plotting the legend
   //---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
   std::cout << " legend " << std::endl;
   
   TString signalLegendRepr = "l" ;
   
   int j=0;
   
   // data: check if there is "data" (just once, j==0)
   if (_data ) { DrawLegend (xPos[j], 0.84 - yOff[j]*_yoffset, _data,          " data",    "lp"); j++; }
   
   if (_hist[iHWW      ]) { DrawLegend (xPos[j], 0.84 - yOff[j]*_yoffset, _hist[iHWW      ], _sampleLabel [iHWW      ], signalLegendRepr); j++; } 
   else if (_hist[iggH      ]==0x0 && _hist[iVBF      ]==0x0 && _hist[iVH      ]==0x0) {  
    //---- do nothing : there is no signal at all!
   }
   else if (_hist[iVH]!=0x0 && _hist[iggH]==0x0 && _hist[iVBF]==0x0){
    //---- if there is only VH (3l case)
    if (_hist[iVH       ]) { DrawLegend (xPos[j], 0.84 - yOff[j]*_yoffset, _hist[iVH       ], _sampleLabel [iVH       ], signalLegendRepr); j++; j++; j++; }
   }
   else {
    //---- or HWW all together xor separate components
    if (_hist[iggH      ]) { DrawLegend (xPos[j], 0.84 - yOff[j]*_yoffset, _hist[iggH      ], _sampleLabel [iggH      ], signalLegendRepr); j++; } else j++;
    if (_hist[iVBF      ]) { DrawLegend (xPos[j], 0.84 - yOff[j]*_yoffset, _hist[iVBF      ], _sampleLabel [iVBF      ], signalLegendRepr); j++; } else j++;
    if (_hist[iVH       ]) { DrawLegend (xPos[j], 0.84 - yOff[j]*_yoffset, _hist[iVH       ], _sampleLabel [iVH       ], signalLegendRepr); j++; } else j++;
   }
   
   int counterPosition = j;
   for ( j=(nSamples-1); j>=0 ; j--) {
    //        for (j=0; j < nSamples ; j++) {
    
    //        std::cout << " j = " << j << " counterPosition = " << counterPosition << std::endl;
    int jSample = _position.at(j);
    //        std::cout << " jSample = " << jSample << " j = " << j << " counterPosition = " << counterPosition << std::endl;
    // signals: check if they are signals: signals must be plotted on top!
    if (jSample==iHWW || jSample==iggH || jSample==iVBF || jSample==iVH) continue;
    
    // backgrounds
    if (_hist[jSample      ]) { DrawLegend (xPos[counterPosition], 0.84 - yOff[counterPosition]*_yoffset, _hist[jSample      ], _sampleLabel [jSample      ], "f" ); counterPosition++;}
    
   }
   
   std::cout << " ended " << std::endl;
   
   //      //---- the "CMS" flag
   //      TPaveText *pt = new TPaveText (0.61,0.8337762,0.9408059,0.8862238,"blNDC");
   //      pt->SetName ("title");
   //      pt->SetBorderSize (0);
   //      pt->SetFillColor (10);
   //      pt->SetTextFont (42);
   //      pt->SetTextSize (_tsize);
   //      pt->AddText ("CMS");
   //      pt->Draw ();
   
   double dist = 0.05 ;
   int distTimes = 0 ;
   double xstart = 0.9 ;
   double ystart = 0.85 ;
   
   //      //---- the CMS label
   //      TLatex* flag_cms = new TLatex (xstart, ystart - dist * distTimes++, TString ("#bf{CMS}"));
   //      flag_cms->SetNDC ();
   //      flag_cms->SetTextAlign (32);
   //      flag_cms->SetTextFont (42);
   //      flag_cms->SetTextSize (_tsize);
   //      flag_cms->Draw ("same");
   
   //---- the lumi label 
   //      if( _lumi < 21. ) { // don't draw this for 7+8 TeV plots 
   //          TLatex* flag_lumi = new TLatex (xstart, ystart - dist * distTimes++, TString::Format ("L = %.1f fb#lower[0.3]{^{-1}}", _lumi)) ;
   //          flag_lumi->SetNDC ();
   //          flag_lumi->SetTextAlign (32);
   //          flag_lumi->SetTextFont (42);
   //          flag_lumi->SetTextSize (_tsize);
   //          flag_lumi->Draw ("same");
   //      } 

   float t = gPad->GetTopMargin();
   
   //---- the lumi label 
   if (_lumiLabel) _lumiLabel->SetTextSize (0.60 * t);
   
   
   //---- the decay channel label
   for (unsigned int i = 0 ; i < _extraLabels.size () ; ++i) {
    TLatex* flag_extra = new TLatex (xstart, ystart - dist * distTimes++, _extraLabels.at (i)) ;
    flag_extra->SetNDC ();
    flag_extra->SetTextAlign (32);
    flag_extra->SetTextFont (42);
    flag_extra->SetTextSize (_tsize);
    flag_extra->Draw ("same");
   }
   
   TText * CMSLabel = new TText (0.18, 0.93, "CMS");
   CMSLabel->SetNDC ();
   CMSLabel->SetTextAlign (10);
   CMSLabel->SetTextFont (61);
//    CMSLabel->SetTextSize (_tsize);
   CMSLabel->SetTextSize (0.75*t);
   CMSLabel->Draw ("same") ;
   
   TText * CMSLabelPreliminary = new TText (0.25 + 0.8 * t, 0.93, "Preliminary");
   CMSLabelPreliminary->SetNDC ();
   CMSLabelPreliminary->SetTextAlign (10);
   CMSLabelPreliminary->SetTextFont (52);
//    CMSLabelPreliminary->SetTextSize (_tsize);
   CMSLabelPreliminary->SetTextSize (0.75*0.76*t);
   CMSLabelPreliminary->Draw ("same") ;
   
   
   _lumiLabel->Draw ("same") ;
   return hstack->GetHistogram () ;
  }
  
  
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  
  void setLumi (const float &l) { _lumi = l; }
  
  void setLabel (const TString &s) { _xLabel = s; }
  
  void setUnits (const TString &s) { _units = s; std::cout << " UNITS = " << s << std::endl;}
  
  void setBreakdown (const int &d = 0) { _breakdown = d; }
  
  void setLumiLabel (const std::string &s) {
   _lumiLabel = new TLatex (0.95, 0.92, TString (s));
   _lumiLabel->SetNDC ();
   _lumiLabel->SetTextAlign (30);
   _lumiLabel->SetTextFont (42);
   _lumiLabel->SetTextSize (_tsize);
  }
  
  void addLabel (const std::string &s) {
   _extraLabel = new TLatex (0.9, 0.74, TString (s));
   _extraLabel->SetNDC ();
   _extraLabel->SetTextAlign (32);
   _extraLabel->SetTextFont (42);
   _extraLabel->SetTextSize (_tsize);
   _extraLabels.push_back (TString (s.c_str ())) ;
  }
  void setIsHWWOverlaid (const bool &b = true) { _isHWWOverlaid = b; }
  
  void set_ErrorBand (TGraphAsymmErrors& grAE) {
   std::cout << " TGraphAsymmErrors:: Error band"  << std::endl;
   _BandError = ((TGraphAsymmErrors*) grAE.Clone());
   
  }
  
  void set_Position (int position, int value) {
   _position[position] = value;
  }
  
  
  
  TString * _sampleLabel ; //---- list of labels for the samples
  TH1F* _data;
  TGraphAsymmErrors* _data_correct_error_bars;
  
  std::vector<TH1F*> _hist;
  std::vector<TH1F*> _bkgHist ;
  std::vector<TH1F*> _sigHist ;
  std::vector<TString> _extraLabels ;
  TGraphAsymmErrors* _BandError; //---- error band from external input (from combine)
  
 private: 
 
  float    _lumi;          //---- lumi on the plot
  TString  _xLabel;        //---- label of the x axis
  TString  _units;         //---- units of the x axis
  TLatex * _lumiLabel;     //---- label with the centre of mass energy and lumi info
  TLatex * _extraLabel;    //---- any additional labels to be put in the plot
  int      _breakdown;     //---- 
  int      _mass;          //---- higgs mass
  int      _signalZoom;    //---- signal scale factor for plotting and legenda writing
  Color_t * _sampleColor ; //---- list of colors for the samples
  Bool_t _isHWWOverlaid;
  std::vector<int> _position; //---- order to plot samples
  
};


