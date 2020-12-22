#include "fs.h"

void test_fs()
{
	struct fs *tfs = new_fs(); // Make the fs
	struct file tfile = new_file("testfile.txt"); // Make a new file

	add_file(tfs, tfile, 60); // Add the file to the fs
	dump(tfs->mem, HEX);
	
	// Test mem reading
	char *some_read = read(tfs->mem, 6, 62);
	printf("random read: ");
	for (int i = 0; i < 6; i++) {
        printf("%2x ", some_read[i]);
	}
	printf("\n");

    // Get a file    
	struct file file_read = get_file(tfs, "testfile.txt");
    print_file(file_read);

    destroy_fs(tfs);
    // destroy_file(tfile); // ?
}

void get(struct fs *tfs, char fname[])
{
    printf("\n\n");
    struct file tread = get_file(tfs, fname);
    print_file(tread);
    for (int i = 0; i < tread.s; i++)
        printf("%c", tread.bytes[i]);
    printf("\n");
    destroy_file(tread);
    printf("\n\n");
}

void test_fs_simple()
{
    struct fs *tfs = new_fs();
    struct file tfile1 = new_file("testfile.txt");
    struct file tfile2 = new_file("testfile2.txt");
    struct file tfile3 = new_file("Makefile");

    add_file(tfs, tfile2, 0);
    add_file(tfs, tfile1, 100);
    add_file(tfs, tfile3, 200);
    add_file(tfs, tfile3, 600);

    dump(tfs->mem, HEX);

    get(tfs, "filename");
    get(tfs, "testfile.txt");
    get(tfs, "testfile2.txt");
    get(tfs, "Makefile");

    destroy_fs(tfs);

    destroy_file(tfile1);
    destroy_file(tfile2);
    destroy_file(tfile3);
}

int test_mem()
{
	return 0;
}

int main()
{
    test_fs_simple();
    test_mem();

    return 0;
}
