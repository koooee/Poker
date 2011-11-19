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
void jtest();
void ctest();
void itcard(int icard, CARD *c);

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
  /*  Simulate(); */
  jtest();
  /* ctest(); */
}

void itcard(int icard, CARD *c)
{
  c->rank = icard % 13;
  c->suit = icard % 4;
}
void ctest()
{

  int i,j,k,l,m,n,o,threadnum,size=40;
  unsigned long long int count=0;
/* #pragma omp parallel shared(threadnum, count, size) private(i, j, k, l, m, n, o) */
/*   { */
/*     threadnum = omp_get_thread_num(); */
#pragma omp parallel for shared(size) private(i, j, k,l,m,n,o) reduction(+:count)
  for(i = 0; i < size; i++){
    printf("Thread %d executing %d\n", omp_get_thread_num(), i);
    for(j = i+1; j < size; j++)
      for(k = j +1; k < size; k++)
	for(l=0; l < size-3; l++)
	  for(m=l+1; m < size-3; m++)
	    for(n=0; n < size-5; n++)
	      for(o=n+1; o < size-5; o++){	   
		  count++;
	      } // END inner most for
  } // END outer most for
/* } // END Parallel Region */
  printf("%lld\n", count);
}
void jtest()
{
  unsigned long long totalcombin = 0;
  int action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river;
  int decksize = 32;
  /* #pragma omp parallel shared(decksize, totalcombin) private(action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river) */
  /* { */
#pragma omp parallel for shared(decksize) private(action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river) reduction(+:totalcombin)
  for(holep0 = 0; holep0 < decksize; holep0++){
    printf("Thread %d executes %d\n", omp_get_thread_num(), holep0);
    for(holep1 = holep0 + 1; holep1 < decksize; holep1++){
      for(flop0 = 0; flop0 < decksize-2; flop0++){
	for(flop1 = flop0 + 1; flop1 < decksize-2; flop1++){
	  for(flop2 = flop1 + 1; flop2 < decksize -2; flop2++){
	    for(turn = 0; turn < decksize - 5; turn++){
	      for(river = turn + 1; river < decksize - 5; river++){
		for(holed0 = 0; holed0 < decksize - 7; holed0++){
		  for(holed1 = holed0 + 1; holed1 < decksize-7; holed1++)
		    {
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

/* } // End Parallel Region */
  printf("Final: %lld\n", totalcombin);
}

void Simulate()
{
  printf("Sim: Entered\n");
  PLAYER p;
  PLAYER d;
  unsigned long long totalcombin = 0;
  int decksize = 10;
  // Pay outs for 9 different states
  float twoxbet_pay[9];
  float onexbet_pay[9];
  float twoxpay, onexpay;
  int i;

  int action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river;
  char *state;
  char *is_my_card;
  printf("Sim: Variables Declared\n");
  for(i = 0; i < 9; i++)
    {
      twoxbet_pay[i] = 0.0;
      onexbet_pay[i] = 0.0;
    }

#pragma omp parallel shared(totalcombin, decksize, twoxbet_pay, onexbet_pay) private(p, d, twoxpay, onexpay, action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river, state, is_my_card)
  {
    printf("Sim: in parallel region %d\n", omp_get_thread_num());
    InitPlayer(&p);
    InitPlayer(&d);
    state = malloc(2 * sizeof(char));
    printf("Sim: Thread %d initialized.\n", omp_get_thread_num());

#pragma omp parallel for
  for(holep0 = 0; holep0 < decksize; holep0++){
    printf("Thread %d Executing Iteration %d\n", omp_get_thread_num(), holep0);
    for(holep1 = holep0+1; holep1 < decksize; holep1++){
      for(holed0 = 0; holed0 < decksize; holed0++){
	for(holed1 = holed0+1; holed1 < decksize; holed1++){
	  for(flop0 = 0; flop0 < decksize; flop0++){
	    for(flop1 = flop0+1; flop1 < decksize; flop1++){
	      for(flop2 = flop1+1; flop2 < decksize; flop2++){
		for(turn = 0; turn < decksize; turn++){
		  for(river = turn+1; river < decksize; river++){
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
		      /* printf("Sim: Players Hole Cards %d\n", omp_get_thread_num()); */
		      itcard(holep0, &p.hand[0]);
		      itcard(holep1, &p.hand[1]);
		      p.hand[0].whos_card = PLAYERS;
		      p.hand[1].whos_card = PLAYERS;
			  
		      // Dealers Hole Cards
		      /* printf("Sim: Dealers Hole Cards %d\n", omp_get_thread_num()); */
		      itcard(holed0, &d.hand[0]);
		      itcard(holed1, &d.hand[1]);
		      d.hand[0].whos_card = DEALERS;
		      d.hand[1].whos_card = DEALERS;

		      // Flop
		      /* printf("Sim: Flop %d\n", omp_get_thread_num()); */
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
		      
		      // Get Payout -- BET 2x
		      /* printf("Sim: Get 2x Payout %d\n", omp_get_thread_num()); */
		      QueryBinArray(&p, state, is_my_card, PLAYERS, 5);

		      // Turn & River
		      /* printf("Sim: Turn and River %d\n", omp_get_thread_num()); */
		      itcard(turn, &p.hand[5]);
		      itcard(river, &p.hand[6]);
		      itcard(turn, &d.hand[5]);
		      itcard(river, &d.hand[6]);
		      p.hand[5].whos_card = BOARDS;
		      p.hand[6].whos_card = BOARDS;
		      d.hand[5].whos_card = BOARDS;
		      d.hand[6].whos_card = BOARDS;
		      
		      // Get Payouts
		      /* printf("Sim: Get Payouts %d\n", omp_get_thread_num()); */
		      twoxpay = Pays(&p, &d, 2.0, 1.0, 1.0);
		      onexpay = Pays(&p, &d, 1.0, 1.0, 1.0);
		      twoxbet_pay[stateToIndex(state)] += twoxpay;
		      QueryBinArray(&p, state, is_my_card, PLAYERS, 7);
		      onexbet_pay[stateToIndex(state)] += onexpay;
		      
		      // Debugging Delete Me
		      /* float test = Pays(&p, &d, 2.0, 1.0, 1.0); */
		      /* printf("Two Bet: %f\n", test); */
		      /* printf("Two Bet: %f\nOne Bet: %f\n", twoxbet_pay[stateToIndex(state)], onexbet_pay[stateToIndex(state)]); */
		      // END Debugging

		      // Clean Up
		      /* printf("Sim: Clean Up %d\n", omp_get_thread_num()); */
		      reset_bin(&p.bin);
		      reset_bin(&d.bin);
		      printf("holep0 %d\nholep1 %d\nholed0 %d\nholed1 %d\nflop0 %d\nflop1 %d\nflop2 %d\nturn %d\nriver %d\n", holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river);

		    } // end giant if
		  } // end river
		} // end turn
	      } // end flop2
	    } // end flop1
	  } // end flop0
	} // end holed1
      } // end holed0
    } // end holep1
  } // end holep0
  } // END Parallel Region
  printf("Done\n");
  int idx = 0;
  for(idx = 0; idx < 9; idx++)
    {
      printf("2x State: %d Payout: %f\n", idx, twoxbet_pay[idx]);
      printf("1x State: %d Payout: %f\n", idx, onexbet_pay[idx]);
    }
  printf("Total Combinations: %lld\n", totalcombin);
}

