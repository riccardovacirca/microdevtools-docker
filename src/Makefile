CC:=clang
CFLAGS:=-std=gnu99 -D_MONGOOSE
INCLUDES:=-I. -I../mongoose -I../microdevtools -I /usr/include/apr-1.0 -I/usr/include/json-c
LDFLAGS:=-lapr-1 -laprutil-1 -ljson-c -lssl -lcrypto
SRC:=../mongoose/mongoose.c ../microdevtools/microdevtools.c hello.c main.c

all:
	$(eval CFLAGS:=$(CFLAGS) -D_DAEMON)
	$(CC) $(CFLAGS) -o /service/build/hello $(SRC) $(INCLUDES) $(LIBS) $(LDFLAGS)

debug:
	$(eval CFLAGS:=$(CFLAGS) -g -D_DEBUG)
	$(CC) $(CFLAGS) -o /service/build/hello $(SRC) $(INCLUDES) $(LIBS) $(LDFLAGS)

run:
	/service/build/hello -h 0.0.0.0 -p 2310 -P 2443 -l /service/logs/hello.log

.PHONY: all debug run