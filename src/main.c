#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 16
#define MAX_ARRAY 256

/**
 * Structure representing operating system
 */
typedef struct {
    char name[MAX_STRING];
    int released_at;
    char family[MAX_STRING];
    bool is_unix_like;
} OperatingSystem;

/**
 * Clears screen
 */
void clear(void)
{return;
    #if defined __WIN32 || defined __WIN64
        system("cls"); // Windows has different command for clearing
    #else 
        system("clear"); // If the os is not windows its propably unix-like.
    #endif
}

void pause(void)
{
    #if defined __WIN32 || defined __WIN64
        system("pause"); 
    #else 
        system("read a");
    #endif
}

/**
 * Prints menu
 */
void menu(void)
{
    clear();   
    printf(
        "Operating systems\n\n"
        "Operations\n"
        "0........exit\n"
        "1........show\n"
        "2........edit\n"
        "3.........add\n"
        "4......delete\n"
        "5......filter\n"
        "Chose operation: "
    );
}

void show(OperatingSystem oses[], int count)
{
    clear();
    OperatingSystem os;
    puts(
        "+------------------+------------------+------------------+------------------+\n"
        "+ Name             + Released at      + Os-Family        + Unix-like?       +"
    );
    for (int index = 0; index < count; index++) {
        os = oses[index];
        printf(
            "+------------------+------------------+------------------+------------------+\n"
            "| %-16s | %-16i | %-16s | %-16s |\n"
            , os.name, os.released_at, os.family, os.is_unix_like ? "Yes" : "No"
        );
    }
    puts("+------------------+------------------+------------------+------------------+");
    pause();
}

int find(OperatingSystem oses[], int count, char name[]) 
{
    for (int index = 0; index < count; index++) {
        if (strcmp(oses[index].name, name) == 0) {
            return index;
        }
    }
    return -1;
}

void edit_menu(char name[])
{
    clear();   
    printf(
        "Editing %s\n\n"
        "Operations:\n"
        "0................exit\n"
        "1...........edit name\n"
        "2...........edit year\n"
        "3......edit os family\n"
        "4......edit unix-like\n"
        "Chose operation: ",
        name
    );
}

void edit(OperatingSystem oses[], int count)
{
    char name[16];
    int index;
    clear();
    printf("Enter name of operating system: ");
    scanf("%16s", name);

    index = find(oses, count, name);

    if (index == -1) {
        printf("Operating system %s does not exis!\n", name);
        pause();
        return;
    }

    int choice;
    char unix;

    while (1) {
        edit_menu(name);
        scanf("%1i", &choice);

        switch (choice) {
            case 0:
                return;
            case 1:
                printf("Enter name: ");
                scanf("%16[a-zA-Z0-9]s", oses[index].name);
                break;
            case 2: 
                printf("Enter year: ");
                scanf("%4i", &oses[index].released_at);
                break;
            case 3: 
                printf("Enter os family: ");
                scanf("%16[a-zA-Z0-9]s", oses[index].family);
                break;
            case 4: 
                printf("Enter whenever its unix like y/n: ");
                scanf("%1c", &unix);
                oses[index].is_unix_like = unix == 'y';
                break;
            default:
                puts("Not implemented.");
                break;
        }
    }
}

void add(OperatingSystem oses[], int count)
{
    clear();
    OperatingSystem os;

    printf("Enter name: ");    
    scanf("%16s", os.name);

    printf("Enter year of release: ");    
    scanf("%4i", &os.released_at);

    printf("Enter family: ");    
    scanf("%16s", os.family);

    printf("Its unix-like? y/n: ");    
    char unix;
    scanf("%c", &unix);
    os.is_unix_like = unix == 'y';

    oses[count] = os;
}

int delete(OperatingSystem oses[], int count)
{
    clear();
    char name[16];
    printf("Enter name: ");    
    scanf("%16s", name);

    bool found = false;
    int result = count;

    for (int index = 0; index < count; index++) {
        if (found) {
            oses[index - 1] = oses[index];
        }

        if (strcmp(oses[index].name, name) == 0) {
            found = true;
            result--;
        }
    }

    return result;
}

int load(char file[], OperatingSystem result[], int limit)
{
    FILE* pointer;
    char ch;
    int index;
    OperatingSystem os;
    int oses = 0;
    int item = 0;
    char last[16];
    int last_index = 0;
     
    pointer = fopen(file, "r");

    if (pointer == NULL) {
        puts("File can't be opened");
        fclose(pointer);
        return -1;
    }

    for (index = 0; ch != EOF; index++) {
        ch = fgetc(pointer);
        switch (ch) {
            case '\n':
                if (oses == limit) {
                    fclose(pointer);
                    return oses;
                }
                last[last_index] = 0;
                os.is_unix_like = strcmp(last, "y") == 0;
                result[oses] = os;
                oses++;
                strcpy(last, "");
                last_index = 0;
                item = 0;
                break;
            case ',':
                last[last_index] = 0;
                switch (item) {
                    case 0:
                        strcpy(os.name, last);
                        break;
                    case 1:
                        os.released_at = atoi(last);
                        break;
                    case 2:
                        strcpy(os.family, last);
                        break;
                    default:
                        puts("Invalid csv file");
                        fclose(pointer);
                        return -1;
    
                }
                strcpy(last, "");
                item++;
                last_index = 0;
                break;
            default:
                if (last_index < 15) {
                    last[last_index] = ch;   
                    last_index++;
                }
        }
    }

    fclose(pointer);
    return oses;
}

void write(char file[], OperatingSystem oses[], int count)
{
    FILE *pointer = fopen(file, "w");

    if (pointer == NULL) {
        puts("File couldnt be opened");
        return;
    }

    for (int index = 0; index < count; index++) {
        fprintf(pointer, "%s,%i,%s,%c\n", oses[index].name, oses[index].released_at, oses[index].family, oses[index].is_unix_like ? 'y' : 'n');
    }

    fclose(pointer);
}

int main(void)
{
    int choice;
    OperatingSystem oses[MAX_ARRAY];
    int count = load("data.csv", oses, MAX_ARRAY);
    if (count == -1) {
        return -1;
    }
    while (1) {
        menu();
        scanf("%1i", &choice);
        switch (choice) {
            case 0:
                write("data.csv", oses, count);
                return 0;
            case 1:
                show(oses, count);
                break;
            case 2:
                edit(oses, count);
                break;
            case 3:
                add(oses, count);
                count++;
                break;
            case 4:
                count = delete(oses, count);
                break;
            default:
                clear();
                puts("not implemented");
        }
    }
    write("data.csv", oses, count);
    return 0;
}
