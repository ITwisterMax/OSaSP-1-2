#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <time.h>

int fcount = 0;
int dcount = 1;

int findFiles(char path[256], char name[256]);

int main(int argc[], char *argv[])
{
	char path[256] = "\0";
	char name[256] = "\0";
	
	strcpy(path, argv[1]);
	strcpy(name, argv[2]);
	
	printf("Start point: %s\n", path);
	printf("Find name: %s\n\n", name);
	findFiles(path, name);
	
	printf("Total file count: %d\n", fcount);
	printf("Total dir count: %d", dcount);
	
	getchar();
	return 0;
}

int findFiles(char path[256], char name[256])
{
	char temp_path[256];
	DIR* dir = opendir(path);
	
	struct dirent *d = readdir(dir);
	struct stat buf;
	
	while (d != NULL)
	{
		strcpy(temp_path, path);
		strcat(temp_path, "/");
		strcat(temp_path, d->d_name);
		
		if ((!strcmp(d->d_name, ".")) || (!strcmp(d->d_name, "..")))
		{
			d = readdir(dir);
			continue;
		}

		if (opendir(temp_path))
		{
			findFiles(temp_path, name);
			d = readdir(dir);
			dcount++;
			continue;
		}
		
		if(stat(temp_path, &buf) != 0)
		{
	        	perror( "Problem getting information" );
	        	switch (errno)
	        	{
			case ENOENT:
				printf("File %s not found.\n", d->d_name);
				d = readdir(dir);
				continue;
				break;
			case EINVAL:
				printf("Invalid parameter to _stat.\n");
				d = readdir(dir);
				continue;
				break;
			default:
				/* Should never be reached. */
				d = readdir(dir);
				continue;
				printf("Unexpected error in _stat.\n");
	        	}
		}
			
		if (!strcmp(name, d->d_name))
		{
			char timebuf[50];
			strcpy(timebuf, ctime(&buf.st_mtime));
			printf("Name: %s\n", d->d_name);
			printf("Full path: %s\n", temp_path);
			printf("Size: %ld bytes\n", buf.st_size);
			printf("Modified time: %s", timebuf);
			printf("Access rights: %d\n", buf.st_mode);
			printf("Number of index descriptor: %ld\n\n", buf.st_ino);
		}
		
		d = readdir(dir);
		fcount++;
	}
	return 0;
}
