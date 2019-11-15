#include "grp_fileblocks.h"

#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <errno.h>

namespace sofs19
{
    /* ********************************************************* */
    #if true
        static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t fbn);
        static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t fbn);
    #endif
    /* ********************************************************* */

    uint32_t grpGetFileBlock(int ih, uint32_t fbn)
    {
        soProbe(301, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        
            SOInode* ip = soGetInodePointer(ih);
			uint32_t IB = N_DIRECT;
			uint32_t DIB = (N_INDIRECT * RPB) + IB;
			uint32_t DIE = (RPB * RPB * N_DOUBLE_INDIRECT) + DIB -1;

			if(fbn >= 0 && fbn <= DIE){

				if(fbn < IB){
					return ip->d[fbn];
				}
				else if(fbn < DIB){
					return grpGetIndirectFileBlock(ip,fbn-IB);
				}
				else {
					return grpGetDoubleIndirectFileBlock(ip,fbn-DIB);
				}

			}
			else{
				throw SOException(EINVAL, __FUNCTION__);
			}
            


    }

    /* ********************************************************* */
    #if true
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

    #if true
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

