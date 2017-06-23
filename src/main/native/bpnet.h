/* Global data for net */

#ifdef __BPNET__
int criteria, maxtar, patcnt, timeout, notify;
float eta, momentum;
unsigned int num_input;
unsigned int num_output;
unsigned int num_inp_bias ;
unsigned int num_hid  ;
unsigned int num_hid_bias ;
unsigned int noi  ;
float err_lev, lrn_lev;
#else
extern int criteria, maxtar, patcnt, timeout, notify;
extern float eta, momentum;
extern unsigned int num_input;
extern unsigned int num_output;
extern unsigned int num_inp_bias ;
extern unsigned int num_hid  ;
extern unsigned int num_hid_bias ;
extern unsigned int noi  ;
extern float err_lev, lrn_lev;
#endif

#define SIGMOID
/*#define SYMSIG*/
/*#define TANH1*/
/*#define TANH*/
#define RANDOM() rand()
#define SRANDOM() srand()

#define squ(x) ((x)*(x))

#ifdef SIGMOID  
#define SQATYP "Sigmoid"
#define BOTTOM 0.00001
#define SQUASH(x) (float)( 1.0 / ( 1.0 + exp (-(double)(x) ) ) )
#define DERIV(x) (float)(x *(1.0 - x))
#elif defined SYMSIG
#define BOTTOM -0.99999
#define SQATYP "Symsig"
#define SQUASH(x) (float) ((1.0 - exp(-(double)(x))) / (1.0 + exp(-(double)(x))))
#define DERIV(x) (float) ((1.0 + (double)x) *(1.0 - (double)x))
#elif defined TANH1
#define SQATYP "Tanh1"
#define BOTTOM -0.99999
#define SQUASH(x) (float) (2.0 / (1.0 + exp(-2.0*(double)x)) - 1.0) 
#define DERIV(x) (float) ((1.0 + (double)x) *(1.0 - (double)x))
#elif defined TANH
#define SQATYP "Tanh"
#define BOTTOM -0.99999
#define SQUASH(x) (float) tanh((double)x)
#define DERIV(x) (float) (1.0 - ((double)x*(double)x))
#endif

#define ZEROERR
#define CONTMOM
#define MOMDO
#define WIND_SIZE 1000
#define AV_CUTOFF 0.001
#define MAX_CUTOFF 0.01
#define TRIG 0.05
#define UPPER (0.5)
#define LOWER (-0.5)

typedef int bool;

typedef struct network{
	float *input;
	float *whi;
	float *dwhi;
	float *hidden;
	float *woh;
	float *dwoh;
	float *output;
        } Network_t;

void forward (Network_t *net);
void init_workspace (void);
void backward(Network_t *net, float *tpat);
void matrix_update(unsigned s,float *m,float *c);
float err_forward(Network_t *net,float *tpat,float *max,bool *crit,bool *maxt);
Network_t *alloc_network (void);
Network_t *create_network (void);
void save_weights (FILE *sfile, Network_t *net);
Network_t *get_network (FILE *sfile);
void free_network (Network_t *net);
void onintr (int junk);
void freeze(void);