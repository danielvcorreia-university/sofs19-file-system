#include "grp_fileblocks.h"

#include "freelists.h"
#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <inttypes.h>
#include <errno.h>
#include <assert.h>

namespace sofs19
{
    /* free all blocks between positions ffbn and RPB - 1
     * existing in the block of references given by i1.
     * Return true if, after the operation, all references become NullReference.
     * It assumes i1 is valid.
     */
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn);

    /* free all blocks between positions ffbn and RPB**2 - 1
     * existing in the block of indirect references given by i2.
     * Return true if, after the operation, all references become NullReference.
     * It assumes i2 is valid.
     */
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn);

    /* ********************************************************* */

    void grpFreeFileBlocks(int ih, uint32_t ffbn)
    {
        soProbe(303, "%s(%d, %u)\n", __FUNCTION__, ih, ffbn);

        /* change the following line by your code */

        SOInode* ip = soGetInodePointer(ih);
        uint32_t i2start = N_DIRECT + N_INDIRECT * RPB;
        uint32_t i2end = i2start + RPB * RPB * N_DOUBLE_INDIRECT;
        uint32_t count = 0;
        uint32_t i = 0;
        uint32_t ffbni1 = 0;
        uint32_t ffbni2 = 0;
        if(ffbn < N_DIRECT) {
            for(uint32_t i = ffbn; i < N_DIRECT; i++) {
                if(ip->d[i] != NullReference) {
                    count++;
                    ip->d[i] = NullReference;
                }
            }
        }
        if(i == N_DIRECT || (ffbn >= N_DIRECT && ffbn < i2start)) {
            if(i == N_DIRECT)
                grpFreeIndirectFileBlocks(ip, 0, 0);
            else
                ffbni1 = ffbn - N_DIRECT;
                grpFreeIndirectFileBlocks(ip, (ffbni1 / RPB) % RPB , ffbni1);

            i = i2start;
        }
        if(i == i2start || (ffbn >= i2start && ffbn < i2end)) {
            if(i == i2start)
                grpFreeDoubleIndirectFileBlocks(ip, 0, 0);
            else
                ffbni2 = ffbn - N_DIRECT - N_INDIRECT * RPB;
                grpFreeDoubleIndirectFileBlocks(ip, ffbni1 / RPB / RPB, ffbni2);
        }

        ip->blkcnt -= count;
        soSaveInode(ih);        
    }

    /* ********************************************************* */

#if true
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i1, ffbn);

        /* change the following line by your code */
        uint32_t count = 0;
        uint32_t refindex = ffbn % RPB;
        uint32_t rdb[RPB];
        bool empty;

        for(uint32_t i = i1; i < N_INDIRECT; i++) {
            if(ip->i1[i] == NullReference)
                continue;
            
            soReadDataBlock(ip->i1[i], &rdb);
            
            for(uint32_t iref = refindex; iref < RPB; iref++) {
                if(rdb[iref] != NullReference) {
                    count++;
                    rdb[iref] = NullReference;
                }
            }
            
            if(i == i1) {
                empty = true;

                for(uint32_t iref = 0; iref < RPB; iref++) {
                    if(rdb[iref] != NullReference)
                        empty = false;
                }
                
                if(empty) {
                    ip->i1[i] = NullReference;
                    count++;
                }
            } else {
                ip->i1[i] = NullReference;
                count++;
            } 

            soWriteDataBlock(ip->i1[i], &rdb);

            refindex = 0;   // o refindex leva reset para os próximos fileblocks serem totalmente libertados  
        }
        ip->blkcnt -= count;
        return !empty;
    }
#endif

    /* ********************************************************* */

#if true
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i2, ffbn);

        /* change the following line by your code */
        //throw SOException(ENOSYS, __FUNCTION__);
        uint32_t count = 0
        uint32_t i2block = (ffbn / RPB) % RPB
        unit32_t i2arr[N_DOUBLE_INDIRECT] = ip -> i2;
        uint32_t db[RPB];

        for(uint32_t i = i2 ; i < N_DOUBLE_INDIRECT; i++){
            if(i2arr[i] == NullReference){
                continue;
            }

            soReadDataBlock(i2arr[i], &db);

            soFreeIndirectFileBlocks(*ip, ,ffbn)

        }
    }
#endif

    /* ********************************************************* */
};