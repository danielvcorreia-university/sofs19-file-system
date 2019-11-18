#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "direntries.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

namespace sofs19
{
    uint32_t grpTraversePath(char *path)
    {
        soProbe(221, "%s(%s)\n", __FUNCTION__, path);

        /* change the following line by your code */
        
        /* Checking if path is absolute */
        if ( *path != '/' )
            { throw SOException( EINVAL , __FUNCTION__ ); }

        /* Recursive stop condition when path is "/" and return inode number 0 */
        if ( strcmp( path, "/" ) == 0 )
            { return 0; }

        /* Replicate path */
        char * baseName = basename( strdupa( path ) );
        char * dirName = dirname( strdupa( path ) );
        /* Get inode pointer */
        uint32_t inode = soTraversePath( dirName );
        uint32_t ih = soOpenInode( inode );
        SOInode * inode_pointer = soGetInodePointer( ih );

        /* Checking if inodes of path are dir ( S_IFDIR == 0040000 wich is */
        /* the mask to check if this inode is a DIR ) */
        if ( ( inode_pointer -> mode & S_IFDIR ) != S_IFDIR )
            { throw SOException( ENOTDIR , __FUNCTION__ ); }
        /* Checking if the process that calls this function has execute */
        /* permission in all the components of the path */
        if ( !soCheckInodeAccess( ih, X_OK ) )
            { throw SOException( EACCES , __FUNCTION__ ); }
        
        uint32_t inode_number = soGetDirEntry( ih, baseName );
        soCloseInode( ih );
        return inode_number;
    }
};

