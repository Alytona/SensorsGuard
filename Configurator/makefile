LIBS=/usr/lib/arm-linux-gnueabihf/libstdc++.so.6
OBJS=./obj/main.o ./obj/SensorConfig.o ./obj/SensorsConfig.o ./obj/SensorsGuardConfig.o

SensorsGuardConfig : $(OBJS)
		gcc -o ../SensorsGuardConfig $(OBJS) $(LIBS)

./obj/main.o : ./main.cpp
		gcc -c -o ./obj/main.o ./main.cpp

./obj/SensorConfig.o : ./SensorConfig.cpp
		gcc -c -o ./obj/SensorConfig.o ./SensorConfig.cpp

./obj/SensorsConfig.o : ./SensorsConfig.cpp
		gcc -c -o ./obj/SensorsConfig.o ./SensorsConfig.cpp

./obj/SensorsGuardConfig.o : ./SensorsGuardConfig.cpp
		gcc -c -o ./obj/SensorsGuardConfig.o ./SensorsGuardConfig.cpp

clean : 
		rm ../SensorsGuardConfig $(OBJS)
