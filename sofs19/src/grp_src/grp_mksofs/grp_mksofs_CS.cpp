#include "grp_mksofs.h"

#include "core.h"
#include "bin_mksofs.h"

#include <inttypes.h>
#include <iostream>

namespace sofs19
{
    void grpComputeStructure(uint32_t ntotal, uint32_t & itotal, uint32_t & nbref)

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

    {
       soProbe(601, "%s(%u, %u, ...)\n", __FUNCTION__, ntotal, itotal);
            
            /* change the following line by your code */
            //bin::computeStructure(ntotal, itotal, btotal);

            // if it is zero initially, the value ntotal/16 should be used as the start value for itotal, 
            // where / stands for the integer division
            if (itotal == 0) itotal = ntotal/16;
            // itotal is always lower than or equal to ntotal/8
            else if (itotal > ntotal/8) itotal = ntotal/8;
            // itotal is always greater than or equal to IPB
            else if (itotal < IPB) itotal = IPB;
            // itotal must be rounded up to be multiple of IPB
            if(itotal % IPB != 0) itotal = itotal + (itotal%IPB); 

            //splitting data blocks between reference data blocks and free data blocks   
            uint32_t nbinodes; // number of blocks of inodes
            u_int32_t nfbrdb; // number of free data blocks and reference data blocks

            nbinodes = itotal/IPB;

            nfbrdb = ntotal - 2 - nbinodes;

            //if a single block remains, assign to to the inode table
    }
};

