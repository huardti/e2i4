args = -Wall -Wextra -g
all: test

# apres l'etiquete (test:) un .o pour chaque .cpp compilé
# cette ligne genere un executable appele 'test'
test: main.o r_file_of_int.o w_file_of_int.o rw_file_of_int.o
	g++ $(args) -o $@ $^

# cette ligne compile tous les pairs .ccp/.hpp et genere des .o avec le meme nom
%.o: %.cpp %.hpp
	g++ $(args) -c -o $@ $<

# cette ligne compile tous les .ccp et genere des .o avec le meme nom
%.o: %.cpp
	g++ $(args) -c -o $@ $<

clean:
	rm -f *.o test