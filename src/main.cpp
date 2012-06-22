/*
*	Copyright 2012, Andrea Mazzotti <amm263@gmail.com>
*
*	Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
*	provided that the above copyright notice and this permission notice appear in all copies.
*
*	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE 
*	INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR 
*	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
* 	OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING 
*	OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
*/

#include <iostream>
#include "diagonalize.h"
#include "walker.h"
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <cstdlib>

using namespace std;

int compare_double(const void* a, const void* b)
{
    double* arg1 = (double*) a;
    double* arg2 = (double*) b;
    if( *arg1 < *arg2 ) return -1;
    else if( *arg1 == *arg2 ) return 0;
    else return 1;
}

int main()
{
    FILE * pFile;

    /*
    *   Vectors V and W.
    */
    long long *V;
    long long *W;

    /*
    *   Length of problem.
    *   Maximum length of V and W.
    */
    long long problemLength=100000;

    /*
    *   accuracy = x is to run each algorithm x times.
    *   Because some instances can be worst than others, so the more accuracy we have,
    *   the more results will be in average.
    */
    long long accuracy=100;


    /*
    *   points to plot the time curve
    */
    long long points=2000;

    /*
    *   dispersion of random numbers to fill V and W vectors.
    */
    long long dispersion=5;

    /*
    *   relativeLength for an instance.
    *   It starts low and increase at each loop. ( relativeLength += problemLength/points )
    */
    long long relativeLength=0;

    /*
    *   Double vectors to store times.
    */
    double *DiagonalizeTimes=new double[points];
    double *WalkerTimes=new double[points];

    /*
    *   Store accuracy times sum.
    */
    double TDiagonalize;
    double TWalker;

    long long i,j,y,kv,kw,k;

    Diagonalize *diagonalize;
    Walker *walker;
    bool check;
    bool *percents= new bool[10];
    for (i=0;i<10;i++)
    {
        percents[i]=true;
    }
    //Timer Stuff
    LARGE_INTEGER ticksPerSecond;
    LARGE_INTEGER start_ticks, end_ticks, cputime;
    QueryPerformanceFrequency(&ticksPerSecond);

    // Start of testing
    cout << "Wait please, it can take a while." << endl << endl;
    for (i=0;i<points;i++)
    {
        relativeLength+=ceil(problemLength/points);

        // This is a fancy output. I like verbose programs.
        if ((percents[9])&&(relativeLength>=ceil(problemLength*0.99)))
        {
            cout << "99%" << endl << endl;
            percents[9]=false;
        }
        else if ((percents[8])&&(relativeLength>=ceil(problemLength*0.90)))
        {
            cout << "90%" << endl;
            percents[8]=false;
        }
        else if ((percents[7])&&(relativeLength>=ceil(problemLength*0.80)))
        {
            cout << "80%" << endl;
            percents[7]=false;
        }
        else if ((percents[6])&&(relativeLength>=ceil(problemLength*0.70)))
        {
            cout << "70%" << endl;
            percents[6]=false;
        }
        else if ((percents[5])&&(relativeLength>=ceil(problemLength*0.60)))
        {
            cout << "60%" << endl;
            percents[5]=false;
        }
        else if ((percents[4])&&(relativeLength>=ceil(problemLength*0.50)))
        {
            cout << "50%" << endl;
            percents[4]=false;
        }
        else if ((percents[3])&&(relativeLength>=ceil(problemLength*0.40)))
        {
            cout << "40%" << endl;
            percents[3]=false;
        }
        else if ((percents[2])&&(relativeLength>=ceil(problemLength*0.30)))
        {
            cout << "30%" << endl;
            percents[2]=false;
        }
        else if ((percents[1])&&(relativeLength>=ceil(problemLength*0.20)))
        {
            cout << "20%" << endl;
            percents[1]=false;
        }
        else if ((percents[0])&&(relativeLength>=ceil(problemLength*0.10)))
        {
            cout << "10%" << endl;
            percents[0]=false;
        }
        // End of fancy output.


        /*
        *   Start of accuracy loop.
        *   Now the tester will create and run new istances, accuracy-times.
        */
        TDiagonalize=0;
        TWalker=0;
        for (j=0;j<accuracy;j++)
        {
            // Filling V and W with increasing random numbers
            V=new long long[relativeLength];
            W=new long long[relativeLength];
            srand(time(NULL));
            V[0]=rand() % dispersion;
            W[0]=rand() % dispersion;
            for (y=1;y<relativeLength;y++)
            {
                V[y]=V[y-1]+(rand() % dispersion)+1;
            }
            for (y=1;y<relativeLength;y++)
            {
                W[y]=W[y-1]+(rand() % dispersion)+1;
            }
            k=rand() % (V[relativeLength-1]+W[relativeLength-1]);
            //Starting Diagonalize
            kv=0;
            kw=0;
            diagonalize=new Diagonalize(relativeLength-1,k,&kv,&kw,V,W);
            QueryPerformanceCounter(&start_ticks); // Timer Start
            check=diagonalize->run(0,relativeLength-1,0,relativeLength-1);
            QueryPerformanceCounter(&end_ticks); // Timer Stop
            cputime.QuadPart=end_ticks.QuadPart-start_ticks.QuadPart;

            //Store time.
            TDiagonalize+=((double)cputime.QuadPart/(double)ticksPerSecond.QuadPart);

            if ((check)&&(!(V[kv]+W[kw]==k)))
            {
                cout << "Result error in Diagonalize";
                return -1;
            }


            //Starting Walker
            kv=0;
            kw=0;
            check=walker=new Walker(relativeLength-1,k,&kv,&kw,V,W);
            QueryPerformanceCounter(&start_ticks); // Timer Start
            check=walker->run();
            QueryPerformanceCounter(&end_ticks); // Timer Stop
            cputime.QuadPart=end_ticks.QuadPart-start_ticks.QuadPart;


            //Store time.
            TWalker+=((double)cputime.QuadPart/(double)ticksPerSecond.QuadPart);

            if ((check)&&(!(V[kv]+W[kw]==k)))
            {
                cout << "Result error in Walker";
                system("pause");
                return -1;
            }

            // Free memory.
            delete [] V;
            delete [] W;
            delete diagonalize;
            delete walker;
        }
        //Storing time in TimeArrays.
        DiagonalizeTimes[i]=TDiagonalize;
        WalkerTimes[i]=TWalker;
    }

    cout << "Writing results to file..." << endl << endl;

    /*
    *   times.m is a Matlab files with the declaration
    *   of D(iagonalize) and W(alker) arrays of times.
    */
    pFile = fopen("times.m","w");
    fprintf(pFile,"D=[");
    for(i=0;i<points-1;i++)
    {
        fprintf(pFile, "%.21e,",DiagonalizeTimes[i]);
    }
    fprintf(pFile,"%.21e];\n",DiagonalizeTimes[points-1]);
    fprintf(pFile,"W=[");
    for(i=0;i<points-1;i++)
    {
        fprintf(pFile, "%.21e,",WalkerTimes[i]);
    }
    fprintf(pFile,"%.21e];\n",WalkerTimes[points-1]);
    fprintf(pFile,"plot(D,'b.');\nhold on;\nplot(W,'r.');\nh=legend('Diagonalize','Walker',2);\n");
    fclose(pFile);

    cout << "Done!" << endl;
    return 0;
}
