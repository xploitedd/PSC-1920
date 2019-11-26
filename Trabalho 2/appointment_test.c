#include<stdio.h>
#include<string.h>

// 8 + 8 bytes struct
typedef struct appointment {
    char *description;
    char *schedule;
} Appointment;

// 4 + 8 bytes struct
typedef struct appointments {
    int length;
    Appointment *events;
} Appointments;

// 8 + 8 * 31 bytes struct
typedef struct agenda {
    char *name;
    Appointments *calendar[31];
} Agenda;

Appointment *get_appointment(Agenda agenda[], size_t month, size_t day, size_t index);

static Agenda yearAgenda[12];

void appointment_test() {
    // define test appointments
    Appointment ev1[] = { { "PSC", "9:30am-12:30pm" }, { "AED", "12:30pm-14:00pm" } };
    Appointments apps1 = { 2, ev1 };
    Appointment ev2[] = { { "SI1", "8:00am-9:30am" }, { "AED", "9:30am-12:30pm" } };
    Appointments apps2 = { 2, ev2 };
    // create test agendas
    Agenda nov = { "November", { NULL }};
    nov.calendar[14] = &apps1;
    Agenda dec = { "December", { NULL }};
    dec.calendar[15] = &apps2;
    // setup year
    yearAgenda[10] = nov;
    yearAgenda[11] = dec;

    // execute the tests...    
    if (!strcmp(get_appointment(yearAgenda, 10, 14, 0)->description, "PSC"))
        printf("PASSED TEST 1...\n");

    if (!strcmp(get_appointment(yearAgenda, 10, 14, 1)->description, "AED"))
        printf("PASSED TEST 2...\n");

    if (get_appointment(yearAgenda, 10, 14, 2) == NULL)
        printf("PASSED TEST 3...\n");

    if (!strcmp(get_appointment(yearAgenda, 11, 15, 0)->description, "SI1"))
        printf("PASSED TEST 4...\n");

    if (!strcmp(get_appointment(yearAgenda, 11, 15, 1)->description, "AED"))
        printf("PASSED TEST 5...\n");

    if (get_appointment(yearAgenda, 11, 15, 2) == NULL)
        printf("PASSED TEST 6...\n");

    if (get_appointment(yearAgenda, 11, 14, 0) == NULL)
        printf("PASSED TEST 7...\n");

    if (get_appointment(yearAgenda, 9, 14, 0) == NULL)
        printf("PASSED TEST 8...\n");
}

int main(int argc, char *argv[]) {
    appointment_test();
    return 0;
}