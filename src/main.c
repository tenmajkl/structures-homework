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
{
    #if defined __WIN32 || defined __WIN64
        system("cls"); // Windows has different command for clearing.
    #else 
        system("clear"); // If the os is not windows its propably unix-like.
    #endif
}

/**
 * Clears input buffer
 */
void buffer_clear(void)
{
    while ((getchar()) != '\n'); // Basically cleans buffer and we can use scanf for char without getting \n
}

/**
 * Waits for enter
 */
void pause(void)
{
    buffer_clear();
    getchar();
}

/**
 * Displays menu
 */
void menu(void)
{
    clear();   
    printf(
        "Operating systems\n\n"
        "Operations\n"
        "x.........................exit\n"
        "s.........................show\n"
        "e.........................edit\n"
        "a..........................add\n"
        "d.......................delete\n"
        "y...............show youngest\n"
        "l.....show linux distributions\n"
        "Chose operation: "
    );
}

/**
 * Displays all data in table
 */
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

/**
 * Returns index of item with given name. -1 if not found
 */
int find(OperatingSystem oses[], int count, char name[]) 
{
    for (int index = 0; index < count; index++) {
        if (strcmp(oses[index].name, name) == 0) {
            return index;
        }
    }
    return -1;
}

/**
 * Displays menu for editing
 */
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

/**
 * Provides editing process
 */
void edit(OperatingSystem oses[], int count)
{
    char name[MAX_STRING];
    int index;
    clear();
    printf("Enter name of operating system: ");
    scanf("%15s", name);

    index = find(oses, count, name);

    if (index == -1) {
        printf("Operating system %s does not exis!\n", name);
        pause();
        return;
    }

    int choice;

    while (1) {
        edit_menu(name);
        scanf("%i", &choice);

        buffer_clear();
        
        switch (choice) {
            case 0:
                return;
            case 1:
                printf("Enter name: ");
                scanf("%15s", oses[index].name);
                break;
            case 2: 
                printf("Enter year: ");
                scanf("%i", &oses[index].released_at);
                break;
            case 3: 
                printf("Enter os family: ");
                scanf("%15s", oses[index].family);
                break;
            case 4: 
                printf("Enter whenever its unix like y/n: ");
                oses[index].is_unix_like = getchar() == 'y';
                break;
        }
    }
}

/**
 * Adds operating system
 */
void add(OperatingSystem oses[], int count)
{
    clear();
    OperatingSystem os;

    printf("Enter name: ");    
    scanf("%15s", os.name);

    printf("Enter year of release: ");    
    scanf("%i", &os.released_at);

    printf("Enter family: ");    
    scanf("%15s", os.family);

    buffer_clear();

    printf("Its unix-like? y/n: ");    
    char is_unix_like;
    scanf("%c", &is_unix_like);
    os.is_unix_like = is_unix_like == 'y';

    oses[count] = os;
}

/**
 * Deltes operating system
 */
int delete(OperatingSystem oses[], int count)
{
    clear();
    char name[MAX_STRING];
    printf("Enter name: ");    
    scanf("%15s", name);

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

/**
 * Displays youngest operating systems
 */
void show_youngest(OperatingSystem oses[], int count)
{
    OperatingSystem filtered[count];
    int filtered_count = 0;
    int max = count > 0 ? oses[1].released_at : 0;
    int released_at; 
    for (int index = 1; index < count; index++) {
        
        released_at = oses[index].released_at;
        
        if (released_at > max) {
            max = released_at;
            filtered_count = 0;
        }

        if (released_at == max) {
            filtered[filtered_count] = oses[index];
            filtered_count++;
        }
    }

    show(filtered, filtered_count);
}

/**
 * Displays only linux distributions
 */
void show_linux_distros(OperatingSystem oses[], int count)
{
    OperatingSystem unix_like[MAX_ARRAY];
    int unix_like_count = 0;

    for (int index = 0; index < count; index++) {
        if (strcmp(oses[index].family, "Linux") == 0) {
            unix_like[unix_like_count] = oses[index];
            unix_like_count++;
        }
    }

    show(unix_like, unix_like_count);
}

/**
 * Loads data from csv file - legacy
 *
int load(char file[], OperatingSystem result[], int limit)
{
    FILE* pointer;
    char ch;
    int index;
    OperatingSystem os;
    int oses = 0;
    int item = 0;
    char last[MAX_STRING];
    int last_index = 0;
     
    pointer = fopen(file, "r");

    if (pointer == NULL) {
        puts("File can't be opened. Its propably missing so create file `data.csv`.");
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
*/

/**
 * Loads oses from file
 */
int load(char file[], OperatingSystem result[])
{
    FILE* pointer;
    int count = 0;
    pointer = fopen(file, "r");

    if (pointer == NULL) {
        return -1;
    }

    OperatingSystem curent;
    int is_unix_like;
    while (fscanf(pointer, "%15s%i%15s%i", curent.name, &curent.released_at, curent.family, &is_unix_like) == 4) {
        if (count == MAX_ARRAY) {
            return count;
        }
        curent.is_unix_like = is_unix_like != 0;
        result[count] = curent;
        count++;
    }

    fclose(pointer);

    return count;
}

/**
 * Writes data back to file
 */
void write(char file[], OperatingSystem oses[], int count)
{
    FILE *pointer = fopen(file, "w");

    if (pointer == NULL) {
        puts("File can't be opened. Its propably missing so create file `data.csv`.");        
        return;
    }

    for (int index = 0; index < count; index++) {
        fprintf(pointer, "%s %i %s %i\n", oses[index].name, oses[index].released_at, oses[index].family, oses[index].is_unix_like ? 1 : 0);
    }

    fclose(pointer);
}

int main(void)
{
    char choice;
    OperatingSystem oses[MAX_ARRAY];
    int count = load("data.txt", oses);
    if (count == -1) {
        puts("File can't be opened. Its propably missing so create file `data.csv`.");
        return -1;
    }
    while (1) {
        menu();
        scanf("%c", &choice);
        switch (choice) {
            case 'x':
                write("data.csv", oses, count);
                return 0;
            case 's':
                show(oses, count);
                break;
            case 'e':
                edit(oses, count);
                break;
            case 'a':
                add(oses, count);
                count++;
                break;
            case 'd':
                count = delete(oses, count);
                break;
            case 'y':
                show_youngest(oses, count);
                break;
            case 'l':
                show_linux_distros(oses, count);
                break;
            default:
                buffer_clear();
        }
    }
    write("data.csv", oses, count);
    return 0;
}
