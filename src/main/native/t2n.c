#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "t2n.h"
#include <jni.h>
#include "MainWindow.h"


JNIEXPORT void JNICALL
Java_MainWindow_t2n (JNIEnv *env, jobject obj, jstring s1, jstring s2)
{
	const jbyte *str;
	const jbyte *str1;
	str=(*env)->GetStringUTFChars(env,s1, NULL);
	if(str==NULL) {
		return;
	}
	str1=(*env)->GetStringUTFChars(env,s2, NULL);
	if(str1==NULL) {
		return;
	}

	strcpy(infilename, str);
	strcpy(outfilename, str1);

	t2n(3);

	(*env)->ReleaseStringUTFChars(env,s1, str);
	(*env)->ReleaseStringUTFChars(env,s2,str1);

	return;
}


void t2n(int t)
{

	if(t==3)
	{
		printf("\n%s",infilename);
		printf("\n%s",outfilename);
	}
	else
	{
		usage();
	}
	
	open_files();
	read_header();
	read_track();
	dump_track();
	close_files();

}

//=======================================================
void usage(void)
{
	printf("\nT2N reads a text file from MF2T and converts it to note");
	printf("\n     numbers and durations");
	printf("\nFormat:\n    T2N infile outfile\n");
	exit(0);
}
//=======================================================
void open_files( void )
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
void close_files(void)
{
	fclose(infile);
	fclose(outfile);
}
//=======================================================
void read_header(void)
{
	char s[10],t[256],u[10],v[10],x[10],y[10],z[10],*yy,*zz;
	int d,i,j,k,n,ve;

	fscanf(infile,"%s %d %d %d", s,&i,&j,&k);
	if(strncmp(s, "MFile",5)==0)
	{
		printf("\nOpened MFILE %s", infilename);
		printf("\nDivision is %d", k);
		division=k;
	}
	while(fgets(t,256,infile)!=0)
	{
		sscanf(t,"%s %s %s %s %s",u,v,x,y,z);
		if(strncmp(v,"On",2)==0)
		{
//			printf("\nWe have a note on");
			if((strncmp(y,"n=",2)==0) && (strncmp(z,"v=",2)==0))
			{
				yy=y;
				yy++;
				yy++;
				zz=z;
				zz++;
				zz++;

				sscanf(yy,"%d",&n);
				sscanf(zz,"%d",&ve);
				sscanf(u, "%d",&d);

//				printf("\nTime %d, Note %d Velocity %d",d,n,ve);
				zerotime=d; /* get the starting time */
				curnote=n;
				curvel=ve;
			}
				break;
		}
	}
}

//=======================================================
void read_track(void)
{
	char t[256],u[10],v[10],x[10],y[10],z[10],*yy,*zz;
	int d,i=1,n,ve;

	note[0]=(float)(((float)curnote+1)/128); //this is the first note on
	lasttime=zerotime;

	while((i<128) && (fgets(t,256,infile)!=0))
	{
		sscanf(t,"%s %s %s %s %s",u,v,x,y,z);
		if(strncmp(v,"On",2)==0)
		{ //we have a note on message
			if((strncmp(y,"n=",2)==0) && (strncmp(z,"v=",2)==0))
			{ // there are note and velocity fields present
				yy=y;
				yy++;
				yy++;
				zz=z;
				zz++;
				zz++;

				sscanf(yy,"%d",&n); //get note number
				sscanf(zz,"%d",&ve);//get velocity
				sscanf(u, "%d",&d);//get time
//				printf("\nNote %d Velocity %d Time %d",n,ve,d);
				if(ve == 0)
				{ //we have note off through velocity field zero
					if(curnote!=n)
						printf("\nNote number mismatch");
					delta=d-lasttime;
					duration[i-1]=(float)((float)delta/(division*4));
					lasttime=d;
//					printf("\nNote Off due to velocity %f",duration[i-1]);
//					printf(" Delta = %d", delta);

				}
				if(ve !=0)
				{ //we have a note on
					curnote = n;
					if((d-lasttime)>3)
					{ //we have a pause first. Small margin for close to zero duration
						note[i]=0.0;
						delta=d-lasttime;
						duration[i]=(float)((float)delta/(division*4));
						lasttime=d;
//						printf("\nA pause before note on, %f", duration[i]);
//						printf(" Delta = %d", delta);
						i++;
						note[i]=(float)(((float)n+1)/128);
						i++;
					}
					else
					{
						note[i]=(float)(((float)n+1)/128);
						lasttime=d;
//						printf("\nNote On due to velocity %f", note[i]);
						i++;
					}
				}
				curnote=n;
				curvel=ve;
			}
		}
	}
	printf("\n%d MIDI events read", i); 
}
/** Dump track modified by JL 
void dump_track(void)
{
	int i;
	int j=0,k=0;
	for(i=0;i<128;i++)
	{
		fprintf(outfile,"%f",note[i]);
		if(j<15) {  
			fprintf(outfile," ");
			j++;
		}
		else {
			fprintf(outfile,"\n");
			j=0;
		}
	}

	for(i=0;i<128;i++)
	{
		fprintf(outfile,"%f",duration[i]);
		if(k<15) { 
			fprintf(outfile," ");
			k++;
		}
		else {
			fprintf(outfile,"\n");
			k=0;
		}
	}
	
}
**/ 

//Original code 
void dump_track(void)
{
	int i;
	for(i=0;i<128;i++)
	{
		fprintf(outfile,"%f",note[i]);
		if(i<127)
			fprintf(outfile," ");
		else
			fprintf(outfile,"\n");
	}
	for(i=0;i<128;i++)
	{
		fprintf(outfile,"%f",duration[i]);
		if(i<127)
			fprintf(outfile," ");
		else
			fprintf(outfile,"\n");
	}
}



