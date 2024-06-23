all: sergio

sergio: main.c lib/actor/actor.h lib/movie/movie.h lib/avl/avl.h lib/node/node.h
	gcc main.c lib/actor/actor.c lib/avl/avl.c lib/movie/movie.c lib/node/node.c -o sergio

# main.o: main.c lib/actor/actor.h lib/movie/movie.h lib/avl/avl.h lib/node/node.h
# 	gcc main.c -o main.o

# actor.o: lib/actor/actor.c lib/actor/actor.h lib/node/node.h
# 	gcc lib/actor/actor.c -o actor.o

# movie.o: lib/movie/movie.c lib/movie/movie.h lib/node/node.h
# 	gcc lib/movie/movie.c -o movie.o

# avl.o: lib/avl/avl.c lib/avl/avl.h 
# 	gcc lib/avl/avl.c -o avl.o

# node.o: lib/node/node.c lib/node/node.h
# 	gcc lib/node/node.c -o node.o

clean:
	rm -rf *.o *~ sergio