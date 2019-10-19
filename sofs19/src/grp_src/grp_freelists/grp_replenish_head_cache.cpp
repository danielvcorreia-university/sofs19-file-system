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
        
        /* Done by: Daniel Vala Correia */

        SOSuperBlock * super = soGetSuperBlockPointer();

        /* Does nothing if the cache isn't empty */
        if ( super -> head_cache.idx != HEAD_CACHE_SIZE )
            { return; }

        /* If there are no Reference Data Blocks it will go get references from the tail cache */
        if ( super -> head_blk == NullReference )
        { 
            /* Fill the head cache with the references from tail cache */
            uint32_t limit_cache_copys = super -> tail_cache.idx;

            if ( limit_cache_copys > HEAD_CACHE_SIZE )
                { limit_cache_copys = HEAD_CACHE_SIZE; }

            uint32_t head_idx_write = ( super -> head_cache.idx ) - limit_cache_copys;
            uint32_t tail_idx_read = ( super -> tail_cache.idx ) - limit_cache_copys;
            
            /* Writing the data from the tail to de head of the cache */
            memcpy( &head_idx_write , &tail_idx_read, limit_cache_copys * sizeof( uint32_t ) );
            /* Deleting references copied from tail of the cache */
            memset( &tail_idx_read, NullReference, limit_cache_copys * sizeof( uint32_t ) );
            /* Update head_cache.idx and tail_cache.idx */
            super -> head_cache.idx = head_idx_write;
            super -> tail_cache.idx = tail_idx_read;
        }

        else 
        {
            /* Boolean variable to check if head reference data block becames empty */
            bool freed_head_block = false;
            /* Pysical number of head reference data block */
            uint32_t number_head_block = ( super -> dz_start ) + ( super -> head_blk );
            /* Aware of first reference of block being the reference to next reference data block */
            uint32_t limit_cache_copys = RPB - ( super -> head_idx );

            if ( limit_cache_copys > HEAD_CACHE_SIZE )
                { limit_cache_copys = HEAD_CACHE_SIZE; }

            else { freed_head_block = true; }

            uint32_t head_idx_write = ( super -> head_cache.idx ) - limit_cache_copys;
            uint32_t block_idx_read = ( super -> head_idx );

            /* Writing the data from head reference data block to de head of the cache */
            memcpy( &head_idx_write , &block_idx_read, limit_cache_copys * sizeof( uint32_t ) );
            /* Changing references copied from head reference data block to NullReference */
            memset( &block_idx_read, NullReference, limit_cache_copys * sizeof( uint32_t ) );
            /* Update head_cache.idx and tail_cache.idx */
            super -> head_cache.idx = head_idx_write;
            super -> head_idx = block_idx_read + limit_cache_copys;

            /* Writing head_idx again but it saves code lines */
            if ( freed_head_block )
            {
                /* Updating head reference data block */
                memcpy( &( super -> head_blk ), &number_head_block, sizeof( uint32_t ) );
                /* Changing reference to next reference data block to NullReference ( unnecessary i think because of binFreeDataBlock() ) */
                memset( &number_head_block, NullReference, sizeof( uint32_t ) );
                /* Updating idx */
                super -> head_idx = 1;

                if ( super -> head_blk == NullReference ) 
                    { super -> head_idx = NullReference; }
                
                binFreeDataBlock( number_head_block );
            }

            else { soWriteDataBlock ( number_head_block, &number_head_block); }
        }

        soSaveSuperBlock ();
    }
};

