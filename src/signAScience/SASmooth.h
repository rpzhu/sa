#ifndef SASMOOTH_H
#define SASMOOTH_H
#include "SAScienceGlobal.h"
namespace SA {
/**
 * @brief The SASmooth class
 */
class SASCIENCE_API SASmooth
{
public:
    SASmooth();
public:
    //3点线性平滑
    static void linearSmooth3(const double *in, double* out, int N );
    template<typename IT_C,typename IT>
    static void linearSmooth3 (INPUT IT_C in_begin,INPUT IT_C in_end,OUTPUT IT out_begin);
    //5点线性平滑
    static void linearSmooth5 (const double *in, double *out, int N );
    template<typename IT_C,typename IT>
    static void linearSmooth5 (INPUT IT_C in_begin,INPUT IT_C in_end,OUTPUT IT out_begin);
    //7点线性平滑
    static void linearSmooth7 (const double* in, double* out, int N );
    template<typename IT_C,typename IT>
    static void linearSmooth7 (INPUT IT_C in_begin,INPUT IT_C in_end,OUTPUT IT out_begin);
    //5点2次平滑
    static void quadraticSmooth5(const double* in, double* out, int N);
    template<typename IT_C,typename IT>
    static void quadraticSmooth5(INPUT IT_C in_begin,INPUT IT_C in_end,OUTPUT IT out_begin);
    //7点2次平滑
    static void quadraticSmooth7(const double* in, double* out, int N);
    template<typename IT_C,typename IT>
    static void quadraticSmooth7(INPUT IT_C in_begin,INPUT IT_C in_end,OUTPUT IT out_begin);
    //5点3次平滑
    static void cubicSmooth5 (const double* in, double* out, int N );
    template<typename IT_C,typename IT>
    static void cubicSmooth5 (INPUT IT_C in_begin,INPUT IT_C in_end,OUTPUT IT out_begin);
    //7点3次平滑
    static void cubicSmooth7(const double* in, double* out, int N);
    template<typename IT_C,typename IT>
    static void cubicSmooth7 (INPUT IT_C in_begin,INPUT IT_C in_end,OUTPUT IT out_begin);
};





template<typename IT_C, typename IT>
void SASmooth::quadraticSmooth7(IT_C in_begin, IT_C in_end, IT out_begin)
{
    size_t N = std::distance(in_begin,in_end);
    size_t i=0;
    IT out = out_begin;
    IT_C in = in_begin;
    if ( N < 7 )
    {
        for ( i = 0; i <= N - 1; i++ )
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = ( 32.0 * in[0] + 15.0 * in[1] + 3.0 * in[2] - 4.0 * in[3] -
                  6.0 * in[4] - 3.0 * in[5] + 5.0 * in[6] ) / 42.0;

        out[1] = ( 5.0 * in[0] + 4.0 * in[1] + 3.0 * in[2] + 2.0 * in[3] +
                  in[4] - in[6] ) / 14.0;

        out[2] = ( 1.0 * in[0] + 3.0 * in [1] + 4.0 * in[2] + 4.0 * in[3] +
                  3.0 * in[4] + 1.0 * in[5] - 2.0 * in[6] ) / 14.0;
        for ( i = 3; i <= N - 4; i++ )
        {
            out[i] = ( -2.0 * (in[i - 3] + in[i + 3]) +
                       3.0 * (in[i - 2] + in[i + 2]) +
                      6.0 * (in[i - 1] + in[i + 1]) + 7.0 * in[i] ) / 21.0;
        }
        out[N - 3] = ( 1.0 * in[N - 1] + 3.0 * in [N - 2] + 4.0 * in[N - 3] +
                      4.0 * in[N - 4] + 3.0 * in[N - 5] + 1.0 * in[N - 6] - 2.0 * in[N - 7] ) / 14.0;

        out[N - 2] = ( 5.0 * in[N - 1] + 4.0 * in[N - 2] + 3.0 * in[N - 3] +
                      2.0 * in[N - 4] + in[N - 5] - in[N - 7] ) / 14.0;

        out[N - 1] = ( 32.0 * in[N - 1] + 15.0 * in[N - 2] + 3.0 * in[N - 3] -
                      4.0 * in[N - 4] - 6.0 * in[N - 5] - 3.0 * in[N - 6] + 5.0 * in[N - 7] ) / 42.0;
    }
}

template<typename IT_C, typename IT>
void SASmooth::linearSmooth3(IT_C in_begin, IT_C in_end, IT out_begin)
{
    size_t N = std::distance(in_begin,in_end);
    IT out = out_begin;
    IT_C in = in_begin;
    if ( N < 3 )
    {
        for(;in != in_end;++in)
        {
            *out = *in;
        }
    }
    else
    {
        *out = ( 5.0 * (*in) + 2.0 * (*(in+1)) - (*(in+2)) ) / 6.0;

        for (size_t i = 1; i <= N - 2; i++ )
        {
            (*(out+i)) = ( (*(in+i-1)) + (*(in+i)) + (*(in+i+1)) ) / 3.0;
        }
        (*(out+N-1)) = ( 5.0 * (*(in+N-1)) + 2.0 * (*(in+N-2)) - (*(in+N-3)) ) / 6.0;
    }
}

template<typename IT_C, typename IT>
void SASmooth::linearSmooth5(IT_C in_begin, IT_C in_end, IT out_begin)
{
    size_t N = std::distance(in_begin,in_end);
    size_t i=0;
    IT out = out_begin;
    IT_C in = in_begin;
    if ( N < 5 )
    {
        for(;in != in_end;++in)
        {
            *out = *in;
        }
    }
    else
    {
        *out = ( 3.0 * (*in) + 2.0 * (*(in+1)) + (*(in+2)) - (*(in+4)) ) / 5.0;
        *(out+1) = ( 4.0 * (*in) + 3.0 * (*(in+1)) + 2 * (*(in+2)) + (*(in+3)) ) / 10.0;
        for ( i = 2; i <= N - 3; i++ )
        {
            out[i] = ( in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] ) / 5.0;
        }
        out[N - 2] = ( 4.0 * in[N - 1] + 3.0 * in[N - 2] + 2 * in[N - 3] + in[N - 4] ) / 10.0;
        out[N - 1] = ( 3.0 * in[N - 1] + 2.0 * in[N - 2] + in[N - 3] - in[N - 5] ) / 5.0;
    }
}

template<typename IT_C, typename IT>
void SASmooth::linearSmooth7(IT_C in_begin, IT_C in_end, IT out_begin)
{
    size_t N = std::distance(in_begin,in_end);
    size_t i=0;
    IT out = out_begin;
    IT_C in = in_begin;
    if ( N < 7 )
    {
        for ( i = 0; i <= N - 1; i++ )
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = ( 13.0 * in[0] + 10.0 * in[1] + 7.0 * in[2] + 4.0 * in[3] +
                  in[4] - 2.0 * in[5] - 5.0 * in[6] ) / 28.0;

        out[1] = ( 5.0 * in[0] + 4.0 * in[1] + 3 * in[2] + 2 * in[3] +
                  in[4] - in[6] ) / 14.0;

        out[2] = ( 7.0 * in[0] + 6.0 * in [1] + 5.0 * in[2] + 4.0 * in[3] +
                  3.0 * in[4] + 2.0 * in[5] + in[6] ) / 28.0;

        for ( i = 3; i <= N - 4; i++ )
        {
            out[i] = ( in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3] ) / 7.0;
        }

        out[N - 3] = ( 7.0 * in[N - 1] + 6.0 * in [N - 2] + 5.0 * in[N - 3] +
                      4.0 * in[N - 4] + 3.0 * in[N - 5] + 2.0 * in[N - 6] + in[N - 7] ) / 28.0;

        out[N - 2] = ( 5.0 * in[N - 1] + 4.0 * in[N - 2] + 3.0 * in[N - 3] +
                      2.0 * in[N - 4] + in[N - 5] - in[N - 7] ) / 14.0;

        out[N - 1] = ( 13.0 * in[N - 1] + 10.0 * in[N - 2] + 7.0 * in[N - 3] +
                      4 * in[N - 4] + in[N - 5] - 2 * in[N - 6] - 5 * in[N - 7] ) / 28.0;
    }
}


template<typename IT_C, typename IT>
void SASmooth::quadraticSmooth5(IT_C in_begin, IT_C in_end, IT out_begin)
{
    size_t N = std::distance(in_begin,in_end);
    size_t i=0;
    IT out = out_begin;
    IT_C in = in_begin;
    if ( N < 5 )
    {
        for ( i = 0; i <= N - 1; i++ )
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = ( 31.0 * in[0] + 9.0 * in[1] - 3.0 * in[2] - 5.0 * in[3] + 3.0 * in[4] ) / 35.0;
        out[1] = ( 9.0 * in[0] + 13.0 * in[1] + 12 * in[2] + 6.0 * in[3] - 5.0 *in[4]) / 35.0;
        for ( i = 2; i <= N - 3; i++ )
        {
            out[i] = ( - 3.0 * (in[i - 2] + in[i + 2]) +
                      12.0 * (in[i - 1] + in[i + 1]) + 17 * in[i] ) / 35.0;
        }
        out[N - 2] = ( 9.0 * in[N - 1] + 13.0 * in[N - 2] + 12.0 * in[N - 3] + 6.0 * in[N - 4] - 5.0 * in[N - 5] ) / 35.0;
        out[N - 1] = ( 31.0 * in[N - 1] + 9.0 * in[N - 2] - 3.0 * in[N - 3] - 5.0 * in[N - 4] + 3.0 * in[N - 5]) / 35.0;
    }
}


template<typename IT_C, typename IT>
void SASmooth::cubicSmooth5(IT_C in_begin, IT_C in_end, IT out_begin)
{
    size_t N = std::distance(in_begin,in_end);
    size_t i=0;
    IT out = out_begin;
    IT_C in = in_begin;
    if ( N < 5 )
    {
        for ( i = 0; i <= N - 1; i++ )
            out[i] = in[i];
    }

    else
    {
        out[0] = (69.0 * in[0] + 4.0 * in[1] - 6.0 * in[2] + 4.0 * in[3] - in[4]) / 70.0;
        out[1] = (2.0 * in[0] + 27.0 * in[1] + 12.0 * in[2] - 8.0 * in[3] + 2.0 * in[4]) / 35.0;
        for ( i = 2; i <= N - 3; i++ )
        {
            out[i] = (-3.0 * (in[i - 2] + in[i + 2])+ 12.0 * (in[i - 1] + in[i + 1]) + 17.0 * in[i] ) / 35.0;
        }
        out[N - 2] = (2.0 * in[N - 5] - 8.0 * in[N - 4] + 12.0 * in[N - 3] + 27.0 * in[N - 2] + 2.0 * in[N - 1]) / 35.0;
        out[N - 1] = (- in[N - 5] + 4.0 * in[N - 4] - 6.0 * in[N - 3] + 4.0 * in[N - 2] + 69.0 * in[N - 1]) / 70.0;
    }
}

template<typename IT_C, typename IT>
void SASmooth::cubicSmooth7(IT_C in_begin, IT_C in_end, IT out_begin)
{
    size_t N = std::distance(in_begin,in_end);
    size_t i=0;
    IT out = out_begin;
    IT_C in = in_begin;
    if ( N < 7 )
    {
        for ( i = 0; i <= N - 1; i++ )
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = ( 39.0 * in[0] + 8.0 * in[1] - 4.0 * in[2] - 4.0 * in[3] +
                  1.0 * in[4] + 4.0 * in[5] - 2.0 * in[6] ) / 42.0;
        out[1] = ( 8.0 * in[0] + 19.0 * in[1] + 16.0 * in[2] + 6.0 * in[3] -
                  4.0 * in[4] - 7.0* in[5] + 4.0 * in[6] ) / 42.0;
        out[2] = ( -4.0 * in[0] + 16.0 * in [1] + 19.0 * in[2] + 12.0 * in[3] +
                  2.0 * in[4] - 4.0 * in[5] + 1.0 * in[6] ) / 42.0;
        for ( i = 3; i <= N - 4; i++ )
        {
            out[i] = ( -2.0 * (in[i - 3] + in[i + 3]) +
                       3.0 * (in[i - 2] + in[i + 2]) +
                      6.0 * (in[i - 1] + in[i + 1]) + 7.0 * in[i] ) / 21.0;
        }
        out[N - 3] = ( -4.0 * in[N - 1] + 16.0 * in [N - 2] + 19.0 * in[N - 3] +
                      12.0 * in[N - 4] + 2.0 * in[N - 5] - 4.0 * in[N - 6] + 1.0 * in[N - 7] ) / 42.0;
        out[N - 2] = ( 8.0 * in[N - 1] + 19.0 * in[N - 2] + 16.0 * in[N - 3] +
                      6.0 * in[N - 4] - 4.0 * in[N - 5] - 7.0 * in[N - 6] + 4.0 * in[N - 7] ) / 42.0;
        out[N - 1] = ( 39.0 * in[N - 1] + 8.0 * in[N - 2] - 4.0 * in[N - 3] -
                      4.0 * in[N - 4] + 1.0 * in[N - 5] + 4.0 * in[N - 6] - 2.0 * in[N - 7] ) / 42.0;
    }
}

} // namespace SA


#endif // SASMOOTH_H
