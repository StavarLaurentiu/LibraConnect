CFLAGS = -Wall -g -Werror -Wno-error=unused-variable

all: client

requests.o: requests.cpp

buffer.o: buffer.cpp

utils.o: utils.cpp

client: client.cpp utils.o buffer.o requests.o

.PHONY: clean run_client

run_client:
	./client

clean:
	rm -f *.o
	rm -f client