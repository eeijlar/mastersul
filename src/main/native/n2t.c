#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "n2t.h"
#include <jni.h>
#include "MainWindow.h"

JNIEXPORT jboolean JNICALL
Java_MainWindow_n2t (JNIEnv *env, jobject obj, jstring n2tin, jstring n2tout)
{
	const jbyte *n2tstr;
	const jbyte *n2tstr1;
	n2tstr=(*env)->GetStringUTFChars(env,n2tin, NULL);
	n2tstr1=(*env)->GetStringUTFChars(env,n2tout, NULL);


	strcpy(infilename, n2tstr);
	strcpy(outfilename, n2tstr1);

	n2t(3);

	(*env)->ReleaseStringUTFChars(env,n2tin, n2tstr);
	(*env)->ReleaseStringUTFChars(env,n2tout,n2tstr1);

	return 1;
}


void n2t(int n2targs)
{
	if(n2targs==3)
	{
		//strcpy(infilename, argv[1]);
		//strcpy(outfilename, argv[2]);
		printf("\n%s",infilename);
		printf("\n%s",outfilename);
	}
	else
	{
		usage_4();
	}

	open_files_1();
	read_track_1();
	write_track_1();
	close_files_1();

}

//=======================================================
void usage_4(void)
{
	printf("\nT2N reads a text file from MF2T and converts it not note");
	printf("\n     numbers and durations");
	printf("\nFormat:\n    T2N infile outfile\n");
	exit(0);
}
//=======================================================
void open_files_1( void )
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
void close_files_1(void)
{
	fclose(infile);
	fclose(outfile);
}
//=======================================================
void write_track_1(void)
{
	int i,j=0;
	
	lasttime=0;

	fprintf(outfile,"MFile 1 1 120\n");
	fprintf(outfile,"MTrk\n");

	for(i=0;i<128;i++)
	{
		if(note[i] !=0.0)
		{
			fprintf(outfile,"%d On ch=1 n=%d v=%d\n", lasttime+=j, (int)(128*note[i])-1, 100); //note on
			fprintf(outfile,"%d On ch=1 n=%d v=%d\n", lasttime+= (int)(480*duration[i]), (int)(128*note[i])-1, 0); //note off
			j=0;
		}
		else
		{
			j=(int)(480*duration[i]);
		}
	}
	fprintf(outfile,"TrkEnd\n");
}

//=======================================================
void dump_track_1(void)
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

//=======================================================
void read_track_1(void)
{
	int i=0, length;
	char inbuf[2048], *ip,*op;

	ip=inbuf;
	op=inbuf;

	for(i=0;i<128;i++)
	{ // initialise arrays
		note[i]=0.0;
		duration[i]=0.0;
	}

	i=0;

	if(fgets(inbuf,2048,infile)!=0)
	{
		length=strlen(inbuf);

		while((i<length)&&(*op!='\0'))
		{
			if((*op ==',')||(*op=='\0')||(*op==' '))
			{
				*op='\0';
				note[i]=(float)atof(ip);
//				printf("\nNote %f", note[i]);
				ip=op;
				ip++;
				op++;
				i++;
			}
			else
			{
				op++;
			}
		}
	}

	ip=inbuf;
	op=inbuf;
	i=0;
	if(fgets(inbuf,2048,infile)!=0)
	{
		length=strlen(inbuf);

		while((i<length)&&(*op!='\0'))
		{
			if((*op ==',')||(*op=='\0'||(*op==' ')))
			{
				*op='\0';
				duration[i]=(float)atof(ip);
//				printf("\nDuration %f", duration[i]);
				ip=op;
				ip++;
				op++;
				i++;
			}
			else
			{
				op++;
			}
		}
	}
/*
	for(i=0;i<128;i++)
	{
		printf("\ni %d note %f duration %f", i,note[i],duration[i]);

	}
*/


}

