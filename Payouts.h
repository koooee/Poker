/* Payouts.h */
/* File Containing Payout tables for winning/loosing hands */
/* There is no functionality in this file */
/* Date: 9/10/2011 */
/* Programmer: Nick Kolegraff */

/* Payout tables are from Michael Shacklefords 'Wizard of Odds' website Ultimate Texas Holdem Strategy Page */

/* we want the index of these arrays to line up with hand ranks, to make life easy */
/* HC, P, TP, TK, S, F, FH, FK, SF, RF */
float blind_pay[10] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.5, 3.0, 10.0, 50.0, 500.0 };
float trips_pay[10] = { -1.0, -1.0, -1.0, 3.0, 4.0, 7.0, 9.0, 30.0, 40.0, 50.0 };

/* ante and plays pay 1:1 for a winning hand */
float ante_pay_win = 1.0;
float ante_pay_loss = -1.0;

