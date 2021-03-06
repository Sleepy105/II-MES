TARGET_EXEC ?= MES.out

CC = clang++
CXX = clang++

BUILD_DIR ?= .build
SRC_DIRS ?= src

SRCS := $(shell find $(SRC_DIRS) -maxdepth 1 -name *.cpp -or -name *.c ! -name test.c -or -name *.s)
SRCS += lib/tinyxml2/tinyxml2.cpp
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)


INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_DIRS += lib
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
LDFLAGS ?= -pthread -lsqlite3 -lopen62541


$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ -Wall -Wextra

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
.PHONY: run
.PHONY: setup

clean:
	$(RM) -r $(BUILD_DIR)

run:
	./$(TARGET_EXEC)

setup:
	sudo add-apt-repository ppa:open62541-team/ppa
	sudo apt update
	sudo apt install libopen62541-1-dev
	sudo apt install libsqlite3-dev
	cp opc-ua-conf.txt.windows opc-ua-conf.txt


-include $(DEPS)

MKDIR_P ?= mkdir -p
