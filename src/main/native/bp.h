#define M1      259200           /*Magic numbers from Recipes*/
#define IA1     7141
#define IC1     54773
#define RM1     (1.0/M1)
#define M2      134456
#define IA2     8121
#define IC2     28411
#define RM2     (1.0/M1)
#define M3      243000
#define IA3     4561
#define IC3     51349
/*end of stuff for random number generator */

#define TRUE 1
#define FALSE 0

/*#define DISPLAY*/
/*#define BATCH*/

typedef struct teachset{
	float *vecout;
	float *vecin;
	struct teachset *next;
        } Teachset_t;

int argum;
char *bpArray[256];
static char lrnArray[256];
static char wtsArray[256];
static char outArray[256];
static char hidArray[256];
static char pathArray[256];
static char option[2];
static char seedArray[256];


int bp (int argum, char *bpArray[]);
void learn (int label, char *ts, char *save);
void cont_learn (int label, char *ts, char *save);
void usage_2 (int i);
FILE *getfile (char *fn, char *os);
void test_net(int label,char *tfname,char *nfname,char *ofname,char *hfname);
void report_learn (int label, char *ts, char *save);
void set_input (Network_t *net, float *s);
void learn_cycle (Teachset_t *tset,Network_t *net,char *logfn);
void onintr (int junk);
void test_cycle(FILE *ofile,FILE *hfile,Network_t *net,Teachset_t *tset);
unsigned report_cycle (Network_t *net, Teachset_t *tset);
Teachset_t *teachset_alloc (Teachset_t *old);
Teachset_t *read_in_data (FILE *ifile, int label);
Teachset_t *reverse_read_in_data (FILE *ifile, int label);
void dump_data (Teachset_t *kkk);
void free_teachset (Teachset_t *ts);
void reverse_net(int label,char *tfname,char *nfname,char *ofname,char *hfname);
void reverse_cycle (FILE *ofile, FILE *hfile,Network_t *net, Teachset_t *tset);
void reverse_set_input (Network_t *net, float *s);


/*Original random number functions */
void   RND_seed (unsigned int ff);
double RND (void);

/*Numerical Recipes Random Number Generator */
double ran(int idum);
