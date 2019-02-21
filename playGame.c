#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<time.h>
#include"Pos.h"
#include"Move.h"
#include "HumanInteraction.h"
#include "GameSpecificHumanInteraction.h"
#include"Bot.h"
#include"myHash.h"
#include "Hashtable_Stuff/hashtable.h"
#include "Hashtable_Stuff/hashtable_itr.h"

//https://github.com/ryantenney/chashtable

/*

Model Name: MacBook Air
     Model Identifier: MacBookAir6,2
     Processor Name: Intel Core i5
     Processor Speed: 1.3 GHz
     Number of Processors: 1
     Total Number of Cores: 2
     L2 Cache (per Core): 256 KB
     L3 Cache: 3 MB
     Memory: 4 GB
     Boot ROM Version: MBA61.0099.B22
     SMC Version (system): 2.13f15
     Serial Number (system): C02LF6W5F5V8
     Hardware UUID: 6DCCC9C9-D506-5259-8ED3-CB6BB414E98D

Results after Valgrind ./Chomp 12 x 11, then makeMove 7x10, 6x10, 6x10 until 2 column hit and 0x0:
==11555==
==11555== HEAP SUMMARY:
==11555==     in use at exit: 0 bytes in 0 blocks
==11555==   total heap usage: 48,622,579 allocs, 48,622,579 frees, 1,905,864,848 bytes allocated
==11555==
==11555== All heap blocks were freed -- no leaks are possible
==11555==
==11555== For counts of detected and suppressed errors, rerun with: -v
==11555== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)





Results after valgrind --tool=cachegrind ./Chomp dimensions 7x6, then move 0,0


Did you really think your tiny human mind could defeat a computer?
==20387==
==20387== I   refs:      45,930,340
==20387== I1  misses:         1,520
==20387== LLi misses:         1,350
==20387== I1  miss rate:       0.00%
==20387== LLi miss rate:       0.00%
==20387==
==20387== D   refs:      12,338,960  (8,303,671 rd   + 4,035,289 wr)
==20387== D1  misses:       818,694  (  419,300 rd   +   399,394 wr)
==20387== LLd misses:       401,332  (    3,441 rd   +   397,891 wr)
==20387== D1  miss rate:        6.6% (      5.0%     +       9.9%  )
==20387== LLd miss rate:        3.3% (      0.0%     +       9.9%  )
==20387==
==20387== LL refs:          820,214  (  420,820 rd   +   399,394 wr)
==20387== LL misses:        402,682  (    4,791 rd   +   397,891 wr)
==20387== LL miss rate:         0.7% (      0.0%     +       9.9%  )

  */

/*
 * NEXT STEPS:
 *
 * 3) optionally load data from textfiles
 * 4) optionally write data to textfiles
 * 1) Profile code for memory leaks and time.
 *    -be sure to destroy hashtables when you're done with them:
 *    -the above entails modifying hashtable_private.h and hashtable.c
 *    -specifically create_hashtable to give it proper function pointers to
 *    -free key and free value
 *    ^Valgrind on eniac for memory leaks, gprof on eniac for time
 *    http://wwwcdf.pd.infn.it/valgrind/cg_main.html - how to use cache profiler
 * 2) ideas on how to tweak time taken
 *     -try change hash function
 *     -try change base of makeMoveHelper from 2 to 1
 *     -BEST IDEA: change freeMovesList and freeUnknowns so that they
 *                skip over one (or more) of their members so that you don't
 *                have to waste time mallocing, copying, and freeing each time
 *                you want to add to your hashtables
 *                -alternatively, change the Move struct to have a flexible array
 *                at the end instead of another pointer. In this way, mList can be
 *               one contiguous block of memory and you have to do way less
 *              mallocing and freeing, and no need for deep freeing mList
 *            can also change Pos to have flexible array for rows, to increase
 *            memory contiguousness and decrease number of mallocs and frees.
 *           Use profiler on your code to see if making these changes could potentially
 *          save a noticeable amount of time
 *          Also can make mList and unKnowns and copy global variables, and only free them once,
 *           at the end of your program.
 *          Furthermore, can consider an unMove, if copyingPos turns out to take a long time.
 * 5) for fun: try to understand in full detail how their hashtable and hashfunction works
 */

void startGame(int playComputer, int humanFirst, int pretty, Pos* p, time_t* elapsedTime) {
  int player = 1; //players are 0 and 1, with 0 going first
  int firstComputerTurn = 1;
  int computerNumber = humanFirst; //computerNumber is 0 or 1 - i.e. the player number of the computer
  while(!gameFinished(p)) {
    player = playerToMove(p, player); //switching turns
    int computerTurn = playComputer && (player == computerNumber); //is it the computer's turn?
    printf("\n");
    printPos(p, pretty, stdout);
    if(computerTurn) {
      printf("Computer moves:\n");
      Move m;
      allocateMoveFields(p, &m);
      if(firstComputerTurn) {
        time_t startTime = time(NULL);
        miniMax(p, 1, 1, &m);
        time_t stopTime = time(NULL);
        *elapsedTime = stopTime - startTime;
        firstComputerTurn = 0;
      }
      else {
        miniMax(p, 1, 1, &m);
      }
      printMove(&m);
      makeMove(&p, &m);
      deallocateMoveFields(&m);
    }
    else {
      printf("Player %d turn:\n", player + 1);
      Move* m = getValidHumanMove(p);
      makeMove(&p, m);
      freeMove(m);
    }
  }

  int winner = getWinner(p, player);

  if(winner == 2) {
    printf("Tied game!");
    return;
  }

  if(playComputer) {
    int computerWon = (winner == computerNumber);
    if(computerWon) {
      printf("Did you really think your primitive human mind could defeat a computer?\n");
    }

    else { //only other possibility is that computer lost, since we dealt with tie case above.
      printf("You have defeated the computer!\n");
    }
  }

  else {
    printf("Player %d wins!\n", winner + 1);
  }
}

void freeKeyPos(Pos* p) {
  freePos(&p);
}
void freeLosVal(void* v) {
//need to define this to satisfy what I wrote for create_hashtable. Los Val is not a pointer to the heap.
}

hashtable* losingPositions = NULL;
hashtable* winningPositions = NULL;
int in = 1;


int main(int argc, char* argv[]) {
  Pos* p = humanInputPos();
  int pretty = 1;
  int playComputer = getYesNo("Would you like to play against the computer? (0/1):");
  int humanFirst = 0;
  if(playComputer) {
    humanFirst = getYesNo("Would you like to go first? (0/1):");
    losingPositions = create_hashtable(1000000, hashPosition, equalPositions, freeKeyPos, freeLosVal);
    winningPositions = create_hashtable(1000000, hashPosition, equalPositions, freeKeyPos, freeMove);
  }
  time_t elapsedTime;
  startGame(playComputer, humanFirst, pretty, p, &elapsedTime);
  setlocale(LC_NUMERIC, "");
  if(playComputer) {
    printf("Time taken to compute all positions: %'ld seconds\n", elapsedTime);
  }
  if(losingPositions) {
    printf("Losing positions hashed: %'u\n", hashtable_count(losingPositions));
    hashtable_destroy(losingPositions, 1);
  }
  if(winningPositions) {
    printf("Winning positions hashed: %'u\n", hashtable_count(winningPositions));
    hashtable_destroy(winningPositions, 1);
  }
  return 0;
}
