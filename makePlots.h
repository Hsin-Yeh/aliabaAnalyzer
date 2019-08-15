#ifndef makePlots_h
#define makePlots_h

#include <iostream>
#include <vector>
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH1.h"
#include "TGraph.h"
#include "TApplication.h"
#include "TCanvas.h"

#define NCHANNELS 128
using namespace std;

class makePlots {
 public :

	makePlots (TChain* inchain);
	~makePlots();

	// public parameter
	TTree               *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t               fCurrent; //!current Tree number in a TChain
	vector<string>      input_fileName;
	bool display_flag;
	
	virtual Int_t    Cut(Long64_t entry);
	void     Init();
	void     Loop();
	virtual void     Show(Long64_t entry = -1);


 private:

	
	//private parameter
	TApplication   *app; //This is often forgotten when one wants to displat a canvas on the screen!
	TCanvas        *c; 

    // Declaration of leaf types
	Int_t           event;          // event number 
	Double_t        clock;          
	Int_t           signal[128];    // signal ADC for the 128 channels 
	Int_t           header[16];     
	Double_t        temperature;   
	Double_t        time;

	// List of branches
	TBranch        *b_event;   //!
	TBranch        *b_clock;   //!
	TBranch        *b_signal;   //!
	TBranch        *b_header;   //!
	TBranch        *b_temperature;   //!
	TBranch        *b_time;   //!

	// If you receive a new root file which you want to check the data in it, you could simply type :
	//
	// $ root your_root_file.root
	// root[0] .ls
	// root[1] thetree->Print();
	// root[2] thetree->Show(1);
	//
	// Or open a TBrowser 
	// root[3] new TBrowser
	//
	// If you want to know all of its declarations of leaf types in the TTrees :
	// root[4] thetree->MakeClass("thetree");
	//
	// And you will find thetree.C and thetree.h in the same directory
	// Open the files, and enjoy copy & pasting the declarations to wherever you need them!
	// I recommend you to try it yourself once.

 private:
	
};

#endif

