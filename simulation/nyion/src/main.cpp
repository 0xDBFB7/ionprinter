#include "nyion.hpp"

int main(int argc, char **argv)
{
  TApplication *myApp = new TApplication("myApp", &argc, argv);
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
	return 0;
}
