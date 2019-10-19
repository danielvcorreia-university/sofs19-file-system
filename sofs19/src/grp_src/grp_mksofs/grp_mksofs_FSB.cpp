#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpFillSuperBlock(const char *name, uint32_t ntotal, uint32_t itotal, uint32_t nbref)
    {
        soProbe(602, "%s(%s, %u, %u, %u)\n", __FUNCTION__, name, ntotal, itotal, nbref);

        /* change the following line by your code */
        SOSuperBlock sb;
        sb.magic = MAGIC_NUMBER;
        sb.version = VERSION_NUMBER;
        sb.name = *name;
        sb.mntstat = 1; // maybe
        sb.mntcnt = 0;
        sb.ntotal = ntotal;

        // Inodes metadata
        sb.it_size = itotal/IPB;
        sb.itotal = itotal;
        sb.ifree = itotal - 1;
        sb.ihead = 1; // maybe
        sb.itail = itotal - 1; // maybe

        // Data blocks metadata
        sb.dz_start = sb.it_size + 1;
        sb.dz_total = sb.ntotal - sb.it_size - 1;
        sb.free = sb.ntotal - nbref;
        sb.head_blk = 1;
        sb.head_idx = 1;                                   
        sb.tail_blk = nbref;
        // sb.tail_idx = 256 - something 
        
        HeadCache head;
        for(int i = 0; i < HEAD_CACHE_SIZE; i++)
            head.ref[i] = nbref + 1 + i;
        head.idx = nbref + 1; // maybe

        TailCache tail;
        for(int i = 0; i < TAIL_CACHE_SIZE; i++)
            tail.ref[i] = NullReference;
        tail.idx = 0; // maybe

        binFillSuperBlock(name, ntotal, itotal, nbref);
    }
};

