
OBJDIR = obj

all: lib html_author tests
lib: $(OBJDIR) libbooks
clean:
	rm -r $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

tests: $(OBJDIR)/tests.o libbooks libbooks.h
	gcc -Wall -g $(OBJDIR)/tests.o ./libbooks.so -o tests -lcurl -ljson-c

html_author: $(OBJDIR)/html_author.o libbooks libbooks.h
	gcc -Wall -g $(OBJDIR)/html_author.o ./libbooks.so -o html_author -lcurl -ljson-c

libbooks: $(OBJDIR)/libbooks.o
	gcc -Wall -g -shared $(OBJDIR)/libbooks.o -o libbooks.so
	
$(OBJDIR)/html_author.o: html_author.c libbooks.h
	gcc -Wall -g -c html_author.c -o $(OBJDIR)/html_author.o

$(OBJDIR)/libbooks.o: libbooks.c libbooks.h
	gcc -Wall -g -c -fPIC libbooks.c -o $(OBJDIR)/libbooks.o

$(OBJDIR)/tests.o: tests.c libbooks.h
	gcc -Wall -g -c tests.c -o $(OBJDIR)/tests.o