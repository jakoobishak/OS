LIBS = -lpthread -lwiringPi -ldl -llttng-ust -llttng-ust-common

make:
	gcc lab_1.c main.c thread_help.c \
	$(LIBS) \
	-O0 \
	-o lab_1.bin
clean:
	rm *.bin
