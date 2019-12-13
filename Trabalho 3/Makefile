
OBJDIR = obj

all: lib prog html_author
lib: $(OBJDIR) libbooks
clean:
	rm -r $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

prog: $(OBJDIR)/prog.o libbooks libbooks.h
	gcc -Wall -g $(OBJDIR)/prog.o ./libbooks.so -o prog -lcurl -ljson-c

html_author: $(OBJDIR)/html_author.o libbooks libbooks.h
	gcc -Wall -g $(OBJDIR)/html_author.o ./libbooks.so -o html_author -lcurl -ljson-c

libbooks: $(OBJDIR)/libbooks.o
	gcc -Wall -g -shared $(OBJDIR)/libbooks.o -o libbooks.so

$(OBJDIR)/prog.o: prog.c
	gcc -Wall -g -c prog.c -o $(OBJDIR)/prog.o

$(OBJDIR)/html_author.o: html_author.c
	gcc -Wall -g -c html_author.c -o $(OBJDIR)/html_author.o

$(OBJDIR)/libbooks.o: libbooks.c libbooks.h
	gcc -Wall -g -c -fPIC libbooks.c -o $(OBJDIR)/libbooks.o