CC = gcc
CFLAGS 	= -c -O2 -Wall -I ./include/ -I ./msg -I ./srv
LDFLAGS =

# define add_existing_msg
# 	$(eval FILE:=$(addprefix ./msg/, $(addsuffix .c, $(1)))) \
# 	if [!$(wildcard $(FILE))] \
# 	then \
# 		echo "cannot find file" \
# 		$(shell exit 3) \
# 	# else \
# 	# $(FILE) \
# 	fi
# 	echo $(FILE)
# endef

define lc
	$(shell echo $(1) | tr '[A-Z]' '[a-z]')
endef

define uc
	$(shell echo $(1) | tr '[a-z]' '[A-Z]')
endef

# Add messages and services
MSGS_ROS 	= std_msgs/String std_msgs/Byte
SRVS	= roscpp_tutorials__TwoInts

MSGS = $(call lc, $(subst /,__, $(MSGS_ROS)))
# Add the needed compiler flags associated
CFLAGS += $(addprefix -D, $(call uc, $(MSGS)))
CFLAGS += $(addprefix -D, $(call uc, $(SRVS)))

# Add the sources
SOURCES		= $(wildcard ./src/*.c)
SOURCES    += $(foreach MSG_M, $(MSGS), $(addprefix ./msg/, $(addsuffix .c, $(MSG_M))))
SOURCES    += $(foreach SRV_M, $(SRVS), $(addprefix ./srv/, $(addsuffix .c, $(SRV_M))))

# Remove the sources associated with different node examples
SOURCES		:= $(filter-out \
				./src/pico_publisher.c ./src/pico_subscriber.c \
				./src/pico_client.c ./src/pico_server.c \
				./src/pico_params.c, \
				$(SOURCES))

OBJ_DIR = ./obj
BIN_DIR = ./bin

# Create the objects names
OBJECTS		= $(SOURCES:./%.c=$(OBJ_DIR)/%.o)

# Define the different node examples
PUB 		= pico_publisher
SUB			= pico_subscriber
CLIENT 		= pico_client
SERVER		= pico_server
PARAMS		= pico_params

EXECUTABLES = $(PUB) $(SUB) $(CLIENT) $(SERVER) $(PARAMS)

# Create object names for the examples
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

show_cflags:
	@echo $(CFLAGS)

show_msgs:
	@echo $(MSGS)

show_srvs:
	@echo $(SRVS)

show_objs:
	@echo $(OBJECTS)

show_srcs:
	@echo $(SOURCES)

msgs:
	@bash $(CURDIR)/msg_gen/msg_gen.sh $(MSGS_ROS)
