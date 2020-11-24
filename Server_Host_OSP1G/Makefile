CC=gcc
CFLAGS=-pthread

sequential_SRC= proc_structures/proc_structure.h proc_structures/proc_structure.c modes/file_descriptor_messager.h modes/file_descriptor_manager.c image_admin/image_receiver.h image_admin/image_receiver.c modes/sequential.c modes/sequential.h
heavy_process_SRC= proc_structures/proc_structure.h proc_structures/proc_structure.c modes/file_descriptor_messager.h modes/file_descriptor_manager.c image_admin/image_receiver.h image_admin/image_receiver.c modes/heavy_process.c modes/heavy_process.h
pre_heavy_process_SRC= proc_structures/proc_structure.h proc_structures/proc_structure.c modes/file_descriptor_messager.h modes/file_descriptor_manager.c image_admin/image_receiver.h image_admin/image_receiver.c modes/pre_heavy_process.c modes/pre_heavy_process.h

sequential: TARGET=sequential
heavy_process: TARGET=heavy_process
pre_heavy_process: TARGET=pre_heavy_process


sequential: all
heavy_process: all
pre_heavy_process: all

all:
	$(MAKE) clean
	$(CC) $($(TARGET)_SRC) $(CFLAGS) -o $(TARGET).o

clean:
	find . -name "*.h.gch" -type f -delete
	find . -name "*.o" -type f -delete

