# PREFIX := /usr/local/lib/
# LDFLAGS += -L/home/jh/annaconda3/lib

OBJECT = main.o chat_server.o chat_list.o chat_database.o chat_thread.o
main:$(OBJECT)
	g++ $(OBJECT) -o main -levent -lmysqlclient


clean:
	rm -f *.o main
