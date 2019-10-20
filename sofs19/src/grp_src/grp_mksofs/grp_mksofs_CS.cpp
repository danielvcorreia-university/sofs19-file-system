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
            binComputeStructure(ntotal, itotal, nbref);

            // if it is zero initially, the value ntotal/16 should be used as the start value for itotal, 
            // where / stands for the integer division
            if (itotal == 0) itotal = ntotal/16;
            // itotal is always lower than or equal to ntotal/8
            else if (itotal > ntotal/8) itotal = ntotal/8;
            // itotal is always greater than or equal to IPB
            else if (itotal < IPB) itotal = IPB;
            // itotal must be rounded up to be multiple of IPB
            while(itotal % IPB != 0) itotal = itotal + 1; 
            
            // 1 representa o superblock
            // itotal/IPB representa o numero de blocos dos inodes
            // segundo 1 representa a raiz
            // 64 representa a head cache size
            ntotal = (ntotal - 1 - (itotal/IPB) - 1 - 64);

            // se o resultado da divisão do ntotal pelo RPB for diferente de 1 os nossos cálculos 
            // estão feitos, senão, teremos que incrementar o numero de blocos de inodes e refazer os cálculos
            if(ntotal % RPB != 1) ntotal = ntotal / RPB;
            while(ntotal % RPB == 1) ntotal = ((ntotal - 1 - ((itotal/IPB) + 1 ) - 1 - 64)/RPB);
    }
};

