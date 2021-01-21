LIBS=/usr/lib/arm-linux-gnueabihf/libstdc++.so.6
OBJS=./obj/main.o ./obj/MessagesProducer.o ./obj/MessagesReader.o

SensorQueueTest : $(OBJS)
		gcc -pthread -o ../SensorQueueTest $(OBJS) $(LIBS)

./obj/main.o : ./main.cpp
		gcc -c -o ./obj/main.o -I../Guard ./main.cpp

./obj/MessagesProducer.o : ./MessagesProducer.cpp
		gcc -c -o ./obj/MessagesProducer.o -I../Guard ./MessagesProducer.cpp

./obj/MessagesReader.o : ./MessagesReader.cpp
		gcc -c -o ./obj/MessagesReader.o -I../Guard ./MessagesReader.cpp

clean : 
		rm ../SensorQueueTest $(OBJS)
