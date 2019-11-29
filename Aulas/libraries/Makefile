
OBJDIR = obj

all: $(OBJDIR) prog

$(OBJDIR):
	mkdir -p $(OBJDIR)

prog: $(OBJDIR)/prog.o libcalc
	gcc -Wall -g $(OBJDIR)/prog.o ./libcalc.so -o prog

libcalc: $(OBJDIR)/calc.o
	gcc -g -shared $(OBJDIR)/calc.o -o libcalc.so

$(OBJDIR)/prog.o: prog.c
	gcc -Wall -g -c prog.c -o $(OBJDIR)/prog.o

$(OBJDIR)/calc.o: calc.c
	gcc -Wall -g -c -fPIC calc.c -o $(OBJDIR)/calc.o