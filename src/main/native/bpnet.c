/* bpnet.c
modified by MF 17/2/2000
*/

#define __BPNET__

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <conio.h> /* due to _getch for debug */

#include "bpnet.h"
#include "bp.h"

static float *dov; 

/**********************************************/
// for all hidden nodes
//	go through all input nodes
//	accumulate in x whi
// store squash in hidden node
// for all output nodes
//	go through all hidden nodes
//	accumulate hidden x woh
// store in output node
/**********************************************/
void forward (Network_t * net){
  
int i, j, k;
float m ;
for (i = 0, k = 0; i < num_hid; i++){	
    m = 0.0;
    for (j = 0; j < num_input ; j++){ 
        m += net->input[j] * net->whi[k]; 
        k++; 
	}
    net->hidden[i] = SQUASH((m+net->whi[k]));
    k++;
    }
net->hidden[num_hid] = 1.0; /* set the last hidden node to 1 */

for (i = 0, k = 0; i < num_output; i++){
    m = 0.0;
    for (j = 0; j < num_hid_bias; j++){
		m += net->hidden[j] * net->woh[k];
        ++k;
	}
    net->output[i] = SQUASH(m);

    }
}   /*  end of function |forward| */

/**********************************************/
void reverse (Network_t * net){
  
int i, j, k;
float m ;

/* MF: this stuff has to change a lot */

for (i = 0, k = 0; i < num_hid; i++){											
    m = 0.0;
    for (j = 0; j < num_output ; j++){ 
        m += net->output[j] * net->woh[k]; 
        k++; 
	}
    net->hidden[i] = SQUASH((m+net->woh[k]));
    k++;
    }

net->hidden[num_hid] = 1.0; 


for (i = 0, k = 0; i < num_input; i++){
    m = 0.0;
    for (j = 0; j < num_hid_bias; j++){
	m += net->hidden[j] * net->whi[k];
        ++k;
	}
    net->input[i] = SQUASH(m);
    }
}   /*  end of function |reverse| */


/*************************************************/
void init_workspace (void){
  
dov = (float *) calloc (num_output, sizeof (float)) ;

}  /*  end of function |init_workspace| */


/*************************************************/
void backward (Network_t *net,float *tpat){
   
int i, j, k, l;
float m;

/* propagate errors from output to hidden */
for (i = 0, k = 0; i < num_output; i++){
#ifdef ZEROERR
    if   (net->output[i] < BOTTOM)
         dov[i] = (tpat[i] * 0.1 * eta);
    else if  (net->output[i] > 0.99999)
	     dov[i] = ((tpat[i] - 1) * 0.1 * eta);
    else
#endif
    dov[i] = (tpat[i] - net->output[i]) * DERIV(net->output[i]) * eta ;
    for (l = 0; l < num_hid_bias; l++, k++){
#ifdef MOMDO
        net->dwoh[k] = (dov[i] * net->hidden[l]) + momentum * net->dwoh[k];
#else
	net->dwoh[k] = (dov[i] * net->hidden[l]);
#endif
        }
    }

/* propogate errors from hidden to input */

for (i = 0, k = 0; i < num_hid; i++){
    m = 0.0 ;
    for (j = 0; j < num_output; j++)
	m += dov[j] * net->woh[i + j * num_hid_bias] ;
    /*m = m * (net->hidden[i]) * (1 - net->hidden[i]) ;*/
    m *= DERIV(net->hidden[i]) ;
    for (j = 0; j < num_inp_bias; j++, k++){
#ifdef MOMDO
        net->whi[k] += (net->dwhi[k] = m * net->input[j] + momentum * net->dwhi[k]) ;
#else
        net->whi[k] += m * net->input[j] ;
#endif
        }
   }
 
/*************************************************/
matrix_update((num_hid_bias * num_output), net->woh, net->dwoh);

} /*  end of function |backward| */


/*************************************************/
void matrix_update (unsigned s, float *m, float *c){

int i;
for (i = 0; i < s ; i++){
    m[i] += c[i];
    }
}  /*  end of function |matrix_update| */


/*************************************************/
float err_forward(Network_t *net,float *tpat,float *max,bool *crit,bool *maxt){
  
int i, j, k, maxoi, maxti;
float m, maxov, maxtv;
float err;
float terr;
*crit = TRUE ;

*maxt = FALSE ;
err = 0.0;

for (i = 0, k = 0; i < num_hid; i++){
    m = 0.0;
    for (j = 0; j < num_input; j++,k++){
	m += net->input[j] * net->whi[k];
        }
    net->hidden[i] = SQUASH((m+net->whi[k]));
    k++ ;
    }

net->hidden[num_hid] = 1.0;
maxov  = 0.0 ;
maxtv  = 0.0 ;
maxoi  = -1  ;
maxti  = -1  ;

for (i = 0, k = 0; i < num_output; i++)
    {
    m = 0.0;
    for (j = 0; j < num_hid_bias; j++,k++)
	m += net->hidden[j] * net->woh[k];
    net->output[i] = SQUASH(m);
    if (net->output[i] > maxov){
       maxoi = i ;
       maxov = net->output[i] ;
       }
    if (tpat[i] > maxtv){
       maxti = i;
       maxtv = tpat[i];
       }
    terr    = fabs (net->output[i] - tpat[i]);
    *crit   = *crit && (terr < err_lev) ;
    err    += terr * terr ;
    *max    = (terr > *max) ? terr : *max;
    }

if (maxoi == maxti)
   *maxt = TRUE ;

return err;
} /*  end of function |err_forward| */


/*************************************************/
/* modified by MF, removed "hid" in parameters, as it is a global */
Network_t *alloc_network(void){

Network_t *ret;
ret = (Network_t *) calloc (1, sizeof (Network_t));
num_hid_bias = num_hid + 1 ;
num_inp_bias = num_input + 1 ;
ret->input  = (float *) calloc(num_inp_bias, sizeof (float))      ;
ret->hidden = (float *) calloc((num_hid_bias), sizeof (float))              ;
ret->output = (float *) calloc(num_output, sizeof (float))              ;

ret->woh    = (float *) calloc(num_output * num_hid_bias , sizeof (float))      ;
ret->whi    = (float *) calloc(num_hid * num_inp_bias, sizeof (float));

ret->dwoh   = (float *) calloc(num_output * num_hid_bias, sizeof (float))       ;
ret->dwhi   = (float *) calloc(num_hid * num_inp_bias, sizeof (float));

//printf("\ninputs %d hidden %d outputs %d woh %d whi %d dwoh %d dwhi %d", 
//	   num_inp_bias, num_hid_bias, num_output, num_output * num_hid_bias,
//	   num_hid * num_inp_bias, num_output * num_hid_bias, num_hid * num_inp_bias);

return ret;
} /*  end of function |alloc_network| */



/*************************************************/
Network_t *create_network (void){

Network_t *ret;
int i;

ret = alloc_network();

for(i = 0; i < (num_hid_bias * num_output) ; i++)
    ret->woh[i] = (float) RND ();
for(i = 0; i < (num_hid * num_inp_bias) ; i++){
    ret->whi[i] = (float) RND ();
    }
return ret;
} /*  end of function |create_network| */


/*************************************************/
void save_weights (FILE *sfile, Network_t *net){
  int i, j, k;
fprintf(sfile, "%d %d\n", num_hid, 0);
  
for (i = 0, k = 0; i < num_hid; i++){
    for (j = 0; j < num_inp_bias; k++){
	fprintf(sfile, "%.12f ", net->whi[k]);
	if (++j % 7 == 0)
	   fprintf(sfile, "\n");
	}
    fprintf(sfile, "\n");
    }
for (i = 0, k = 0; i < num_output; i++){
    for (j = 0; j < num_hid_bias; ++j, ++k){
	fprintf(sfile, "%f ", net->woh[k]);
	if ((j + 1) % 7 == 0)
	   fprintf(sfile, "\n");
	}
    fprintf(sfile, "\n");
    }
}   /*  end of function |save_weights| */



/*************************************************/
Network_t *get_network (FILE * sfile){

unsigned hid, con;
int i, j, k;
Network_t *ret;

fscanf (sfile, "%d%d", &hid, &con);
ret = alloc_network ();

num_hid = hid;

for (i = 0, k = 0; i < num_hid; i++)
    for (j = 0; j < num_inp_bias; j++, k++)
        fscanf (sfile, "%f ", &(ret->whi[k]));
for (i = 0, k = 0; i < num_output; i++)
    for (j = 0; j < num_hid_bias; j++, k++)
        fscanf (sfile, "%f ", &(ret->woh[k]));
return ret;
}  /*  end of function |get_network_ascii| */


/*************************************************/
void free_network (Network_t * net){
  
free (net->input);
free (net->hidden);
free (net->output);
free (net->woh);
free (net->whi);
free (net->dwoh);
free (net->dwhi);
free (net);
}  /*  end of function |free_network| */

/*************************************************/
void freeze(void)
{ int ch;
  ch = _getch();
}
