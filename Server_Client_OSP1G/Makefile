CC=gcc
LIBS = -fopenmp
TARGET = client
FILES = main.c Validations/image_validations.c


$(TARGET): $(OBJFILES)
	$(CC) $(FILES) $(LIBS) -o $(TARGET) 

clean:
	rm -f $(FILES) $(TARGET) *~
