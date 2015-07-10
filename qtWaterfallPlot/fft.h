#ifndef FFT
#define FFT
#include "fftw/fftw3.h"
#include <string>
#include <iostream>
#include <vector>

#define NUMTHREADS 4

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
    Fft(fftw_complex* &in)
    {
        fftw_init_threads();
        fftw_plan_with_nthreads(NUMTHREADS);
        fftSize = 16384;
        inData = in;
        outData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftSize);
        my_plan = fftw_plan_dft_1d(int(fftSize),
                  in, outData,FFTW_FORWARD, FFTW_ESTIMATE_PATIENT);
        std::cout <<" New FFT created:" << std::endl;
        std::cout << "===================" << std::endl;
        std::cout << "Address of my_plan: " << &my_plan << std::endl;
        std::cout << "Address of outData: " << &outData << std::endl;
    }
    /*
        Constructor that takes size as an argument to set the initial FFT
        size to more or less than 10000.
    */
    Fft(fftw_complex* &in, unsigned int size)
    {
        fftw_init_threads();
        fftw_plan_with_nthreads(NUMTHREADS);
        fftSize = size;
        inData = in;
        outData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftSize);
        my_plan = fftw_plan_dft_1d(int(fftSize),
                  inData, outData,FFTW_FORWARD, FFTW_ESTIMATE_PATIENT);

    }

    //Destructor
    ~Fft()
    {
//        std::cout<< "~Fft() is called and address of my_plan is: " << std::endl;
//        std::cout << &my_plan << std::endl;

        // Destroying plan does not work...
        //fftw_destroy_plan(my_plan);
        fftw_cleanup();
        //fftw_cleanup_threads();
    }


    /*
        Updates the input data to work with a new size FFT. If the new size is
        greater than the old size, only data up to the old size is copied into the
        new sized FFT. If the new size is less than the old FFT size, data up to the
        smaller FFT size is passed.
    */
    void update(unsigned int size)
    {
        outData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*size);
        fftSize = size;
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
        fftw_complex *inData, *outData;
        fftw_plan my_plan;
        unsigned int fftSize;

};

#endif // FFT

