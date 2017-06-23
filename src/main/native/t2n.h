/* t2n.h */

char infilename[256], outfilename[256];
FILE *infile, *outfile;
int zerotime,lasttime, curnote,curvel,delta,division;
float note[128], duration[128];

void usage(void);
void open_files(void);
void close_files(void);
void read_header(void);
void read_track(void);
void dump_track(void);
void t2n(int);
int t;