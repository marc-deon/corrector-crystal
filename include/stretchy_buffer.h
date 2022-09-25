/////////////////////////
// API
//
// declaration:
//  TYPE* myBuffer = NULL;
// Nulling is very important! sb_add() expects a 0 on the first run!
//
//         sb_free(TYPE *a)           free the array
//         sb_count(TYPE *a)          the number of elements in the array
//         sb_push(TYPE *a, TYPE v)   adds v on the end of the array, a la push_back
//         sb_add(TYPE *a, int n)     adds n uninitialized elements at end of array & returns pointer to first added
//         sb_last(TYPE *a)           returns an lvalue of the last item in the array
//         a[n]                       access the nth (counting from 0) element of the array
//         sb_pop(a)                  remove the last element from the array; does not free it


#ifndef STB_STRETCHY_BUFFER_H_INCLUDED
#define STB_STRETCHY_BUFFER_H_INCLUDED



#ifndef NO_STRETCHY_BUFFER_SHORT_NAMES
#define sb_free   stb_sb_free
#define sb_push   stb_sb_push
#define sb_count  stb_sb_count
#define sb_add    stb_sb_add
#define sb_last   stb_sb_last
#define sb_from_back stb_sb_from_back
#define sb_pop    stb_sb_pop
#endif

#define stb_sb_free(a)         ((a) ? free(stb__sbraw(a)),0 : 0)
#define stb_sb_push(a,v)       (stb__sbmaybegrow(a,1), (a)[stb__sbn(a)++] = (v))
#define stb_sb_count(a)        ((a) ? stb__sbn(a) : 0)
#define stb_sb_add(a,n)        (stb__sbmaybegrow(a,n), stb__sbn(a)+=(n), &(a)[stb__sbn(a)-(n)])
#define stb_sb_last(a)         ((a)[stb__sbn(a)-1])
// Note that this does NOT free whatever it pops; that must be done manually
#define stb_sb_pop(a)          ((a)[stb__sbn(a)-- -1])

// Access the given array in reverse.
#define stb_sb_from_back(a, i)   (a[sb_count(a)-1-i])


#define stb__sbraw(a) ((int *) (void *) (a) - 2)
#define stb__sbm(a)   stb__sbraw(a)[0]
#define stb__sbn(a)   stb__sbraw(a)[1]

#define stb__sbneedgrow(a,n)  ((a)==0 || stb__sbn(a)+(n) >= stb__sbm(a))
#define stb__sbmaybegrow(a,n) (stb__sbneedgrow(a,(n)) ? stb__sbgrow(a,n) : 0)
#define stb__sbgrow(a,n)      (*((void **)&(a)) = stb__sbgrowf((a), (n), sizeof(*(a))))



#include <stdlib.h>

static void * stb__sbgrowf(void *arr, int increment, int itemsize)
{
   int dbl_cur = arr ? 2*stb__sbm(arr) : 0;
   int min_needed = stb_sb_count(arr) + increment;
   int m = dbl_cur > min_needed ? dbl_cur : min_needed;
   int *p = (int *) realloc(arr ? stb__sbraw(arr) : 0, itemsize * m + sizeof(int)*2);
   if (p) {
      if (!arr)
         p[1] = 0;
      p[0] = m;
      return p+2;
   } else {
      #ifdef STRETCHY_BUFFER_OUT_OF_MEMORY
      STRETCHY_BUFFER_OUT_OF_MEMORY ;
      #endif
      return (void *) (2*sizeof(int)); // try to force a NULL pointer exception later
   }
}
#endif // STB_STRETCHY_BUFFER_H_INCLUDED


// How it works:
//
//   A long-standing tradition in things like malloc implementations
//   is to store extra data before the beginning of the block returned
//   to the user. The stretchy buffer implementation here uses the
//   same trick; the current-count and current-allocation-size are
//   stored before the beginning of the array returned to the user.
//   (This means you can't directly free() the pointer, because the
//   allocated pointer is different from the type-safe pointer provided
//   to the user.)
//
//   The details are trivial and implementation is straightforward;
//   the main trick is in realizing in the first place that it's
//   possible to do this in a generic, type-safe way in C.
//
// Original source retrieved 2020-7 from, modified thereafter
// https://github.com/nothings/stb/blob/master/stretchy_buffer.h
