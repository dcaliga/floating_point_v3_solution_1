/* $Id: ex16.mc,v 2.1 2005/06/14 22:16:51 jls Exp $ */

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

void subr (double  d_arr[], double  d_sum[], int msize, int64_t *tm, int mapnum) {

    int64_t t0, t1;

    Stream_64 SA;
    Stream_64 Sval, Scnt, Ssum;

    read_timer (&t0);

#pragma src parallel sections
{
#pragma src section
{
    streamed_dma_cpu_64 (&SA, PORT_TO_STREAM, d_arr, msize*msize*sizeof(int64_t));
}
#pragma src section
{
    int i,row,col;
    double  val;

    for (i=0;i<msize*msize; i++)  {
        cg_count_ceil_32 (1, 0, i==0, msize-1, &col);
        cg_accum_add_32  (1, col==0, 0, i==0, &row);

        get_stream_dbl_64 (&SA, &val);

        put_stream_64     (&Scnt, msize, col==0);
        put_stream_dbl_64 (&Sval, val, 1);
   }

   stream_64_term (&Scnt);
   stream_64_term (&Sval);
}
#pragma src section
{
     stream_fp_accum_strm_counts_64_term (&Sval, &Scnt, &Ssum);
}
#pragma src section
{
    streamed_dma_cpu_64 (&Ssum, STREAM_TO_PORT, d_sum, msize*sizeof(int64_t));
}
}

    read_timer (&t1);
    *tm = t1 - t0;

    }
