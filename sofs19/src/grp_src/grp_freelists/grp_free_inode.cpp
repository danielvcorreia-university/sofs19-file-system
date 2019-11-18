/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
 */

#include "grp_freelists.h"

#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

namespace sofs19
{
    void grpFreeInode(uint32_t in)
    {
        soProbe(402, "%s(%u)\n", __FUNCTION__, in);

        //printf("inode: %d\n",in);

        /* change the following line by your code */
        //binFreeInode(in);

        SOSuperBlock *superBlock = soGetSuperBlockPointer();
        if (superBlock -> ifree == 0){
            superBlock -> ihead = in;
        }

        //printf("itail: %d\n",superBlock -> itail);
        if(superBlock -> itail != -1){
            int inode_tail = soOpenInode(superBlock -> itail);
            SOInode *tail = soGetInodePointer(inode_tail);

            tail -> next = in;
            soCloseInode(inode_tail);
        }

        //printf("teste1%d\n",in);
        int inode_h = soOpenInode(in);

        //printf("teste2%d\n",in);

        //sofs19::soCheckInodeHandler(inode_h, 0);

        SOInode *inode = soGetInodePointer(inode_h);

        superBlock -> itail = in;

        inode -> mode = INODE_FREE;
        inode -> next = NullReference;
        inode -> lnkcnt = 0;
        inode -> owner = 0;
        inode -> group = 0;
        inode -> size = 0;
        inode -> blkcnt = 0;
        inode -> mtime = 0;
        inode -> ctime = 0;

        superBlock -> ifree ++;

        soSaveInode(inode_h);

        soSaveSuperBlock();

        soCloseInode(inode_h);

        //printf("tail: %d\n", superBlock -> itail);

    }
};

