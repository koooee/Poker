/*
Hands.h
This contains some data structures that will compose the different hands for the BINS struct in Bins.h
I'm not really sure the best way to go about this bue this seemed SOMEWHAT logical
*/

/* Multi Bin */
/* This bin is for special cases ie. Straight flush and Flush because we need to keep track of each suit */
/* Naming Convention: b - buffer */

typedef struct MULTI_BIN {
  CARD b[4][5]; /* This will always need to be a 4 by 5 */
  CARD *b_free[5]; /* Next free space in mb multi array */
  int b_max; /* Max size of the bin */
  int b_count[5]; /* Current count of the elements in the bin */
} MULTI_BIN;

/* Strandard Bin */
/* Most bins will follow this format. Since they are a unique case of a five card hand */
typedef struct STD_BIN { /* hahahhahah std... */
  /* Make this a pointer because some bins need to be different size like pair (2) and three kind (3) See initbins*/
  CARD *b; 
  CARD *b_free;
  int b_max;
  int b_count;
} STD_BIN;


 
