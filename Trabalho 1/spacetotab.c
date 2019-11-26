#include<stdio.h>
#include<string.h>

void space_to_tab(char *string, int tab_size) {
    // in this method we have to break the string
    // into groups of #tab_size and find the spaces that need
    // to be replaced
    int first_space = -1, len = strlen(string), lc = 0, dif = 0;
    for (int i = 0; i < len; ++i) {
        if (string[i] == '\n') {
            // handle the new line by resetting everything
            first_space = -1;
            // we need to ignore \n so lc exists
            lc++;
            dif = 0;
        } else if (first_space != -1) {
            // calculate the spaces removed (except one that will be used
            // for the \t)
            int spaces_rem = i - first_space - 1;
            // check if the spaces we're going to remove > 0
            // and if it's a #tab_size block
            if (spaces_rem > 0 && (i + lc + dif) % tab_size == 0) {
                // set the first space as a tab
                string[first_space] = '\t';
                // remove the remaining and shift the content
                dif += spaces_rem;
                for (int j = first_space + 1; j < len; ++j)
                    string[j] = string[j + spaces_rem];

                i = first_space;
                len -= spaces_rem;
                first_space = -1;
            } else if (string[i] != ' ') {
                // if a char breaks the sequence
                first_space = -1;
            }
        } else if (string[i] == ' ') {
            // start the sequence
            first_space = i;
        }
    }
}

int main(int argc, char *argv[]) {
    char string[] = "    Hello World!!\n    sda ds ad saddddd\n        Herro";
    //char string[] = "ab  c   def ";
    space_to_tab(string, 4);
    for (int i = 0; i < strlen(string); ++i) {
        if (string[i] == ' ')
            printf("(s)");
        else if (string[i] == '\t')
            printf("[t]");
        else
            putchar(string[i]);
    }

    printf("\n%s\n", string);
    return 0;
}