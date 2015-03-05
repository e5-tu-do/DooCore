////////////////////////////////////////////////////////////////////////////////
//
// Main module for the EFFIC program
//
// This program reads a file containing two vectors, which represent "before
// cut" and "after cut" histograms. It then uses a Bayesian analysis to
// determine the posterior probability for the cut efficiency in each bin.
// A flat prior is used for the efficiency. The outputs calculated from the
// posterior probability are the most probable value of the efficiency, and
// the upper and lower edges of the shortest C% confidence interval; C is
// entered on the command line
//
// The calling syntax is:
//     EFFIC datafile conf
// where
//     datafile is the name of the data file from which we read
//     conf     is required probability content of the confidence interval
//
// Created 14 Feb 1996        Marc Paterno
////////////////////////////////////////////////////////////////////////////////

#include "effic/effic2.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

void printUsage()
{
  cerr << "Syntax is: effic datafile conflevel\n";
}

int main(int argc, char* argv[])
{
  if ( argc != 3 )
    {
      printUsage();
      return 1;
    }

  // Read command line parameters
  const char* filename = argv[1];
  double conflevel = atof(argv[2]);

  ifstream ifs(filename);
  if ( !ifs )
    {
      cerr << "Failed to open file '" 
	   << filename 
	   << "'\n";
      return 2;
    }

  // Read file a line at a time, calculating and writing output as we
  // go. This is not at all forgiving of errors in the input file!
  int k, n;
  int status = 0;
  double mode, low, high;
  while ( ifs >> k >> n )
    {
      try
	{
	  effic2(k, n, conflevel, mode, low, high);
	  cout << scientific <<  mode << " " 
	       << scientific <<  low << " " 
	       << scientific <<  high << endl;;
	}
      catch ( exception& x )
	{
	  cerr << x.what() << endl;
	  status = 3;
	}
    }

  return status;
}
