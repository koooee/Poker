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
    ,tconvert("Ac7c3cTc9cQcKc", 7)
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
	  printf("Flush Failed on hand: %d\n", i+1);
	  printf("with a rank of: %d\n", bestfull(p.bin.is_full));
	  return 0;
	}
      reset_bin(&p.bin);
    }
  return 1;
  
}
int tsf()
{
  int i,j;
  int s[8] = {7,7,7,6,5,5,5,7};
  PLAYER p;
  CARD *h[8] = {
    /* Pretty Standard Cases */
    tconvert("AsKsQsJsTs5c8d", 7)
    ,tconvert("5cAsKsQsJsTs8d", 7)
    ,tconvert("5c8dAsKsQsJsTs", 7)
    ,tconvert("2s3s4s5s6sAs", 6)
    ,tconvert("4d5d6d7d8d", 5)
    ,tconvert("TsJsQsKsAs", 5)
    ,tconvert("2d3d4d5dAd", 5)
    /*Special Cases: like TP & SF */
    ,tconvert("TsJsQsKsAsKdAd", 7)

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
      printb(&p.bin);
      if(bestfull(p.bin.is_full) != 8)
	{
	  printf("Straight Flush Failed on hand: %d\n", i+1);
	  printf("with a rank of: %d\n", bestfull(p.bin.is_full));
	  return 0;
	}
      reset_bin(&p.bin);
    }
  return 1;
  
}

int ts()
{
  int i,j;
  int s[5] = {7,7,7,6,5};
  PLAYER p;
  CARD *h[5] = {
    tconvert("AdKsQsJsTs5c8d", 7)
    ,tconvert("5cAsKcQsJdTs8d", 7)
    ,tconvert("5c8dAhKsQsJcTs", 7)
    ,tconvert("2s3c4h5d6sAs", 6)
    ,tconvert("4s5d6c7d8s", 5)


  };

  /*
     TODO: the cases above are only testing rankings that ARE high cards...we also want to make sure we are not
     ranking other hands like Pair, Two Pair etc.. as High Card.
  */
  init_bin(&p.bin);
  for(i = 0; i < 5; i++)
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

void big_test()
{
  int one, two, three, four, five, six, seven, count;
  int freqs[9];
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
  test_hand = malloc(10 * sizeof(char));
  for(one = 0; one < size; one++)
    for(two = one+1; two < size; two++)
      for(three = two+1; three < size; three++)
  	for(four = three+1; four < size; four++)
  	  for(five = four+1; five < size; five++)
	    for(six = five+1; six < size; six++)
	      for(seven = six+1; seven < size; seven++)
		{
		  if(
		     one != two && one != three && one != four && one != five
		     && two != three && two != four && two != five
		     && three != four && three != five
		     && four != five
		     )
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
		      /* printf("Player Address: %x\n", plyr); */
		      /* printf("Player Hand Address: %x\n", plyr.hand); */
		      /* printf("Player Bin Address: %x\n", plyr.bin); */

		      rank = rank_hand(plyr.hand, &plyr.bin, 5);
		      freqs[rank]++;
		    }
		  reset_bin(&plyr.bin);
		}
  long total = 0;
  for(z = 0; z < 9; z++)
    {
      printf("Freq %d %d\n", z, freqs[z]);
      total += freqs[z];
    
    }
  printf("Total Number of Hands: %d\n", total);
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
  if(tk == 0){
    printf("Four of a Kind Failed\n");
    exit(EXIT_FAILURE);
  }
  int f = tf();
  if(f == 0){
    printf("Flush Failed\n");
    exit(EXIT_FAILURE);
  }
  int sf = tsf();
  if(sf == 0){
    printf("Straight Flush Failed\n");
    exit(EXIT_FAILURE);
  }
  int s = ts();
  if(sf == 0){
    printf("Straight Failed\n");
    exit(EXIT_FAILURE);
  }
  int fh = tfh();
  if(sf == 0){
    printf("Full House Failed\n");
    exit(EXIT_FAILURE);
  }

  printf("Tests Passed.\n");
  /* printf("Running Big Test...\n"); */
  /* big_test(); */
  
}
