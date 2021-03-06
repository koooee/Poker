/* Sim.c
   The purpose of this file is to run the game simulations -- Think Main()
   Programmer: Nick Kolegraff
   Date: 5/29/2011
*/

#include "Player.c"
#include "Test.c"

void Simulate();
void jtest();
void itcard(int icard, CARD *c);
void RandomSimulation();
// 'state' is defined as an abstraction of the current event -- ie, Drawing Hand, Made Hand.
// the function will return a char array of two elements:
//  0th index: made a hand
//      1 -- your card makes the hand
//      2 -- hand is played by the board
//      3 -- you don't have a made hand
//  1th index: drawing to a hand
//      1 -- your card makes the draw
//      2 -- draw is played by the board
//      3 -- you don't have a draw

char *state_names[9] = {
  "You have a made hand, your card makes the hand, you also havd a draw, your card makes the draw",
  "You have a made hand, your card makes the hand, you have a draw, draw is played by the board",
  "You have a made hand, your card makes the hand, you do not have a draw",
  "You have a made hand played by the board, you have a draw, your hand makes the draw",
  "You have a made hand played by the board, you also have a draw played by the board",
  "You have a made hand played by the board, you do not have a draw",
  "You do not have a made hand, you have a draw, your hand makes the draw",
  "You do not have a made hand, you have a draw, draw is played by the board",
  "You do not have a made hand, you do not have a draw"
};

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
  /* Simulate(); */
  /* jtest(); */
  /* test_factorial(); */
  /* test_choose(); */
  /* test_largestv(); */
  /* test_combinadic(); */
  long long x;
  x = 10LL;
  ttest(12345);
  RandomSimulation(x);
}

void itcard(int icard, CARD *c)
{
  c->rank = icard % 13;
  c->suit = icard % 4;
}

void jtest()
{
  unsigned long long totalcombin = 0;
  int action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river;
  int decksize = 52;
  for(holep0 = 0; holep0 < decksize; holep0++){
    for(holep1 = holep0+1; holep1 < decksize; holep1++){
      printf("%d\n", holep1);
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
  printf("Final: lld\n", totalcombin);
}

void Simulate()
{
  PLAYER p;
  PLAYER d;
  unsigned long long totalCombin = 0;
  int decksize = 52;
  // Pay outs for 9 different states
  float twoxbet_pay[9];
  float onexbet_pay[9];
  float twoxpay, onexpay;
  int i;
  for(i = 0; i < 9; i++)
    {
      twoxbet_pay[i] = 0.0;
      onexbet_pay[i] = 0.0;
    }

  int action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river;
  InitPlayer(&p);
  InitPlayer(&d);
  char *state = malloc(2 * sizeof(char));
  char *is_my_card;
  for(holep0 = 0; holep0 < decksize; holep0++){
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
		      totalCombin++;
		      
		      /* // There are four actions described below */
		      /* // Bet 4x                  0 */
		      /* // Check First, Bet 2x     1 */
		      /* // Check, Check, Bet 1x    2 */
		      /* // Check, Check, Fold      3 */

		      /* // Players Hole Cards */
		      /* itcard(holep0, &p.hand[0]); */
		      /* itcard(holep1, &p.hand[1]); */
		      /* p.hand[0].whos_card = PLAYERS; */
		      /* p.hand[1].whos_card = PLAYERS; */
			  
		      /* // Dealers Hole Cards */
		      /* itcard(holed0, &d.hand[0]); */
		      /* itcard(holed1, &d.hand[1]); */
		      /* d.hand[0].whos_card = DEALERS; */
		      /* d.hand[1].whos_card = DEALERS; */

		      /* // Flop */
		      /* itcard(flop0, &p.hand[2]); */
		      /* itcard(flop1, &p.hand[3]); */
		      /* itcard(flop2, &p.hand[4]); */
		      /* itcard(flop0, &d.hand[2]); */
		      /* itcard(flop1, &d.hand[3]); */
		      /* itcard(flop2, &d.hand[4]); */
		      /* p.hand[2].whos_card = BOARDS; */
		      /* p.hand[3].whos_card = BOARDS; */
		      /* p.hand[4].whos_card = BOARDS; */
		      /* d.hand[2].whos_card = BOARDS; */
		      /* d.hand[3].whos_card = BOARDS; */
		      /* d.hand[4].whos_card = BOARDS; */
		      
		      /* // Get Payout -- BET 2x */
		      /* QueryBinArray(&p, state, is_my_card, PLAYERS, 5); */

		      /* // Turn & River */
		      /* itcard(turn, &p.hand[5]); */
		      /* itcard(river, &p.hand[6]); */
		      /* itcard(turn, &d.hand[5]); */
		      /* itcard(river, &d.hand[6]); */
		      /* p.hand[5].whos_card = BOARDS; */
		      /* p.hand[6].whos_card = BOARDS; */
		      /* d.hand[5].whos_card = BOARDS; */
		      /* d.hand[6].whos_card = BOARDS; */
		      
		      /* // Get Payouts */
		      /* twoxpay = Pays(&p, &d, 2.0, 1.0, 1.0); */
		      /* onexpay = Pays(&p, &d, 1.0, 1.0, 1.0); */
		      /* twoxbet_pay[stateToIndex(state)] += twoxpay; */
		      /* QueryBinArray(&p, state, is_my_card, PLAYERS, 7); */
		      /* onexbet_pay[stateToIndex(state)] += onexpay; */
		      
		      /* // Debugging Delete Me */
		      /* /\* float test = Pays(&p, &d, 2.0, 1.0, 1.0); *\/ */
		      /* /\* printf("Two Bet: %f\n", test); *\/ */
		      /* /\* printf("Two Bet: %f\nOne Bet: %f\n", twoxbet_pay[stateToIndex(state)], onexbet_pay[stateToIndex(state)]); *\/ */
		      /* // END Debugging */

		      /* // Clean Up */
		      /* reset_bin(&p.bin); */
		      /* reset_bin(&d.bin); */

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
      
  int idx = 0;
  for(idx = 0; idx < 9; idx++)
    {
      printf("2x State: %d Payout: %f\n", idx, twoxbet_pay[idx]);
      printf("1x State: %d Payout: %f\n", idx, onexbet_pay[idx]);
    }
  printf("Total Combinations: %lld\n", totalCombin);
}

CARD *random_hand(){
  long long *out;
  out = (long long *)malloc((9*sizeof(long long)));
  combinadic(52,9,rand()%choose(52,9), out);
  int i;
  CARD *h;
  h = (CARD *)malloc(sizeof(CARD)*9);
  for(i = 0; i < 9; i++){
    /* FIXME we are casing a long long to int, will break for large games */
    itcard((int)out[i], &h[i]);
  }
  free(out);
  return h;
}

/* FIXME for some reason gcc is not compiling  this parameter corectly
I can't pass a value to this function without getting the max value for the
long long x parameter.  But if I define it inside the function it is fine, yet other
functions can be used in the same way */
void RandomSimulation(){
  PLAYER p;
  PLAYER d;
  unsigned long long totalCombin = 0;
  int decksize = 52;
  // Pay outs for 9 different states
  float twoxbet_pay[9];
  float onexbet_pay[9];
  float twoxpay, onexpay;
  int i;
  long wins2x, losses2x, wins1x, losses1x;
  
  wins2x = wins1x = 0L;
  losses2x = losses1x = 0L;
  for(i = 0; i < 9; i++)
    {
      twoxbet_pay[i] = 0.0;
      onexbet_pay[i] = 0.0;
    }

  int action, holep0, holep1, holed0, holed1, flop0, flop1, flop2, turn, river;
  InitPlayer(&p);
  InitPlayer(&d);
  char *state = malloc(2 * sizeof(char));
  char *is_my_card;
  long long s;
  long long num_times;
  num_times = 1000000LL;
  for(s = 0LL; s < num_times; s++){
    /*  There are four actions described below */
    /*  Bet 4x                  0 */
    /*  Check First, Bet 2x     1 */
    /*  Check, Check, Bet 1x    2 */
    /*  Check, Check, Fold      3 */

    /* Generate a Random Hand */
    CARD *rhand = random_hand();
    /*  Players Hole Cards */
    p.hand[0] = rhand[0];
    p.hand[1] = rhand[1];
    p.hand[0].whos_card = PLAYERS;
    p.hand[1].whos_card = PLAYERS;
			  
    /*  Dealers Hole Cards */
    d.hand[0] = rhand[2];
    d.hand[1] = rhand[3];
    d.hand[0].whos_card = DEALERS;
    d.hand[1].whos_card = DEALERS;

    /*  Flop */
    p.hand[2] = rhand[4];
    p.hand[3] = rhand[5];
    p.hand[4] = rhand[6];
    d.hand[2] = rhand[4];
    d.hand[3] = rhand[5];
    d.hand[4] = rhand[6];

    p.hand[2].whos_card = BOARDS;
    p.hand[3].whos_card = BOARDS;
    p.hand[4].whos_card = BOARDS;
    d.hand[2].whos_card = BOARDS;
    d.hand[3].whos_card = BOARDS;
    d.hand[4].whos_card = BOARDS;
		      
    /*  Get Payout -- BET 2x */
    /* QueryBinArray(&p, state, is_my_card, PLAYERS, 5); */

    /*  Turn & River */
    p.hand[5] = rhand[7];
    p.hand[6] = rhand[8];
    d.hand[5] = rhand[7];
    d.hand[6] = rhand[8];
    p.hand[5].whos_card = BOARDS;
    p.hand[6].whos_card = BOARDS;
    d.hand[5].whos_card = BOARDS;
    d.hand[6].whos_card = BOARDS;
		      
    /*  Get Payouts */
    twoxpay = Pays(&p, &d, 2.0, 1.0, 1.0);

    /* Count the 2x bet wins */
    if(twoxpay > 0){
      wins2x++;
    }
    else {
      losses2x++;
    }

    onexpay = Pays(&p, &d, 1.0, 1.0, 1.0);
    
    /* Count the 1x bet wins */
    if(onexpay > 0){
      wins1x++;
    }
    else {
      losses1x++;
    }

    twoxbet_pay[stateToIndex(state)] += twoxpay;
    QueryBinArray(&p, state, is_my_card, PLAYERS, 7);
    onexbet_pay[stateToIndex(state)] += onexpay;
		      
    /*  Debugging Delete Me */
    /* /\* float test = Pays(&p, &d, 2.0, 1.0, 1.0); *\/ */
    /* /\* printf("Two Bet: %f\n", test); *\/ */
    /* /\* printf("Two Bet: %f\nOne Bet: %f\n", twoxbet_pay[stateToIndex(state)], onexbet_pay[stateToIndex(state)]); *\/ */
    /*  END Debugging */

    /* Clean Up */
    reset_bin(&p.bin);
    reset_bin(&d.bin);
    free(rhand);
  }
  
  /* Output Results */
// 'state' is defined as an abstraction of the current event -- ie, Drawing Hand, Made Hand.
// the function will return a char array of two elements:
//  0th index: made a hand
//      1 -- your card makes the hand
//      2 -- hand is played by the board
//      3 -- you don't have a made hand
//  1th index: drawing to a hand
//      1 -- your card makes the draw
//      2 -- draw is played by the board
//      3 -- you don't have a draw

  int idx = 0;
  for(idx = 0; idx < 9; idx++)
    {
      printf("2x State: %s Payout: %f\n", state_names[idx], twoxbet_pay[idx]);
      printf("1x State: %s Payout: %f\n", state_names[idx], onexbet_pay[idx]);
    }
  printf("Wins2x: %ld\nLosses2x: %ld\nWins1x: %ld\nLosses1x: %ld\n", wins2x, losses2x, wins1x, losses1x);
  printf("Total Iterations: %lld\n", num_times);

}
