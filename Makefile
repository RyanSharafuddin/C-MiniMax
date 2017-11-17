
#if you want to profile with Valgrind or Cachegrind or gprof, add -pg flag to CFLAGS
CFLAGS = -Wall -O3 -std=c99
OBJECTS = Chomp.o Pos.o Move.o HumanInteraction.o Bot.o ArrayUtilities.o hashtable.o hashtable_itr.o myHash.o

all: $(OBJECTS) Makefile
	cc $(CFLAGS) -lm $(OBJECTS) -o Chomp

HumanInteraction.o: HumanInteraction.c HumanInteraction.h Makefile
	cc $(CFLAGS) -c HumanInteraction.c

Move.o: Move.c Move.h Makefile
	cc $(CFLAGS) -c Move.c

Chomp.o: Chomp.c Makefile
	cc $(CFLAGS) -c Chomp.c

Pos.o: Pos.c Pos.h Makefile
	cc $(CFLAGS) -c Pos.c

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
