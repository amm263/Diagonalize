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

#include "diagonalize.h"

Diagonalize::Diagonalize(long long length, long long k, long long *kv, long long *kw, long long *V, long long *W)
{
    Diagonalize::k=k;
    Diagonalize::length=length;
    Diagonalize::kv=kv;
    Diagonalize::kw=kw;
    Diagonalize::sv=0;
    Diagonalize::sw=0;
    Diagonalize::V=V;
    Diagonalize::W=W;

}


bool Diagonalize::run(long long startv, long long endv, long long startw, long long endw)
{
    long long v=endv-startv;
    long long w=endw-startw;
    long long stopv,stopw; // i need stopv and stopw to locally save the results of search()
    long long temp; // temporary variable, used to split rectangles and retrieve squares to work with
    if ((v==0)&&(w==0)) // First case: Start index and End index of both vectors are equals. I'm analyzing a single cell.
    {
        if ((Diagonalize::V[startv]+Diagonalize::W[startw])==Diagonalize::k)
        {
            // k found in the single cell.
            *Diagonalize::kv=startv;
            *Diagonalize::kw=startw;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (v==w) // Second case: Square matrix.
    {
        Diagonalize::search(startv,endv,startw,endw); // Run binary search on diagonal.
        stopv=Diagonalize::sv; // Save Stops on vectors V and W.
        stopw=Diagonalize::sw;
        if ((Diagonalize::V[stopv]+Diagonalize::W[stopw])==Diagonalize::k) // Second case A: If the value found is equal to k...
        {
            // ... I just stop and save k indexes.
            // In this case k is found directly on the diagonal, by the binary search();
            *Diagonalize::kv=stopv;
            *Diagonalize::kw=stopw;
            return true;
        }
        else if ((Diagonalize::V[stopv]+Diagonalize::W[stopw])>Diagonalize::k) // Second case B: The value found on diagonal is higher than k.
        {
            if ((stopv<=endv)&&(startw<=stopw-1)) // Check to not exit from both vectors (array outbound).
            {
                if (!Diagonalize::run(stopv,endv,startw,stopw-1)) // If diagonalize() in the top matrix return false, i have to check the bottom one.
                {
                    if ((startv<=stopv-1)&&(stopw<=endw)) // Check to not exit from both vectors (array outbound).
                        return Diagonalize::run(startv,stopv-1,stopw,endw);
                }
                else
                {
                    return true; // Else i found k and return true.
                }
            }
        }
        else if ((Diagonalize::V[stopv]+Diagonalize::W[stopw])<Diagonalize::k) // Second case C: The value found on diagonal is lower than k.
        {
            if ((stopv+1<=endv)&&(startw<=stopw))
            {
                if (!Diagonalize::run(stopv+1,endv,startw,stopw))
                {                                                          // As above, but considering (a bit)different matrices.
                    if((startv<=stopv)&&(stopw+1<=endw))
                        return Diagonalize::run(startv,stopv,stopw+1,endw);
                }
                else
                {
                    return true;
                }
            }
        }
    }
    else if (v>w)       // Third case:  vector V is longer than W.
    {
        temp=startv+w; // Temp is to create a square with the side equal to the length of the shorter vector.
        if ((startv<=temp)&&(startw<=endw)) // Check to not exit from both vectors (array outbound)
        {
            if (!Diagonalize::run(startv,temp,startw,endw))
            {
                if ((temp+1<=endv)&&(startw<=endw)) // Check to not exit from both vectors (array outbound). Maybe this is reduntant, but i'm paranoid.
                    return Diagonalize::run(temp+1,endv,startw,endw);
            }
            else
            {
                return true;
            }
        }
    }
    else if (v<w)   // Fourth case: vector V is shorter than W. As above, but "reversed".
    {
        temp=startw+v;
        if ((startv<=endv)&&(startw<=temp))
        {
            if (!Diagonalize::run(startv,endv,startw,temp))
            {
                if ((startv<=endv)&&(temp+1<=endw))
                    return Diagonalize::run(startv,endv,temp+1,endw);
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}


/*
*   Binary search.
*/
void Diagonalize::search(long long startv, long long endv, long long startw, long long endw)
{
    long long mv;
    long long mw;
    while(startv<=endv) {
        mv = (startv+endv)/2;
        mw = (startw+endw)/2;
        // If k is found, save k indexes and return.
        if((Diagonalize::V[mv]+Diagonalize::W[mw])==Diagonalize::k)
        {
            Diagonalize::sw=mw;
            Diagonalize::sv=mv;
            return;
        }
        // Else synchronously move indexes to the top bound...
        if((Diagonalize::V[mv]+Diagonalize::W[mw])<Diagonalize::k)
        {
            startv = mv+1;
            startw = mw+1;
        }
        // ...or to the bottom bound of hypotetical vector V[mv]+W[mw].
        else
        {
            endv = mv-1;
            endw = mw-1;
        }
    }

    // Se vado troppo oltre mi fermo e prendo nota degli indici. Significa che non ho trovato k, ma che il primo numero dell'ipotetico vettore diagonale è maggiore di k, oppure l'ultimo è minore.
    Diagonalize::sw=mw;
    Diagonalize::sv=mv;
    return;
}

Diagonalize::~Diagonalize()
{
    //dtor
}
