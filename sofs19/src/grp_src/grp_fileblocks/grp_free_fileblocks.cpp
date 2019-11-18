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
            for(i = ffbn; i < N_DIRECT; i++) {
                if(ip->d[i] != NullReference) {
                    count++;
                    soFreeDataBlock(ip->d[i]);
                    ip->d[i] = NullReference;
                }
            }
            ffbn = 8;
        }
        if(i == N_DIRECT || (ffbn >= N_DIRECT && ffbn < i2start)) {
            ffbni1 = ffbn - N_DIRECT;
            grpFreeIndirectFileBlocks(ip, (ffbni1 / RPB) % RPB , ffbni1);
            i = i2start;
            ffbn = i2start;
        }
        if(i == i2start || (ffbn >= i2start && ffbn < i2end)) {
            ffbni2 = ffbn - N_DIRECT - N_INDIRECT * RPB;
            grpFreeDoubleIndirectFileBlocks(ip, ffbni2 / RPB / RPB, ffbni2);
        }

        ip->blkcnt -= count;
        soSaveInode(ih);        
    }

    /* ********************************************************* */

    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i1, ffbn);

        /* change the following line by your code */
        uint32_t count = 0;
        uint32_t refindex = ffbn % RPB;
        uint32_t db[RPB];
        bool empty = true;

        for(uint32_t i = i1; i < N_INDIRECT; i++) {
            if(ip->i1[i] == NullReference)
                continue;
            
            soReadDataBlock(ip->i1[i], &db);
            
            for(uint32_t iref = refindex; iref < RPB; iref++) {
                if(db[iref] != NullReference) {
                    count++;
                    printf("%d", iref);
                    soFreeDataBlock(db[iref]);
                    db[iref] = NullReference;
                }
            }
            
            if(i == i1) {

                for(uint32_t iref = 0; iref < RPB; iref++) {
                    if(db[iref] != NullReference)
                        empty = false;
                }
                
                if(empty) {
                    soFreeDataBlock(ip->i1[i]);
                    ip->i1[i] = NullReference;
                    count++;
                }
            } else {
                soFreeDataBlock(ip->i1[i]);
                ip->i1[i] = NullReference;
                count++;
            } 

            soWriteDataBlock(ip->i1[i], &db);

            refindex = 0;   // o refindex leva reset para os próximos fileblocks serem totalmente libertados  
        }
        ip->blkcnt -= count;
        return empty;
    }

    /* ********************************************************* */

    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i2, ffbn);

        /* change the following line by your code */
        uint32_t count = 0;
        uint32_t refindex = (ffbn / RPB) % RPB;
        uint32_t refindex2 = ffbn % RPB;
        uint32_t db1[RPB];
        uint32_t db2[RPB];
        bool empty = true;

        for(uint32_t i = i2; i < N_DOUBLE_INDIRECT; i++) {
            if(ip->i2[i] == NullReference)
                continue;
            
            soReadDataBlock(ip->i2[i], &db1);

            for(uint32_t i1 = refindex; i1 < RPB; i1++) {
                if(db1[i1] == NullReference)
                    continue;
            
                soReadDataBlock(db1[i], &db2);

                for(uint32_t iref = refindex2; iref < RPB; iref++) {
                    if(db2[iref] != NullReference) {
                        count++;
                        soFreeDataBlock(db2[iref]);
                        db2[iref] = NullReference;
                    }
                }
                
                if(i1 == refindex) {

                    for(uint32_t iref = 0; iref < RPB; iref++) {
                        if(db2[iref] != NullReference)
                            empty = false;
                    }
                    
                    if(empty) {
                        soFreeDataBlock(db1[i1]);
                        db1[i1] = NullReference;
                        count++;
                    }
                } else {
                    soFreeDataBlock(db1[i1]);
                    db1[i1] = NullReference;
                    count++;
                } 

                soWriteDataBlock(db1[i1], &db2);

                refindex2 = 0;   // o refindex leva reset para os próximos fileblocks serem totalmente libertados
            }

            if(i == i2) {
                if(empty) {
                    soFreeDataBlock(ip->i2[i]);
                    ip->i2[i] = NullReference;
                    count++;
                }                
            } else {
                soFreeDataBlock(ip->i2[i]);
                ip->i2[i];
                count++;
            }

            soWriteDataBlock(ip->i2[i], &db1);
            refindex = 0;
        }
        ip->blkcnt -= count;
        return empty;
    }

    /* ********************************************************* */
};