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
        binDepleteTailCache();
    }
};

