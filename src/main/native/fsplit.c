/* fsplit.c */
/* 2/3/2000 JMF */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <jni.h>
#include "MainWindow.h"
#include "fsplit.h"

// this program reads an input file where every second line is pitch and
// every other line is duration, to allow for netz1 to train a separate
// pitch and separate duration network.


static char infilename[256], pitchfile[256], durfile[256];
FILE *infile, *pfile, *dfile;

#define BUF_SIZE 8192

JNIEXPORT void JNICALL
Java_MainWindow_fsplit (JNIEnv *env, jclass j1, jstring s3, jstring s4, jstring s5, jboolean b1) {

	const jbyte *tstFile;
	const jbyte *pitchf;
	const jbyte *durf;
	tstFile=(*env)->GetStringUTFChars(env,s3, NULL);

	if(tstFile==NULL) {
		return;
	}

	pitchf=(*env)->GetStringUTFChars(env,s4, NULL);

	if(pitchf==NULL) {
		return;
	}

	durf=(*env)->GetStringUTFChars(env,s5, NULL);

	if(durf==NULL) {
		return;
	}


	strcpy(infilename, tstFile);
	strcpy(pitchfile, pitchf);
	strcpy(durfile, durf);

	if(b1 == 1) {
		split_j='s';
	} else {
		split_j='j';
	}

	split_join(5);

	(*env)->ReleaseStringUTFChars(env,s3, tstFile);
	(*env)->ReleaseStringUTFChars(env,s4, pitchf);
	(*env)->ReleaseStringUTFChars(env,s5, durf);


return;
}


void split_join (int t)
{
	if(t <5)
	{
		usage1();
	}

	if(parameter == '-')
	{
		switch(split_j)
		{
		case 's' :
			printf("\nSplitting file");
			//strcpy(infilename, argv[2]);
			//strcpy(pitchfile, argv[3]);
			//strcpy(durfile, argv[4]);
			open_files1(0);
		 //	write_lines();
			splitfile();
			close_files1();
			break;
		case 'j' :
			printf("\nJoining files");
			//strcpy(infilename, argv[2]);
			//strcpy(pitchfile, argv[3]);
			//strcpy(durfile, argv[4]);
			open_files1(1);
			join_files();
			close_files1();
			break;
		default :
			usage1();
			break;
		}
	}
	else
		usage1();
}

void usage1 (void)
{
	printf("\nFSPLIT [-s|-j] completefile pitchfile durationfile \
			\n-s is SPLIT completefile into pitchfile and durationfile \
			\n    where completefile is read and pitchfile and durationfile are created \
			\n-j is JOIN pitchfile and durationfile into completefile \
			\n    where pitchfile and durationfile are read and completefile created");
	exit(0);
}


void open_files1( int mode )
{
	if(mode==0)
	{
		infile = fopen(infilename, "r");
		pfile = fopen(pitchfile, "w");
		dfile = fopen(durfile, "w");
	}
	if(mode==1)
	{
		infile = fopen(infilename, "w");
		pfile = fopen(pitchfile, "r");
		dfile = fopen(durfile, "r");
	}
	if(!infile)
	{
		printf("\nFailed to open %s", infilename);
		exit(0);
	}
	if(!pfile)
	{
		printf("\nFailed to open %s", pitchfile);
		exit(0);
	}
	if(!dfile)
	{
		printf("\nFailed to open %s", durfile);
		exit(0);
	}
}

void close_files1(void)
{
	fclose(infile);
	fclose(pfile);
	fclose(dfile);
}


void splitfile( void)
{
	char *line_buf;		/* buffer to hold line of data */
	int even=0;			/* keep track of odd and even lines */

	if ((line_buf = (char *) malloc (BUF_SIZE)) == NULL)
	{
		perror ("read_in_data(): run out of memory\n");
		exit (0);
	}

	while(fgets(line_buf, BUF_SIZE, infile))
	{

		if(even==0)
		{
			
			even=1;
			fprintf(pfile,"%s",line_buf);
		}
		else
		{
			
			fprintf(dfile,"%s",line_buf);
			even=0;
		}
	}

}

void write_lines( void) {
	fprintf(pfile, "%d ",7);
	fprintf(pfile, "%d",0);
	fprintf(pfile, "\n");

	fprintf(dfile, "%d ",7);
	fprintf(dfile, "%d",0);
	fprintf(dfile, "\n");

}

void join_files(void)
{
	char *line_buf;		/* buffer to hold line of data */

	if ((line_buf = (char *) malloc (BUF_SIZE)) == NULL)
	{
		perror ("read_in_data(): run out of memory\n");
		exit (0);
	}

	while(fgets(line_buf, BUF_SIZE, pfile))
	{
		fprintf(infile, "%s", line_buf);
		if(!fgets(line_buf, BUF_SIZE, dfile))
		{
			printf("Mismatching number of lines in %s and $s", pitchfile, durfile);
			exit(0);
		}
		fprintf(infile, "%s", line_buf);
	}
}
