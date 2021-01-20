LIBS=/usr/lib/gcc/arm-linux-gnueabihf/6/libstdc++.so /usr/lib/arm-linux-gnueabihf/libglib-2.0.so
OBJS=./obj/main.o ./obj/SensorsGuard.o ./obj/StopFile.o ./obj/SensorConfig.o ./obj/SensorsConfig.o ./obj/SensorsGuardConfig.o
CONFIG=-I../Configurator ../Configurator

SensorsGuard : $(OBJS)
		gcc -pthread -o ../SensorsGuard $(OBJS) $(LIBS) -lwiringPi

./obj/main.o : ./main.cpp
		gcc -c -o ./obj/main.o -I../Configurator ./main.cpp

./obj/SensorsGuard.o : ./SensorsGuard.cpp
		gcc -c -o ./obj/SensorsGuard.o -I../Configurator ./SensorsGuard.cpp

./obj/StopFile.o : ./StopFile.cpp
		gcc -c -o ./obj/StopFile.o ./StopFile.cpp

./obj/SensorConfig.o : ../Configurator/SensorConfig.cpp
		gcc -c -o ./obj/SensorConfig.o $(CONFIG)/SensorConfig.cpp

./obj/SensorsConfig.o : ../Configurator/SensorsConfig.cpp
		gcc -c -o ./obj/SensorsConfig.o $(CONFIG)/SensorsConfig.cpp

./obj/SensorsGuardConfig.o : ../Configurator/SensorsGuardConfig.cpp
		gcc -c -o ./obj/SensorsGuardConfig.o $(CONFIG)/SensorsGuardConfig.cpp

clean : 
		rm ../SensorsGuard $(OBJS)
