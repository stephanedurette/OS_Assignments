gcc shm_write.c -o shm_write
gcc shm_read.c -o shm_read

gcc client.c -o client -lpthread -lrt
gcc server.c -o server -lpthread -lrt
