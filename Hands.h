/*
Hands.h
This contains some data structures that will compose the different hands for the BINS struct in Bins.h
I'm not really sure the best way to go about this bue this seemed SOMEWHAT logical
*/

/* Multi Bin */
/* This bin is for special cases ie. Straight flush and Flush because we need to keep track of each suit */
/* Naming Convention: b - buffer */
#define NUM_CARDS_TO_RANK 5

typedef struct MULTI_BIN {
  CARD b[MAX_NUM_SUITS][NUM_CARDS_TO_RANK]; /* actual bin where cards are stored */
  int b_max; /* Max size of the bin */
  int b_count[MAX_NUM_SUITS]; /* Current count of the elements in the bin */
} MULTI_BIN;

/* Strandard Bin */
/* Most bins will follow this format. Since they are a unique case of a five card hand */
typedef struct STD_BIN { /* hahahhahah std... */
  /* Make this a pointer because some bins need to be different size like pair (2) and three kind (3) See initbins*/
  CARD b[NUM_CARDS_TO_RANK]; /* this use to be a pointer to a card, need to change since can't call malloc from device code in cuda */
  int b_max;
  int b_count;
} STD_BIN;

/* 

I use to have structs like this: 


typedef struct STD_BIN {
CARD *b;
CARD *free;  next free element in the array 
int b_max;
int b_count; 
} STD_BIN;

But, Decide this was redundant and could get away without using the 'free' concept 

*/
