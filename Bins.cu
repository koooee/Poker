#include "Bins.h"

__device__ void AddMulti(CARD c, CARD hand[4][5], int max, int suit, int count[4], BIN *b, int hand_rank)
{/*FIXME: this interface is retarded */
 if(count[suit] >= max)
    {
      b->is_full[hand_rank] = TRUE;
    }
  else 
    {
      hand[suit][count[suit]++] = c;
      if(count[suit] == max)
      	b->is_full[hand_rank] = TRUE;
    }
 
}

__device__ void Add(CARD c, CARD hand[5], int max, int *count, BIN *b, int hand_rank)
{/* Yes, I know, this interface seems a bit retarded.  FIXME */

  if(*count >= max)
    {
      b->is_full[hand_rank] = TRUE;
    }
  else
    {
      hand[(*count)++] = c;
      if(*count == max)
      	b->is_full[hand_rank] = TRUE;
    }
}

__device__ void init_bin(BIN *bin)
{
  int i;
  /* don't need to malloc any more since we changed the struct definition */
  /* bin->P.b = malloc(MAX_SIZE_P * card_size);  */
  bin->P.b_max = MAX_SIZE_P;
  bin->P.b_count = 0;

  /* bin->TP.b = malloc(MAX_SIZE_TP * card_size); */
  bin->TP.b_max = MAX_SIZE_TP;
  bin->TP.b_count = 0;

  /* bin->TK.b = malloc(MAX_SIZE_TK * card_size); */
  bin->TK.b_max = MAX_SIZE_TK;
  bin->TK.b_count = 0;

  /* bin->S.b = malloc(MAX_SIZE_S * card_size); */
  bin->S.b_max = MAX_SIZE_S;
  bin->S.b_count = 0;

  /* bin->FH.b = malloc(MAX_SIZE_FH * card_size); */
  bin->FH.b_max = MAX_SIZE_FH;
  bin->FH.b_count = 0;

  /* bin->FK.b = malloc(MAX_SIZE_FK * card_size); */
  bin->FK.b_max = MAX_SIZE_FK;
  bin->FK.b_count = 0;

  bin->F.b_max = 5;
  bin->SF.b_max = 5;
  /* TODO: Enhancement */
  /* We should dynamically malloc in the Add function (Below)...thus, only using memory when needed...but, this 
     might have an effect on preformance since we would be calling malloc frequently..and I am very concerned
     about runtime preformance
  */
  /* bin->F.b_count = malloc(MAX_NUM_SUITS * sizeof(CARD)); */
  /* bin->SF.b_count = malloc(MAX_NUM_SUITS * sizeof(CARD)); */

  for(i = 0; i < MAX_NUM_SUITS; i++)
    {
      /* bin->F.b[i] = malloc(NUM_CARDS_TO_RANK * sizeof(CARD)); */
      /* bin->SF.b[i] = malloc(NUM_CARDS_TO_RANK * sizeof(CARD)); */
      bin->F.b_count[i] = 0;
      bin->SF.b_count[i] = 0;
    }

  for(i = 0; i < MAX_HAND_RANKS; i++)
    {
      bin->is_full[i] = FALSE;
      bin->drawing[i] = 'Z';
    }
}

/* void clearbin(CARD **hand, int *count, int max) */
__device__ void clearbin(int *count)
{/* free the bin and return a fresh new pointer */
  /* free(*hand); */
  *count = 0;
  /* return malloc(max * sizeof(CARD)); */
}
__device__ void clearmultibin(CARD bin[4][5], int counts[4], int suit)
{
  int j;
  counts[suit] = 0;
  for(j = 0; j < 5; j++)
    {
      CARD junk;
      junk.rank = -1;
      junk.suit = -1;
      bin[suit][j] = junk;
    }
}


void reset_bin(BIN *bin)
{
  CARD blank;
  blank.rank = -1;
  blank.suit = -1;
  int i;
  bin->HC = blank;
  bin->P.b_count = 0;
  bin->TP.b_count = 0;
  bin->TK.b_count = 0;
  bin->S.b_count = 0;
  bin->FH.b_count = 0;
  bin->FK.b_count = 0;
  
  for(i = 0; i < MAX_NUM_SUITS; i++) {
    bin->SF.b_count[i] = 0;
    bin->F.b_count[i] = 0;
    }
  
  for(i = 0; i < MAX_HAND_RANKS; i++) {
    bin->is_full[i] = FALSE;
    bin->drawing[i] = NOTHING;
  }
}


/* void Remove(CARD **free, int *count) */
/* { */
/*   if(*count != 0) */
/*     { */
/*       CARD blank; */
/*       **free = blank; */
/*       --*free; */
/*       --*count;   */
/*     } */
/* } */

/* void printb(BIN *bin); */

/* void printh(CARD *h, int hand_size) */
/* {/\* Debugging *\/ */

/*   int i; */
/*   for(i =0; i < hand_size; i++) */
/*     { */
/*       printf("%c%c  ", ranks[h[i].rank], suits[h[i].suit]); */
/*     } */
/*   printf("\n"); */
/* } */

/* void printb(BIN *bin) */
/* {/\* Print Bin *\/ */
/*   int i,j; */
/*   printf("\n******************************\n"); */
/*   printf("*        BINS                *\n"); */
/*   printf("******************************\n"); */
/*   printf("High Card: "); */
/*   printc(bin->HC); */
/*   printf("\n"); */

/*   printf("Pair: "); */
/*   for(i = 0; i < bin->P.b_count; i++) */
/*     { */
/*       printc(bin->P.b[i]); */
/*     } */
/*   printf("\n"); */

/*   printf("Two Pair: "); */
/*   for(i = 0; i < bin->TP.b_count; i++) */
/*     { */
/*       printc(bin->TP.b[i]); */
/*     } */
/*   printf("\n"); */

/*   printf("Flush:\n"); */
/*   for(i = 0; i < MAX_NUM_SUITS; i++) */
/*     { */
/*       printf("\t%c: ", suits[i]); */
/*       for(j = 0; j < bin->F.b_count[i]; j++) */
/*   	{ */
/*   	  printc(bin->F.b[i][j]); */
/*   	} */
/*       printf("\n"); */
/*     } */

/*   printf("Straight Flush:\n"); */
/*   for(i = 0; i < MAX_NUM_SUITS; i++) */
/*     { */
/*       printf("\t%c: ", suits[i]); */
/*       for(j = 0; j < bin->SF.b_count[i]; j++) */
/*   	{ */
/*   	  printc(bin->SF.b[i][j]); */
/*   	} */
/*       printf("\n"); */
/*     } */

/*   printf("Straight: "); */
/*   for(i = 0; i < bin->S.b_count; i++) */
/*     { */
/*       printc(bin->S.b[i]); */
/*     } */
/*   printf("\n"); */
/* printf("******************************\n\n"); */

/* } */
