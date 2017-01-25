// On-disk file system format.
// Both the kernel and user programs use this header file.


#define ROOTINO 1  // root i-number
#define BSIZE 512  // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
  uint nlog;         // Number of log blocks
  uint logstart;     // Block number of first log block
  uint inodestart;   // Block number of first inode block
  uint bmapstart;    // Block number of first free map block
};


// NDIRECT is reduced by 1 to 11 - free one slot for another indirect slot
// *********************************************************************//
#define NDIRECT 11
// *********************************************************************//
#define NINDIRECT (BSIZE / sizeof(uint))

// NDIRECT (11) + NIDIRECT (128) + NINDIRECT*NINDIRECT (128*128) = 16,523 BLOCKS
// *********************************************************************//
#define MAXFILE (NDIRECT + NINDIRECT + NINDIRECT*NINDIRECT)
// *********************************************************************//

// On-disk inode structure
struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEV only)
  short minor;          // Minor device number (T_DEV only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)

// We decreased NDIRECT by 1, hence we add 1 to addrs array so it stays constant
// *********************************************************************//
  uint addrs[NDIRECT+2];   // Data block addresses
// *********************************************************************//

};

// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB           (BSIZE*8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) (b/BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};

