LIBS=/usr/lib/arm-linux-gnueabihf/libstdc++.so.6
OBJS=./obj/main.o

MessagesTest : $(OBJS)
		gcc -o ../MessagesTester $(OBJS) $(LIBS)

./obj/main.o : ./main.cpp
		gcc -c -o ./obj/main.o ./main.cpp

clean : 
		rm ../MessagesTest $(OBJS)
