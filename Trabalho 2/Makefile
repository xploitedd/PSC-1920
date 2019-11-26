
OBJDIR = obj

all: $(OBJDIR) rotate_right memchr appointment find_and_copy

$(OBJDIR):
	mkdir -p $(OBJDIR)

rotate_right: $(OBJDIR)/rotate_right.o $(OBJDIR)/rotate_right_test.o
	gcc -Wall -g $(OBJDIR)/rotate_right.o $(OBJDIR)/rotate_right_test.o -o rotate_right

memchr: $(OBJDIR)/memchr.o $(OBJDIR)/memchr_test.o
	gcc -Wall -g $(OBJDIR)/memchr.o $(OBJDIR)/memchr_test.o -o memchr

appointment: $(OBJDIR)/appointment.o $(OBJDIR)/appointment_test.o
	gcc -Wall -g $(OBJDIR)/appointment.o $(OBJDIR)/appointment_test.o -o appointment

find_and_copy: $(OBJDIR)/find_and_copy.o $(OBJDIR)/find_and_copy_test.o
	gcc -Wall -g $(OBJDIR)/find_and_copy.o $(OBJDIR)/find_and_copy_test.o -o find_and_copy

$(OBJDIR)/%.o: %.c
	gcc -Wall -g -c $< -o $@

$(OBJDIR)/%.o: %.s
	as --64 -g -c $< -o $@
