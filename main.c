static char const cvsid[] = "$Id: main.c,v 2.1 2005/06/14 22:16:51 jls Exp $";

/*
 * Copyright 2005 SRC Computers, Inc.  All Rights Reserved.
 *
 *	Manufactured in the United States of America.
 *
 * SRC Computers, Inc.
 * 4240 N Nevada Avenue
 * Colorado Springs, CO 80907
 * (v) (719) 262-0213
 * (f) (719) 262-0223
 *
 * No permission has been granted to distribute this software
 * without the express permission of SRC Computers, Inc.
 *
 * This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
 */

#include <libmap.h>
#include <stdlib.h>


void subr (double d_arr[], double *res, int m, int64_t *tm, int mapmsize);


int main (int argc, char *argv[]) {
    FILE *res_map, *res_cpu;
    int i,j,ix, msize;
    double *D_src, *D_sum, res, acc;
    int64_t tm;
    int mapnum = 0;

    if ((res_map = fopen ("res_map", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_map'\n");
        exit (1);
        }

    if ((res_cpu = fopen ("res_cpu", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_cpu'\n");
        exit (1);
        }

    if (argc < 2) {
	fprintf (stderr, "need msizeber of elements as arg\n");
	exit (1);
	}

    if (sscanf (argv[1], "%d", &msize) < 1) {
	fprintf (stderr, "need msizeber of elements as arg\n");
	exit (1);
	}

    if ((msize < 1) || (msize > 724)) {
        fprintf (stderr, "number of elements must be in the range 1 through %d\n", 724);
	exit (1);
	}

    D_src = (double*) malloc (msize*msize * sizeof (double));
    D_sum = (double*) malloc (msize * sizeof (double));

    srandom (99);


    for (j=0; j<msize; j++) {
       acc = 0.0;
       for (i=j; i<msize; i++) {
           ix = i + j*msize;
           D_src[ix] = (double)random() / random();
	       acc += D_src[ix];
	       }
       fprintf (res_cpu, "%lf\n", acc);
    }
     


    map_allocate (1);

    subr (D_src, D_sum, msize, &tm, mapnum);

    printf ("%lld clocks\n", tm);

    for (i=0; i<msize; i++) fprintf (res_map, "%lf\n", D_sum[i]);

    map_free (1);

    exit(0);
    }

