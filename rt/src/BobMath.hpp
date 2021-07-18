#pragma once
#include "defs.hpp"
namespace bMath {

#ifdef HUGE_NUM
#undef HUGE_NUM
#endif

#define HUGE_NUM (1.0e8)

/*
    generic helpful macros
*/
#ifndef PI
#define PI (3.14159265358979323844)
#endif /* PI */

//#define degtorad(x) (((double)(x)) * PI / 180.0)
template <typename T>
inline double degtorad(T x) { return (((double)(x)) * PI / 180.0); }

//#define abs(a) (((a) < 0) ? (-(a)) : (a))
template <typename T>
inline T abs(T a) { return (((a) < 0) ? (-(a)) : (a)); }

//#define FLOOR(a) ((a) > 0 ? (int)(a) : -(int)(a))
//template <typename T>
//inline int FLOOR(T a) { return ((a) > 0 ? (int)(a) : -(int)(a)); }

//#define CEILING(a)    ((a)==(int)(a)?(a):(a)>0?1+(int)(a):-(1+(int)(-a))
//#define ROUND(a) ((a) > 0 ? (int)((a) + 0.5) : -(int)(0.5 - a))

//#define min(a, b) (((a) < (b)) ? (a) : (b))
//#define max(a, b) (((a) > (b)) ? (a) : (b))

template <typename T>
inline T min(T a, T b) { return (((a) < (b)) ? (a) : (b)); }

template <typename T>
inline T max(T a, T b) { return (((a) > (b)) ? (a) : (b)); }

} /* end namespace bMath */