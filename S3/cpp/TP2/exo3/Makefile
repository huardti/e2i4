args = -g -Wall -Wextra
all: test

# apres l'etiquete (test:) un .o pour chaque .cpp compilé
# cette ligne genere un executable appele 'test'
test: main.o point.o
	g++ $(args) -o $@ $^

# cette ligne compile tous les .ccp et genere des .o avec le meme nom
%.o: %.cpp
	g++ $(args) -c -o $@ $<

# cette ligne compile tous les pairs .ccp/.hpp et genere des .o avec le meme nom
%.o: %.cpp %.hpp
	g++ $(args) -c -o $@ $<

clean:
	rm -f *.o test