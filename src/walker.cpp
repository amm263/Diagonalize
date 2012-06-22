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

#include "walker.h"

Walker::Walker(long long length, long long k, long long *kv, long long *kw, long long *V, long long *W)
{
    Walker::k=k;
    Walker::length=length;
    Walker::kv=kv;
    Walker::kw=kw;
    Walker::V=V;
    Walker::W=W;
}

bool Walker::run()
{
    long long startv=0;
    long long endv=Walker::length;
    long long startw=0;
    long long endw=Walker::length;
    do{
        if ((startv<=endv)&&(startw<=endw))
        {
            if ((Walker::V[startv]+Walker::W[endw])==Walker::k) // If the considered cell is = k, save the indexes and return true.
            {
                *Walker::kv=startv;
                *Walker::kw=endw;
                return true;
            }
            else if ((Walker::V[startv]+Walker::W[endw])>Walker::k) // If the cell is higher than k, move up.
            {
                endw--;
            }
            else if ((Walker::V[startv]+Walker::W[endw])<Walker::k) // If the cell is lower than k, move right.
            {
                startv++;
            }
        }
        else
        {
            return false; // If went out of boundaries of the matrix, k do not exist.
        }
    }while(1);
    return false;
}

Walker::~Walker()
{
    //dtor
}
