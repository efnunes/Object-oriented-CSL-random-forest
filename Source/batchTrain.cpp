// Header
// File: batchTrain
// Author: Ashok Chandrashekar, brain engineering laboratory, Dartmouth
// Author: Eric Nunes , brain engineering laboratory, Dartmouth
// Description: Entry mex function for training batch classifier directory

#include "../Include/cslForest.h"
//#include <windows.h>
#include <time.h>

cslForest *forest;
union addrConverter convAddr;

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
    int N, M, mrows,ncols;
	double *matPtr = NULL;
    double *dataPtr = NULL;
    double *retPtr = NULL;
    vector <int> labels;

	//SYSTEMTIME lt_begin, lt_end;
	//FILETIME FileTimeBegin, FileTimeEnd;
	//ULARGE_INTEGER uli_begin, uli_end; 
	

    /* Check for proper number of arguments. */
    if (nrhs != 7)
        mexErrMsgTxt("7 inputs required.");
    else if(nlhs>3)
        mexErrMsgTxt("Too many output arguments");
  
    
    /* The 1st argument is the dataset, with one row for each sample*/
    N = (int)mxGetM(prhs[0]);
    M = (int)mxGetN(prhs[0]);
    dataPtr = (double*)mxGetPr(prhs[0]);

    float **data = new float* [N];
	for (int i = 0; i < N; i++)
		data[i] = new float [M];

	for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            data[i][j] =  (float)dataPtr[j * N + i];
	
    /* The 2nd input is the set of labels */
    mrows = (int)mxGetM(prhs[1]);
    ncols = (int)mxGetN(prhs[1]);
    if( !(mrows==N && ncols==1) )
        mexErrMsgTxt("Input 2 incorrect.");
    
    matPtr = mxGetPr(prhs[1]);
    for (int i = 0; i < N; i++)
        labels.push_back ((int)matPtr[i]);

    /* The 3rd argument is the number of trees */
    mrows = (int)mxGetM(prhs[2]);
    ncols = (int)mxGetN(prhs[2]);
    if( !(mrows==1 && ncols==1) )
        mexErrMsgTxt("Input 3 incorrect.");
    
    /* The 4th argument is the max branch factor */
    mrows = (int)mxGetM(prhs[3]);
    ncols = (int)mxGetN(prhs[3]);
    if( !(mrows==1 && ncols==1) )
        mexErrMsgTxt("Input 4 incorrect.");

	/* The 5th argument is the bagging fraction */
    mrows = (int)mxGetM(prhs[4]);
    ncols = (int)mxGetN(prhs[4]);
    if( !(mrows==1 && ncols==1) )
        mexErrMsgTxt("Input 5 incorrect.");

	/* The 6th argument is the feat fraction */
    mrows = (int)mxGetM(prhs[5]);
    ncols = (int)mxGetN(prhs[5]);
    if( !(mrows==1 && ncols==1) )
        mexErrMsgTxt("Input 6 incorrect.");
	
	int totNodeCount = 0;
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    plhs[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
    plhs[2] = mxCreateDoubleMatrix(1, 1, mxREAL);
    plhs[3] = mxCreateDoubleMatrix(1, 1, mxREAL);
    
    
    forest = new cslForest((int)mxGetScalar(prhs[2]), M, (int)mxGetScalar(prhs[3]), mxGetScalar(prhs[4]), mxGetScalar(prhs[5]));
	//GetLocalTime(&lt_begin);
    forest->batchTrain(data, labels,  M);
	//GetLocalTime(&lt_end);
	//forest->batchTrain(data, M);
   	
	
	/*SystemTimeToFileTime(&lt_begin, &FileTimeBegin);
	SystemTimeToFileTime(&lt_end, &FileTimeEnd);

	uli_begin.LowPart = FileTimeBegin.dwLowDateTime; // could use memcpy here!
	uli_begin.HighPart = FileTimeBegin.dwHighDateTime;
	uli_end.LowPart = FileTimeEnd.dwLowDateTime; // could use memcpy here!
	uli_end.HighPart = FileTimeEnd.dwHighDateTime;

	ULONGLONG systemTimeIn_ms1( uli_begin.QuadPart/10000 );
	ULONGLONG systemTimeIn_ms2( uli_end.QuadPart/10000 );
	double ElapsedTrainTime = (double)(systemTimeIn_ms2 - systemTimeIn_ms1)/1000;*/

	for (int i = 0; i < N; i++)
		delete []  data[i];
	delete[] data;

	//retPtr = mxGetPr(plhs[1]);
    //*retPtr = ElapsedTrainTime;



    
    //Testing

    //SYSTEMTIME t_begin, t_end;
	//FILETIME TestTimeBegin, TestTimeEnd;
	//ULARGE_INTEGER ul_begin, ul_end;


    /* The 7th argument is the document histogram map*/
    N = (int)mxGetM(prhs[6]);
    M = (int)mxGetN(prhs[6]);
    matPtr = mxGetPr(prhs[6]);
	float *testData = new float [N*M];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            testData[i*M+j] =  (float)(matPtr[j * N + i]);
    plhs[0] = mxCreateDoubleMatrix(N, 1, mxREAL);
    retPtr = mxGetPr(plhs[0]);
    //GetLocalTime(&t_begin);
    for (int i = 0; i < N; i++)
	{
		std::vector<float> temp = forest->predictLabel(&(testData[i*M]),M);
		//retPtr[i] = temp;
		mxArray* ithCell = mxCreateDoubleMatrix(N, 20, mxREAL);
		retPtr = mxGetPr(ithCell);
		for (int j = 0; j <= temp.size(); j++)
			retPtr[j] = temp[j];

		mxSetCell(plhs[1],i,ithCell);
        //retPtr[i] = forest->predictLabel(&(testData[i*M]), M);

	    //GetLocalTime(&t_end);
	}
		
		/* Timing code */
	/*SystemTimeToFileTime(&t_begin, &TestTimeBegin);
	SystemTimeToFileTime(&t_end, &TestTimeEnd);

	ul_begin.LowPart = TestTimeBegin.dwLowDateTime; // could use memcpy here!
	ul_begin.HighPart = TestTimeBegin.dwHighDateTime;
	ul_end.LowPart = TestTimeEnd.dwLowDateTime; // could use memcpy here!
	ul_end.HighPart = TestTimeEnd.dwHighDateTime;

	ULONGLONG systemTimeIn_ms3( ul_begin.QuadPart/10000 );
	ULONGLONG systemTimeIn_ms4( ul_end.QuadPart/10000 );
	double ElapsedTestTime = (double)(systemTimeIn_ms4 - systemTimeIn_ms3)/1000;*/

	//retPtr = mxGetPr(plhs[2]);
    //*retPtr = ElapsedTestTime;

	//retPtr = mxGetPr(plhs[3]);
    //*retPtr = totNodeCount;
	
	delete [] testData;
    delete forest;
    return;   
       
    
    convAddr.fptr = forest;
    *retPtr = convAddr.addr;

}
