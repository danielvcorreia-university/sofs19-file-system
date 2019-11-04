#include "grp_fileblocks.h"

#include "freelists.h"
#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <errno.h>

#include <iostream>

namespace sofs19
{

#if true
    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn);
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn);
#endif

    /* ********************************************************* */

    uint32_t grpAllocFileBlock(int ih, uint32_t fbn)
    {
        soProbe(302, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        uint32_t dIndirectRange[] = {N_DIRECT + N_INDIRECT * RPB, N_DIRECT + N_INDIRECT * RPB + N_DOUBLE_INDIRECT * RPB * RPB};
        uint32_t newBlockRef;

        if(fbn < 0 || fbn >= dIndirectRange[1]){
            throw SOException(EINVAL, __FUNCTION__);
        }

        SOInode* ip = soGetInodePointer(ih);

        if(fbn < N_DIRECT){
            newBlockRef = soAllocDataBlock();
            ip->d[fbn] = newBlockRef;
            ip->blkcnt++;
        }else if(fbn < dIndirectRange[0]){
            newBlockRef = grpAllocIndirectFileBlock(ip, fbn - N_DIRECT);
        }else if(fbn < dIndirectRange[1]){
            newBlockRef = grpAllocDoubleIndirectFileBlock(ip, fbn - dIndirectRange[0]);
        }
        soSaveInode(ih);
        return newBlockRef;
    }


    /* ********************************************************* */

#if true
    /*
    */
    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        uint32_t indirect_index = (afbn/RPB) % RPB;

        uint32_t ref = afbn % RPB;

        uint32_t referencesBlock[RPB];

        uint32_t newBlockRef;

        if(ip->i1[indirect_index] == NullReference){


            uint32_t referencesBlockRef = soAllocDataBlock();
            for(uint32_t i = 0; i < RPB; i++) {
                referencesBlock[i] = NullReference;
            }

            soWriteDataBlock(referencesBlockRef, referencesBlock);
            ip->i1[indirect_index] = referencesBlockRef;
            soReadDataBlock(ip->i1[indirect_index], &referencesBlock);

            newBlockRef = soAllocDataBlock();
            referencesBlock[ref] = newBlockRef;

            ip->blkcnt += 2;
        }else{
            soReadDataBlock(ip->i1[indirect_index], &referencesBlock);
            newBlockRef = soAllocDataBlock();
            ip->i1[indirect_index] = newBlockRef;
            ip->blkcnt += 1;
        }
        return newBlockRef;

    }
#endif

    /* ********************************************************* */

#if true
    /*
    */
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        uint32_t d_indirect_index = afbn/ RPB / RPB;


        uint32_t referencesBlock[RPB];

        uint32_t newBlockRef;

        if(ip->i2[d_indirect_index] == NullReference){

            uint32_t referencesBlockRef = soAllocDataBlock();
            for(uint32_t i = 0; i < RPB; i++) {
                referencesBlock[i] = NullReference;
            }
            soWriteDataBlock(referencesBlockRef, referencesBlock);

            ip->i2[d_indirect_index] = referencesBlockRef;
            soReadDataBlock(ip->i2[d_indirect_index], &referencesBlock);

            newBlockRef = grpAllocIndirectFileBlock(ip, afbn);
            ip->blkcnt++;
        }else{
            soReadDataBlock(ip->i2[d_indirect_index], &referencesBlock);
            newBlockRef = grpAllocIndirectFileBlock(ip, afbn);
        }

        return newBlockRef;

    }
#endif
};

