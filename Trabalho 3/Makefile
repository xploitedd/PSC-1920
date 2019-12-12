
OBJDIR = obj

all: lib prog
lib: $(OBJDIR) libbooks
clean:
	rm -r $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

prog: $(OBJDIR)/prog.o libbooks
	gcc -Wall -g $(OBJDIR)/prog.o ./libbooks.so -o prog -lcurl -ljson-c

libbooks: $(OBJDIR)/libbooks.o
	gcc -Wall -g -shared $(OBJDIR)/libbooks.o -o libbooks.so

$(OBJDIR)/prog.o: prog.c libbooks.h
	gcc -Wall -g -c prog.c -o $(OBJDIR)/prog.o

$(OBJDIR)/libbooks.o: libbooks.c libbooks.h
	gcc -Wall -g -c -fPIC libbooks.c -o $(OBJDIR)/libbooks.o
