/*
 Test.h 
 Mainly for testing the ranking algo.  I am assuming that the sorting function works.  
 Programmer: Nick Kolegraff 
 Date: 6/3/2011 
*/

/* Naming convention: any method in Test.h will contain a 't' prefix */

/* 
   Note: I gave each hand rank their own test function just incase there is custom logic 
   I wanted to do to an individual rank.  I know it is a bit reduntant.
*/

#include "string.h"


char *hands[] = {
  "High Card",
  "Pair",
  "Two Pair",
  "Three Kind",
  "Straight",
  "Flush",
  "Full House",
  "Four Kind",
  "Straight Flush"};


CARD tstoc(char r, char s)
{/* string to card */
  int rank, suit,i;
  for(i = 0; i < MAX_NUM_RANKS; i++)
    {
      if(r == ranks[i])
	{
	  rank = i;
	}
    }
  for(i = 0; i < MAX_NUM_SUITS; i++)
    {
      if(s == suits[i])
	{
	  suit = i;
	}
    }
  CARD c;
  c.rank = rank;
  c.suit = suit;
  c.whos_card = NO_ONES;
  return c;
  
}

CARD *tconvert(char *s, int num_cards)
{/* input is a string of cards: AsTcJh8c2d4s */
  CARD *hand = malloc(num_cards * sizeof(CARD));
  int i;
  for(i = 0; i < 2*num_cards; i += 2)
    {
      hand[i / 2] = tstoc(s[i], s[i+1]);
    }
  return hand;
}
int bestfull(char *f)
{
  int i;
  for(i = MAX_HAND_RANKS-1; i >= 0; i--)
    {
      if(f[i] == TRUE)
	{
	  return i;
	}
    }
}

char *tinvert(CARD *hand, int num_cards, char *result)
{
  int i;
  for(i = 0; i < num_cards; i++)
    {
      result[i+i] = ranks[hand[i].rank];
      result[i+i+1] = suits[hand[i].suit];
    }
  return result;
}

char test_for_sf(BIN *bin)
{
  int i,j,is_one, total;
  CARD c_one, c_two;
  is_one = 0;
  for(i = 0; i < 4; i++)
    {
      if(bin->F.b_count[i] == 5)
	{
	  is_one++;
	  sort_hand(bin->SF.b[i], 5);
	  total = 0;
	  for(j = 0; j < 4; j++)
	    {
	      c_one = bin->F.b[i][j];
	      c_two = bin->F.b[i][j+1];
	      total += distance(c_one, c_two);
	    }
	  if(total == 4)
	    {
	      return TRUE;
	    }

	}
    }
  return FALSE;
}



int thc()
{/* Test High Card, 0 - Failure; 1 - Success */
  int i,j;
  int card_lengths[8] = {7,7,7,6,5,4,3,2};
  PLAYER p;
  CARD *high_cards[8] = {
    tconvert("AsTcJh9d2c8h5c", 7)
    ,tconvert("KsQhJdAc4h7d3c", 7)
    ,tconvert("9hTd3s6cKhQs2d", 7)
    ,tconvert("8s6c2c3c7hTd", 6)
    ,tconvert("9hTd3s6c4d", 5)
    ,tconvert("9hTd3s6c", 4)
    ,tconvert("9hTd3s", 3)
    ,tconvert("9hTd", 2)
  };

CARD *non_high_cards[8] = {
    tconvert("AsAcJh9d2c8h5c", 7)
    ,tconvert("QsQhQdAc4h7d3c", 7)
    ,tconvert("2s3s4s5sAs6s7d", 7)
    ,tconvert("8s6s2s7s7dTd", 6)
    ,tconvert("2d3d4d5d6d", 5)
    ,tconvert("9h3d3s6c", 4)
    ,tconvert("9h9d3s", 3)
    ,tconvert("ThTd", 2)
  };

  /*
     TODO: the cases above are only testing rankings that ARE high cards...we also want to make sure we are not
     ranking other hands like Pair, Two Pair etc.. as High Card.
  */
  init_bin(&p.bin);

  /* These cases should equal the high card rank */
  for(i = 0; i < 8; i++)
    {
      for(j = 0; j < card_lengths[i]; j++)
	{
	  p.hand[j] = high_cards[i][j];
	}
      rank_hand(p.hand, &p.bin, card_lengths[i]);
      int rank = bestfull(p.bin.is_full);
      if(rank != 0)
	{
	  printf("High Card failed on hand: %d\nExpecting rank of 0 got %d\n", i+1, rank);
	  return 0;
	}
      reset_bin(&p.bin);
    }

  /* These cases SHOULD NOT equal high card */
  for(i = 0; i < 8; i++)
    {
      for(j =0; j < card_lengths[i]; j++)
	{
	  p.hand[j] = non_high_cards[i][j];
	}
      rank_hand(p.hand, &p.bin, card_lengths[i]);
      int rank = bestfull(p.bin.is_full);
      if(rank == 0)
	{
	  printf("High Card FAILED on hand: %d\nExpecting rank NOT equal to 0 and got %d\n", i+1, rank);
	  return 0;
	}
      reset_bin(&p.bin);
    }
  return 1;
 
}

int tp()
{
  int i,j;
  int s[11] = {7,7,7,7,7,7,6,5,4,3,2};
  PLAYER p;
  CARD *h[11] = {
    tconvert("AsAcJh9d2c8h5c", 7)
    ,tconvert("KsQhQdAc4h7d3c", 7)
    ,tconvert("9hTd3s3cKhQs2d", 7)
    ,tconvert("9hTdJs3c3hQs2d", 7)
    ,tconvert("9hTd3sJcKhKs2d", 7)
    ,tconvert("8hTd3sJcKhQsQd", 7)
    ,tconvert("8s6c2c3c3hTd", 6)
    ,tconvert("9hTdTs6c4d", 5)
    ,tconvert("9hTd6s6c", 4)
    ,tconvert("9hTdTs", 3)
    ,tconvert("9h9d", 2)
  };

  /*
     TODO: the cases above are only testing rankings that ARE high cards...we also want to make sure we are not
     ranking other hands like Pair, Two Pair etc.. as High Card.
  */
  init_bin(&p.bin);
  for(i = 0; i < 11; i++)
    {

      for(j = 0; j < s[i]; j++)
	{
	  p.hand[j] = h[i][j];
	}
      rank_hand(p.hand, &p.bin, s[i]);
      if(bestfull(p.bin.is_full) != 1)
	{
	  printf("Pair Failed on hand: %d\n", i+1);
	  return 0;
	}
      reset_bin(&p.bin);
    }
  return 1;
  
}

int ttp()
{
  int i,j;
  int s[9] = {7,7,7,7,7,7,6,5,4};
  PLAYER p;
  CARD *h[9] = {
    tconvert("AsAcJhJd2c8h5c", 7)
    ,tconvert("KsQhQdAc4h7d7c", 7)
    ,tconvert("9hTd3s3cKhKs9d", 7)
    ,tconvert("9hTdJs3c3hQsQd", 7)
    ,tconvert("2hTd3sJcKhKs2d", 7)
    ,tconvert("ThTd3sJcKhQsQd", 7)
    ,tconvert("2s6c2c3c3hTd", 6)
    ,tconvert("9hTdTs6c6d", 5)
    ,tconvert("9h9d6s6c", 4)
  };

  /*
     TODO: the cases above are only testing rankings that ARE high cards...we also want to make sure we are not
     ranking other hands like Pair, Two Pair etc.. as High Card.
  */
  init_bin(&p.bin);
  for(i = 0; i < 9; i++)
    {

      for(j = 0; j < s[i]; j++)
	{
	  p.hand[j] = h[i][j];
	}
      rank_hand(p.hand, &p.bin, s[i]);
      int rank = bestfull(p.bin.is_full);
      if(rank != 2)
	{
	  printf("Two Pair Failed on hand: %d\n", i+1);
	  printf("With a rank of %d\n", rank);
	  return 0;
	}
      reset_bin(&p.bin);
    }
  return 1;
  
}

int ttk()
{
  int i,j;
  int s[10] = {7,7,7,7,7,7,6,5,4,3};
  PLAYER p;
  CARD *h[10] = {
    tconvert("AsAcAhTdQd8h5c", 7)
    ,tconvert("JsAcAhAdQd8h5c", 7)
    ,tconvert("JsKcAhAdAc8h5c", 7)
    ,tconvert("JsKc4hAdAsAh5c", 7)
    ,tconvert("JsKc9hTdAdAhAc", 7)
    ,tconvert("ThTdTsJcKh7sQd", 7)
    ,tconvert("2s6c2c3c2hTd", 6)
    ,tconvert("ThTdTs6c5d", 5)
    ,tconvert("9h9d9s6c", 4)
    ,tconvert("9h9d9s", 3)
  };

  /*
     TODO: the cases above are only testing rankings that ARE high cards...we also want to make sure we are not
     ranking other hands like Pair, Two Pair etc.. as High Card.
  */
  init_bin(&p.bin);
  for(i = 0; i < 10; i++)
    {

      for(j = 0; j < s[i]; j++)
	{
	  p.hand[j] = h[i][j];
	}
      rank_hand(p.hand, &p.bin, s[i]);
      if(bestfull(p.bin.is_full) != 3)
	{
	  printf("Three of a Kind Failed on hand: %d\n", i+1);
	  return 0;
	}
      reset_bin(&p.bin);
    }
  return 1;
  
}
int tfk()
{
  int i,j;
  int s[9] = {7,7,7,7,7,7,6,5,4};
  PLAYER p;
  CARD *h[9] = {
    tconvert("AsAcAhAdQd8h5c", 7)
    ,tconvert("JsAcAhAdAs8h5c", 7)
    ,tconvert("JsKcAhAdAcAs5c", 7)
    ,tconvert("JsKc4hAdAsAhAc", 7)
    ,tconvert("AsKc9hTdAdAhAc", 7)
    ,tconvert("ThTdTsTcKh7sQd", 7)
    ,tconvert("2s6c2c2d2hTd", 6)
    ,tconvert("ThTdTsTc5d", 5)
    ,tconvert("9h9d9s9c", 4)

  };

  /*
     TODO: the cases above are only testing rankings that ARE high cards...we also want to make sure we are not
     ranking other hands like Pair, Two Pair etc.. as High Card.
  */
  init_bin(&p.bin);
  for(i = 0; i < 9; i++)
    {

      for(j = 0; j < s[i]; j++)
	{
	  p.hand[j] = h[i][j];
	}
      rank_hand(p.hand, &p.bin, s[i]);
      if(bestfull(p.bin.is_full) != 7)
	{
	  printf("Four of a Kind Failed on hand: %d\n", i+1);
	  return 0;
	}
      reset_bin(&p.bin);
    }
  return 1;
  
}

int tf()
{
  int i,j;
  int s[8] = {7,7,7,7,7,7,6,5};
  PLAYER p;
  CARD *h[8] = {
    tconvert("As7s3sTs9sQcKd", 7)
    ,tconvert("As7c3sTs9sQsKd", 7)
    ,tconvert("As7c3sTs9sQsKs", 7)
    ,tconvert("As7s3sTs9sQsKs", 7)
    ,tconvert("As7c3sTs9sQhKs", 7)
    ,tconvert("AcAsKsQsQcJs2s", 7)
    ,tconvert("Ac7c3cTc9cQs", 6)
    ,tconvert("Ah7h3hTh9h", 5)


  };

  /*
     TODO: the cases above are only testing rankings that ARE high cards...we also want to make sure we are not
     ranking other hands like Pair, Two Pair etc.. as High Card.
  */
  init_bin(&p.bin);
  for(i = 0; i < 8; i++)
    {

      for(j = 0; j < s[i]; j++)
	{
	  p.hand[j] = h[i][j];
	}
      rank_hand(p.hand, &p.bin, s[i]);
      if(bestfull(p.bin.is_full) != 5)
	{
	  printh(p.hand, s[i]);
	  printf("Flush Failed on hand: %d\n", i+1);
	  printf("with a rank of: %d\n", bestfull(p.bin.is_full));
	  printb(&p.bin);
	  return 0;
	}
      reset_bin(&p.bin);
    }
  return 1;
  
}
int tsf()
{
  int i,j;
  int s[10] = {7,7,7,6,5,5,5,7,7,7};
  PLAYER p;
  CARD *h[10] = {
    /* Pretty Standard Cases */
    tconvert("AcAsKs5s4s3s2s", 7)
    ,tconvert("5cAsKsQsJsTs8d", 7)
    ,tconvert("5c8dAsKsQsJsTs", 7)
    ,tconvert("2s3s4s5s6sAs", 6)
    ,tconvert("4d5d6d7d8d", 5)
    ,tconvert("TsJsQsKsAs", 5)
    ,tconvert("2d3d4d5dAd", 5)
    /*Special Cases */
    ,tconvert("TsJsQsKsAsKdAd", 7) /* P and SF */
    ,tconvert("TsTcJsJcQsKsAs",7) /* TP and SF */
    ,tconvert("TsTcThJsQsKsAs",7) /*TK and SF */

  };

  /*
     TODO: the cases above are only testing rankings that ARE high cards...we also want to make sure we are not
     ranking other hands like Pair, Two Pair etc.. as High Card.
  */
  init_bin(&p.bin);
  for(i = 0; i < 10; i++)
    {

      for(j = 0; j < s[i]; j++)
	{
	  p.hand[j] = h[i][j];
	}
      rank_hand(p.hand, &p.bin, s[i]);
      if(bestfull(p.bin.is_full) != 8)
	{
	  printh(p.hand, s[i]);
	  printf("Straight Flush Failed on hand: %d\n", i+1);
	  printf("With a rank of: %d\n", bestfull(p.bin.is_full));
	  printb(&p.bin);
	  return 0;
	}
      /* else */
      /* 	{ */
      /* 	  if(test_for_sf(&p.bin) == FALSE) */
      /* 	    { */
      /* 	      printf("IMPOSTER!\n"); */
      /* 	      printh(p.hand, 7); */
      /* 	      printb(&p.bin); */
      /* 	      printf("\n"); */
      /* 	    } */
      /* 	} */
      reset_bin(&p.bin);
    }
  return 1;
  
}

int ts()
{
  int i,j;
  int s[6] = {7,7,7,6,5,5};
  PLAYER p;
  CARD *h[6] = {
    tconvert("AdKsQsJsTs5c8d", 7)
    ,tconvert("5cAsKcQsJdTs8d", 7)
    ,tconvert("5c8dAhKsQsJcTs", 7)
    ,tconvert("2s3c4h5d6sAs", 6)
    ,tconvert("4s5d6c7d8s", 5)
    ,tconvert("As2d3c5s4h", 5)


  };

  /*
     TODO: the cases above are only testing rankings that ARE high cards...we also want to make sure we are not
     ranking other hands like Pair, Two Pair etc.. as High Card.
  */
  init_bin(&p.bin);
  for(i = 0; i < 6; i++)
    {

      for(j = 0; j < s[i]; j++)
	{
	  p.hand[j] = h[i][j];
	}
      rank_hand(p.hand, &p.bin, s[i]);
      if(bestfull(p.bin.is_full) != 4)
	{
	  printf("Straight Failed on hand: %d\n", i+1);
	  printf("with a rank of: %d\n", bestfull(p.bin.is_full));
	  return 0;
	}
      reset_bin(&p.bin);
    }
  return 1;
  
}
int tfh()
{
  int i,j;
  int s[6] = {7,7,7,6,6,5};
  PLAYER p;
  CARD *h[6] = {
    tconvert("AsAcAh2c2d7c9h", 7)
    ,tconvert("9hAsAcAh2c2d7c", 7)
    ,tconvert("9h7cAsAcAh2c2d", 7)
    ,tconvert("9hAsAcAh2c2d", 6)
    ,tconvert("AsAcAh2c2d9h", 6)
    ,tconvert("AsAcAh2c2d", 5)


  };

  /*
     TODO: the cases above are only testing rankings that ARE high cards...we also want to make sure we are not
     ranking other hands like Pair, Two Pair etc.. as High Card.
  */
  init_bin(&p.bin);
  for(i = 0; i < 6; i++)
    {

      for(j = 0; j < s[i]; j++)
	{
	  p.hand[j] = h[i][j];
	}
      rank_hand(p.hand, &p.bin, s[i]);
      if(bestfull(p.bin.is_full) != 6)
	{
	  printf("Full House Failed on hand: %d\n", i+1);
	  printf("with a rank of: %d\n", bestfull(p.bin.is_full));
	  return 0;
	}
      reset_bin(&p.bin);
    }
  return 1;
  
}

void check_for_same(CARD * cards, int size)
{/* Debugging */
  int i,j;
  for(i = 0; i < size; i++)
    for(j = i+1; j < size; j++)
      if((cards[i].rank == cards[j].rank))
	{
	  printc(cards[i]);
	  printc(cards[j]);
	  printf("\nFAIL!!! Matchnig Cards!!!\n");
	  exit(EXIT_FAILURE);
	}
}

void big_test_seven()
{
  int one, two, three, four, five, six, seven, count, sumer;
  int freqs[9];
  int sums[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int z;
  for(z = 0; z < 9; z++)
    freqs[z] = 0;
  count = 0;
  int size = 52, i, rank;
  char *cards[52];
  char *test_hand;
  CARD *hnd;
  PLAYER plyr;

  init_bin(&plyr.bin);
  test_hand = malloc(14 * sizeof(char));
  for(one = 0; one < size; one++)
    for(two = one+1; two < size; two++)
      for(three = two+1; three < size; three++)
      	for(four = three+1; four < size; four++)
      	  for(five = four+1; five < size; five++)
      	    for(six = five+1; six < size; six++)
      	      for(seven = six+1; seven < size; seven++)
		{
		  /* if( */
		  /*    one != two && one != three && one != four && one != five && one != six && one != seven */
		  /*    && two != three && two != four && two != five && two != six && two != seven */
		  /*    && three != four && three != five && three != six && three != seven */
		  /*    && four != five && four != six && four != seven */
		  /*    && five != six && five != seven */
		  /*    && six != seven */
		  /*    ) */
		      sprintf(test_hand, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
		      	      ,ranks[one % 13]
		      	      ,suits[one % 4]
		      	      ,ranks[two % 13]
		      	      ,suits[two % 4]
		      	      ,ranks[three % 13]
		      	      ,suits[three % 4]
		      	      ,ranks[four % 13]
		      	      ,suits[four % 4]
		      	      ,ranks[five % 13]
		      	      ,suits[five % 4]
		      	      ,ranks[six % 13]
		      	      ,suits[six % 4]
		      	      ,ranks[seven % 13]
		      	      ,suits[seven % 4]);

		      hnd = tconvert(test_hand, 7);
		      /* sort_hand(hnd, 7); */
		      /* printh(hnd, 7); */
		      for(i = 0; i < 7; i++){
			plyr.hand[i] = hnd[i];
		      }
		      /* tconvert() above calls malloc...so if you don't do this...you will eat memory */
		      free(hnd);
		      rank = rank_hand(plyr.hand, &plyr.bin, 7);
		      /* if(rank == 8) */
		      /* 	{ */
		      /* 	  if(test_for_sf(&plyr.bin) == FALSE) */
		      /* 	    { */
		      /* 	      printf("IMPOSTER!!\n"); */
		      /* 	      printh(plyr.hand,7); */
		      /* 	      printb(&plyr.bin); */
		      /* 	    } */
		      /* 	} */
		      /* if(rank == 5) */
		      /* 	{ */
		      /* 	  if(test_for_sf(&plyr.bin) == TRUE) */
		      /* 	    { */
		      /* 	      printf("IMPOSTER2!!\n"); */
		      /* 	      printh(plyr.hand,7); */
		      /* 	      printb(&plyr.bin); */
		      /* 	    } */
		      /* 	} */
		      /* 	  int j; */
		      /* 	  for(j = 0; j < 4; j++) */
		      /* 	    { */
		      /* 	      check_for_s(plyr.bin.F.b[j], plyr.bin.F.b_count[j]); */
		      /* 	      check_for_s(plyr.bin.SF.b[j], plyr.bin.SF.b_count[j]); */
		      /* 	    }} */
		      if(rank == 5)
		      	{
			  int x, rnk;
			  PLAYER xs;
			  init_bin(&xs.bin);
			  for(x = 0; x < 4; x++)
			    {
			      if(plyr.bin.F.b_count[x] == 5)
				{
				  rnk = sum_of_distances(plyr.bin.F.b[x], 5);
				  /* rnk = rank_hand(plyr.bin.F.b[x], &xs.bin, 5); */
				  /* check_for_same(plyr.bin.F.b[x], 5); */
				  if(rnk == 4)
				    {
				      printf("Problem Hand: "); printh(plyr.bin.F.b[x], 5);
				    }

				}
			    }
			}			 

		      	  /* int x,r; */
		      	  /* for(x = 0; x < 4; x++) */
		      	  /*   { */
		      	  /*     if(plyr.bin.F.b_count[x] == 5) */
		      	  /* 	{ */
		      	  /* 	  r = rank_hand(plyr.bin.F.b[x], &plyr.bin, 5); */
		      	  /* 	  if(r != 5) */
		      	  /* 	    { */
		      	  /* 	      printf("IMPOSTER2!!\n"); */
		      	  /* 	      printh(plyr.hand,7); */
		      	  /* 	      printb(&plyr.bin); */
		      	  /* 	    } */
		      	  /* 	} */
		      	  /*   } */

		      /* sumer= sum_of_distances(plyr.hand, 7); */

		      /* sums[sumer]++; */
		      /* if(sumer == 1) */
		      /* 	printh(plyr.hand, 7); */

		      freqs[rank]++;

		  reset_bin(&plyr.bin);
		    /* } */
		}
  /* for(z = 0; z < 52; z++) */
  /*   { */
  /*     printf("Sum_%d: %d\n", z, sums[z]); */
  /*   } */
  /* printf("Done with Sums\n"); */
  long total = 0;
  for(z = 0; z < 9; z++)
    {
      printf("Freq %d %d\n", z, freqs[z]);
      total += freqs[z];
    
    }
  printf("Total Number of Hands: %d\n", total);
}

void big_test_five()
{
  int one, two, three, four, five, six, seven, count, sumer;
  int freqs[9];
  int sums[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int z;
  for(z = 0; z < 9; z++)
    freqs[z] = 0;
  count = 0;
  int size = 52, i, rank;
  char *cards[52];
  char *test_hand;
  CARD *hnd;
  PLAYER plyr;

  init_bin(&plyr.bin);
  test_hand = malloc(14 * sizeof(char));
  for(one = 0; one < size; one++)
    for(two = one+1; two < size; two++)
      for(three = two+1; three < size; three++)
      	for(four = three+1; four < size; four++)
      	  for(five = four+1; five < size; five++)
		{
		      sprintf(test_hand, "%c%c%c%c%c%c%c%c%c%c"
		      	      ,ranks[one % 13]
		      	      ,suits[one % 4]
		      	      ,ranks[two % 13]
		      	      ,suits[two % 4]
		      	      ,ranks[three % 13]
		      	      ,suits[three % 4]
		      	      ,ranks[four % 13]
		      	      ,suits[four % 4]
		      	      ,ranks[five % 13]
		      	      ,suits[five % 4]);

		      hnd = tconvert(test_hand, 5);
		      for(i = 0; i < 5; i++){
			plyr.hand[i] = hnd[i];
		      }
		      /* tconvert() above calls malloc...so if you don't do this...you will eat memory */
		      free(hnd);
		      rank = rank_hand(plyr.hand, &plyr.bin, 5);
		      freqs[rank]++;

		  reset_bin(&plyr.bin);
		    /* } */
		}
  long total = 0;
  for(z = 0; z < 9; z++)
    {
      printf("Freq %d %d\n", z, freqs[z]);
      total += freqs[z];
    
    }
  printf("Total Number of Hands: %d\n", total);
}

void big_test_2()
{
  const char* deck[52] = {
    "As","Ks","Qs","Js","Ts","9s","8s","7s","6s","5s","4s","3s","2s"
    ,"Ac","Kc","Qc","Jc","Tc","9c","8c","7c","6c","5c","4c","3c","2c"
    ,"Ah","Kh","Qh","Jh","Th","9h","8h","7h","6h","5h","4h","3h","2h"
    ,"Ad","Kd","Qd","Jd","Td","9d","8d","7d","6d","5d","4d","3d","2d"};

  int one, two, three, four, five, six, seven, count=0, summer, size=52, i, rnk, z;
  int freqs[9];
  char hand[14];
  CARD *hnd;
  PLAYER p;
  char *r = malloc(14 * sizeof(char));
  FILE *sf;
  sf = fopen("sf_big.txt", "a");
  
  strcpy(hand, "");
  hnd = malloc(7 * sizeof(CARD));
  init_bin(&p.bin);
  for(z = 0; z < 9; z++)
    freqs[z] = 0;
  freqs[0] = 0;
  for(one = 0; one < size; one++){
    /* printf("one: %d\n ", one); */
    for(i = 0; i < 14; i++)
      hand[i] = '\0';
    strcat(hand, deck[one]);
    for(two = one+1; two < size; two++){
      for(i = 2; i < 14; i++)
	hand[i] = '\0';
      strcat(hand, deck[two]);
      for(three = two+1; three < size; three++){
	for(i = 4; i < 14; i++)
	  hand[i] = '\0';

	strcat(hand, deck[three]);
      	for(four = three+1; four < size; four++){
	  for(i = 6; i < 14; i++)
	    hand[i] = '\0';

	  strcat(hand, deck[four]);
      	  for(five = four+1; five < size; five++){
	    for(i = 8; i < 14; i++)
	      hand[i] = '\0';

	    strcat(hand, deck[five]);
      	    for(six = five+1; six < size; six++){
	      for(i = 10; i < 14; i++)
		hand[i] = '\0';

	      strcat(hand, deck[six]);
      	      for(seven = six+1; seven < size; seven++)
		{

		  for(i = 12; i < 14; i++)
		    hand[i] = '\0';

		  strcat(hand, deck[seven]);
		  /* printf("Hand is: %s\n", hand); */

		  hnd = tconvert(hand, 7);
		  for(z = 0; z < 7; z++)
		    p.hand[z] = hnd[z];


		  free(hnd);
		  rnk = rank_hand(p.hand, &p.bin, 7);
		  tinvert(p.hand, 7, r);
		  if(rnk == 8)
		    {
		      fwrite(r, sizeof(char), 14, sf);
		      fwrite("\n", sizeof(char), 1, sf);
		  
		    }
		  freqs[rnk]++;
		  /* printf("Hand is: %s\twith rank: %d\n", hand, rnk); */
		  reset_bin(&p.bin);
		  count++;
		}
	    }
	  }
	}
      }
    }
  }
  summer = 0;
  for(z = 0; z < 9; z++)
    {
      printf("%s\t%d\n", hands[z], freqs[z]);
      summer += freqs[z];
    }

  printf("Count is: %d\n", count);
  printf("Sum is: %d\n", summer);
  fclose(sf);

}

void all_sf_combos()
{
  const int s_ranks[4] = {0, 13, 26, 39}; /* starting ranks for Straights (indexes for deck below) */
  const char* deck[52] = {"As","Ks","Qs","Js","Ts","9s","8s","7s","6s","5s","4s","3s","2s"
		    ,"Ac","Kc","Qc","Jc","Tc","9c","8c","7c","6c","5c","4c","3c","2c"
		    ,"Ah","Kh","Qh","Jh","Th","9h","8h","7h","6h","5h","4h","3h","2h"
		    ,"Ad","Kd","Qd","Jd","Td","9d","8d","7d","6d","5d","4d","3d","2d"};
  char used[52];

  PLAYER player;

  char hand[14];
  char *r = malloc(14 * sizeof(char));
  CARD *hnd;
  int i, j, k, l, m, n, p, rnk, count=0, index;
  FILE *sf;
  sf = fopen("sf_combos.txt", "a");
  for(p = 0; p < 52; p++)
    used[p] = FALSE;

  strcpy(hand, "");
  hnd = malloc(7 *sizeof(CARD));
  init_bin(&player.bin);
  for (i =0; i < 4; i++)
    {
      j = 0;
      /* there are 10 unique ranks for a Straight */
      while(j < 10)
	{
	  for(k = 0; k < 5; k++)
	    {
	      if(j != 0)
		{/* have to choose from remaining 47 cards EXCEPT j+1 since that will change the rank of the hand */
		  used[s_ranks[i]+j-1]=TRUE;
		}

	      if(j == 9 && k == 4)
		{
		  strcat(hand, deck[s_ranks[i]]);
		  used[s_ranks[i]] = TRUE;
		}
	      else
		{
		  strcat(hand, deck[s_ranks[i]+j+k]);
		  used[s_ranks[i]+j+k] = TRUE;
		}
	    }


	  /*can choose from any of the remaining 47 cards*/
	  for(m = 0; m < 52; m++)
	    for(n = m+1; n < 52; n++)
	      {
		hand[10] = '\0';
		hand[11] = '\0';
		hand[12] = '\0';
		hand[13] = '\0';
		if(used[n] == FALSE && used[m] == FALSE)
		  {
		    strcat(hand, deck[m]);
		    strcat(hand, deck[n]);
		    hnd = tconvert(hand, 7);

		    for(index = 0; index < 7; index++)
		      player.hand[index] = hnd[index];

		    rnk = rank_hand(player.hand, &player.bin, 7);

		    tinvert(player.hand, 7, r);
		    fwrite(r, sizeof(char), 14, sf);
		    fwrite("\n", sizeof(char), 1, sf);
		    free(hnd);
		    /* printf("Hand: %s", hand); */
		    /* printh(hnd, 7); */

		    for(p = 10; p < 14; p++)
		      hand[p] = '\0';
		    count++;
		  }
		
	      }



	  /* reset */
	  strcpy(hand, "");
	  for(p = 0; p < 52; p++)
	    used[p] = FALSE;
	  j++;
	}
    }
  printf("Count is: %d\n", count);
  fclose(sf);
}

void TestDistance()
{
  int i, j, k, l;
  CARD c, c2;
  for(k = 0; k < 4; k++)
    for(l = 0; l < 13; l++)
      {
	for(i = 0; i < 4; i++)
	  for(j = 0; j < 13; j++)
	    {
	      if(((j == 0 || j==12) && (l==0 || l==12) || ((j == 3 || j==12) && (l==3 || l==12))))
		{
		  c.rank = j;
		  c.suit = i;
		  c2.rank = l;
		  c2.suit = k;
		  printf("The Distance Between "); printc(c); printf(" And "); printc(c2); printf(" is: %d\n", distance(c, c2));
		}
	    }
      }

}

void ttest()
{/* this is the main function for test cases */

  int hc = thc();
  if(hc == 0){
    printf("High Card Failed\n");
    exit(EXIT_FAILURE);
  }
  int p = tp();
  if(p == 0){
    printf("Pair Failed\n");
    exit(EXIT_FAILURE);
  }
  int tp = ttp();
  if(tp == 0){
    printf("Two Pair Failed\n");
    exit(EXIT_FAILURE);
  }
  int tk = ttk();
  if(tk == 0){
    printf("Three of a Kind Failed\n");
    exit(EXIT_FAILURE);
  }
  int fk = tfk();
  if(fk == 0){
    printf("Four of a Kind Failed\n");
    exit(EXIT_FAILURE);
  }
  int f = tf();
  if(f == 0){
    printf("Flush Failed\n");
    exit(EXIT_FAILURE);
  }
  int s = ts();
  if(s == 0){
    printf("Straight Failed\n");
    exit(EXIT_FAILURE);
  }
  int fh = tfh();
  if(fh == 0){
    printf("Full House Failed\n");
    exit(EXIT_FAILURE);
  }
  int sf = tsf();
  if(sf == 0){
    printf("Straight Flush Failed\n");
    exit(EXIT_FAILURE);
  }
  
  /* printf("Tests Passed.\n"); */
  /* test_SF(); */
  /* printf("Running Big Test...\n"); */
  /* big_test_five(); */
  /* big_test_seven(); */
  /* TestDistance(); */
  /* all_sf_combos(); */
  big_test_2();
}
