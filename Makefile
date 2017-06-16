TARGET=read_lease
OBJ=read_lease.o

all: $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $(TARGET) $(OBJ)

clean:
	rm $(OBJ) $(TARGET)
