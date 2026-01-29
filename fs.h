/* fs.h */
#pragma once
#include <stdio.h>
#include <osapi.h>
#include <disk.h>

#define Magic1          (0xdd05)
#define Magic2          (0xaa55)
#define InodesPerBlock  (16)
#define PtrPerInode     (8)
#define PtrPerBlock     (256)
#define ValidChars      ($1 "abcdefghijklmnopqrstuvwxyz0123456789_-")
#define ValidPathChars  ($1 "abcdefghijklmnopqrstuvwxyz0123456789_-/:.")
#define MaxDepth        (16)
#define MaxFilesPerDir  (PtrPerInode+PtrPerBlock)

typedef int8 bootsector[500];
typedef bool bitmap;

internal packed enum e_type {
    TypeNotValid = 0x00,
    TypeFile = 0x01,
    TypeDir = 0x03
};
typedef enum e_type type;

internal struct s_superblock {
    bootsector boot;
    int16 _;
    int16 blocks;
    int16 inodeblocks;
    int16 inodes;
    int16 magic1;
    int16 magic2;
} packed;
typedef struct s_superblock superblock;

internal struct s_filesystem {
    int8 drive;
    disk *dd;
    bool *bitmap;
    superblock metadata;
} packed;
typedef struct s_filesystem filesystem;

internal struct s_filename {
    int8 name[8];
    int8 ext[3];
} packed;
typedef struct s_filename filename;

internal struct s_inode {
    int8 validtype;
    int16 size;
    filename name;
    ptr indirect;
    ptr direct[PtrPerInode];
} packed;
typedef struct s_inode inode;

internal union u_fsblock {
    superblock super;
    int8 data[Blocksize];
    ptr pointers[PtrPerBlock];
    inode inodes[InodesPerBlock];
} packed;
typedef union u_fsblock fsblock;

public struct s_fileinfo {
    ptr idx;
    int16 size;
} packed;
typedef struct s_fileinfo fileinfo;

internal struct s_path {
    filesystem *fs;
    int8 drive;
    filename target;
    int8 dirpath[(MaxDepth+1)][9];
} packed;
typedef struct s_path path;

public struct s_fileentry {
    ptr inode;
    filename name;
    int16 size;
    type filetype;
} packed;
typedef struct s_fileentry fileentry;

public struct s_directory {
    int16 drive;
    filesystem *fs;
    ptr inode;
    filename dirname;
    int16 len;
    fileentry *filelist;
} packed;
typedef struct s_directory directory;

public struct s_tuple {
    int16 numfiles;
    fileentry *filelist;
} packed;
typedef struct s_tuple tuple;

#define indestroy(f,p) (bool)inunalloc((f),(p))
#define filename2low(x) tolowercase($1 (x))

/* internal -> private? */
public filesystem *fsformat(disk*,bootsector*,bool);
internal bitmap *mkbitmap(filesystem*,bool);
internal int16 bitmapalloc(filesystem*,bitmap*);
internal void bitmapfree(filesystem*,bitmap*,int16);
internal void fsshow(filesystem*,bool);
internal inode *findinode(filesystem*,ptr);
internal int8 *file2str(filename*);
internal filename *str2file(int8*);
internal filesystem *fsmount(int8);
internal void fsunmount(filesystem*);

internal ptr increate(filesystem*,filename*,type);
internal ptr inalloc(filesystem*);
internal bool inunalloc(filesystem*,ptr);
internal ptr fssaveinode(filesystem*,inode*,ptr);
//public fileinfo *fsstat(path*);
public fileinfo *fsstat(filesystem*,ptr);

private bool validfname(filename*,type);
private bool validpname(int8*);
private bool validchar(int8);
private bool validchar_(int8);
internal void showpath(const path*);
internal ptr path2inode(path*);

internal ptr readdir1(filesystem*,ptr,filename*);
internal path *mkpath(int8*,filesystem*);
internal tuple *mkfilelist(filesystem*,inode*);
