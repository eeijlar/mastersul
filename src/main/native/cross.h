/* cross.h */
/* 3/3/2000 JMF */
#define MAXNAMES 220
#define MAXPOINTS 400

FILE *infile, *outfile;
static char infilename[256], outfilename[256];
static double allrain[MAXNAMES][MAXPOINTS];
static char allnames[MAXNAMES][128];
static int lastname, lastpoint, maxpoint;

void usage_3 (void);
void open_files_2( void );
void close_files_2(void);
void read_record(void);
void write_record(void);
void stripstring( char *instr );
void init_workspace_1(void);
void cross (int);
int args;


