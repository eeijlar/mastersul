/* cross.c */
/* 3/3/2000 JMF */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "cross.h"
#include <jni.h>
#include "MainWindow.h"

JNIEXPORT void JNICALL
Java_MainWindow_cross (JNIEnv *env, jobject obj3, jstring in, jstring out)
{
	const jbyte *infile;
	const jbyte *outfile;
	infile=(*env)->GetStringUTFChars(env,in, NULL);

	if(infile=NULL){
		return;
	}

	outfile=(*env)->GetStringUTFChars(env,out, NULL);
	
	if(outfile=NULL){
		return;
	}

	strcpy(infilename,infile);
	strcpy(outfilename,outfile);

	cross(3);

	(*env)->ReleaseStringUTFChars(env,in, infile);
	(*env)->ReleaseStringUTFChars(env,out, outfile);

	return;
}

void cross (int args)
{
	lastname=0;
	lastpoint=0;
	maxpoint=0;

	if(args==3)
	{
		//strcpy(infilename, argv[1]);
		//strcpy(outfilename, argv[2]);
		printf("\n%s",infilename);
		printf("\n%s",outfilename);
	}
	else
	{
		usage_3();
	}
	init_workspace_1();
	open_files_2();
	read_record();
	write_record();
	close_files_2();
}

//=======================================================
void usage_3(void)
{
	printf("\nCROSS infile outfile\n");
	exit(0);
}


//=======================================================
void open_files_2( void )
{	
	infile = fopen(infilename, "r");
	outfile = fopen(outfilename, "w");
	if(!infile)
	{
		printf("\nFailed to open %s", infilename);
		exit(0);
	}
	if(!outfile)
	{
		printf("\nFailed to open %s", outfilename);
		exit(0);
	}
}

//=======================================================
void close_files_2(void)
{
	fclose(infile);
	fclose(outfile);
}

//=======================================================
void read_record(void)
{
	double rain;
	int result=1;
	char name[255],oldname[255],line[255], *ip;
	
	name[0]='\0';
	oldname[0]='\0';

	while(result>0 && !feof(infile))
	{
		result = (int)fgets(line,255, infile);
		ip=line;
		if(strlen(line)>0)
		{
			while((*ip!=',') && (*ip!='\0'))	/* find the comma */
				ip++;		/* by traversing the string */
			*ip='\0';		/* delimit the name part with a null char */
			ip++;			/* move to first location of rain number */
			rain = atof(ip);	/* convert to float */
			strcpy(name,line);
			stripstring(name); /* remove quotation marks and spaces */

			if(!strcmp(oldname,name)) /* check if we have a new name */
			{
				allrain[lastname-1][lastpoint]=rain;
				lastpoint++;
			}
			else
			{
				if(lastpoint>maxpoint)
					maxpoint=lastpoint;
				lastpoint=0;
				strcpy(allnames[lastname],name);
				lastname++;
			}
			strcpy(oldname,name);	/* copy the name to oldname */
		}
	}
}

//=======================================================
void write_record(void)
{
	int i,j;

	for(i=0;i<=lastname;i++)
	{
		fprintf(outfile,"%s",allnames[i]);
		if(i<lastname)
			fprintf(outfile,",");
		else
			fprintf(outfile,"\n");
	}
	for(j=0;j<maxpoint;j++)
	{
		for(i=0;i<lastname;i++)
		{
			fprintf(outfile,"%f",allrain[i][j]);
			if(i<(lastname-1))
				fprintf(outfile,",");
			else
				fprintf(outfile,"\n");
		}
	}
}

void stripstring( char *instr )
{

	char *ip,*op,st[255];
	ip = instr;
	op = st;
	while( *ip!='\0')
	{
		if(*ip != '"' && *ip != ' ')
		{
			*op=*ip;
			op++;
		}
		ip++;
	}
	*op=*ip;
	strcpy(instr,st);
}

void init_workspace_1(void)
{
	int i,j;
	for(i=0;i<MAXNAMES;i++)
		for(j=0;j<MAXPOINTS;j++)
			allrain[i][j]=0.0;
}