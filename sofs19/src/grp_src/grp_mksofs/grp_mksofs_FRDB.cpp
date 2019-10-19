#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include "superblock.h"

#include <inttypes.h>
#include <string.h>

#include <iostream>

namespace sofs19
{
    void grpFillReferenceDataBlocks(uint32_t ntotal, uint32_t itotal, uint32_t nbref)
    {
        soProbe(605, "%s(%u, %u, %u)\n", __FUNCTION__, ntotal, itotal, nbref);

        /* change the following line by your code */
        
        //return binFillReferenceDataBlocks(ntotal, itotal, nbref);

        uint32_t next_block = itotal/IPB + 1; 
        uint32_t free_block = next_block + nbref; 

        for(uint32_t i = 1; i <= nbref; i++){
            uint32_t block[RPB];

            if(i == nbref){
                block[0] = NullReference;
            }else{
                block[0] = next_block + 1;
            }

            for(uint32_t k = 1; k < RPB; k++){
                if(free_block < ntotal + itotal/IPB + 1){
                    block[k] = free_block++;
                }else{
                    block[k] = NullReference;
                }
            }
            
            soWriteRawBlock(next_block++, &block);

        }




        //std::cout << next_block;


    }
};

