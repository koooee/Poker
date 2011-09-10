/* Payouts.h */
/* File Containing Payout tables for winning/loosing hands */
/* There is no functionality in this file */
/* Date: 9/10/2011 */
/* Programmer: Nick Kolegraff */

/* Payout tables are from Michael Shacklefords 'Wizard of Odds' website Ultimate Texas Holdem Strategy Page */
/* RF, SF, FK, FH, F, S, all others push */
float blind_pay[7] = { 500.0, 50.0, 10.0, 3.0, 1.5, 1.0, 0.0 };

/* RF, SF, FK, FH, F, S, TK, all others loose */
float trips_pay[8] = { 50.0, 40.0, 30.0, 9.0, 7.0, 4.0, 3.0, -1.0 };

/* ante and plays pay 1:1 for a winning hand */
float ante_pay_win = 0.0;
float ante_pay_loss = 0.0;
