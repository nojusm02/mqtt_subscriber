C:=gcc
CFLAGS:= -Wall
BIN:=mqtt_subscriber
SRC:=$(wildcard *.c)
OBJ:=$(SRC:.c=.o)
LDFLAGS:= -lmosquitto -largp -lsqlite3 -luci -ljson-c -lcurl

.PHONY: all clean 

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(BIN) $(OBJ)