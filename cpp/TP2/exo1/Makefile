args = -g -Wall -Wextra
all: test

# apres l'etiquete (test:) un .o pour chaque .cpp compilé
# cette ligne genere un executable appele 'test'
test: main.o
	g++ $(args) -o $@ $^

# cette ligne co mpile tous les .ccp et genere des .o avec le meme nom
%.o: %.cpp *.hpp
	g++ $(args) -c -o $@ $<

clean:
	rm -f *.o test
