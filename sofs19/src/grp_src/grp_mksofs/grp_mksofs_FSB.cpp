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
        strncpy(sb.name, name, PARTITION_NAME_SIZE);
        sb.mntstat = 1; 
        sb.mntcnt = 0;
        sb.ntotal = ntotal;

        // Inodes metadata
        sb.it_size = itotal/IPB;
        sb.itotal = itotal;
        sb.ifree = itotal - 1;
        sb.ihead = 1; 
        sb.itail = itotal - 1; 

        // Data blocks metadata
        sb.dz_start = sb.it_size + 1;
        sb.dz_total = sb.ntotal - sb.it_size - 1;
        sb.dz_free = sb.dz_total - 1 - nbref;
        sb.head_blk = 1;
        sb.head_idx = 1;                                   
        sb.tail_blk = nbref;
        sb.tail_idx = sb.dz_total - 65 - (RPB * (nbref - 1)); // não percebi o 65, mas segue este padrão 
        
        for(int i = 0; i < HEAD_CACHE_SIZE; i++)
            sb.head_cache.ref[i] = nbref + 1 + i;
        sb.head_cache.idx = 0; 
        
        for(int i = 0; i < TAIL_CACHE_SIZE; i++)
            sb.tail_cache.ref[i] = NullReference;
        sb.tail_cache.idx = 0; 

        soWriteRawBlock(0, &sb);

        //binFillSuperBlock(name, ntotal, itotal, nbref);
    }
};

