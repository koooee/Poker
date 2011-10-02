/* Sim.c
   The purpose of this file is to run the game simulations -- Think Main()
   Programmer: Nick Kolegraff
   Date: 5/29/2011
*/

#include "Player.c"
#include "Test.h"

void Simulate();
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
  Simulate();
}

void itcard(int icard, CARD *c)
{
  c->rank = icard % 13;
  c->suit = icard % 4;
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
    for(holep1 = 0; holep1 < decksize; holep1++){
      for(holed0 = 0; holed0 < decksize; holed0++){
	for(holed1 = 0; holed1 < decksize; holed1++){
	  for(flop0 = 0; flop0 < decksize; flop0++){
	    for(flop1 = 0; flop1 < decksize; flop1++){
	      for(flop2 = 0; flop2 < decksize; flop2++){
		for(turn = 0; turn < decksize; turn++){
		  for(river = 0; river < decksize; river++){
		    if(holep0 != holep1 && holep0 != holed0 && holep0 != holed1 && holep0 != flop0 && holep0 != flop1 && holep0 != flop2 && holep0 != turn && holep0 != river
		       && holep1 != holed0 && holep1 != holed1 && holep1 != flop0 && holep1 != flop1 && holep1 != flop2 && holep1 != turn && holep1 != river
		       && holed0 != holed1 && holed0 != flop0 && holed0 != flop1 && holed0 != flop2 && holed0 != turn && holed0 != river
		       && holed1 != flop0 && holed1 != flop1 && holed1 != flop2 && holed1 != turn && holed1 != river
		       && flop0 != flop1 && flop0 != flop2 && flop0 != turn && flop0 != river
		       && flop1 != flop2 && flop1 != turn && flop1 != river
		       && flop2 != turn && flop2 != river
		       && turn != river){
		      totalCombin++;

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
			  
		      // Dealers Hole Cards
		      itcard(holed0, &d.hand[0]);
		      itcard(holed1, &d.hand[1]);
		      d.hand[0].whos_card = DEALERS;
		      d.hand[0].whos_card = DEALERS;

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
		      
		      // Get Payout -- BET 2x
		      QueryBinArray(&p, state, is_my_card, PLAYERS, 5);

		      // Turn & River
		      itcard(turn, &p.hand[5]);
		      itcard(river, &p.hand[6]);
		      itcard(turn, &d.hand[5]);
		      itcard(river, &d.hand[6]);
		      p.hand[5].whos_card = BOARDS;
		      p.hand[6].whos_card = BOARDS;
		      d.hand[5].whos_card = BOARDS;
		      d.hand[6].whos_card = BOARDS;
		      
		      // Get Payouts
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
		      reset_bin(&p.bin);
		      reset_bin(&d.bin);

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

