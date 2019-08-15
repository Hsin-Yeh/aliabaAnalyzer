#include "TROOT.h"
#include "TChain.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "makePlots.h"


/// global parameters
bool makePlots_flag = true;
bool display_flag = false;
bool help_flag = false;

/// utility
void main_makePlots();
void main_help();


/// -------------------- main -------------------- ///
int main (int argc, char* argv[]) {

	// Define arguments 
	int iarg = 1;
	while ( iarg < argc ) {
		string arg( argv[iarg] );

		if ( arg == "-s" || "-showPlots" ) {
			display_flag = true;
			iarg++;
		}
		else if ( arg == "-h" ) {
			makePlots_flag = false;
			help_flag = true;
			break;
		}
		else {
			std::cout << "Unknown option... print usage" << std::endl;
			makePlots_flag = false;
			help_flag = true;
			break;
		}
	}
	
	if ( makePlots_flag ) { main_makePlots(); }
	else if ( help_flag ) { main_help(); }
	
	return 0;
}

void main_makePlots() {

	std::ifstream intxtFile;
	intxtFile.open("input.txt"); 
	std::vector<std::string> filename;
	TChain *chain=new TChain("tree");

	while(!intxtFile.eof()) {
		string str;
		std::getline(intxtFile, str);
		if (str.size() > 2) {
			cout << "input file = " << str << endl;
			chain->Add(str.c_str());
			filename.push_back(str);
		}
	}
	
	makePlots makePlots(chain);
	makePlots.input_fileName = filename;
	makePlots.display_flag = display_flag;
	makePlots.Loop();
}


void main_help() {
	cout << endl;
	cout << "Arguments for this code" << endl << endl;
	cout << "-s : display the plots" << endl << endl;
}
