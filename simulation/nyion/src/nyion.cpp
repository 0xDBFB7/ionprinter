// #include <iostream>
#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TBrowser.h"
#include "TFrame.h"
#include "TApplication.h"

void testah(){
  TApplication *myApp = new TApplication("myApp", 0, NULL);
  TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 1280, 720);
  TH1F h1("h1","Histo from a Gaussian",100,-3,3);
  h1.FillRandom("gaus", 10000);
  h1.Draw();

  TFile *file = new TFile("../Debug/test.root", "RECREATE");
  file->WriteTObject(c1);

  c1->Update();
  c1->GetFrame()->SetBorderSize(12);
  c1->Modified();

  myApp->Run();
}



//
// int main(int ac, char** av){
//     TApplication theApp("App", &argc, argv);
//     TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,500,300);
//     Double_t x[100], y[100];
//     Int_t n = 20;
//     for (Int_t i=0;i<n;i++) {
//     x[i] = i*0.1;
//     y[i] = 10*sin(x[i]+0.2);
//     }
//     TGraph* gr = new TGraph(n,x,y);
//     gr->Draw("AC*");
//   return RUN_ALL_TESTS(ac, av);
// }
