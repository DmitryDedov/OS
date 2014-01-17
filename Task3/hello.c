#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define DEBUG 

#ifdef DEBUG
	#define TRACE printf("%d\n", __LINE__);
#endif

//static const char *hello_str = "Hello World!\n";
static const char* hello_path = "/home/dmitry/Dropbox/VSU/3 cource/OS/Task3/hellofile";

typedef struct str_iFolder 
{ 
	char name[10][5];
	int nodes[10];
}iFolder;

typedef struct str_iNode * inode;
struct str_iNode
{
	int type;
	iFolder folder;
};

struct str_FileSystem 
{
	int istart;
	int iend;
	int isize;
}fileSystem;

int KnowSizeFile()
{
	FILE* fs = fopen(hello_path, "rb+");
	fseek(fs, 0, SEEK_END);   
	int size = ftell(fs);
	return size; 	
}

void FormattingFile() 
{
	int size = KnowSizeFile();
	FILE* fs = fopen(hello_path, "wb+"); 
	fseek(fs, 0, SEEK_SET);
	fileSystem.isize = sizeof(struct str_iNode);
	fileSystem.istart = sizeof(struct str_FileSystem);
	fileSystem.iend = size;
	fwrite(&fileSystem, sizeof(struct str_FileSystem), 1, fs);
	for (int i = fileSystem.istart; i < fileSystem.iend - fileSystem.isize; i += fileSystem.isize) 
	{
		fseek(fs, i, SEEK_SET);  
		int x = 0;
		fwrite(&x, 4, 1, fs);
	}
	int type = 1;
	inode in = malloc(sizeof(struct str_iNode));
	in->type = type;
	for (int i = 0; i < 10; i++) 
	{
		in->folder.nodes[i] = NULL;
	}
	fseek(fs, fileSystem.istart, SEEK_SET);
	fwrite(in, sizeof(struct str_iNode), 1, fs);

	fclose(fs);
}

inode ReadiNode(int index) 
{
    FILE* fs = fopen(hello_path, "rb+");
    fseek(fs, index, SEEK_SET);
    inode in = malloc(sizeof(struct str_iNode));
	fread(in, sizeof(struct str_iNode), 1, fs);
    fclose(fs);
    return in;
}

void FillFileToZero()
{
	FILE* fs = fopen(hello_path, "wt");
	for (int i = 0; i < 1000000; i++)
	{
		fprintf(fs, "%d\n", 0);
		fprintf(fs, "\n");
	}
	fclose(fs);
}

void PrintiNode(inode n) 
{
    if (n->type == 1) 
    {
        for (int i = 0; i < 10; i++) 
        {
        	if (n->folder.nodes[i] != NULL)
        		printf("%s\n", n->folder.name[i]);
            printf("%d, %d\n", i, n->folder.nodes[i]);
        }
    }
}

void AddChildiNode(inode parent, int parentIndex, inode child, const char* nameChild)
{
	int pos = FindFreeiNode();
	FILE *fs = fopen(hello_path, "wb+");	
	if (pos < 0 || parent->type != 1)
	{
		return;
	}
	int i = 0;
	while (i < 10 && parent->folder.nodes[i] != NULL)
	{
		i++;
	}
	if (i == 10)
	{
		return;
	}
	parent->folder.nodes[i] = pos;
	CopyName(parent->folder.name[i], nameChild);	
	fseek(fs, parentIndex, SEEK_SET);
	fwrite(parent, sizeof(struct str_iNode), 1, fs);
	fseek(fs, pos, SEEK_SET);
	fwrite(child, sizeof(struct str_iNode), 1, fs);
}

void CopyName(char* dest, char* source) 
{
    int len = strlen(source);
    len = len > 3 ? 3 : len;
    int i = 0;
    for (i; i < len; i++)
    {
        dest[i] = source[i];
    }
    dest[i] = NULL;
}

int FindFreeiNode()
{
	FILE *fs = fopen(hello_path, "rb+");
	inode n = malloc(sizeof(struct str_iNode));	
	int pos = fileSystem.istart;
	do
	{
		if (pos >= fileSystem.iend - fileSystem.isize)
		{
			return -1;
		}
		fseek(fs, pos, SEEK_SET);
		fread(n, sizeof(struct str_iNode), 1, fs);
		if (n->type == 0)
		{
			return pos;
		}
		pos += fileSystem.isize;
	}while (pos < fileSystem.iend);
	return pos;
}

int main(int argc, char *argv[])
{
	FillFileToZero();
	FormattingFile();
	inode parent = ReadiNode(12);
	inode child = malloc(sizeof(struct str_iNode));
	child->type = 1;
	AddChildiNode(parent, 12, child, "www");
	PrintiNode(parent);
	return 0;
	//return fuse_main(argc, argv, &hello_oper, NULL);
}

// static int hello_getattr(const char *path, struct stat *stbuf)
// {
// 	int res = 0;

// 	memset(stbuf, 0, sizeof(struct stat));
// 	if (strcmp(path, "/") == 0) 
// 	{
// 		stbuf->st_mode = S_IFDIR | 0755;
// 		stbuf->st_nlink = 2;
// 	} 
// 	else if (strcmp(path, hello_path) == 0) 
// 	{
// 		stbuf->st_mode = S_IFREG | 0444;
// 		stbuf->st_nlink = 1;
// 		stbuf->st_size = strlen(hello_str);
// 	} 
// 	else
// 		res = -ENOENT;

// 	return res;
// }

// static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
// 			 off_t offset, struct fuse_file_info *fi)
// {
// 	(void) offset;
// 	(void) fi;

// 	if (strcmp(path, "/") != 0)
// 		return -ENOENT;

// 	filler(buf, ".", NULL, 0);
// 	filler(buf, "..", NULL, 0);
// 	filler(buf, hello_path + 1, NULL, 0);

// 	return 0;
// }

// static int hello_open(const char *path, struct fuse_file_info *fi)
// {
// 	if (strcmp(path, hello_path) != 0)
// 		return -ENOENT;

// 	if ((fi->flags & 3) != O_RDONLY)
// 		return -EACCES;

// 	return 0;
// }

// static int hello_read(const char *path, char *buf, size_t size, off_t offset,
// 		      struct fuse_file_info *fi)
// {
// 	size_t len;
// 	(void) fi;
// 	if(strcmp(path, hello_path) != 0)
// 		return -ENOENT;

// 	len = strlen(hello_str);
// 	if (offset < len) 
// 	{
// 		if (offset + size > len)
// 			size = len - offset;
// 		memcpy(buf, hello_str + offset, size);
// 	} else
// 		size = 0;

// 	return size;
// }

// static struct fuse_operations hello_oper = {
// 	.getattr	= hello_getattr,
// 	.readdir	= hello_readdir,
// 	.open		= hello_open,
// 	.read		= hello_read,
// };