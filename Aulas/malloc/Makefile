
OBJDIR = obj

all: $(OBJDIR) prog

$(OBJDIR):
	mkdir -p $(OBJDIR)

prog: $(OBJDIR)/prog.o $(OBJDIR)/xmalloc.o
	gcc -Wall -g $(OBJDIR)/prog.o $(OBJDIR)/xmalloc.o -o prog

$(OBJDIR)/%.o: %.c xmalloc.h
	gcc -Wall -g -c $< -o $@
