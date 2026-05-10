#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node{
    char data[40];
    char mood[20];
    char filename[200];
    struct node* next;
    struct node* prev;
} node;

char temp[40];
node* head = NULL;
node* currentnode = NULL;

// Helper: Convert to lowercase
void to_lowercase(char* str){
    int i;
	for ( i=0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

// Helper: Convert to Title Case
void to_title_case(char* str){
    int capitalize = 1;
    int i;
	for ( i = 0; str[i]; i++){
        if (capitalize && isalpha(str[i])){
            str[i] = toupper(str[i]);
        }
        else{
            str[i] = tolower(str[i]);
            capitalize = (str[i] == ' ');
        }
    }
}

/*-------------------- FUNCTIONS --------------------*/

void insert(){
    printf("\nEnter song name: ");
    while ((getchar())!='\n');
    scanf("%[^\n]%*c", temp);
    to_title_case(temp);  // format the name

    node* newnode = (node*)malloc(sizeof(node));
    strcpy(newnode->data, temp);

    int mood_choice;
    printf("Select Mood:\n");
    printf("1. Happy\n2. Sad\n3. Angry\n4. Emotional\n");
    printf("\nOption (1-4): ");
    scanf("%d", &mood_choice);

    // Set filename from song name (lowercase + .mp3)
    char lower_name[200];
    strcpy(lower_name, temp);
    to_lowercase(lower_name);
    sprintf(newnode->filename, "%s.mp3", lower_name);

    switch (mood_choice){
        case 1: strcpy(newnode->mood, "happy"); break;
        case 2: strcpy(newnode->mood, "sad"); break;
        case 3: strcpy(newnode->mood, "angry"); break;
        case 4: strcpy(newnode->mood, "emotional"); break;
        default: strcpy(newnode->mood, "unknown"); break;
    }

    if (head == NULL){
        newnode->next = newnode->prev = newnode;
        head = currentnode = newnode;
        printf("\n---> Song added successfully!\n");
        return;
    }
    
    node* last = head->prev;
    newnode->prev = last;
    last->next = newnode;
    newnode->next = head;
    head->prev = newnode;
    printf("\n---> Song added successfully!\n");
}

void delete(){
    if (head == NULL){
        printf("\n---> Playlist is empty!\n");
        return;
    }
    printf("\nEnter song name to delete: ");
    while ((getchar())!='\n');
    scanf("%[^\n]%*c", temp);
    to_title_case(temp);

    char tempLower[40], songLower[40];
    strcpy(tempLower, temp);
    to_lowercase(tempLower);

    node* ptr = head;
    do{
        strcpy(songLower, ptr->data);
        to_lowercase(songLower);
        if (strcmp(songLower, tempLower)==0){
            if (ptr->next == ptr){
                printf("\n---> Song deleted successfully! List is now empty.\n");
                head = NULL;
                free(ptr);
                return;
            }
            node* prev = ptr->prev;
            node* next = ptr->next;
            prev->next = next;
            next->prev = prev;
            head = next;
            free(ptr);
            printf("\n---> Song deleted successfully!\n");
            return;
        }
        ptr = ptr->next;
    } while (ptr != head);
    printf("\n---> No such file exists!\n");
}

void display(){
    if (head == NULL){
        printf("\n---> Playlist is empty!\n");
        return;
    }
    node* showptr = head;
    printf("\nMY PLAYLIST:\n");
    int i = 1;
    do{
        printf("%d) %s [%s]\n", i, showptr->data, showptr->mood);
        i++;
        showptr = showptr->next;
    } while (showptr != head);
}

void play_song(char* filename, char* songname){
    char command[200];
    sprintf(command, "start \"\" \"%s\"", filename);
    system(command);
    printf("\nNow Playing: %s...\n", songname);
}

void nextnode(){
    if (currentnode == NULL){
        printf("\n---> Playlist is empty!\n");
    }
    else{
        currentnode = currentnode->next;
        play_song(currentnode->filename, currentnode->data);
    }
}

void prevnode(){
    if (currentnode == NULL){
        printf("\n---> Playlist is empty!\n");
    }
    else{
        currentnode = currentnode->prev;
        play_song(currentnode->filename, currentnode->data);
    }
}

void firstnode(){
    if (head == NULL){
        printf("\n---> Playlist is empty!\n");
    }
    else{
        currentnode = head;
        play_song(currentnode->filename, currentnode->data);
    }
}

void lastnode(){if (head == NULL){
        printf("\n---> Playlist is empty!\n");
    }
    else{
        currentnode = head->prev;
        play_song(currentnode->filename, currentnode->data);
    }
}

void specificdata(){
    if (head == NULL){
        printf("\n---> Playlist is empty!\n");
        return;
    }
    printf("\nEnter song name to play: ");
    while ((getchar())!='\n');
    scanf("%[^\n]%*c", temp);
    to_title_case(temp);
    
    char tempLower[40], songLower[40];
    strcpy(tempLower, temp);
    to_lowercase(tempLower);

    node* ptr = head;
    do{
        strcpy(songLower, ptr->data);
        to_lowercase(songLower);
        if (strcmp(ptr->data, temp)==0){
            printf("\n---> Song Found!\n");
            currentnode = ptr;
            play_song(ptr->filename, ptr->data);
            return;
        }
        ptr = ptr->next;
    } while (ptr != head);
    printf("\n---> No such file exists!\n");
}

void show_by_mood(){
    if (head == NULL){
        printf("\n---> Playlist is empty!\n");
        return;
    }

    char mood[20], ptr_mood[20];
    int mood_choice;
    printf("\nSelect Mood to filter:\n");
    printf("1. Happy\n2. Sad\n3. Angry\n4. Emotional\n");
    printf("Option (1-4): ");
    scanf("%d", &mood_choice);

    switch (mood_choice){
        case 1: strcpy(mood, "happy"); break;
        case 2: strcpy(mood, "sad"); break;
        case 3: strcpy(mood, "angry"); break;
        case 4: strcpy(mood, "emotional"); break;
        default: 
        printf("Invalid mood choice.\n");
        return;
    }

    node* ptr = head;
    int found = 0;
    printf("\n---> Songs with mood [%s]:\n", mood);
    int i = 1;
    do{
        strcpy(ptr_mood, ptr->mood);
        to_lowercase(ptr_mood);
        if (strcmp(ptr_mood, mood) == 0){
            printf("%d) %s\n", i, ptr->data);
            i++;
            found = 1;
        }
        ptr = ptr->next;
    } while (ptr != head);

    if (!found){
        printf("No songs found with this mood.\n");
    }
}

void load_from_file(){
    FILE* file = fopen("playlist.txt", "r");
    if (!file) return;

    char name[40], mood[20], filename[200];
    while (fscanf(file, "%[^|]|%[^|]|%[^\n]\n", name, mood, filename) != EOF){
        node* newnode = (node*)malloc(sizeof(node));
        strcpy(newnode->data, name);
        strcpy(newnode->mood, mood);
        strcpy(newnode->filename, filename);
        if (head == NULL){
            newnode->next = newnode->prev = newnode;
            head = currentnode = newnode;
        }
        else{
            node* last = head->prev;
            newnode->prev = last;
            last->next = newnode;
            newnode->next = head;
            head->prev = newnode;
        }
    }
    fclose(file);
}

void save_to_file(){
    FILE* file = fopen("playlist.txt", "w");
    if (!file || head == NULL){
        return;
    }
    node* ptr = head;
    do{
        fprintf(file, "%s|%s|%s\n", ptr->data, ptr->mood, ptr->filename);
        ptr = ptr->next;
    } while (ptr != head);
    fclose(file);
}

/*-------------------- MAIN --------------------*/

int main(){
    load_from_file();
    int choice;
    while (1){
        printf("\n------------ MENU ------------\n");
        printf("1. Add Song\n");
        printf("2. Remove Song\n");
        printf("3. Show Playlist\n");
        printf("4. Play Next Song\n");
        printf("5. Play Previous Song\n");
        printf("6. Play First Song\n");
        printf("7. Play Last Song\n");
        printf("8. Play Specific Song\n");
        printf("9. Show songs by mood\n");
        printf("10. Exit\n");

        printf("\n---> Enter your choice: ");
        scanf("%d", &choice);

        switch (choice){
        case 1:
            insert();
            save_to_file();
            break;
        
        case 2:
            delete();
            save_to_file();
            break;
        
        case 3:
            display();
            break;

        case 4:
            nextnode();
            break;

        case 5:
            prevnode();
            break;

        case 6:
            firstnode();
            break;
        
        case 7:
            lastnode();
            break;

        case 8:
            specificdata();
            break;

        case 9:
            show_by_mood();
            break;

        case 10:
            save_to_file();
            exit(0);
            break;
        
        default:
            printf("\nInvalid Input!\n");
            break;
        }   
    }
    return 0;
}
