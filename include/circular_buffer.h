/////////////////////////
// API
//
// declaration:
//  TYPE* myBuffer = NULL;
// Nulling is very important! cb_init() expects a 0 on the first run!
//
//         cb_init(TYPE *a, int n)    creates a circular array with n spots
//         cb_free(TYPE *a)           free the array
//         cb_count(TYPE *a)          the number of elements currently in the array
//         cb_size(TYPE *a)           the max number of elements in the array
//         cb_push(TYPE *a, TYPE v)   adds v on the end of the array, a la push_back
//         cb_pop(TYPE *a)            remove the last (circular) element from the array; does not free it
//         a[n]                       access the nth (linear) element of the array
//         cb_last(TYPE *a)


#ifndef STB_CIRCULAR_BUFFER_H_INCLUDED
#define STB_CIRCULAR_BUFFER_H_INCLUDED



#ifndef NO_CIRCULAR_BUFFER_SHORT_NAMES
#define cb_init   stb_cb_init
#define cb_free   stb_cb_free
#define cb_count  stb_cb_count
#define cb_size   stb_cb_size
#define cb_push   stb_cb_push
#define cb_pop    stb_cb_pop
#define cb_last   stb_cb_last
#define cb_from_back stb_cb_from_back
#endif

#define stb_cb_init(a,m)       (a = stb__cbinitf(a, m, sizeof(*(a))))

#define stb_cb_free(a)         ((a) ? free(stb__cbraw(a)), 0 : 0)
#define stb_cb_count(a)        ((a) ? stb__cbn(a) : 0)
#define stb_cb_size(a)         ((a) ? stb__cbm(a) : 0)
#define stb_cb_push(a,v)       (stb__cbh(a)=v, stb__incho(a))
#define stb_cb_pop(a)          (stb__decho(a))
#define stb_cb_last(a)         (stb_cb_from_back(a, 0))
#define stb_cb_from_back(a,i)  ((a)[(((-i)%stb__cbn(a))+stb__cbn(a)-1)%stb__cbn(a)])

#define stb__inch(a)
#define stb__incho(a) (stb__cbn(a)+=(stb__cbn(a)<stb__cbm(a)),stb__cbho(a)=(stb__cbho(a)+1)%stb__cbm(a))
#define stb__decho(a) (stb__cbn(a)-=(stb__cbn(a)>0)          ,stb__cbho(a)=(stb__cbn(a)==0) ? 0 : (stb__cbho(a)==0 ? (stb__cbm(a)-1) : stb__cbho(a)-1))
#define stb__cbraw(a) ((long int *) (void *) (a) - 3)
#define stb__cbm(a)   (stb__cbraw(a)[0]) // max
#define stb__cbn(a)   (stb__cbraw(a)[1]) // num
#define stb__cbho(a)  (stb__cbraw(a)[2]) // headoffset
#define stb__cbh(a)   ((a)[stb__cbho(a)])// head

#include <stdlib.h>

static void *stb__cbinitf(void *arr, int m, int itemsize) {
   long int *p = (long int *)realloc(arr ? stb__cbraw(arr) : 0, (itemsize * m) + sizeof(long int) * 3);
   if (p) {
      p[0] = m;
      if (!arr)
         p[1] = 0;
      p[2] = 0;
      return p + 3;
   }
   else {
      #ifdef CIRCULAR_BUFFER_OUT_OF_MEMORY
      CIRCULAR_BUFFER_OUT_OF_MEMORY;
      #endif
      return (void *)(3 * sizeof(long int)); // try to force a NULL pointer exception later
   }
}
#endif // STB_CIRCULAR_BUFFER_H_INCLUDED


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
// Based on stb's stretchy buffer, retrieved 2020-7, from
// https://github.com/nothings/stb/blob/master/stretchy_buffer.h
