CC = gcc
CFLAGS 	= -c -O2 -Wall -I ./include/ -I ./msg -I ./srv
LDFLAGS =

#Add messages
#TODO: Figure out how not to compile to object file when not used
CFLAGS += -DSTD_MSGS__STRING -DROSCPP_TUTORIALS__TWO_INTS

ALL_SOURCES		= $(wildcard ./src/*.c ./msg/*.c ./srv/*.c)
SOURCES			= $(filter-out \
              		./src/pico_publisher.c ./src/pico_subscriber.c \
              		./src/pico_client.c ./src/pico_server.c \
              		./src/pico_params.c, \
              		$(ALL_SOURCES))

OBJ_DIR = ./obj
BIN_DIR = ./bin

OBJECTS		= $(SOURCES:./%.c=$(OBJ_DIR)/%.o)


PUB 		= pico_publisher
SUB			= pico_subscriber
CLIENT 		= pico_client
SERVER		= pico_server
PARAMS		= pico_params

EXECUTABLES = $(PUB) $(SUB) $(CLIENT) $(SERVER) $(PARAMS)

PUB_OBJ		= $(OBJECTS) $(OBJ_DIR)/src/$(PUB).o
SUB_OBJ		= $(OBJECTS) $(OBJ_DIR)/src/$(SUB).o
CLIENT_OBJ	= $(OBJECTS) $(OBJ_DIR)/src/$(CLIENT).o
SERVER_OBJ	= $(OBJECTS) $(OBJ_DIR)/src/$(SERVER).o
PARAMS_OBJ  = $(OBJECTS) $(OBJ_DIR)/src/$(PARAMS).o


all: $(EXECUTABLES)

publisher: $(PUB)
$(PUB): $(OBJ_DIR) $(PUB_OBJ)
	$(CC) $(PUB_OBJ) $(LDFLAGS) -o $(BIN_DIR)/$@

subscriber: $(SUB)
$(SUB): $(OBJ_DIR) $(SUB_OBJ)
	$(CC) $(SUB_OBJ) $(LDFLAGS) -o $(BIN_DIR)/$@

client: $(CLIENT)
$(CLIENT): $(OBJ_DIR) $(CLIENT_OBJ)
	$(CC) $(CLIENT_OBJ) $(LDFLAGS) -o $(BIN_DIR)/$@

server: $(SERVER)
$(SERVER): $(OBJ_DIR) $(SERVER_OBJ)
	$(CC) $(SERVER_OBJ) $(LDFLAGS) -o $(BIN_DIR)/$@

params: $(PARAMS)
$(PARAMS): $(OBJ_DIR) $(PARAMS_OBJ)
	$(CC) $(PARAMS_OBJ) $(LDFLAGS) -o $(BIN_DIR)/$@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/src $(OBJ_DIR)/srv $(OBJ_DIR)/msg $(BIN_DIR)

$(OBJ_DIR)/src/%.o: ./src/%.c
	$(CC) $(CFLAGS) $(CURDIR)/$< -o $@

$(OBJ_DIR)/srv/%.o: ./srv/%.c
	$(CC) $(CFLAGS) $(CURDIR)/$< -o $@

$(OBJ_DIR)/msg/%.o: ./msg/%.c
	$(CC) $(CFLAGS) $(CURDIR)/$< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
