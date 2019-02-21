
#if you want to profile with Valgrind or Cachegrind or gprof, add -pg flag to CFLAGS
CFLAGS = -Wall -O3 -std=c99
GAME = Chomp
GAME_DIRECTORY = $(GAME)_DIRECTORY
OBJECTS = playGame.o Pos.o Move.o HumanInteraction.o Bot.o ArrayUtilities.o \
 					hashtable.o hashtable_itr.o myHash.o GameSpecificHumanInteraction.o

all: $(OBJECTS) Makefile
	cc $(CFLAGS) -lm $(OBJECTS) -o $(GAME)

HumanInteraction.o: HumanInteraction.c HumanInteraction.h Makefile
	cc $(CFLAGS) -c HumanInteraction.c

GameSpecificHumanInteraction.o: $(GAME_DIRECTORY)/GameSpecificHumanInteraction.c  GameSpecificHumanInteraction.h Move.h Pos.h Makefile
	cc $(CFLAGS) -c $(GAME_DIRECTORY)/GameSpecificHumanInteraction.c

Move.o: $(GAME_DIRECTORY)/Move.c Move.h $(GAME_DIRECTORY)/definitions.h Makefile
	cc $(CFLAGS) -c $(GAME_DIRECTORY)/Move.c

playGame.o: playGame.c Pos.h Move.h Bot.h myHash.h HumanInteraction.h Makefile
	cc $(CFLAGS) -c playGame.c

Pos.o: $(GAME)_DIRECTORY/Pos.c Pos.h $(GAME_DIRECTORY)/definitions.h Makefile
	cc $(CFLAGS) -c $(GAME)_DIRECTORY/Pos.c

Bot.o: Bot.c Bot.h Makefile
	cc $(CFLAGS) -c Bot.c

ArrayUtilities.o: ArrayUtilities.c ArrayUtilities.h Makefile
	cc $(CFLAGS) -c ArrayUtilities.c

myHash.o: myHash.c Makefile
	cc $(CFLAGS) -c myHash.c

hashtable.o: Hashtable_Stuff/hashtable.h Hashtable_Stuff/hashtable_private.h Hashtable_Stuff/hashtable.c Makefile
	cc $(CFLAGS) -c Hashtable_Stuff/hashtable.c

hashtable_itr.o: Hashtable_Stuff/hashtable_itr.h Hashtable_Stuff/hashtable_private.h Hashtable_Stuff/hashtable_itr.c
	cc $(CFLAGS) -c Hashtable_Stuff/hashtable_itr.c

clean:
	rm *.o



HashTest: hashTester.o hashtable.o hashtable_itr.o myHash.o Makefile
	cc -g -Wall -O -lm hashTester.o hashtable.o hashtable_itr.o myHash.o -o HashTest

hashTester.o: hashTester.c Makefile
	cc $(CFLAGS) -c hashTester.c
