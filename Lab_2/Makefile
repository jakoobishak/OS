LIBPT 		= -lpthread 
LIBWP 		= -lwiringPi
LIBLTTNG  = -ldl -llttng-ust
LIBMATH   = -lm

# Run the following line in the terminal to be abple to view
# lock()/trylock()/unlock() in your traces, if you're running them
# $ export LD_PRELOAD=liblttng-ust-pthread-wrapper.so

dining:
		gcc dining.c \
		$(LIBPT) \
		-o dining.bin

prod-cons:
		gcc prod-cons.c circular_buffer.c \
		$(LIBPT) \
		$(LIBWP) \
		$(LIBMATH) \
		-o prod-cons.bin

clean:
		rm *.bin

