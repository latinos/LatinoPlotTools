// .x finalPlot.C+(0,5,"E_{T}^{miss}","GeV","histo.root","histo_zhpresel_met",1,125,24.4)

#include "TROOT.h"
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

void 
finalPlot (int nsel             = 0, 
           int ReBin            = 1, 
           char XTitle[300]     = "N_{jets}", 
           char units[300]      = "", 
           char plotName[300]   = "histo_nice.root", 
           char outputName[300] = "njets",
           bool isLogY          = false, 
           int MassH            = 160, 
           double lumi          = 4.6) 
{
  gInterpreter->ExecuteMacro("GoodStyle.C");
  gROOT->LoadMacro("StandardPlot.C");

  TFile* file = new TFile(plotName, "read");

  StandardPlot myPlot;
  myPlot.setLumi(lumi);
  myPlot.setLabel(XTitle);
  if     (lumi ==    4.9) myPlot.addLabel("#sqrt{s} = 7 TeV");
  else if(lumi ==   19.5) myPlot.addLabel("#sqrt{s} = 8 TeV");
  else if(lumi ==   24.4) myPlot.addLabel("#sqrt{s} = 7+8 TeV");
  else if(lumi ==   4.91) myPlot.addLabel("SSSF #sqrt{s} = 7 TeV");
  else if(lumi ==  19.51) myPlot.addLabel("SSSF #sqrt{s} = 8 TeV");
  else if(lumi ==   4.92) myPlot.addLabel("OSSF #sqrt{s} = 7 TeV");
  else if(lumi ==  19.52) myPlot.addLabel("OSSF #sqrt{s} = 8 TeV");
  else                    myPlot.addLabel(""); 
  myPlot.setUnits(units);

  TH1F* hWW     = (TH1F*)file->Get("histo0");
  TH1F* hZJets  = (TH1F*)file->Get("histo1");
  TH1F* hTop    = (TH1F*)file->Get("histo2");
  TH1F* hVV     = (TH1F*)file->Get("histo3");
  TH1F* hWJets  = (TH1F*)file->Get("histo4");
  //TH1F* hggH   = (TH1F*)file->Get("histoggH");
  //TH1F* hqqH   = (TH1F*)file->Get("histoqqH");
  //TH1F* hVH    = (TH1F*)file->Get("histoVH");
  double scale = 1;
  hWW	->Scale(scale);
  hZJets->Scale(scale);
  hTop  ->Scale(scale);
  hVV	->Scale(scale);
  hWJets->Scale(scale);
  if(nsel == 0 || nsel == 1){
    if(hWW->GetSumOfWeights(   ) > 0) myPlot.setMCHist(iWW,    (TH1F*)hWW   ->Clone("hWW"));
    if(hZJets->GetSumOfWeights() > 0) myPlot.setMCHist(iZJets, (TH1F*)hZJets->Clone("hZJets"));
    if(hTop->GetSumOfWeights()   > 0) myPlot.setMCHist(iTop,   (TH1F*)hTop  ->Clone("hTop"));
    if(hVV->GetSumOfWeights()	 > 0) myPlot.setMCHist(iVV,    (TH1F*)hVV   ->Clone("hVV")); 
    if(hWJets->GetSumOfWeights() > 0) myPlot.setMCHist(iWJets, (TH1F*)hWJets->Clone("hWJets"));
  }
  else if(nsel == 2 || nsel == 3) {
    TH1F* hZGamma = (TH1F*)file->Get("histo6");
    if(hZGamma->GetSumOfWeights() > 0) myPlot.setMCHist(iZGamma, (TH1F*)hZGamma->Clone("hZGamma"));

    //myPlot.setMCHist(iWW,    (TH1F*)hWW   ->Clone("hWW"));
    //myPlot.setMCHist(iZJets, (TH1F*)hZJets->Clone("hZJets"));
    myPlot.setMCHist(iZZ,    (TH1F*)hTop  ->Clone("hTop"));
    myPlot.setMCHist(iWZ,    (TH1F*)hVV   ->Clone("hVV")); 
    myPlot.setMCHist(iFakes, (TH1F*)hWJets->Clone("hWJets"));
  }
  else if(nsel == 4) {
    myPlot.setMCHist(iZJets, (TH1F*)hWW   ->Clone("hWW"));
    myPlot.setMCHist(iVVV,   (TH1F*)hZJets->Clone("hZJets"));
    myPlot.setMCHist(iWZ,    (TH1F*)hTop  ->Clone("hTop"));
    myPlot.setMCHist(iZZ,    (TH1F*)hVV   ->Clone("hVV")); 
    myPlot.setMCHist(iEM,    (TH1F*)hWJets->Clone("hWJets"));
  }

  TH1F* hHWW   = (TH1F*)file->Get("histos");
  hHWW->Scale(scale);
  if(nsel != 1 && nsel != 3){
    myPlot.setMCHist(iHWW, (TH1F*)hHWW->Clone("hHWW"));
    myPlot._mass = MassH;
  }

  TH1F *hData = (TH1F*)file->Get("histo5");
  myPlot.setDataHist((TH1F*)hData->Clone("data"));

  printf("%f + %f + %f + %f + %f = %f - %f - sig: %f\n",
          hWW->GetSumOfWeights(),hZJets->GetSumOfWeights(),hTop->GetSumOfWeights(),
  	  hVV->GetSumOfWeights(),hWJets->GetSumOfWeights(),
	  hWW->GetSumOfWeights()+hZJets->GetSumOfWeights()+hTop->GetSumOfWeights()+
	  hVV->GetSumOfWeights()+hWJets->GetSumOfWeights(),
	  hData->GetSumOfWeights(),hHWW->GetSumOfWeights());
/*
  TH1F* www    = (TH1F*)file->Get("histo0");
  www->Add( hZJets );
  www->Add( hTop   );
  www->Add( hVV    );
  www->Add( hWJets );
  hData->Divide(www);
  hData->Draw();
  for(int i=1; i<=hData->GetNbinsX(); i++){
    cout << hData->GetBinContent(i)<<endl;
  }
  return;
*/
  TCanvas* c1 = new TCanvas("c1", "c1");

  if(isLogY == true) c1->SetLogy();
  myPlot.Draw(ReBin);  // Can pass a rebin 
  c1->GetFrame()->DrawClone();

  //hggH->Rebin(ReBin);
  //hqqH->Rebin(ReBin);
  //hVH ->Rebin(ReBin);
  //hggH->Draw("same,hist");
  //hqqH->Draw("same,hist");
  //hVH ->Draw("same,hist");
  //cout << hggH->GetSumOfWeights() << " " << hqqH->GetSumOfWeights() << " " << hVH->GetSumOfWeights() << endl;

  char myOutputFile[300];
  sprintf(myOutputFile,"plots/%s.eps",outputName);
  c1->SaveAs(myOutputFile);
  sprintf(myOutputFile,"plots/%s.png",outputName);
  c1->SaveAs(myOutputFile);
  sprintf(myOutputFile,"plots/%s.pdf",outputName);
  c1->SaveAs(myOutputFile);

  bool doDataMCRatio = false;
  if(doDataMCRatio == true){
    // Data - MC
    //--------------------------------------------------------------------------
    TCanvas* c2 = new TCanvas("c2", "c2", 575, 10, 550, 600);

    TH1F* dt = (TH1F*)hData->Clone();
    TH1F* mc = (TH1F*)hWW->Clone();

    dt->SetNameTitle("dt", "dt");
    mc->SetNameTitle("mc", "mc");

    mc->Add(hZJets);
    mc->Add(hTop  );
    mc->Add(hVV   );
    mc->Add(hWJets);
    mc->Add(hHWW  );

    dt->Rebin(ReBin);
    mc->Rebin(ReBin);
    
    TH1F* diff = (TH1F*)dt->Clone();

    diff->SetNameTitle("", "");

    diff->Reset();
    double sum[2] = {0., 0.};

    for (Int_t i=1; i<=diff->GetNbinsX(); i++) {

      Float_t a = dt->GetBinContent(i);
      Float_t b = mc->GetBinContent(i);
      sum[0] = sum[0] + dt->GetBinContent(i);
      sum[1] = sum[1] + mc->GetBinContent(i);

      Float_t sa = dt->GetBinError(i);
      Float_t sb = mc->GetBinError(i);

      Float_t d  = 1.0;
      Float_t sd = 1.0;      
      if(a > 0 && b >0){
      	d  = a / b;
      	sd = sqrt(sa/a*sa/a+sb/b*sb/b)*d;
      	//d  = a - b;
      	//sd = sqrt(sa*sa+sb*sb);

        printf("data(%d): %f mc: %f -> data/mc = %f\n",i,a,b,a/b);
      	diff->SetBinContent(i, d);
      	diff->SetBinError  (i, sd);
      } else {
      	diff->SetBinContent(i, 1.0);
      	diff->SetBinError  (i, 0.0);
      	//diff->SetBinContent(i, 0.0);
      	//diff->SetBinError  (i, 0.0);
      }
    }
    printf("data: %f mc: %f -> data/mc = %f\n",sum[0],sum[1],sum[0]/sum[1]);

    diff->Draw();
    //hHWW->Rebin(ReBin);
    //hHWW->Draw("same,hist");
    
    diff->SetMinimum(0.4);
    diff->SetMaximum(1.6);

    gStyle->SetOptStat(0);

    //TBox* tenPercentBox = new TBox(diff->GetXaxis()->GetXmin(), 0.90,
    //			   diff->GetXaxis()->GetXmax(), 1.10);

    //tenPercentBox->SetFillColor(kAzure-9);

    //tenPercentBox->Draw("same");

    AxisFonts((TAxis*)diff->GetXaxis(), "x", XTitle);
    AxisFonts((TAxis*)diff->GetYaxis(), "y", "data / MC");

    TLine* oneLine = new TLine(diff->GetXaxis()->GetXmin(), 0,
			       diff->GetXaxis()->GetXmax(), 0);

    oneLine->SetLineStyle(3);
    oneLine->SetLineWidth(3);

    oneLine->Draw("same");

    diff->SetMarkerStyle(kFullCircle);
    diff->SetLineWidth(0);
    diff->SetFillColor(kGray+1);
    diff->SetFillStyle(1001);
    diff->Draw("AE2,same"); 
    diff->Draw("sameaxis");
    diff->Draw("same");

    c2->GetFrame()->DrawClone();
  }
}
