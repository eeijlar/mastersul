/* bp.c
modified by MF 17/2/2000
*/


#define  __BP__

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <signal.h>

#include "bpnet.h"
#include "bp.h"

#include <jni.h>
#include "MainWindow.h"

			 //	0   1     2      3    4    5 
enum option {trainS,train,contin,test,view,reverse};
FILE *errfp ;


static FILE *fp;
static int checkerr;
static int dump;
static int seed;


JNIEXPORT void JNICALL Java_MainWindow_bp 
(JNIEnv *env, jclass j1, jint value, jstring lrn, jstring wts, jstring out, jstring hid, jstring seed,jstring path)
{
/*
Max number of arguments = 6
to train & specify seed:	bp -n seed file.lrn file.wts (5)
to train:             		bp -l file.lrn file.wts (4)
to continue training: 		bp -c file.lrn file.wts (4)
to test:              		bp -t file.lrn file.wts file.out file.hid (6)
to view performance   		bp -v file.lrn file.wts (4)
to reverse:           		bp -r file.lrn file.wts file.out file.hid (6)
*/
	const jbyte *lrnFile;
    const jbyte *wtsFile;
	const jbyte *outFile;
	const jbyte *hidFile;
	const jbyte *seedFile;
	const jbyte *pathFile;


	lrnFile=(*env)->GetStringUTFChars(env,lrn, NULL);
	
	if(lrnFile==NULL) {
		return;
	}

	wtsFile=(*env)->GetStringUTFChars(env,wts, NULL);	

	if(wtsFile==NULL) {
		return;
	}

	outFile=(*env)->GetStringUTFChars(env,out, NULL);

	if(outFile==NULL) {
		return;
	}
	
	hidFile=(*env)->GetStringUTFChars(env,hid, NULL);

	if(hidFile==NULL) {
		return;
	}	

	seedFile=(*env)->GetStringUTFChars(env,seed, NULL);

	if(seedFile==NULL) {
		return;
	}

	pathFile=(*env)->GetStringUTFChars(env,path, NULL);

	if(pathFile==NULL) {
		return;
	}
	
	strcpy(seedArray,seedFile);
	strcpy(lrnArray,lrnFile);
	strcpy(wtsArray,wtsFile);
	strcpy(outArray,outFile);
	strcpy(hidArray,hidFile);
	strcpy(pathArray,pathFile);
	strcat(pathArray,"logFile.log");

	
	bpArray[0]="bp.exe";
	
	switch(value) {

		case trainS: strcpy(option,"-n");
					 argum=5;
					 bpArray[1]=option;
					 bpArray[2]=seedArray;
					 bpArray[3]=lrnArray;
					 bpArray[4]=wtsArray;
			break;
		case train:  strcpy(option,"-l");
					 argum=4;
					 bpArray[1]=option;
					 bpArray[2]=lrnArray;
					 bpArray[3]=wtsArray;
			break;
		case contin: strcpy(option,"-c");
					 argum=4;
					 bpArray[1]=option;
					 bpArray[2]=lrnArray;
					 bpArray[3]=wtsArray;
			break;
		case test:   strcpy(option,"-t");
					 argum=6;
					 bpArray[1]=option;
					 bpArray[2]=lrnArray;
					 bpArray[3]=wtsArray;
					 bpArray[4]=outArray;
					 bpArray[5]=hidArray;
			break;
		case view:   strcpy(option,"-v");
					 argum=4;
					 bpArray[1]=option;
					 bpArray[2]=lrnArray;
					 bpArray[3]=wtsArray;
			break;
	    case reverse: strcpy(option,"-r");
					  argum=6;
					 bpArray[1]=option;
					 bpArray[2]=lrnArray;
					 bpArray[3]=wtsArray;
					 bpArray[4]=outArray;
					 bpArray[5]=hidArray;
					
			break;
		default: strcpy(option,"-n");
				 argum=5;
				 bpArray[1]=option;
				 bpArray[2]=seedArray;
				 bpArray[3]=lrnArray;
				 bpArray[4]=wtsArray;

	}
	
	bp(argum,bpArray); 
		
	(*env)->ReleaseStringUTFChars(env,seed,seedFile);
	(*env)->ReleaseStringUTFChars(env,lrn,lrnFile);
	(*env)->ReleaseStringUTFChars(env,wts,wtsFile);
	(*env)->ReleaseStringUTFChars(env,out,outFile);
	(*env)->ReleaseStringUTFChars(env,hid,hidFile);
	(*env)->ReleaseStringUTFChars(env,path,pathFile);

	return;
	
}

int bp (int argum, char *bpArray[]){

int label, x;

	if(argum ==6) {
		errfp = fopen(pathArray,"a+");
	}
	else { 
		errfp = fopen(pathArray,"w"); 
	}

fprintf(errfp,"Squash Function is %s\n", SQATYP);


fp = getfile ("bpars", "r");
if (fscanf (fp, "%i %i %i %f %f %i %i %i %f %d %d %f",
	      &timeout, &checkerr, &notify, &eta, &momentum, &num_input,
          &num_output, &num_hid, &err_lev, &label,&criteria,&lrn_lev) != 12){
   fprintf (stderr, "bp : parameters wrong\n");
   exit (4);
   }

init_workspace ();
fprintf (errfp, "%i %i %i %f %f %i %i %i %f %d %d %f\n",
	      timeout, checkerr, notify, eta, momentum, num_input, num_output,
	      num_hid, err_lev, label,criteria,lrn_lev);

for (x = 0; x < argum; x++){
    fprintf(errfp, "%s ", bpArray[x]);
    }
fprintf (errfp, "\n");

srand(1) ;
if   (argum < 2)
     usage_2 (0);
else if (*bpArray[1] == '-')
        {
        if   (bpArray[1][2] != '\0')
	     sscanf (&(bpArray[1][2]), "%d", &seed);
        else seed = 13;
        fprintf (errfp, "seed = %d\n", seed);

        if (seed <= 0)
           seed = seed *= -1 ;
        RND_seed (seed);
        srand(seed) ;
  switch ((int) (bpArray[1])[1]){
/* training with command line specified seed number */
	case 'n': if (argum == 5){
	             if  (sscanf (bpArray[2], "%d", &seed) != 1)
		         usage_2 (0);
	             else{
		         RND_seed (seed);
                         printf ("seed = %d\n", seed);
		         learn (label, bpArray[3], bpArray[4]);
		         }
	             }
	          else usage_2 (0);
	          break ;
/* training */
    case 'l': if   (argum == 4)
	               learn (label, bpArray[2], bpArray[3]);
	          else usage_2 (0);
	          break;
/* continue training */
	case 'c': if   (argum == 4) {
	               cont_learn (label, bpArray[2], bpArray[3]);
			  	//	printf("Got here");
			  }
			  else  {usage_2 (0);}
	          break;
/* view performance */
	case 'v': if   (argum == 4)
	               report_learn (label, bpArray[2], bpArray[3]);
	          else usage_2 (0);
	          break;
/* test */
	case 't': if   (argum == 6)
	               test_net (label, bpArray[2], bpArray[3], bpArray[4], bpArray[5]);
	          else usage_2 (0);
	          break;
/* added by MF */
/* reverse test */
			  case 'r': if   (argum == 6)
	               reverse_net (label, bpArray[2], bpArray[3], bpArray[4], bpArray[5]);
	          else usage_2 (0);
	          break;


	default:  usage_2 (0);
	}
    }
else usage_2 (0);

fclose(errfp) ;

return (0);
} /*  end of function |main| */

/*************************************************/
void learn (int label, char *ts, char *save){

FILE *fts;
FILE *fnet;
Teachset_t *tset;
Network_t *net;
char *wtsbak;
wtsbak = (char *) calloc (1024, sizeof (char));
strcpy (wtsbak, save);
strcat (wtsbak, ".log"); 
fts = getfile (ts, "r");

net = create_network ();
fnet = getfile (wtsbak, "w");
save_weights (fnet, net);
fclose (fnet);
fprintf(errfp," network created\n");

tset = read_in_data (fts,label);
fprintf(errfp," data read in\n");
fflush (errfp);
fclose (fts);

learn_cycle (tset, net, wtsbak);

fprintf(errfp, "Saving weights ....\n");

fnet = getfile (save, "w");
save_weights (fnet, net);

fprintf(errfp, "finish \n");

fclose (fnet);
free (wtsbak);
free_teachset (tset);
free_network (net);

}  /*  end of function |learn| */


/*************************************************/
void cont_learn (int label, char *ts, char *save){
FILE *fts;
FILE *fnet;
Teachset_t *tset;
Network_t *net;
char *wtsbak;

wtsbak = (char *) calloc (1024, sizeof (char));
strcpy (wtsbak, save);
strcat (wtsbak, ".log");

fnet = getfile (save, "r");
net = get_network (fnet);
fclose (fnet);
fflush(errfp) ;
fts = getfile (ts, "r");
tset = read_in_data (fts,label);
fclose (fts);
fprintf(errfp," data read in\n");
fflush(errfp) ;

learn_cycle (tset, net, wtsbak);
fnet = getfile (save, "a+");
fprintf(errfp, "Saving weights ....\n");
save_weights (fnet, net);
fprintf(errfp, "finish \n");

fclose (fnet);
free (wtsbak);
free_teachset (tset);
free_network (net);
} /*  end of function |cont_learn| */


/*************************************************/
void usage_2 (int i){

fprintf(stderr,"\bp -[l[Seed]cvt] file[.lrn|.tst] file.wts [file.out file.hid]\n");
exit (i);
}  /*  end of function |usage_2| */


/*************************************************/
FILE *getfile (char *fn, char *os){

FILE *ret;

ret = fopen (fn, os);
if   (ret == NULL){
     fprintf(errfp, "Error: cannot open file %s\n", fn);
     exit (0);
     }
else return ret;
} /*  end of function |getfile| */


/*************************************************/
void test_net(int label,char *tfname,char *nfname,char *ofname,char *hfname){

Network_t *net;
Teachset_t *tset;
FILE *tfile;
FILE *nfile;
FILE *ofile;
FILE *hfile;

nfile = getfile (nfname, "r");
net = get_network (nfile);
fclose (nfile);

tfile = getfile (tfname, "r");
tset = read_in_data (tfile,label);
fclose (tfile);

ofile = getfile (ofname, "w");
hfile = getfile (hfname, "w");

test_cycle (ofile, hfile, net, tset);

fprintf(errfp, "finish test cycle\n");
fclose (ofile);
fclose (hfile);
free_teachset (tset);
free_network (net);
} /*  end of function |test_net| */


/*************************************************/
void reverse_net(int label,char *tfname,char *nfname,char *ofname,char *hfname){

Network_t *net;
Teachset_t *tset;
FILE *tfile;
FILE *nfile;
FILE *ofile;
FILE *hfile;

nfile = getfile (nfname, "r");
net = get_network (nfile);
fclose (nfile);

tfile = getfile (tfname, "r");
tset = reverse_read_in_data (tfile,label); /* tset holds a teachset linked list with input and output vectors */
fclose (tfile);

ofile = getfile (ofname, "w");
hfile = getfile (hfname, "w");

test_cycle (ofile, hfile, net, tset);

fprintf(errfp, "finished reverse cycle\n");
fclose (ofile);
fclose (hfile);
free_teachset (tset);
free_network (net);
} /*  end of function |reverse_net| */


/*************************************************/


/*************************************************/
void report_learn (int label, char *ts, char *save){

FILE *fts;
FILE *fnet;
Teachset_t *tset;
Network_t *net;
unsigned number_learnt = 0;

fnet = getfile (save, "r");
net = get_network (fnet);
fclose (fnet);

fts = getfile (ts, "r");
tset = read_in_data (fts,label);
fclose (fts);

number_learnt = report_cycle (net, tset);

free_teachset (tset);
free_network (net);
} /*  end of function |report_learn| */


/*************************************************/
void set_input (Network_t *net, float *s){

int i;
	for (i = 0; i < num_input ; i++) 
     net->input[i] = s[i];
	 net->input[num_input] = 1.0;
	
} /*  end of function |set_input| */

/*************************************************/
void reverse_set_input (Network_t *net, float *s){

int i;
for (i = 0; i < num_output; i++)
     net->output[i] = s[i];
//net->output[num_output] = 1.0;
} /*  end of function |set_input| */

/*************************************************/
void learn_cycle(Teachset_t *tset, Network_t *net,char *logfn){

float error, percent_learned;
int notif;
int epoch = 0;
Teachset_t *tsp;
FILE *log;
bool crit, maxt ;
bool finish = FALSE;
float max_error = 0.0;	/* holds the maximum error */
float ms_error =  0.0;	/* holds the mean square error*/
float dms_error = 1.0;	/* holds the difference of mean square error*/
float lms_error = 1.0;	/* holds the last mean square error*/
int no_learnt  = 0;
int no_learnt1 = 0;
dump = FALSE;

signal (SIGTERM, onintr);

finish = FALSE;

while(!finish && (epoch < timeout)){
     epoch++;
     /*     patc=0;*/
     notif = ((epoch % notify) == 0);
     finish = FALSE;
     {
     no_learnt = 0;
     no_learnt1= 0;
     error     = 0.0;
     ms_error  = 0.0;
     max_error = 0.0;
     tsp = tset;

     while(tsp != NULL){
          set_input (net, tsp->vecin);
          if  (notif){
	      error      += err_forward(net,tsp->vecout,&max_error,&crit,&maxt);
	      no_learnt  += crit ;
              no_learnt1 += maxt ;
              ms_error   += (error*error) ;
              }
          else{
              forward (net);
              }
          backward(net,tsp->vecout);
          tsp = tsp->next;
          }
     if  (notif){
         if   (criteria == 0)
              percent_learned = no_learnt/patcnt ;
         else if (criteria == 1)
              percent_learned = no_learnt1/patcnt ;
         ms_error = (ms_error/patcnt) ;
         /*printf("ms= %f\n",ms_error) ;*/
         if  (ms_error < lms_error)
                dms_error = (1.0 - (ms_error/lms_error)) * 100.0 ;
         else dms_error = 1.0 ;
         lms_error = ms_error ;
         if (criteria < 2 && percent_learned >= lrn_lev )
              finish = TRUE ;
         else if (criteria == 2 && dms_error < lrn_lev)
                  finish = TRUE ;
         }
     }

     if ((notif || dump) || finish){
	if (dump){
	   no_learnt = 0;
	   max_error = 0.0;
	   ms_error  = 0.0;
           error = 0.0;
           tsp = tset;

	   while(tsp != NULL){
                 set_input (net, tsp->vecin);
		 error += err_forward(net,tsp->vecout,&max_error,&crit,&maxt);
		 no_learnt  += crit ;
                 no_learnt1 += maxt ;
                 ms_error   += (error*error) ;
                 tsp = tsp->next;
                 }
           if   (criteria == 0)
                 percent_learned = no_learnt/patcnt ;
           else if (criteria == 1)
                   percent_learned = no_learnt1/patcnt ;
           ms_error = ms_error/patcnt ;
           if  (ms_error < lms_error)
                dms_error = (1.0 - (ms_error/lms_error)) * 100.0 ;
           else dms_error = 1.0 ;
           lms_error = ms_error ;
           if (criteria < 2 && percent_learned >= lrn_lev )
                finish = TRUE ;
           else if (criteria == 2 && dms_error < lrn_lev)
                    finish = TRUE ;
           fprintf (errfp, "Learned = %i %i Patcnt = %i Perlrn = %1.12f  %1.12f\n",
                    no_learnt,no_learnt1,patcnt,percent_learned,lrn_lev) ;
	   }

        log = getfile (logfn, "w");
	save_weights (log, net);
	fclose (log);
	fprintf(errfp,"epoch = %d  err = %1.6f Maxerr = %1.6f ",
                       epoch,error,max_error);
	fprintf(errfp,"no learnt = %8d %3.1f%% %8i %3.1f%%\n",
                       no_learnt,((float)no_learnt/patcnt)*100.0,no_learnt1,
                       ((float)no_learnt1/patcnt)*100.0);
	fflush(errfp) ;

	if (dump)
	   epoch = timeout;
	}
     }
if   ((epoch == timeout) && (!dump))
     fprintf(errfp,"Timeout \n");
else if (dump)
        fprintf(errfp,"Dumped weights\n");
else if (finish)
        fprintf(errfp,"finished learning\n");
else fprintf(errfp,"Halt for unknown reason\n");
}  /*  end of function |learn_cycle| */


/*************************************************/
void onintr (int junk){

dump = TRUE;
}  /*  end of function |onintr| */


/*************************************************/
void test_cycle (FILE *ofile, FILE *hfile,Network_t *net, Teachset_t *tset){

int j;

while(tset != NULL){
     set_input (net, tset->vecin);
     forward (net);
     for (j = 0; j < num_hid; j++)
         fprintf(hfile, "%f ", net->hidden[j]);
     for (j = 0; j < num_output; j++)
    	 fprintf(ofile, "%f ", net->output[j]);
     fprintf(hfile, "\n");
     fprintf(ofile, "\n");
     fflush (ofile);
     fflush (hfile);
     tset = tset->next;
     }
}  /*  end of function |test_cycle| */


/*************************************************/
void reverse_cycle (FILE *ofile, FILE *hfile,Network_t *net, Teachset_t *tset){

int j;

/* MF: a lot of stuff has to be done here... */

while(tset != NULL){
     set_input (net, tset->vecin);
     forward(net);
     for (j = 0; j < num_hid; j++)
         fprintf(hfile, "%f ", net->hidden[j]);
     for (j = 0; j < num_input; j++)
    	 fprintf(ofile, "%f ", net->input[j]); /* we're getting the output from the former inputs */
     fprintf(hfile, "\n");
     fprintf(ofile, "\n");
     fflush (ofile);
     fflush (hfile);
     tset = tset->next;
     }
}  /*  end of function |reverse_cycle| */


/*************************************************/
unsigned report_cycle (Network_t *net,Teachset_t *tset){

#define  BP_LEARNT_FNAM "bp_learnt"
#define  BP_UNLEARNT_FNAM "bp_unlearnt"
#define  BP_ERRORS_FNAM "bp_err"

int j, fin;
bool crit, maxt;
int number_learnt  = 0;
int number_learnt1 = 0;
float max_error    = 0.0;
float error        = 0.0;
float total_error  = 0.0;
float fnol, fnol1 ;

unsigned tset_pattern_no = 0;
FILE *lfp;			/* patterns learnt file pointer */
FILE *ulfp;			/* file pointer for patterns not learnt */
FILE *efp;			/* errors file pointer */

if ((lfp = fopen (BP_LEARNT_FNAM, "w")) == NULL){
   fprintf(errfp, "failed to open file \"%s\"\n", BP_LEARNT_FNAM);
   exit (2);
   }
if ((ulfp = fopen (BP_UNLEARNT_FNAM, "w")) == NULL){
   fprintf(errfp, "failed to open file \"%s\"\n", BP_UNLEARNT_FNAM);
   exit (2);
   }
if ((efp = fopen (BP_ERRORS_FNAM, "w")) == NULL){
   fprintf(errfp, "failed to open file \"%s\"\n", BP_ERRORS_FNAM);
   exit (2);
   }

while(tset != NULL){
     fprintf(efp, "---------------------\n") ;
     set_input (net, tset->vecin) ;
     error = err_forward(net,tset->vecout,&max_error,&crit,&maxt) ;
     number_learnt  += crit ;
     number_learnt1 += maxt ;
     total_error += error;
     fprintf(efp, "%d:\terr = %f\n", tset_pattern_no, error) ;
     if   (criteria == 0)
          fin = (int)crit ;
     else fin = (int)maxt ;
     fprintf(lfp, "%d\n",fin);

     if  (fin == 0){
    	 unsigned k;
         fprintf(ulfp, "T ");
         for (k = 0; k < num_output; ++k){
    	     fprintf(ulfp, "%f ", tset->vecout[k]);
    	     }
    	 fprintf(ulfp, "\nO ");
    	 for (k = 0; k < num_output; ++k){
             fprintf(ulfp, "%f ", net->output[k]);
             }
         fprintf(ulfp, "\n");
         }

     fprintf(efp, "I ");
     for (j = 0; j < num_inp_bias; j++)
         fprintf(efp, "%f ", net->input[j]);
     fprintf(efp, "\nH ");
     for (j = 0; j < num_hid_bias; j++)
    	 fprintf(efp, "%f ", net->hidden[j]);
     fprintf(efp, "\nO ");
     for (j = 0; j < num_output; j++)
         fprintf(efp, "%f ", net->output[j]);
     fprintf(efp, "\nT ");
     for (j = 0; j < num_output; j++)
         fprintf(efp, "%f ", tset->vecout[j]);
     fprintf(efp, "\nD ");
     for (j = 0; j < num_output; j++)
         fprintf(efp, "%f ", (fabs (tset->vecout[j] - net->output[j])));
     fprintf(efp, "\n");
     tset = tset->next;
     ++tset_pattern_no;
     }
fprintf(errfp,"\n") ;
fnol  = (float) number_learnt/patcnt ;
fnol1 = (float) number_learnt1/patcnt ;

fprintf(efp,"no learnt = %8d %3.1f%% %8i %3.1f%%\nmax error = %1.12f total error = %1.12f\n",
	   number_learnt,fnol*100.0,number_learnt1,fnol1*100.0,max_error, total_error);
fprintf(errfp,"no learnt = %8d %3.1f%% %8i %3.1f%%\nmax error = %1.12f total error = %1.12f\n",
	   number_learnt,fnol*100.0,number_learnt1,fnol1*100.0,max_error, total_error);

fflush(errfp) ;
fclose (lfp);
fclose (ulfp);
fclose (efp);

return (number_learnt);
}  /*  end of function |report_cycle| */


/*************************************************/
Teachset_t *teachset_alloc (Teachset_t *old){

struct teachset *ret;
ret = (struct teachset *) calloc (1, sizeof (struct teachset));

if (ret == NULL){
   fprintf(errfp, "teachset_alloc:0 can't allocate memory\n");
   exit (0);
   }
ret->vecout = (float *) calloc(num_output,   sizeof(float)) ;
ret->vecin  = (float *) calloc(num_input,sizeof(float)) ;

if ((ret->vecout == NULL) || (ret->vecin == NULL)){
   fprintf(errfp, "teachset_alloc:1 can't allocate memory\n");
   exit (0);
   }
ret->next = old;
return ret;
}  /*  end of function |teachset_alloc| */

/*************************************************/
Teachset_t *read_in_data (FILE * ifile,int label){

int lablen = 0 ;
unsigned line_no = 0, vector_el_no;
unsigned long pattern_count, hash_count;
struct teachset *tmp, *ret = NULL;

char *line_buf;		/* buffer to hold line of data */
char *buf_ptr;		/* pointer into line buffer */
char *n_ptr;		/* pointer for next value to convert in |line_buf| */

#define BUF_SIZE 8192

if ((line_buf = (char *) malloc (BUF_SIZE)) == NULL){
   perror ("read_in_data(): run out of memory\n");
   exit (2);
   }

if ((buf_ptr = fgets (line_buf, BUF_SIZE, ifile)) == NULL){
   fprintf(errfp, "read_in_data(): read past end of data file\n");
   fprintf(errfp, "\t\t line %d\n", line_no);
   exit (2);
   }


pattern_count = strtoul (buf_ptr, &n_ptr, 10); /* read the first line with number of lines stated */

hash_count = 0 ;

for (line_no = 0; line_no < pattern_count; line_no++){
    if  (ret == NULL) /* check if we have initialised ret yet */
        tmp = ret = teachset_alloc (NULL); /* point ret to tmp which is the allocated teachset */
    else{
	tmp->next = teachset_alloc (NULL); /* otherwise, keep linking through the teachset */
	tmp = tmp->next; /* point to next number */
	}

    if ((buf_ptr = fgets (line_buf, BUF_SIZE, ifile)) == NULL){
       fprintf(errfp, "read_in_data(): read past end of data file\n");
       fprintf(errfp, "\t\t line %d\n", line_no);
       exit (2);
       }

         if (label == TRUE){
             sscanf(buf_ptr, "%*s%n",&lablen);
             buf_ptr += lablen ;
             }

/* do the input vector */
         for (vector_el_no = 0; vector_el_no < num_input; ++vector_el_no){
             tmp->vecin[vector_el_no] = strtod (buf_ptr, &n_ptr);
             if (n_ptr == buf_ptr){
                fprintf(errfp,
	               "read_in_data(): num_input[%d] strtod() failed?, pattern %d \"%s\"\n",
	               vector_el_no, line_no, buf_ptr);
                exit (4);
                }
             buf_ptr = n_ptr;
             }
/* do the output vector */
         for (vector_el_no = 0; vector_el_no < num_output; ++vector_el_no){
             tmp->vecout[vector_el_no] = strtod (buf_ptr, &n_ptr);
             if (n_ptr == buf_ptr){
                fprintf(errfp,
	               "read_in_data(): num_output[%d] strtod() failed?, pattern %d \"%s\"\n",
	               vector_el_no, line_no, buf_ptr);
                exit (4);
                }
             buf_ptr = n_ptr;
             }
    }
  noi = pattern_count;
  patcnt = pattern_count - hash_count ;
  fprintf(errfp,"patcnt = %i ",patcnt) ;

  return ret; /* ret holds a linked list of the type teachset */

#undef BUF_SIZE
}/*  end of function |read_in_data| */

/*************************************************/
Teachset_t *reverse_read_in_data (FILE * ifile,int label){

int lablen = 0 ;
unsigned line_no = 0, vector_el_no;
unsigned long pattern_count, hash_count;
struct teachset *tmp, *ret = NULL;

char *line_buf;		/* buffer to hold line of data */
char *buf_ptr;		/* pointer into line buffer */
char *n_ptr;		/* pointer for next value to convert in |line_buf| */

#define BUF_SIZE 8192

if ((line_buf = (char *) malloc (BUF_SIZE)) == NULL){
   perror ("read_in_data(): run out of memory\n");
   exit (2);
   }

if ((buf_ptr = fgets (line_buf, BUF_SIZE, ifile)) == NULL){
   fprintf(errfp, "read_in_data(): read past end of data file\n");
   fprintf(errfp, "\t\t line %d\n", line_no);
   exit (2);
   }


pattern_count = strtoul (buf_ptr, &n_ptr, 10); /* read the first line with number of lines stated */

hash_count = 0 ;

for (line_no = 0; line_no < pattern_count; line_no++){
    if  (ret == NULL) /* check if we have initialised ret yet */
        tmp = ret = teachset_alloc (NULL); /* point ret to tmp which is the allocated teachset */
    else{
	tmp->next = teachset_alloc (NULL); /* otherwise, keep linking through the teachset */
	tmp = tmp->next; /* point to next number */
	}

    if ((buf_ptr = fgets (line_buf, BUF_SIZE, ifile)) == NULL){
       fprintf(errfp, "read_in_data(): read past end of data file\n");
       fprintf(errfp, "\t\t line %d\n", line_no);
	   fprintf(errfp, "Ensure that the stimulus file (rawdata file) contains the correct number\n");
	   fprintf(errfp, "of rows printed at the top of the file \n");	
       exit (2);
       }

         if (label == TRUE){
             sscanf(buf_ptr, "%*s%n",&lablen);
             buf_ptr += lablen ;
             }

/* do the input vector */
         for (vector_el_no = 0; vector_el_no < num_input; ++vector_el_no){
             tmp->vecin[vector_el_no] = strtod (buf_ptr, &n_ptr);
             if (n_ptr == buf_ptr){
                fprintf(errfp,
	               "read_in_data(): num_input[%d] strtod() failed?, pattern %d \"%s\"\n",
	               vector_el_no, line_no, buf_ptr);
				   fprintf(errfp, "You may have attempted to train with too many files\n");
				   fprintf(errfp, "Ensure that the number of columns in your raw data file\n");
				   fprintf(errfp, "is greater than the number of files you trained with.\n");
                exit (4);
                }
             buf_ptr = n_ptr;
             }
/* do the output vector
         for (vector_el_no = 0; vector_el_no < num_output; ++vector_el_no){
             tmp->vecout[vector_el_no] = strtod (buf_ptr, &n_ptr);
             if (n_ptr == buf_ptr){
                fprintf(errfp,
	               "read_in_data(): num_output[%d] strtod() failed?, pattern %d \"%s\"\n",
	               vector_el_no, line_no, buf_ptr);
                exit (4);
                }
             buf_ptr = n_ptr;
             } */
    }
  noi = pattern_count;
  patcnt = pattern_count - hash_count ;
  fprintf(errfp,"patcnt = %i ",patcnt) ;

  return ret; /* ret holds a linked list of the type teachset */

#undef BUF_SIZE
}/*  end of function |reverse_read_in_data| */


/*************************************************/
void dump_data (Teachset_t * kkk){

int i;
fprintf(errfp,"no of patterns = %i\n", noi);
while (kkk != NULL){
      for (i = 0; i < num_input; i++)
	  fprintf(stdout,"%f ", kkk->vecin[i]);
      fprintf(stdout,"\n\t\t");
      for (i = 0; i < num_output; i++)
	  fprintf(stdout,"%f ", kkk->vecout[i]);
      fprintf(stdout,"\n");
      kkk = kkk->next;
      }
}/*  end of function |dump_data| */

/*************************************************/
void free_teachset (Teachset_t * ts){

Teachset_t *tmp;
while (ts != NULL){
      tmp = ts;
      ts = ts->next;
      if (tmp->vecin != NULL){
	 free (tmp->vecin);
	 free (tmp->vecout);
	 }
      free (tmp);
      }
}  /*  end of function |free_teachset| */


void RND_seed (unsigned int ff)
{
int i;

for (i = 0; i < ff; i++){
    seed = RANDOM ();
    }
}   /*  end of function |RND_seed| */


/*************************************************/
double RND (void){

int a, b, y;
double rn;

a = 5;
b = 9999;
y = (a * seed) % b;
seed = RANDOM ();
rn = fabs ((double) y);
do  rn = rn / 10.0;
    while (rn >= 1.0);
rn = (((rn - 0.1) * 5.5 / 4.5) * (UPPER - LOWER)) + LOWER;
if ((rn > UPPER) || (rn < LOWER))
    return RND ();
else
    return (rn);
} /*  end of function |RND| */


/*************************************************/
/*************************************************/