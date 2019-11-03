#include "grp_fileblocks.h"

#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <errno.h>

namespace sofs19
{
    /* ********************************************************* */

#if false
    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t fbn);
    static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t fbn);
#endif

    /* ********************************************************* */

    uint32_t grpGetFileBlock(int ih, uint32_t fbn)
    {
        soProbe(301, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        /*
        SOInode* ip = soGetInodeNumber(ih);
			uint32_t IndirectBegin = N_DIRECT;
			uint32_t DoubleIndirectBegin = (N_INDIRECT * RPB) + IndirectBegin;
			uint32_t DoubleIndirectEnd = (RPB * RPB * N_DOUBLE_INDIRECT) + DoubleIndirectBegin -1;

			if(fbn >= 0 && fbn <= DoubleIndirectEnd){

				if(fbn < IndirectBegin){
					return ip->d[fbn];
				}
				else if(fbn < DoubleIndirectBegin){
					return grpGetIndirectFileBlock(ip,fbn-IndirectBegin);
				}
				else {
					return grpGetDoubleIndirectFileBlock(ip,fbn-DoubleIndirectBegin);
				}

			}
			else{
				throw SOException(EINVAL, __FUNCTION__);
			}
            */

        /* change the following line by your code */
        return binGetFileBlock(ih, fbn);
    }

    /* ********************************************************* */

#if false
    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);

            uint32_t db[RPB];
            uint32_t pos1=afbn / RPB;
            uint32_t pos2= afbn % RPB;

            if(ip->i1[pos1] == NullReference){
            	return NullReference;
            }
            else{
                soReadDataBlock(ip->i1[pos1],&db);
                return db[pos2];
            }
    }
#endif

    /* ********************************************************* */

#if false
    static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);

            uint32_t db[RPB];
            uint32_t pos1 = afbn / (RPB*RPB);
            uint32_t pos2 = afbn / RPB-(pos1*RPB);
            uint32_t pos3 = afbn  % RPB;

            if(ip->i2[pos1] == NullReference){
            	return NullReference;
            }
            else{
                soReadDataBlock(ip->i2[pos1],&db);

                if(db[pos2] == NullReference){
                	return NullReference;
                }
                else{
                	soReadDataBlock(db[pos2],&db);
                	return db[pos3];
                }

            }
    }
#endif
};

