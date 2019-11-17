/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
 */

#include "grp_freelists.h"

#include <string.h>
#include <errno.h>
#include <iostream>

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

namespace sofs19
{
    void grpReplenishHeadCache(void)
    {
        soProbe(443, "%s()\n", __FUNCTION__);

        /* change the following line by your code */
        /* there are test scrips in /so1920-p5g2/sofs19/test */

        SOSuperBlock * super = soGetSuperBlockPointer();

        /* Does nothing if the cache isn't empty */
        if ( super -> head_cache.idx != HEAD_CACHE_SIZE )
            { return; }
        /* If there are no more free data block throw exception */
        if ( super -> dz_free == 0 ) 
            { return; }
        /* There are no Blocks with References to free Data Blocks, */
        /* Then it will go get references from the tail cache */
        if ( super -> head_blk == NullReference )
        {
            /* There are more references from tail cache than head cache size */
            if ( super -> tail_cache.idx > HEAD_CACHE_SIZE )
            { 
                //&((sb->ircache).ref[destStart]
                memcpy( super -> head_cache.ref , super -> tail_cache.ref , \
                        HEAD_CACHE_SIZE * sizeof( uint32_t ) );
                memcpy( super -> tail_cache.ref , &( ( super -> tail_cache ).ref[ HEAD_CACHE_SIZE ] ), \
                        ( super -> tail_cache.idx - HEAD_CACHE_SIZE ) * sizeof( uint32_t ) );
                memset( &( ( super -> tail_cache ).ref[ super -> tail_cache.idx - HEAD_CACHE_SIZE ] ), NullReference, \
                        HEAD_CACHE_SIZE * sizeof( uint32_t ) );
                /* Updating super block variables */
                super -> head_cache.idx = 0;
                super -> tail_cache.idx -= HEAD_CACHE_SIZE;
            }
            /* There are fewer references from tail cache than head cache size */
            else 
            {
                memcpy( &( ( super -> head_cache ).ref[ HEAD_CACHE_SIZE - super -> tail_cache.idx ] ), \
                        super -> tail_cache.ref, super -> tail_cache.idx * sizeof( uint32_t ) );
                memset( super -> tail_cache.ref, NullReference, \
                        super -> tail_cache.idx * sizeof( uint32_t ) );
                /* Updating super block variables */
                super -> head_cache.idx = HEAD_CACHE_SIZE - super -> tail_cache.idx;
                super -> tail_cache.idx = 0;
            }
        }
        /* There are Blocks with References to free Data Blocks */
        else 
        {
            uint32_t limit_copys = RPB - super -> head_idx;
            uint32_t old_head = super -> head_blk;
            uint32_t ref_head_blk[RPB];
            soReadDataBlock( super -> head_blk, ref_head_blk );
            if ( ref_head_blk[0] == NullReference )
                { limit_copys = super -> tail_idx - super -> head_idx; }
            
            if ( limit_copys > HEAD_CACHE_SIZE ) 
            { 
                limit_copys = HEAD_CACHE_SIZE;
                memcpy( super -> head_cache.ref, &( ref_head_blk[ super -> head_idx ] ), \
                            limit_copys * sizeof( uint32_t ) ); 
                memset( &( ref_head_blk[ super -> head_idx ] ), NullReference, \
                        limit_copys * sizeof( uint32_t ) );
                /* Updating super block variables */
                super -> head_cache.idx = 0;
                super -> head_idx += HEAD_CACHE_SIZE;
            }
            else
            { 
                memcpy( &( ( super -> head_cache ).ref[ HEAD_CACHE_SIZE - limit_copys ] ), \
                        &( ref_head_blk[ super -> head_idx ] ), limit_copys * sizeof( uint32_t ) ); 
                memset( &( ref_head_blk[ super -> head_idx ] ), NullReference, \
                        limit_copys * sizeof( uint32_t ) );
                /* Updating super block variables */
                super -> head_cache.idx = HEAD_CACHE_SIZE - limit_copys;
                /* There are no more Blocks with References to Free Data Blocks */
                if ( ref_head_blk[0] == NullReference )
                {
                    /* There are no more references to free data blocks in blocks with references */
                    super -> head_blk = NullReference; 
                    super -> head_idx = NullReference; 
                    super -> tail_blk = NullReference; 
                    super -> tail_idx = NullReference;
                }
                else
                {
                    super -> head_blk = ref_head_blk[0];
                    super -> head_idx = 1;
                    ref_head_blk[0] = NullReference;
                }
                binFreeDataBlock( old_head );
            }
            soWriteDataBlock( old_head, ref_head_blk );
        }
        soSaveSuperBlock ( );
    }
};

