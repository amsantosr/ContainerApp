#ifndef TESTCONT_H
#define TESTCONT_H

extern "C" {

typedef short         boolean; /* logical variable       */
typedef short         ntype;   /* number of states,bins  */
typedef short         itype;   /* can hold up to W,H,D   */
typedef long          stype;   /* can hold up to W*H*D   */
typedef double        ptype;   /* product type           */

/* item record */
typedef struct irec {
  short    no;    /* item number */
  itype    dx;    /* box x-size */
  itype    dy;    /* box y-size */

  itype    dz;    /* box z-size */
  itype    x;     /* optimal x-position */
  itype    y;     /* optimal y-position */
  itype    z;     /* optimal z-position */
  boolean  k;     /* knapsack solution */
} item;

void maketest(item *f, item **l, int *mx, int *my, int *mz,
              int mindim, int maxdim, int fillpct, int maxtyp);

}

//void prepareitems(item *f, item *l, int *w, int *h, int *d);

#endif // TESTCONT_H

