gcc model.c -o model -lpthread -lrt
gcc controller.c -o controller -lpthread -lrt
gcc view.c -o view -lpthread -lrt

./model
