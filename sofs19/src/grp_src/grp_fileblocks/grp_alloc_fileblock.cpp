#include "grp_fileblocks.h"

#include "freelists.h"
#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <errno.h>

#include <iostream>

namespace sofs19
{


    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn);
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn);


    /* ********************************************************* */
    // ih -> inode handler; fbn -> file block number
    uint32_t grpAllocFileBlock(int ih, uint32_t fbn)
    {
        soProbe(302, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        /* change the following two lines by your code */
        //creates a pointer for a inode
        SOInode *inode;
        //get pointer to an open inode
        inode = soGetInodePointer(ih);

        uint32_t allocblock;
       
        //if fbn<N_DIRECT(number of direct block references in the inode)
        // allocDataBlock
        if(fbn<N_DIRECT){
            allocblock = soAllocDataBlock();
            //d[N_DIRECT] -> number of direct block references in the inode
            (*inode).d[fbn] = allocblock;
            //blkcnt -> block count: total number of blocks used by the file
            (*inode).blkcnt++;
        // if fbn < (N_DIRECT + N_INDIRECT (number of indirect
        // block references in the inode)* RPB) -> INDIRECT ALLOCATED
        }else if(fbn <(N_DIRECT + N_INDIRECT*RPB)){
            allocblock = grpAllocIndirectFileBlock(inode, fbn - N_DIRECT);
        // if fbn < (N_DIRECT + N_INDIRECT * RPB*2) -> DOUBLE INDIRECT ALLOCATED   
        }else if(fbn < N_DIRECT + N_INDIRECT*RPB*N_INDIRECT){
            allocblock = grpAllocDoubleIndirectFileBlock(inode, fbn - N_INDIRECT +N_INDIRECT*RPB);
        //Error EINVAL is thrown if fbn is not valid;
        }else{
            throw SOException(EINVAL, __FUNCTION__);
            return 0;
        }

        //Save an open inode to disk
        soSaveInode(ih);

        //return the number of the allocated block
        return allocblock;
        //return binAllocFileBlock(ih, fbn);
    }
    /* ********************************************************* */


    /*
    */
    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* change the following two lines by your code */
        throw SOException(ENOSYS, __FUNCTION__); 
        return 0;
    }


    /* ********************************************************* */


    /*
    */
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* change the following two lines by your code */
        throw SOException(ENOSYS, __FUNCTION__); 
        return 0;
    }

};

