/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
 */

#include "grp_freelists.h"

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;

/**
 *  Replenish the head cache.

    References to free data blocks should be transferred from the head reference data block or from the tail cache, if no reference data blocks exist, to the head cache:

    Remarks
        nothing is if the cache is not empty;
        the tail cache is only used if there are no reference data blocks;
        only a single block is processed, even if it is not enough to fulfill the head cache;
        the block processed is the one pointed to by the head_blk field of the superblock;
        after transferring a reference from A to B, the value in A becomes NullReference;
        if after the replenish the head reference data block gets empty, the head passes to the next, if one exists, and the previous is freed;
        in the previous situation, all entries of the freed data block sre set to NullReference;
        when calling a function of any layer, the version with prefix so is used.
 */

namespace sofs19
{
    /* only fill the current block to its end */
    void grpDepleteTailCache(void)
    {
        soProbe(444, "%s()\n", __FUNCTION__);

        /* change the following line by your code */
        //binDepleteTailCache();

        SOSuperBlock *sb = soGetSuperBlockPointer();

        uint32_t block = sb->tail_blk / RPB;
        uint32_t used_br = sb->tail_blk % RPB;
        uint32_t free_br;
        uint32_t block_pointer[RPB];
        uint32_t new_block;
        soReadDataBlock(block, &block_pointer);

        if(sb->tail_cache.idx != TAIL_CACHE_SIZE){
            return;
        }


        if(sb->tail_blk == NullReference && sb->head_blk == NullReference){
           new_block = soAllocDataBlock();
           sb->tail_blk = new_block;
           sb->head_blk = new_block;
        }

        if(sb->tail_idx == RPB){
            new_block = soAllocDataBlock();
            sb->tail_blk = new_block;
        }

        if(block == sb->head_blk / RPB){
            if(sb->head_blk % RPB > sb->tail_blk % RPB){
                free_br =  (sb->head_blk % RPB) - (sb->tail_blk % RPB);
            }
            else{
                free_br = RPB - used_br;
            }
        }
        else {
            free_br = RPB - used_br;
        }

        if(sb->tail_idx > free_br){
            memcpy(&(block_pointer[used_br]),&(sb->tail_cache), free_br * sizeof(uint32_t));

            if(sb->it_size - 1 > block){
                sb->tail_blk += free_br;
            }
            else {
                sb->tail_blk = 0;
            }

            memcpy(&(sb->tail_cache),&(sb->tail_cache.ref[free_br]),((sb->tail_cache.idx) - free_br) * sizeof(uint32_t));
            sb->tail_cache.idx -= free_br;

            for(uint32_t i=0; i < free_br; i++){
                sb->tail_cache.ref[(sb->tail_cache.idx)+i] = NullReference;
            }

        }
        else{
            memcpy(&(block_pointer[used_br]),&(sb->tail_cache),(sb->tail_cache.idx)*sizeof(uint32_t));
            sb->tail_blk += sb->tail_cache.idx;
            sb->tail_cache.idx = 0;

            for(uint32_t i=0; i < TAIL_CACHE_SIZE;i++){
                sb->tail_cache.ref[i] = NullReference;
            }
        }

        soSaveSuperBlock();
    }
};

