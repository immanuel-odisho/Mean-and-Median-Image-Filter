CFLAGS = -Wall -g -ansi
OBJS = main1.o 

denoise: $(OBJS)
	$(CC) -o $@ $?


clean:
	rm main *.o