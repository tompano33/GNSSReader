#ifndef FFT
#define FFT
#include "fftw/fftw3.h"
#include <string>
#include <iostream>
#include <vector>

#define NUMTHREADS 10

/*
    The FFT class
    This class is the container for the Fast Fourier Transform and utilizes the
    FFTW library for all FFT calculations. It sets up the number of threads to be
    used and the plan for the FFTW's work. Memory is allocated for input and output
    data.
    The program executes the plan to perform the actual FFT. After the FFT is done,
    all memory used is freed to the O/S.

*/
class Fft
{
public:
    // Default constructor
    Fft(double* &in)
    {
        fftw_init_threads();
        fftw_plan_with_nthreads(NUMTHREADS);
        fftSize = 16384;
        inData = in;
        outData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftSize);
        my_plan = fftw_plan_dft_r2c_1d(int(fftSize),
                  in, outData,FFTW_ESTIMATE_PATIENT);
    }
    /*
        Constructor that takes size as an argument to set the initial FFT
        size.
    */
    Fft(double* &in, unsigned int size)
    {
        fftw_init_threads();
        fftw_plan_with_nthreads(NUMTHREADS);
        fftSize = size;
        inData = in;
        outData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftSize);
        my_plan = fftw_plan_dft_r2c_1d(int(fftSize),
                  inData, outData,FFTW_ESTIMATE_PATIENT);

    }

    //Destructor
    ~Fft()
    {
        fftw_cleanup();
    }


    /*
        Updates the input data to work with a new size FFT or new data for input.
        The output data is reallocated to the new size. The old plan is destroyed
        and a new one is created for the new size.
    */
    void update(unsigned int size, double* &in)
    {
        outData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*size);
        fftSize = size;
        fftw_destroy_plan(my_plan);
        my_plan = fftw_plan_dft_r2c_1d(int(fftSize),
                    in, outData,FFTW_ESTIMATE_PATIENT);
    }

    /*
        Executes the FFT based on the current plan.
    */
    fftw_complex* exec()
    {
        // FFT execution
        fftw_execute(my_plan);
        return outData;
    }

    /*
        Returns the size of the FFT.
    */
    unsigned int size()
    {
        return fftSize;
    }

private:

        // Member variables
        fftw_complex *outData;
        double *inData;
        fftw_plan my_plan;
        unsigned int fftSize;
};

#endif // FFT

