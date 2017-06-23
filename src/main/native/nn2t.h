/* nn2t.h */

char infilename[256], outfilename[256];
FILE *infile, *outfile;
int zerotime,lasttime, curnote,curvel,delta,division, yearpoint, winsize, records,jj;
float note[128], duration[128];


void usage_4(void);
void open_files_1(void);
void close_files_1(void);
void write_track_1(int);
void read_track_1(void);
void dump_track_1(void);
int num_lines(void);
void nn2t(int);
int nn2targs;