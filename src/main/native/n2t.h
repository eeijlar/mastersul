/* t2n.h */

char infilename[256], outfilename[256];
FILE *infile, *outfile;
int zerotime,lasttime, curnote,curvel,delta,division;
float note[128], duration[128];

void usage_4(void);
void open_files_1(void);
void close_files_1(void);
void write_track_1(void);
void read_track_1(void);
void dump_track_1(void);
void n2t(int);
int n2targs;