#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpResetFreeDataBlocks(uint32_t ntotal, uint32_t itotal, uint32_t nbref)
    {
        soProbe(607, "%s(%u, %u, %u)\n", __FUNCTION__, ntotal, itotal, nbref);

        /* change the following line by your code */
        //binResetFreeDataBlocks(ntotal, itotal, nbref);

        uint32_t next_block = (itotal/ IPB) +1 + nbref;

        for(uint32_t i = 0; i < itotal - nbref; i++ ){
            char block[BlockSize];
            for(uint32_t j = 0; j < BlockSize; j++){
                block[j] = 0;
            }
            soWriteRawBlock(next_block++,&block);
        }

    }
};

