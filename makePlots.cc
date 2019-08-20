#hi~~~~~~~~~~~~~~~~~~
#ifndef makePlots_cxx
#define makePlots_cxx

//#define DEBUG   //Uncomment to compile under DEBUG mode

#include "makePlots.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TF1.h"
#include "TChain.h"
#include "TStyle.h"


//Constructor-initializer
makePlots::makePlots(TChain* inchain):fChain(inchain)
{
}

makePlots::~makePlots()
{
	cout << endl;
	cout << "Calling Destructor" << endl;
}


void makePlots::Init()
{
	// Set object pointer (if there are vectors etc... need to be initialized to 0)
	//* SetBranchAddresses
	fChain->SetBranchAddress("event", &event, &b_event);
	fChain->SetBranchAddress("clock", &clock, &b_clock);
	fChain->SetBranchAddress("signal", signal, &b_signal);
	fChain->SetBranchAddress("header", header, &b_header);
	fChain->SetBranchAddress("temperature", &temperature, &b_temperature);
	fChain->SetBranchAddress("time", &time, &b_time);

	// Set batch mode 
	if(display_flag)
		gROOT->SetBatch(kFALSE); 
	else
		gROOT->SetBatch(kTRUE);
	
	app = new TApplication("app",0,0); //This is often forgotten when one wants to displat a canvas on the screen!
	c = new TCanvas();
	
	cout << "----------Init complete----------" << endl << endl;
}

void makePlots::Show(Long64_t entry)
{
	// Print contents of entry.
	// If entry is not specified, print current entry
	if (!fChain) return;
	fChain->Show(entry);
}

Int_t makePlots::Cut(Long64_t entry)
{
	// This function may be called from Loop.
	// returns  1 if entry is accepted.
	// returns -1 otherwise.
	return 1;
}


void makePlots::Loop()
{
	Init();
	char title[100];
	
	/// Set Output Root File
	int start = input_fileName[0].find_last_of("/");
	int end   = input_fileName[0].find(".root");
	string outf = input_fileName[0].substr(start+1,end-start-1);
	
	sprintf(title,"root_plot/plot_%s.root",outf.c_str());
	TFile* outfile = new TFile(title,"RECREATE");

	cout << "output file = " <<  title << endl << endl;
	
	/// Define some variables
	int nentries = fChain->GetEntries();
	int passedEvents=0;
	double FitMean[NCHANNELS], FitSigma[NCHANNELS];
	double ChannelID[NCHANNELS];
	std::cout << "Entries: " << nentries << std::endl;

	/// Define TDirectories
	sprintf(title,"Pedestal Histograms");
	TDirectory *cdped = outfile->mkdir(title);
	cdped->cd();

	/// Define Histograms
	TH1F* h_pedestal[NCHANNELS];

	
	/// Initialize 
	for( int ch = 0; ch < NCHANNELS; ch++){
		sprintf(title,"Ch%d",ch);
		h_pedestal[ch] = new TH1F(title,title,100,400,850);
		FitMean[ch] = 0;
		FitSigma[ch] = 0;
		ChannelID[ch] = ch;
	}

	
	// Loop Over Events
	for (int entry=0;entry<nentries;++entry){
		
		if(entry%1000==0) std::cout << "Processed ... " << entry 
								   << "/" << nentries << " events" << std::endl;
		fChain->GetEntry(entry);

		/// Fill Histograms
		for (int ch = 0; ch < NCHANNELS; ch++){
						
#ifdef DEBUG
			// You could define DEBUG to show these on terminal ( Remove comments from line 4
			std::cout << "Channel: " << ch << "  signal: " << signal[ch] << " ADC" << std::endl; 
#endif
			
			h_pedestal[ch]->Fill(signal[ch]);
		}
		passedEvents++;
	}
	// End Loop
	cout << "PassedEvents : " << passedEvents << endl;
	cout << "Entries      : " << nentries << endl;
	cout << "Efficiency : " << ((float) passedEvents)/nentries << endl;
	
	// Fit histograms
	for( int ch = 0; ch < NCHANNELS; ch++){
		
#ifdef DEBUG
		std::cout << ChannelID[ch] << " " << FitMean[ch] << std::endl;
#endif
		
		h_pedestal[ch]->Fit("gaus","Q"); 
		FitMean [ch] = h_pedestal[ch]->GetFunction("gaus")->GetParameter(1);
		FitSigma[ch] = h_pedestal[ch]->GetFunction("gaus")->GetParameter(2);
	}
													   
	/// Define Graphs 
	TGraph *g_pedestal = new TGraph(NCHANNELS,ChannelID,FitMean); 
	g_pedestal->SetName("Pedestal Mean");
	g_pedestal->SetTitle("Pedestal Mean");
	g_pedestal->SetMarkerStyle(20);
	g_pedestal->SetMarkerSize(0.8);
	g_pedestal->SetLineWidth(0);
	g_pedestal->GetXaxis()->SetTitle("ChannelID");
	g_pedestal->GetYaxis()->SetTitle("ADC");
	
	TGraph *g_noise = new TGraph(NCHANNELS,ChannelID,FitSigma);
	g_noise->SetName("Noise");
	g_noise->SetTitle("Noise");
	g_noise->SetMarkerStyle(20);
	g_noise->SetMarkerSize(0.8);
	g_noise->SetLineWidth(0);
	g_noise->GetXaxis()->SetTitle("ChannelID");
	g_noise->GetYaxis()->SetTitle("ADC");

	if( display_flag ){
		h_pedestal[40]->Draw();
		c->Update();
		gPad->WaitPrimitive();
		
		g_pedestal->Draw("AP");
		c->Update();
		gPad->WaitPrimitive();
		
		g_noise->Draw("AP");
		c->Update();
		gPad->WaitPrimitive();
	}

	// Save plots to pdf
	sprintf(title,"plots/%s_pedestal.pdf",outf.c_str());
	g_pedestal->SaveAs(title);
	sprintf(title,"plots/%s_noise.pdf",outf.c_str());
	g_noise->SaveAs(title);

	/// Write Graph to file 
	outfile->cd();  // You could create another TDirectory to write the graphs 
	g_pedestal->Write();
	g_noise->Write();
	outfile->Write();
	outfile->Close();
}


#endif // #ifdef makePlots_cxx
 

