#ifndef EFFIC_INC
#define EFFIC_INC

void effic(int k, int N, double conflevel, 
	   double& mode, double& low, double& high);

extern "C"
{
  void effic2(int k, int N, double conflevel, 
	      double& mode, double& low, double& high);

  void effic2_v(unsigned length,
		int* k, int* N, double conflevel,
		double* mode, double* low, double* high);
  /*  This is the function version to call from R */
  void effic2_r(int* length,
		int* k, int* N, double* conflevel,
		double* mode, double* low, double* high);
  
}

#endif  //EFFIC_INC
