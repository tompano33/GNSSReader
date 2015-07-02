#ifndef FFT
#define FFT
#include "fftw/fftw3.h"
#include <string>
#include <iostream>

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
    Fft()
    {
        fftw_init_threads();
        fftw_plan_with_nthreads(NUMTHREADS);
        fftSize = 10000;
        inData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftSize);
        outData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftSize);
        m_inDataDouble = (double*) malloc(sizeof(double)*fftSize);
        my_plan = fftw_plan_dft_1d(int(fftSize),
                  inData, outData,FFTW_FORWARD, FFTW_ESTIMATE_PATIENT);
        std::cout <<" New FFT created:" << std::endl;
        std::cout << "===================" << std::endl;
        std::cout << "Address of my_plan: " << &my_plan << std::endl;
        std::cout << "Address of outData: " << &outData << std::endl;
    }
    /*
        Constructor that takes size as an argument to set the initial FFT
        size to more or less than 10000.
    */
    Fft(unsigned int size)
    {
        fftw_init_threads();
        fftw_plan_with_nthreads(NUMTHREADS);
        fftSize = size;
        inData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftSize);
        outData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftSize);
        m_inDataDouble = (double*) malloc(sizeof(double)*fftSize);
        my_plan = fftw_plan_dft_1d(int(fftSize),
                  inData, outData,FFTW_FORWARD, FFTW_ESTIMATE_PATIENT);
    }

    //Destructor
    ~Fft()
    {
//        std::cout<< "~Fft() is called and address of my_plan is: " << std::endl;
//        std::cout << &my_plan << std::endl;
//        fftw_destroy_plan(my_plan);
//        fftw_cleanup_threads();
        free(m_inDataDouble);
        fftw_cleanup();
    }

    /*
        Updates the input data with new input passed as a parameter.
    */
    void update(fftw_complex* in)
    {
        memcpy(inData,in,sizeof(fftw_complex)*fftSize);
        for(unsigned int i=0; i < fftSize; i++)
        {
            m_inDataDouble[i] = inData[i][0];
        }
    }

    /*
        Updates the input data to work with a new size FFT. If the new size is
        greater than the old size, only data up to the old size is copied into the
        new sized FFT. If the new size is less than the old FFT size, data up to the
        smaller FFT size is passed.
    */
    void update(unsigned int size)
    {        
        fftw_plan_with_nthreads(NUMTHREADS);
        unsigned int oldSize = fftSize;
        fftSize = size;
        fftw_complex* NewInData = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*size);
        fftw_complex* NewOutData  = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*size);
        double* NewInDataDouble = (double*) malloc(sizeof(double)*size);

        if(oldSize <= size)
        {
            memmove(NewInData,inData,oldSize);
            memmove(NewOutData,outData,oldSize);
            memmove(NewInDataDouble, m_inDataDouble, oldSize);
        }
        else
        {
            memmove(NewInData,inData,fftSize);
            memmove(NewOutData,outData,fftSize);
            memmove(NewInDataDouble, m_inDataDouble, fftSize);
        }
        inData = NewInData;
        outData = NewOutData;
        m_inDataDouble = NewInDataDouble;

        for(unsigned int i=0; i < size; i++)
        {
            m_inDataDouble[i] = inData[i][0];
        }
        my_plan = fftw_plan_dft_1d(int(fftSize),
                  inData, outData,FFTW_FORWARD, FFTW_ESTIMATE_PATIENT);
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

    /*
        Returns the input data.
    */
    double* getInData()
    {
        return m_inDataDouble;
    }


private:

        // Member variables
        fftw_complex *inData, *outData;
        fftw_plan my_plan;
        double *m_inDataDouble;
        unsigned int fftSize;

};

#endif // FFT

