/**
* (basic description of the program or class)
*
* Completion time: (35)
*
* @author (Lorenzo Ifarada), Acuna
* @version (03/18/2023)
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//ENUM STRUCT
typedef enum {
    SER,
    EGR,
    CSE,
    EEE,
} Subject;


// Structure of a courseNODE
typedef struct CourseNode {
    Subject subject;
    int courseNumber;
    char teacherName[1024];int creditHours;
    struct CourseNode *next;
} CourseNode;

struct CourseNode* course_collection= NULL;


const char* ThisSubject(Subject subject)
{
    switch (subject)
    {
        case SER: return "SER";
        case EGR: return "EGR";
        case CSE: return "CSE";
        case EEE: return "EEE";
    }
}




//calculate total credits
int totalCred(struct CourseNode *head)
{
    int totalCred = 0;
    struct CourseNode *iter = head;
    while(iter != NULL)
    {
        totalCred =  totalCred + iter->creditHours;
        iter = iter->next;
    }
    return totalCred;
}

// Course going into Linked LIST
void course_insert(struct CourseNode** head, int subjectNumber, int courseNumber, int credits, char *teacherName)
{

    struct CourseNode *newNode;
    struct CourseNode *currentNode;
    newNode = (struct CourseNode*)malloc(sizeof(struct CourseNode));

    newNode->subject = subjectNumber;
    newNode->courseNumber = courseNumber;
    strcpy(newNode->teacherName, teacherName);
    newNode->creditHours = credits;

// special case to insert data at head
    if (*head == NULL || (*head)->courseNumber >= newNode->courseNumber)
    {
        newNode->next = *head;
        *head = newNode;
    }
    else
    {
// Locate node before the point of insertion
        currentNode = *head;
        while (currentNode->next!=NULL && currentNode->next->courseNumber < newNode->courseNumber)
        {
            currentNode = currentNode->next;
        }
        newNode->next = currentNode->next;
        currentNode->next = newNode;
    }
}

void course_drop(struct CourseNode **head, int courseNumber){

// Store head
    struct CourseNode* iter = *head, *prev;

// Deleting Head Node
    if (iter != NULL && iter->courseNumber == courseNumber)
    {
// head update
        *head = iter->next;
        free(iter);
        return;
    }

// keep track of previous node and search for the course to be deleted
    while (iter != NULL && iter->courseNumber != courseNumber)
    {
        prev = iter;
        iter = iter->next;
    }

// Not Found
    if (iter == NULL){
        printf("ERROR! Course %d doesn't exist\n", courseNumber);
        return;
    }
// Delink Node
    prev->next = iter->next;

    free(iter); // Free memory
}


void schedule_print(struct CourseNode *head){

    struct CourseNode *iter = head;
    printf("Class Schedule:\n");
    while(iter != NULL)
    {
        printf("%s%d\t%d\t%s\n", ThisSubject(iter->subject), iter->courseNumber, iter->creditHours, iter->teacherName);
        iter = iter->next;
    }
}

void schedule_load(struct CourseNode** course_collection)
{
    FILE *somefile;

    somefile = fopen("Classes.txt", "r");
    struct CourseNode* iter = *course_collection;
    if (somefile == NULL)
    {
        char *filename = "Classes.txt";
        FILE *somefile = fopen(filename, "w");
        fclose(somefile);
    }
    else {

        rewind(somefile);
                do{
                    fread(&iter, sizeof(CourseNode), 1, somefile);
                   iter= iter->next;
                }
                while(iter != NULL);

    }
    fclose(somefile);
}

void schedule_save(struct CourseNode** course_collection){

    char *filename = "Classes.txt";
    FILE *somefile = fopen(filename, "w");

    struct CourseNode* iter = *course_collection;
    if (filename == NULL)
    {
        printf("Error opening file %s", filename);
        return;
    }

    else {
        do {
        fwrite(&iter, sizeof(struct CourseNode), 1, somefile);
        iter->next;
    }
                while(iter !=NULL);

        }
        fclose(somefile);
    }





int main(void) {

    int quit = 0;
    int subjectNumber;
    int courseNumber;
    int credits;
    char teacherName[1024];


    do{

        schedule_load(course_collection);
        printf("\n\n");
        printf("Welcome to ASU Class Schedule\n\n");
        printf("Menu Options\n");
        printf("-------------------------------------------------------------------------\n");
        printf("a: Add a class\n");
        printf("d: Drop a class\n");
        printf("s: Show your classes\n");
        printf("q: Quit\n\n");

        printf("Total credits: %d\n\n", totalCred(course_collection));
        printf("Please enter a choice ---> ");
        char choice;
        scanf(" %c", &choice);

        switch (choice){

            case 'a':
                while(1){
                    printf("What is the subject? (SER=0, EGR=1, CSE=2, EEE=3)?\n");
                    scanf("%d", &subjectNumber);
                    if (subjectNumber <0 || subjectNumber > 3)
                        printf("Enter number in range 0-3\n");
                    else
                        break;
                }
                printf("\n");
                printf("What is the number (e.g. 240)?\n");
                scanf("%d", &courseNumber);
                printf("\n"); printf("How many credits is the class (e.g. 3)?\n");
                scanf("%d", &credits);
                printf("\n"); printf("What is the name of the teacher?\n");
                scanf("%s", teacherName);
                printf("\n");


                course_insert(&course_collection, subjectNumber, courseNumber, credits, teacherName);

                break;
            case 'd':
                printf("Enter course number to delete: ");
                scanf("%d", &courseNumber);
                course_drop(&course_collection, courseNumber);
                break;
            case 's':
                schedule_print(course_collection);
                break;
            case 'q':
                printf(" Quitting the program\n\n");
                schedule_save(course_collection);
                quit = 1;
                break;
            default:
                printf("ERROR! No such choice. PleaseEnter a valid choice.\n");
                break;
        }


    }while(quit !=1);

    free(course_collection);
    return 0;
}

