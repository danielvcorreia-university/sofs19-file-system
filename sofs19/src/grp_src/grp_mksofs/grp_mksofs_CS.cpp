#include "grp_mksofs.h"

#include "core.h"
#include "bin_mksofs.h"

#include <inttypes.h>
#include <iostream>

namespace sofs19
{
    void grpComputeStructure(uint32_t ntotal, uint32_t & itotal, uint32_t & nbref){
        soProbe(601, "%s(%u, %u, ...)\n", __FUNCTION__, ntotal, itotal);
    /*
      Remarks
        - this function does not change any block of the disk;
        - if it is zero initially, the value ntotal/16 should be used as the start value for itotal, 
            where / stands for the integer division;
        - itotal is always lower than or equal to ntotal/8;
        - itotal is always greater than or equal to IPC;
        - itotal must be rounded up to be multiple of IPB;
        - if, after splitting data blocks between reference data blocks and free data blocks, a 
            single data block remains, it is assigned to the inode table;
     */

             /* change the following line by your code */
            //binComputeStructure(ntotal, itotal, nbref);

            if(itotal == 0 || itotal > ntotal/8 || itotal < IPB) itotal = ntotal/16;

            if(itotal % IPB == 0) itotal = itotal/IPB;
            else{
                while(itotal % IPB != 0) itotal = itotal + 1;
            }

            uint32_t ib = itotal/IPB;

            if((1 + ib + 1 + HEAD_CACHE_SIZE) >= ntotal) nbref = 0;
            else{
                if((ntotal - 1 - ib - 1 - HEAD_CACHE_SIZE) % RPB == 1) {
                    itotal = itotal + 8;
                    nbref = ((ntotal - 1 - ib - 1 - HEAD_CACHE_SIZE) - 1) / RPB;
                }
                else nbref = (ntotal - 1 - ib - 1 - HEAD_CACHE_SIZE) / RPB+1;
            }

    }
};

