# Compiler
CC := g++

# Compiler flags
#  -g   : enable debugging
#  -O3  : compile for fastest execution possible (increases compiling time)
#  -Wall: Turn on ALL warnings 
CFLAGS := -O3 -Wall

# Linking flags
LFLAGS := -lboost_system -lboost_filesystem -ldl -lcpprest -lpthread -lcrypto

DIR_SRC := src
DIR_BUILD := build
DIR_BIN := bin
TARGET_EXEC := restapi.app

SRC_FILES := $(shell find $(DIR_SRC) -name "*.cpp" -or -name "*.c")
OBJ_FILES := $(patsubst $(DIR_SRC)/%,$(DIR_BUILD)/%,$(SRC_FILES:.cpp=.o))

all: print app_structure linking clean

print:
	@echo "Listing found src... $(SRC_FILES)"
	@echo "Listing found obj... $(OBJ_FILES)"

app_structure:
	@mkdir -p $(DIR_BUILD)
	@mkdir -p $(DIR_BUILD)/include
	@mkdir -p $(DIR_BIN)
	
linking: $(OBJ_FILES)
	@echo "Linking... $(CC) -o $(DIR_BIN)/$(TARGET_EXEC) $(shell find $(DIR_BUILD) -name "*.o") $(LFLAGS)"
	@$(CC) -o $(DIR_BIN)/$(TARGET_EXEC) $(shell find $(DIR_BUILD) -name "*.o") $(LFLAGS)

$(OBJ_FILES): main.o
	@echo "Compiling... $(CC) $(CFLAGS) -c $(subst $(DIR_BUILD),$(DIR_SRC),$(@:.o=.cpp)) -o $@"
	@$(CC) $(CFLAGS) -c $(subst $(DIR_BUILD),$(DIR_SRC),$(@:.o=.cpp)) -o $@

main.o:
	@echo "Compiling... $(CC) $(CFLAGS) -I$(DIR_SRC) -c main.cpp -o $(DIR_BUILD)/$@"
	@$(CC) $(CFLAGS) -I$(DIR_SRC) -c main.cpp -o $(DIR_BUILD)/$@

clean:
	@echo "Cleaning... rm -rf $(DIR_BUILD)";
	@rm -rf $(DIR_BUILD)
	@echo "Done!"

#Rule to compile individual files passed by argument to make
# %.cpp:
# @echo "Compiling... $(CC) $(CFLAGS) -c $(DIR_SRC)/$@"
# @$(CC) $(CFLAGS) -c $(DIR_SRC)/$@
