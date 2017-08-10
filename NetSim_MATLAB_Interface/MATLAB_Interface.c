/*
 *
 *	This is a simple program that illustrates how to call the MATLAB
 *	Engine functions from NetSim C Code.
 *
 */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#include "mat.h"
#include "mex.h"



int status;
mxArray *out;
Engine *ep;
double *result;
double* arr;


_declspec(dllexport)double fn_netsim_matlab_init()
{
	/*
	 * Start the MATLAB engine 
	 */
	if (!(ep = engOpen(NULL))) {
		MessageBox ((HWND)NULL, (LPCWSTR)"Can't start MATLAB engine", 
			(LPCWSTR) "MATLAB_Interface.c", MB_OK);
		exit(-1);
	}
	return 0;
}

_declspec(dllexport)double fn_matlab_execute_command(char *str)
{
	status=engEvalString(ep,str);
	return status;
}

_declspec(dllexport)double fn_matlab_get_variable(char *str)
{
	out=engGetVariable(ep,str);
	result=mxGetPr(out);
	return *result;
}

_declspec(dllexport)double fn_netsim_matlab_Finish()
{
	status=engEvalString(ep,"exit");
	return 0;
}



