#pragma once
#include "defs.hpp"
#include "Object_3D.hpp"

class Bound_3D {
  private:
    static long total; /* # objects in main list */
    static Flt Median; /* 2*median value along axis */
    static int Axis;   /* axis to split along */
    Bound_3D() {}
    ~Bound_3D() {}

    static void FindAxis(Object *top, long count);
    static int SortAndSplit(Object **top_handle, long count);
  public:
    static long nPrims;
    static void BuildBoundingSlabs(void);
};