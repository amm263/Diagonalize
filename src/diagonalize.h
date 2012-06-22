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

#ifndef DIAGONALIZE_H
#define DIAGONALIZE_H


class Diagonalize
{
    public:
        Diagonalize(long long length, long long k, long long *kv, long long *kw, long long *V, long long *W);
        bool run(long long startv, long long endv, long long startw, long long endw);
        void search(long long startv, long long endv, long long startw, long long endw);
        ~Diagonalize();
    protected:
    private:
        long long length;
        long long k;
        long long *kv;
        long long *kw;
        long long sv;
        long long sw;
        long long *V;
        long long *W;
};

#endif // DIAGONALIZE_H
