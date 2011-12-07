/* Sim.c
The purpose of this file is to run the game simulations -- Think Main()
Programmer: Nick Kolegraff
Date: 5/29/2011
*/

#include "Player.cu"
#include "cutil_inline.h"
#include <cuPrintf.cu>
#include "stdio.h"
#include "unistd.h" /* hahahah, unistd.... */

// #define GRID_DIMX 52
// #define GRID_DIMY 52
// #define GRID_DIMZ 1
// #define BLOCK_DIMX 52
// #define BLOCK_DIMY 1
// #define BLOCK_DIMZ 1

#define GRID_DIMX 52
#define GRID_DIMY 52
#define GRID_DIMZ 1
#define BLOCK_DIMX 52
#define BLOCK_DIMY 1
#define BLOCK_DIMZ 1

int  *h_results, *d_results;

__device__
void printc(int rank, int suit)
{
  char *ranks = "23456789TJQKA";
  char *suits = "sdch";
  cuPrintf("%c%c ", ranks[rank], suits[suit]);
}

__device__
void printh(CARD *hand, int size)
{
  int i;
  for(i =0; i < size; i++)
    {
      printc(hand[i].rank, hand[i].suit);
    }
  cuPrintf("\n");
}

/* Kernel Method..where the magic happens*/
/* Quora is the shit...just found an answer to a combinatorics function that removes the iteration dependency!!! You sent an email to youself search for combinatorics */
__global__ 
void RunSim(int *results)
{
  PLAYER p;
  InitPlayer(&p);
  int start, i, j, k, l, m, n, rank, size=52;
  int threadsPerBlock = blockDim.x * blockDim.y;
  int threadNumInBlock = threadIdx.x;
  int blockNumInGrid = blockIdx.x + (gridDim.x * blockIdx.y);
  unsigned long long gindex = threadNumInBlock + threadsPerBlock * blockNumInGrid;
  // unsigned long long gdex;
  // for(start = 0; start < 4; start++)
  // if(threadIdx.x < blockIdx.x && threadIdx.x < blockIdx.y && blockIdx.y < blockIdx.x)
  //   {
  for(i = 0; i < size; i++)
    for(j = i+1; j < size; j++)
      for(k = 0; k < size; k++)
  	for(l = k+1; l < size; l++)
  	  // for(m = l+1; m < size; m++)
	  //   for(n = m+1; n < size; n++)

	      {
		if(
		   threadIdx.x != blockIdx.y && threadIdx.x != blockIdx.x && threadIdx.x != i && threadIdx.x != j && threadIdx.x != k && threadIdx.x != l
		   && blockIdx.y != blockIdx.x && blockIdx.y != i && blockIdx.y != j && blockIdx.y != k && blockIdx.y != l
		   && blockIdx.x != i && blockIdx.x != j && blockIdx.x != k && blockIdx.x != l
		   && i != j && i != k && i != l
		   && j != k && j != l
		   && k != l
		   )
		  {

		    p.hand[0].suit = blockIdx.x % 4;
		    p.hand[0].rank = blockIdx.x % 13;
		    // p.hand[0].whos_card = BOARDS;
		    
		    p.hand[1].suit = blockIdx.y % 4;
		    p.hand[1].rank = blockIdx.y % 13;
		    // p.hand[0].whos_card = BOARDS;
		    
		    p.hand[2].suit = threadIdx.x % 4;
		    p.hand[2].rank = threadIdx.x % 13;
		    // p.hand[0].whos_card = BOARDS;
		    
		    p.hand[3].suit = i % 4;
		    p.hand[3].rank = i % 13;
		    // p.hand[0].whos_card = PLAYERS;
		    
		    p.hand[4].suit = j % 4;
		    p.hand[4].rank = j % 13;
		    // p.hand[0].whos_card = PLAYERS;
		    
		    p.hand[5].suit = k % 4;
		    p.hand[5].rank = k % 13;
		    // p.hand[0].whos_card = BOARDS;
		    
		    p.hand[6].suit = l % 4;
		    p.hand[6].rank = l % 13;
		    // p.hand[0].whos_card = BOARDS;

		    rank = rank_hand(p.hand, &p.bin, 7);
		    // atomicAdd(&results[rank], 1);
		  } 
	      }
  // }
}

int main(int argc, char *argv[])
{
  cudaEvent_t evt;
  cudaEventCreate(&evt);
  cudaPrintfInit();
  int size = 9;

  // initialize memory on host
  printf("Malloc-ing memory on host...");
  h_results = (int *)malloc(size * sizeof(int *));
  int x;
  for(x = 0; x < size; x++)
    {
      h_results[x] = 0;
    }
  printf("Done.\n");

  // copy host memory to device
  printf("Malloc-ing memory on device...");
  cudaMalloc(&d_results, size * sizeof(int *));
  cudaMemcpy(d_results, h_results, size * sizeof(int *), cudaMemcpyHostToDevice);
  printf("Done.\n");
  
  // set up kernel dimensions
  dim3 GRID(GRID_DIMX, GRID_DIMY, GRID_DIMZ);
  dim3 BLOCK(BLOCK_DIMX, BLOCK_DIMY, BLOCK_DIMZ);

  // printf("Executing Kernel.");
  RunSim<<<GRID, BLOCK>>>(d_results);
  cutilCheckMsg("\nKernel execution failed\n");
  
  // Wait for kernel
  cudaEventRecord(evt, NULL);
  while(cudaEventQuery(evt) == cudaErrorNotReady) { usleep(1000000); }

  printf("\nCopying resulsts from device to host...");
  cudaMemcpy(h_results, d_results, size * sizeof(int *), cudaMemcpyDeviceToHost);
  printf("Done.\n");


  cudaPrintfDisplay(stdout, true);
  cudaEventDestroy(evt);  
  int i;
  for(i = 0; i < size; i++)
    {
      printf("Hand Rank %d: %d\n",i,  h_results[i]);
    }
}
