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

//PG NB nSamples is the actual size of the enum
// 0/1 jet PAS order:
enum samp { iWW, iZJets, iTop, iVV, iWJets, iWZ, iZZ, iFakes, iZGamma, iVVV, iEM, iWgamma, iWgammaS, iHWW, iggH, iVBF, iVH, nSamples };

// VH and VBF PAS order:
// enum samp { iVV, iWJets, iWZ, iZZ, iFakes, iZGamma, iVVV, iEM, iWgamma, iWgammaS, iTop, iZJets, iWW, iHWW, iggH, iVBF, iVH, nSamples };


//                         data
float xPos[nSamples+1] = {0.19      ,0.19,0.19,0.19,0.41,0.41,0.41,0.41,0.41,0.41,0.41,0.41}; 
float yOff[nSamples+1] = {0         ,1   ,2   ,3   ,0   ,1   ,2   ,3   ,4   ,5   ,6   ,7   ,8   ,9};

const Float_t _tsize   = 0.033;
const Float_t _xoffset = 0.20;
const Float_t _yoffset = 0.05;


//------------------------------------------------------------------------------
// GetMaximumIncludingErrors
//------------------------------------------------------------------------------
Float_t GetMaximumIncludingErrors (TH1F* h)
{
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
void AxisFonts (TAxis*  axis,
        TString coordinate,
        TString title)
{
    axis->SetLabelFont (   42);
    axis->SetLabelOffset (0.015);
    axis->SetLabelSize (0.050);
    axis->SetNdivisions (  505);
    axis->SetTitleFont (   42);
    axis->SetTitleOffset (  1.5);
    axis->SetTitleSize (0.050);

    if (coordinate == "y") 
      {
        axis->SetTitleOffset (1.8);
        TString titolo = axis->GetTitle () ;
        titolo.ReplaceAll ("Entries", "Events") ;
        axis->SetTitle (titolo) ;
      
      }

    axis->SetTitle (title);
}


//------------------------------------------------------------------------------
// THStackAxisFonts
//------------------------------------------------------------------------------
void THStackAxisFonts (THStack* h,
        TString  coordinate,
        TString  title)
{
    TAxis* axis = NULL;

    if (coordinate.Contains ("x")) axis = h->GetHistogram ()->GetXaxis ();
    if (coordinate.Contains ("y")) axis = h->GetHistogram ()->GetYaxis ();

    AxisFonts (axis, coordinate, title);
}


//------------------------------------------------------------------------------
// DrawLegend
//------------------------------------------------------------------------------
void DrawLegend (Float_t x1,
        Float_t y1,
        TH1F*   hist,
        TString label,
        TString option)
{
    TLegend* legend = new TLegend (x1,
            y1,
            x1 + _xoffset,
            y1 + _yoffset);

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
        StandardPlot () 
          { 
            _hist.resize (nSamples, 0); 
            _bkgHist.resize (nSamples, 0); 
            _sigHist.resize (nSamples, 0); 
            _data = 0; 
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


        void setMCHist  (const samp & s, TH1F * h)   
          { 
            _hist[s] = h ; 
            std::cout << "READING SAMPLE " << s << " :: " << _sampleLabel[s] << std::endl ; 
            std::cout << "                  int = " << _hist[s]->Integral() << std::endl ; 
            if (s != iHWW && s != iggH && s != iVBF && s != iVH)
              {
                std::cout << "DEBUG reading background\n" ;
                _bkgHist[s] = h ;
              }
            else  
              {
                std::cout << "DEBUG reading signal\n" ;
                _sigHist[s] = h ;
              }
          } 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void SetColorsAndLabels ()
  {
    _sampleColor = new Color_t [nSamples] ;
    _sampleColor[iHWW    ] = kRed + 1 ;
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
    if (_signalZoom > 1) 
      {
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
    else                 
      {
        higgsLabel.Form (" H #rightarrow WW");
        _sampleLabel[iHWW  ] = higgsLabel ;
        higgsLabel.Form (" VBF");
        _sampleLabel[iVBF  ] = higgsLabel ;
        higgsLabel.Form (" ggH");
        _sampleLabel[iggH  ] = higgsLabel ;
        higgsLabel.Form (" VH");
        _sampleLabel[iVH   ] = higgsLabel ;
      }

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


  void setDataHist (TH1F * h)                 { _data          = h;  } 
  void setHWWHist  (TH1F * h)                 { setMCHist (iHWW  ,h); } 
  void setWWHist   (TH1F * h)                 { setMCHist (iWW   ,h); } 
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


  void setMass (const int &m)         
    { 
      _mass = m ; 
      TString dummy = "m#lower[0.3]{_{H}} = " ;
      dummy += _mass ;
      dummy += " GeV";
      _extraLabels.push_back (dummy) ;
    }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


  void setSignalZoom (const int zoom) { _signalZoom = zoom ; }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


  TH1* DrawAndRebinTo (const int &rebinTo) 
    {
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


  TH1* Draw (const int &rebin=1) 
    {
      //setUpStyle ();
      //if (!gPad) new TCanvas ();
    
      //PG prepare the THStack
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

       
      THStack* hstack = new THStack ();
      TH1D* hSum = (TH1D*)_data->Clone ();
      hSum->Rebin (rebin);
      hSum->Scale (0.0);

      //PG fill the THStack
      for (int itemp = 0 ; itemp < nSamples ; itemp++) 
        {
       
          int i = _position.at(itemp);
       
          if (_bkgHist[i] == 0) 
            {
              if (!_isHWWOverlaid ) continue ;
              else if (_sigHist[i] == 0) continue ;
            }  

          if (_hist[i] == 0)
            {
              std::cerr << "error: something is messed up in reading samples, exiting" << std::endl ;
              exit (1) ;
            }
    
          bool modifyXAxis = false;
          if (modifyXAxis == true)
            {
              TAxis *xa =_hist[i]->GetXaxis ();
              for (Int_t k=1;k<=_hist[i]->GetNbinsX ();++k)
                {
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
          if (i==13 || i==14 || i==15 || i==16) 
            {
              _hist[i]->SetLineWidth (3);
              _hist[i]->SetFillStyle (0);
            }
	   
          TH1F* temp_hist = (TH1F*) _hist[i]->Clone();
          hstack->Add (temp_hist);
          hSum->Add (temp_hist);
//           hstack->Add (_hist[i]);
//           hSum->Add (_hist[i]);
        } //PG fill the THStack
    
      //PG setup signal samples
      for (int i=0; i<nSamples; i++) 
        {
          if (_sigHist[i] == 0 ) continue ;
          _hist[i]->SetLineWidth (3) ;
          _hist[i]->SetLineColor (_sampleColor[i]) ;
          _hist[i]->SetFillStyle (0) ;          
          if (i == iVBF) _hist[i]->SetLineStyle (2) ;
        } //PG setup signal samples
    
      if (_data) _data->Rebin (rebin);
      if (_data) _data->SetLineColor (kBlack);
      if (_data) _data->SetMarkerStyle (kFullCircle);

      hstack->Draw ("hist");
    
      bool addTenPerCentSyst = false;
      bool plotSystErrorBars = true;
      if (plotSystErrorBars == true)
        {
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
    
      //PG draw signal samples
      for (int i = 0 ; i < nSamples; i++) 
        {
          if (_sigHist[i]) _hist[i]->Rebin(rebin);
          if (_sigHist[i]) _hist[i]->Draw ("hist,same") ;
        } //PG draw signal samples

      if (_data) 
        {
          bool plotCorrectErrorBars = true;
          if (plotCorrectErrorBars == true) 
            {
              TGraphAsymmErrors * g = new TGraphAsymmErrors (_data);
              for (int i = 0; i < g->GetN (); ++i) 
                {
                  double N = g->GetY ()[i];
                  double alpha= (1-0.6827);
                  double L = (N==0) ? 0  : (ROOT::Math::gamma_quantile (alpha/2,N,1.));
                  double U = (N==0) ? ( ROOT::Math::gamma_quantile_c (alpha,N+1,1.) ) :
                                       ( ROOT::Math::gamma_quantile_c (alpha/2,N+1,1.) );
                  g->SetPointEYlow (i,double (N)-L);
                  if (N > 0) g->SetPointEYhigh (i, U-double (N));
                  else  g->SetPointEYhigh (i, 1.14); // --> bayesian interval Poisson with 0 events observed    
// 		    g->SetPointEYhigh (i, 0.0);
                }
              g->Draw ("P");
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
        hstack->SetMaximum (1.55 * theMax);
      }

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
              THStackAxisFonts (hstack, "x", _xLabel.Data ());
//               THStackAxisFonts (hstack, "y", "Events / bin");
              THStackAxisFonts (hstack, "y", TString::Format ("Events / %.1f", _data->GetBinWidth (0)));
          } else {
              THStackAxisFonts (hstack, "x", TString::Format ("%s [%s]",_xLabel.Data (),_units.Data ()));
              if (_data->GetBinWidth (0) < 1) THStackAxisFonts (hstack, "y", TString::Format ("Events / %.1f %s", _data->GetBinWidth (0),_units.Data ()));
              else                            THStackAxisFonts (hstack, "y", TString::Format ("Events / %.0f %s", _data->GetBinWidth (0),_units.Data ()));
          }
      }

      //PG plotting the legend
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      
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
      
//      //PG the "CMS" flag
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
  
//      //PG the CMS label
//      TLatex* flag_cms = new TLatex (xstart, ystart - dist * distTimes++, TString ("#bf{CMS}"));
//      flag_cms->SetNDC ();
//      flag_cms->SetTextAlign (32);
//      flag_cms->SetTextFont (42);
//      flag_cms->SetTextSize (_tsize);
//      flag_cms->Draw ("same");
      
      //PG the lumi label 
//      if( _lumi < 21. ) { // don't draw this for 7+8 TeV plots 
//          TLatex* flag_lumi = new TLatex (xstart, ystart - dist * distTimes++, TString::Format ("L = %.1f fb#lower[0.3]{^{-1}}", _lumi)) ;
//          flag_lumi->SetNDC ();
//          flag_lumi->SetTextAlign (32);
//          flag_lumi->SetTextFont (42);
//          flag_lumi->SetTextSize (_tsize);
//          flag_lumi->Draw ("same");
//      } 

      //PG the lumi label
      for (unsigned int i = 0 ; i < _extraLabels.size () ; ++i) 
        {
          TLatex* flag_extra = new TLatex (xstart, ystart - dist * distTimes++, _extraLabels.at (i)) ;
          flag_extra->SetNDC ();
          flag_extra->SetTextAlign (32);
          flag_extra->SetTextFont (42);
          flag_extra->SetTextSize (_tsize);
          flag_extra->Draw ("same");
        }

      TLatex * CMSLabel = new TLatex (0.18, 0.93, "#bf{CMS}");
      CMSLabel->SetNDC ();
      CMSLabel->SetTextAlign (10);
      CMSLabel->SetTextFont (42);
      CMSLabel->SetTextSize (_tsize);
      CMSLabel->Draw ("same") ;

      _lumiLabel->Draw ("same") ;
      return hstack->GetHistogram () ;
    }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


        void setLumi (const float &l) { _lumi = l; }
        
        void setLabel (const TString &s) { _xLabel = s; }
        
        void setUnits (const TString &s) { _units = s; std::cout << " UNITS = " << s << std::endl;}
        
        void setBreakdown (const int &d = 0) { _breakdown = d; }

        void setLumiLabel (const std::string &s) {
            _lumiLabel = new TLatex (0.95, 0.93, TString (s));
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
        
    private: 
        std::vector<TH1F*> _hist;
        std::vector<TH1F*> _bkgHist ;
        std::vector<TH1F*> _sigHist ;
        std::vector<TString> _extraLabels ;
        TH1F* _data;

        //MWL
        float    _lumi;          //PG lumi on the plot
        TString  _xLabel;        //PG label of the x axis
        TString  _units;         //PG units of the x axis
        TLatex * _lumiLabel;     //PG label with the centre of mass energy and lumi info
        TLatex * _extraLabel;    //PG any additional labels to be put in the plot
        int      _breakdown;     //PG 
        int      _mass;          //PG higgs mass
        int      _signalZoom;    //PG signal scale factor for plotting and legenda writing
        TString * _sampleLabel ; //PG list of labels for the samples
        Color_t * _sampleColor ; //PG list of colors for the samples
        Bool_t _isHWWOverlaid;
        std::vector<int> _position; //---- order to plot samples
        TGraphAsymmErrors* _BandError; //---- error band from external input (from combine)
};


