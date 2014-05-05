// Header
// File: classify.cpp
// Author: Ashok Chandrashekar, brain engineering laboratory, Dartmouth 
// Description: Entry mex function for label prediction
//

#include "../Include/cslForest.h"
#include <io.h>
union addrConverter convAddr;

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
    int M, N,  mrows,ncols;
    float *data = NULL;
    double *retPtr = NULL;

    /* Check for proper number of arguments. */
    if (nrhs<2)
        mexErrMsgTxt("Two inputs required.");
    else if(nlhs>1)
        mexErrMsgTxt("Too many output arguments");
  
    /* The 1st argument is the document histogram map*/
    N = (int)mxGetM(prhs[0]);
    M = (int)mxGetN(prhs[0]);
	
    /* The 2nd input is the addr of root */
    mrows = (int)mxGetM(prhs[1]);
    ncols = (int)mxGetN(prhs[1]);
    if( !(mrows==1 && ncols==1) )
        mexErrMsgTxt("Input 2 incorrect.");
    convAddr.addr  = (unsigned int)mxGetScalar(prhs[1]);

    	
    
    double *matPtr = mxGetPr(prhs[0]);
	data = new float [N*M];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            data[i*M+j] =  (float)(matPtr[j * N + i]);


	plhs[0] = mxCreateDoubleMatrix(N, 1, mxREAL);
    retPtr = mxGetPr(plhs[0]);
    
    for (int i = 0; i < N; i++)
        retPtr[i] = convAddr.fptr->predictLabel(&(data[i*M]), M);
	
	
    delete convAddr.fptr;
    delete [] data;
}
