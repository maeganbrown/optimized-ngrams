CC = g++ 
CFLAGS = -g -pg -c -std=c++0x
OBJS = NgramList.o WordList.o main.o
MY_OBJS = myNgramList.o WordList.o my-main.o
LFLAGS = -pg 
#Opt flag coming soon
OFLAG = -O2 

.C.o:
	$(CC) $(CFLAGS) $< -o $@

ngram: NgramList.o WordList.o main.o
	$(CC) $(LFLAGS) $(OFLAG) $(OBJS) -o ngram

my-ngram: myNgramList.o WordList.o my-main.o
	$(CC) $(LFLAGS) $(OFLAG) $(MY_OBJS) -o my-ngram

main.o: WordList.h NgramList.h

my-main.o: WordList.h myNgramList.h

WordList.o: WordList.h

NgramList.o: NgramList.h WordList.h

myNgramList.o: myNgramList.h WordList.h

clean:
	rm *.o ngram my-ngram gmon.out 

submit:
	submit efb 3482-lab3 NgramList.h NgramList.C WordList.h WordList.C main.C report.txt makefile
