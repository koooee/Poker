/* Sim.c
   The purpose of this file is to run the game simulations -- Think Main()
   Programmer: Nick Kolegraff
   Date: 5/29/2011
*/

// Note: Compile with -fopenmp option

#include "Player.c"
#include "Test.h"
#include "omp.h"

void Simulate();
void loop_test();
void ctest();
void itcard(int icard, CARD *c);
int randome(int max, int *exclude, int len_exclude, int *result); /* Random Integers with Exclusion */
int main(int argc, char *argv[])
{
  PLAYER p;
  int i, seed;
  CARD *h;
  /* Debugging -- Delete Section when done */
  /* Debugging Init the hand as a straight */
  /* seed = atoi(argv[1]); */
  /* ttest(seed); */
  /* End Debugging */
   Simulate();
  /* loop_test(); */
  /* srand(seed); */
  /* ctest(); */
}

int randome(int max, int *exclude, int len_exclude, int *result)
{/* will return a random integer on the interval 0, max with integers in exclude array excluded */
  *result = rand() % max;
  int i;
  for(i = 0; i < len_exclude; i++){
    if(*result == exclude[i]){
      randome(max, exclude, len_exclude, result);
    }
  }
  return *result;
}
void itcard(int icard, CARD *c)
{
  c->rank = icard % 13;
  c->suit = icard % 4;
}
void ctest()
{

  int i,j,k,l,m,n,o,threadnum,size=52;
  unsigned long long int count=0;
  /* int *e = malloc(3 * sizeof(int)); */
  /* int *r = malloc(sizeof(int)); */
  /* *r = -1; */
  /* for(i = 0; i < 10000; i++) */
  /*   { */
  /*     e[0] = 20; */
  /*     e[1] = 45; */
  /*     e[2] = 10; */
  /*     int myRand = randome(52, e, 3, r); */
  /*     printf("%d\n", myRand); */
  /*   } */
/* #pragma omp parallel shared(threadnum, count, size) private(i, j, k, l, m, n, o) */
/*   { */
/*     threadnum = omp_get_thread_num(); */
/* #pragma omp parallel for shared(size) private(i, j, k,l,m,n,o) reduction(+:count) */
/*   for(i = 0; i < size; i++){ */
/*     printf("Thread %d executing %d\n", omp_get_thread_num(), i); */
/*     for(j = 0; j < size; j++) */
/*       for(k = 0 ; k < size; k++) */
/* 	for(l=0; l < size; l++) */
/* 	  for(m=0; m < size; m++) */
/* 	    for(n=0; n < size; n++) */
/* 	      for(o=0; o < size; o++){ */
/* 		  count++; */
/* 	      } // END inner most for */
/*   } // END outer most for */
/* /\* } // END Parallel Region *\/ */
/*   printf("%lld\n", count); */
}
void loop_test()
{
  unsigned long long totalcombin = 0;
  unsigned long long iterations = 0;
  int action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river;
  int decksize = 20;
  /* #pragma omp parallel shared(decksize, totalcombin) private(action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river) */
  /* { */
  /* holep0 = holep1 = holed0 = holed1 = flop0 = flop1 = flop2 = turn = river = 0; */
  /* while(holep0 < decksize){ */
  /*   while(holep1 != holep0 && holep1 < decksize){ */
  /*     while(holed0 != holep1 && holed0 != holep0 && holed0 < decksize){ */
  /* 	while(holed1 != holed0 && holed1 != holep1 && holed1 != holep0 && holed1 < decksize){ */
  /* 	  while(flop0 != holed1 && flop0 != holed0 && flop0 != holep1 && flop0 != holep0 && flop0 < decksize){ */
  /* 	    while(flop1 != flop0 && flop1 != holed1 && flop1 != holed0 && flop1 != holep1 && flop1 != holep0 && flop1 < decksize){ */
  /* 	      while(flop2 != flop1 && flop2 != flop0 && flop2 != holed1 && flop2 != holed0 && flop2 != holep1 && flop2 != holep0 && flop2 < decksize){ */
  /* 		while(turn != flop2 && turn != flop1 && turn != flop0 && turn != holed1 && turn != holed0 && turn != holep1 && turn != holep0 && turn < decksize){ */
  /* 		  while(river != turn && river != flop2 && river != flop1 && river != flop0 && river != holed1 && river != holed0 && river != holep1 && river != holep0 && river < decksize) */

#pragma omp parallel for shared(decksize) private(action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river) reduction(+:totalcombin, iterations)
  for(holep0 = 0; holep0 < decksize; holep0++){
    printf("Thread %d executes %d\n", omp_get_thread_num(), holep0);
    for(holep1 = holep0+1; holep1 < decksize; holep1++){
      for(flop0 = 0; flop0 < decksize && flop0 != holep0 && flop0 != holep1; flop0++){
    	for(flop1 = flop0+1; flop1 < decksize && flop1 != holep0 && flop1 != holep1; flop1++){
    	  for(flop2 = flop1+1; flop2 < decksize && flop2 != holep0 && flop2 != holep1; flop2++){
    	    for(turn = 0; turn < decksize && turn != holep0 && turn != holep1 && turn != flop0 && turn != flop1 && turn != flop2; turn++){
    	      for(river = 0; river < decksize && river != turn && river != holep0 && river != holep1 && river != flop0 && river != flop1 && river != flop2; river++){
    		for(holed0 = 0; holed0 < decksize && holed0 != river && holed0 != turn && holed0 != holep0 && holed0 != holep1 && holed0 != flop0 && holed0 != flop1 && holed0 != flop2; holed0++){
    		  for(holed1 = holed0+1; holed1 < decksize && holed1 != river && holed1 != turn && holed1 != holep0 && holed1 != holep1 && holed1 != flop0 && holed1 != flop1 && holed1 != flop2; holed1++)
		    {
		      iterations++;
		    if(holep0 != holep1 && holep0 != holed0 && holep0 != holed1 && holep0 != flop0 && holep0 != flop1 && holep0 != flop2 && holep0 != turn && holep0 != river
		       && holep1 != holed0 && holep1 != holed1 && holep1 != flop0 && holep1 != flop1 && holep1 != flop2 && holep1 != turn && holep1 != river
		       && holed0 != holed1 && holed0 != flop0 && holed0 != flop1 && holed0 != flop2 && holed0 != turn && holed0 != river
		       && holed1 != flop0 && holed1 != flop1 && holed1 != flop2 && holed1 != turn && holed1 != river
		       && flop0 != flop1 && flop0 != flop2 && flop0 != turn && flop0 != river
		       && flop1 != flop2 && flop1 != turn && flop1 != river
		       && flop2 != turn && flop2 != river
		       && turn != river){

		      /* int flop0n = flop0 % decksize; */
		      /* int flop1n = flop1 % decksize; */
		      /* int flop2n = flop2 % decksize; */
		      /* int turnn = turn % decksize; */
		      /* int rivern = river % decksize; */
		      /* int holed0n  = holed0 % decksize; */
		      /* int holed1n = holed1 % decksize; */
		      /* printf("%d %d %d %d %d %d %d %d %d\n", holep0, holep1, flop0, flop1, flop2, turn, river, holed0 , holed1); */

		      totalcombin++;
		    }
		    }
		}
	      }
	    }
	  }
	}
      }
    }
  }


/* } // End Parallel Region */
  printf("Total Iterations: %lld\n", iterations);
  printf("Total Combinations: %lld\n", totalcombin);
}

void Simulate()
{
  PLAYER aPlay[52*2];
  PLAYER p;
  PLAYER d;
  unsigned long long totalcombin = 0;
  unsigned long long iterations = 0;
  int decksize = 20;
  // Pay outs for 9 different states
  float twoxbet_pay[9];
  float onexbet_pay[9];
  char *state;
  char *is_my_card;
  float twoxbet_payl[9]; //Local for individual threads (hint: the ending l)
  float onexbet_payl[9];
  int action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river, flop0n, flop1n, flop2n, turnn, rivern, holed0n,  holed1n;

  float twoxpay, onexpay;
  int i;

  for(i = 0; i < 9; i++)
    {
      twoxbet_pay[i] = 0.0;
      onexbet_pay[i] = 0.0;
    }




#pragma omp parallel for shared(aPlay, decksize, onexbet_pay, twoxbet_pay) private(p, d, twoxpay, onexpay, onexbet_payl, twoxbet_payl, action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river, state, is_my_card, i, flop0n, flop1n, flop2n, turnn, rivern, holed0n,  holed1n) reduction(+:totalcombin, iterations)
  for(holep0 = 0; holep0 < decksize; holep0++){
    printf("Thread %d executes %d\n", omp_get_thread_num(), holep0);
    p = aPlay[holep0];
    d = aPlay[holep0+1];
    /* Start Variable init */
    state = malloc(2 * sizeof(char));
    is_my_card = malloc(sizeof(char));

    InitPlayer(&p);
    InitPlayer(&d);

    for(i = 0; i < 9; i++)
      {
	twoxbet_payl[i] = 0.0;
	onexbet_payl[i] = 0.0;
      }
    twoxpay = 0.0;
    onexpay = 0.0;


    /* END variable init */

    /* printf("Sim: Variables Declared for Thread %d\n", omp_get_thread_num()); */
    // This concept of for loops doesn't work for this combinatoric game -- I will explain
    // You can't iterate sequentially starting each loop at 0 means those are the exact same card since integers 0-52 represent different cards in a deck
    // One solution is to use while loops and just start at different index for each while loop and keep a counter and stop once you reach 50
    // Another solution -- which i'm working on and it is really difficult -- is to create a function that can take an integer as input and return the 9 cards associated with that integer
    // For example: There are 55 trillion combinations in this game -- in the case of this function f(0) -> As 2s 3s 4s 5s 6s 7s 8s 9s; f(1) -> Ks 2s 3s 4s 5s 6s 7s 8s 9s; all the way to the 55 trillion
    for(holep1 = holep0+1; holep1 < decksize; holep1++){
      for(flop0 = 0; flop0 < decksize; flop0++){
    	for(flop1 = flop0+1; flop1 < decksize; flop1++){
    	  for(flop2 = flop1+1; flop2 < decksize; flop2++){
    	    for(turn = 0; turn < decksize; turn++){
    	      for(river = 0; river < decksize; river++){
    		/* for(holed0 = 0; holed0 < decksize; holed0++){ */
    		/*   for(holed1 = holed0+1; holed1 < decksize; holed1++) */
		    /* { */
		holed0 = 23;
		holed1 = 12;
		      iterations++;
		      if(holep0 != holep1 && holep0 != holed0 && holep0 != holed1 && holep0 != flop0 && holep0 != flop1 && holep0 != flop2 && holep0 != turn && holep0 != river
			 && holep1 != holed0 && holep1 != holed1 && holep1 != flop0 && holep1 != flop1 && holep1 != flop2 && holep1 != turn && holep1 != river
			 && holed0 != holed1 && holed0 != flop0 && holed0 != flop1 && holed0 != flop2 && holed0 != turn && holed0 != river
			 && holed1 != flop0 && holed1 != flop1 && holed1 != flop2 && holed1 != turn && holed1 != river
			 && flop0 != flop1 && flop0 != flop2 && flop0 != turn && flop0 != river
			 && flop1 != flop2 && flop1 != turn && flop1 != river
			 && flop2 != turn && flop2 != river
			 && turn != river){

			totalcombin++;
			// There are four actions described below
			// Bet 4x                  0
			// Check First, Bet 2x     1
			// Check, Check, Bet 1x    2
			// Check, Check, Fold      3

			// Players Hole Cards
			itcard(holep0, &p.hand[0]);
			itcard(holep1, &p.hand[1]);
			p.hand[0].whos_card = PLAYERS;
			p.hand[1].whos_card = PLAYERS;
			/* printf("Sim: Players Hole Cards %d\n", omp_get_thread_num()); */

			// Dealers Hole Cards
			itcard(holed0, &d.hand[0]);
			itcard(holed1, &d.hand[1]);
			d.hand[0].whos_card = DEALERS;
			d.hand[1].whos_card = DEALERS;
			/* printf("Sim: Dealers Hole Cards %d\n", omp_get_thread_num()); */

			// Flop
			itcard(flop0, &p.hand[2]);
			itcard(flop1, &p.hand[3]);
			itcard(flop2, &p.hand[4]);
			itcard(flop0, &d.hand[2]);
			itcard(flop1, &d.hand[3]);
			itcard(flop2, &d.hand[4]);
			p.hand[2].whos_card = BOARDS;
			p.hand[3].whos_card = BOARDS;
			p.hand[4].whos_card = BOARDS;
			d.hand[2].whos_card = BOARDS;
			d.hand[3].whos_card = BOARDS;
			d.hand[4].whos_card = BOARDS;
			/* printf("Sim: Flop %d\n", omp_get_thread_num()); */
		      
			// Get Payout -- BET 2x

			#pragma omp critical (twoxbet)
			{
			  QueryBinArray(&p, state, is_my_card, PLAYERS, 5);
			  /* printf("Sim: Get 2x Payout %d\n", omp_get_thread_num()); */

			  // Turnn & River
			  itcard(turn, &p.hand[5]);
			  itcard(river, &p.hand[6]);
			  itcard(turn, &d.hand[5]);
			  itcard(river, &d.hand[6]);
			  p.hand[5].whos_card = BOARDS;
			  p.hand[6].whos_card = BOARDS;
			  d.hand[5].whos_card = BOARDS;
			  d.hand[6].whos_card = BOARDS;
			  /* printf("Sim: Turn and River %d\n", omp_get_thread_num()); */
		      
			  // Get Payouts
			  twoxpay = Pays(&p, &d, 2.0, 1.0, 1.0);
			  onexpay = Pays(&p, &d, 1.0, 1.0, 1.0);

			  twoxbet_payl[stateToIndex(state)] += twoxpay;
			  QueryBinArray(&p, state, is_my_card, PLAYERS, 7);
			  onexbet_payl[stateToIndex(state)] += onexpay;
			} // END Parallel Critical

			/* printf("Sim: Get Payouts %d\n", omp_get_thread_num()); */
		      
			// Debugging Delete Me
			/* float test = Pays(&p, &d, 2.0, 1.0, 1.0); */
			/* printf("Two Bet: %f\n", test); */
			/* printf("Two Bet: %f\nOne Bet: %f\n", twoxbet_pay[stateToIndex(state)], onexbet_pay[stateToIndex(state)]); */
			// END Debugging

			// Clean Up
			reset_bin(&p.bin);
			reset_bin(&d.bin);
			/* printf("holep0 %d\nholep1 %d\nholed0 %d\nholed1 %d\nflop0 %d\nflop1 %d\nflop2 %d\nturn %d\nriver %d\n", holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river); */
			/* printf("Sim: Clean Up %d\n", omp_get_thread_num()); */
			/* printf("Current Combination %d\n", totalcombin); */

		      } // end giant if
		/*     } // end river */
		/* } // end turn */
	      } // end flop2
	    } // end flop1
	  } // end flop0
	} // end holed1
      } // end holed0
    } // end holep1
    for(i =0; i < 9; i++)
      {
	twoxbet_pay[i] += twoxbet_payl[i];
	onexbet_pay[i] += onexbet_pay[i];
      }

  } // end holep0
  /* printf("Done\n"); */
  int idx = 0;
  for(idx = 0; idx < 9; idx++)
    {
      printf("2x State: %d Payout: %f\n", idx, twoxbet_pay[idx]);
      printf("1x State: %d Payout: %f\n", idx, onexbet_pay[idx]);
    }
  printf("Total Combinations: %lld\n", totalcombin);
  printf("Total Iterations: %lld\n", iterations);
}

