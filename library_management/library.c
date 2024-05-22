
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_LINE_LENGTH 256
#define MAX_BOOKS 100
#define MAX_STUDENTS 100
#define MAX_STRING_LENGTH 100

struct book 
{
    int bookID;
    char bookName[100];
    char authorName[100];
    char genre[100];
    int availability;
    struct book* next;
};
typedef struct QueueNode_BOOK
{
    int ID;
    char name[100];
    char author[100];
    char genre[100];
    int availability;
    struct QueueNode_BOOK *next;
}QueueNode_book;

typedef struct QueueNode_STUDENT
{
    int ID;
    char name[100];
    struct QueueNode_STUDENT *next;
}QueueNode_student;

typedef struct 
{
    QueueNode_book *bookFront;
    QueueNode_student *studentFront;
}PriorityQueue;

typedef struct book BOOK;



struct student 
{
    int studentID;
    char studentName[100];
    struct student* next;
};

typedef struct student STUDENT;

typedef struct Issue_book
{
    int bookId;
    char title[MAX_LINE_LENGTH];
    char author[MAX_LINE_LENGTH];
    char genre[MAX_LINE_LENGTH];
    int studentId;
    char studentName[MAX_LINE_LENGTH];
    char dateTime[MAX_LINE_LENGTH];
}ISSUE_BOOK;

//Function protoypes
void addBook(BOOK** library,PriorityQueue* bookQueue, int bookID, const char* bookName, const char* authorName,const char *genre);
void displayBooks(BOOK* library);
void addStudent(STUDENT** students,PriorityQueue* studentQueue, int studentID, const char* studentName);
void displayStudents(STUDENT* students);
BOOK* findBook(BOOK* library, int bookID);
STUDENT* findStudent(STUDENT* students, int studentID);
void issueBook(BOOK *books, STUDENT *students, int maxBooks);
void appendToCSV(const char *filename, const char *data);
void readStudentCSV(const char *filename, STUDENT *students, int maxRecords);
void readBookCSV(const char *filename, BOOK *books, int maxRecords);
PriorityQueue *createPriorityQueue();
void enqueue_book(PriorityQueue *queue,int ID,const char *name,char *author,const char *genre,int availability);
void enqueue_student(PriorityQueue *queue,int ID,const char *name);
void displayQueueInOrder(PriorityQueue *queue);

PriorityQueue* createPriorityQueue()
{
    PriorityQueue *queue=(PriorityQueue*)malloc(sizeof(PriorityQueue));
    if(queue!=NULL)
        queue->bookFront = NULL;
        queue->studentFront = NULL;
    return queue;
}
void displayQueueInOrder_book(PriorityQueue *queue)
{
    printf("Names in alphabetical order\n");
    printf(">---------------------------<\n");
    QueueNode_book *current=queue->bookFront;
    while (current!=NULL)
    {
        printf("%s\n",current->name);
        current=current->next;
    }
    printf(">-----------------------<\n");
}


void enqueue_book(PriorityQueue *queue,int ID,const char *name,char *author,const char *genre,int availability)
{
   
    QueueNode_book *newnode=(QueueNode_book *)malloc(sizeof(QueueNode_book));
    if(newnode!=NULL)
    {
        newnode->ID=ID;
        strcpy(newnode->name,name);
        strcpy(newnode->author,author);
        strcpy(newnode->genre,genre);
        newnode->availability=availability;
        newnode->next=NULL;
    
        if(queue->bookFront==NULL||strcmp(queue->bookFront->name,name)>0)
        {
            newnode->next=queue->bookFront;
            queue->bookFront=newnode;
        }
        else
        {
            QueueNode_book *current=queue->bookFront;
            while (current->next!=NULL && strcmp(current->next->name,name)<0)
            {
                current=current->next;
            }
            newnode->next=current->next;
            current->next=newnode;
        }
    }
}

void enqueue_books_from_file(PriorityQueue *bookQueue)
{
     FILE *file = fopen("Book.csv", "r");
    if (file == NULL)
    {
        printf("Could not open Book.csv for reading.\n");
        return;
    }
    char line[1024];
    while (fgets(line,sizeof(line),file)!=NULL)
    {
        int bookID, availability;
        char bookName[100], authorName[100], bookGenre[100];
        sscanf(line, "%d,%99[^,],%99[^,],%99[^,],%d", &bookID, bookName, authorName, bookGenre, &availability);
        enqueue_book(bookQueue, bookID,bookName,authorName, bookGenre,availability);
    }
    fclose(file);
    displayQueueInOrder_book(bookQueue);
}

void displayQueueInOrder_student(PriorityQueue *queue)
{
    printf("Names in alphabetical order\n");
    printf(">---------------------------<\n");
    QueueNode_student *current=queue->studentFront;
    while (current!=NULL)
    {
        printf("%s\n",current->name);
        current=current->next;
    }
    printf(">-----------------------<\n");
}

void getCurrentDateTime(char datetime[]) {
    time_t t;
    struct tm* tm_info;

    time(&t);
    tm_info = localtime(&t);

    strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", tm_info);
}

//Function for issuing book
//reading csv
void readBookCSV(const char *filename, BOOK *books, int maxRecords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && i < maxRecords) {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%d\n", &books[i].bookID, books[i].bookName, books[i].authorName,books[i].genre, &books[i].availability);
        i++;
    }

    fclose(file);
}
void readStudentCSV(const char *filename, STUDENT *students, int maxRecords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && i < maxRecords) {
        sscanf(line, "%d,%[^\n]", &students[i].studentID, students[i].studentName);
        i++;
    }

    fclose(file);
}

void appendToCSV(const char *filename, const char *data) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s for appending\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s\n", data);
    
    fclose(file);
}

void issued_book_display(const char *filename, ISSUE_BOOK *books, int maxRecords)
{
     FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && i < maxRecords) {
        int result = sscanf(line, "%d,%[^,],%[^,],%[^,],%d,%[^,],%[^\n]",&books[i].bookId, books[i].title, books[i].author, books[i].genre,&books[i].studentId, books[i].studentName, books[i].dateTime);
        
        i++;
    }
     for (int i = 0; i < maxRecords; i++) 
     {
        if (books[i].bookId != 0) 
        {
            printf("Book ID: %d\n", books[i].bookId);
            printf("Title: %s\n", books[i].title);
            printf("Author: %s\n", books[i].author);
            printf("Genre: %s\n", books[i].genre);
            printf("Student ID: %d\n", books[i].studentId);
            printf("Student Name: %s\n", books[i].studentName);
            printf("Date and Time: %s\n", books[i].dateTime);
            printf(">--------------------------<\n");
        }
    }
    fclose(file);

}
void issueBook(BOOK *books, STUDENT *students, int maxBooks) {
    int bookId,studentId;

    printf("Enter Book ID: ");
    scanf("%d",&bookId);

    printf("Enter Student ID: ");
    scanf("%d",&studentId);

    // Find book and student details based on IDs (you may need to modify this part)
    BOOK selectedBook;
    for (int i = 0; i < maxBooks; i++) {
        if ((books[i].bookID==bookId) ) {
            selectedBook = books[i];
            break;
        }
    }
    STUDENT selectedStudent;
    for (int i = 0; i < maxBooks; i++) {
        if ((students[i].studentID==studentId) ) {
            selectedStudent = students[i];
            break;
        }

    }

    // Get current date and time
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char datetime[MAX_LINE_LENGTH];
    strftime(datetime, MAX_LINE_LENGTH, "%Y-%m-%d %H:%M:%S", tm_info);
    //printf("date-time %s",datetime);

    // Append data to issuebook.csv
    char data[MAX_LINE_LENGTH * 5];  // Adjust as needed
    snprintf(data, sizeof(data), "%d,%s,%s,%s,%d,%s, %s", bookId, selectedBook.bookName,selectedBook.authorName, selectedBook.genre,
             studentId, selectedStudent.studentName, datetime);

    appendToCSV("issuebook.csv", data);
    printf("Book is successfully issued to Student");
}


void enqueue_student(PriorityQueue *queue,int ID,const char *name)
{

    
    QueueNode_student *newnode=(QueueNode_student *)malloc(sizeof(QueueNode_student));
    if(newnode!=NULL)
    {
        newnode->ID=ID;
        strcpy(newnode->name,name);
        newnode->next=NULL;
    
        if(queue->studentFront==NULL||strcmp(queue->studentFront->name,name)>0)
        {
            newnode->next=queue->studentFront;
            queue->studentFront=newnode;
        }
        else
        {
            QueueNode_student *current=queue->studentFront;
            while (current->next!=NULL && strcmp(current->next->name,name)<0)
            {
                current=current->next;
            }
            newnode->next=current->next;
            current->next=newnode;
        }
    }
}

void enqueue_student_from_file (PriorityQueue *studentQueue)
{
    FILE *file=fopen("Student.csv","r");
    if(file==NULL)
    {
        printf("Could not open Book.csv for reading.\n");
        return;
    }
    char line[1024];
    while (fgets(line,sizeof(line),file)!=NULL)
    {
        int studentID;
        char studentName[100];
        sscanf(line,"%d,%99[^,]",&studentID,studentName);
        enqueue_student(studentQueue,studentID,studentName);
    }
    fclose(file);
    displayQueueInOrder_student(studentQueue);
}


void addBook(BOOK** library, PriorityQueue *bookQueue,int bookID, const char* bookName, const char* authorName,const char *genre) 
{
    BOOK* newBook = (BOOK*)malloc(sizeof(BOOK));
    if(newBook==NULL)
    {
        printf("Memory allocation error\n");
        return ;
    }
    
        newBook->bookID = bookID;
        strcpy(newBook->bookName, bookName);
        strcpy(newBook->authorName, authorName);
        strcpy(newBook->genre,genre);
        newBook->availability = 1;
        newBook->next = *library;
        *library = newBook;
        FILE *file=fopen("Book.csv","a");
        if(file!=NULL)
        {
            fprintf(file,"%d,%s,%s,%s,%d\n",newBook->bookID,newBook->bookName,newBook->authorName,newBook->genre,newBook->availability);
            fclose(file);
        }
        else
        {
            printf("Could not open file for writing\n");
        }
        printf("Book added successfully.\n");
}


void displayBooks(BOOK* library) 
{
    printf("Books in the library:\n");
    printf(">--------------------------<\n");
    FILE *file;
    file=fopen("Book.csv","r");
    if(file==NULL)
    {
        printf("could not open file\n");
    }
    char line[1024];
    while (fgets(line,sizeof(line),file)!=NULL) 
    {
        int bookID,availability;
        char bookName[100],authorName[100],genre[100];
        sscanf(line,"%d,%99[^,],%99[^,],%99[^,],%d",&bookID,bookName,authorName,genre,&availability);
        printf("ID: %d\n", bookID);
        printf("Book Name: %s\n", bookName);
        printf("Author: %s\n",authorName);
        printf("Genre:%s\n",genre);
        printf("Availability: %s\n", (availability ? "Available" : "Issued"));
        printf(">--------------------------<\n");
    }
}

void addStudent(STUDENT** students,PriorityQueue* studentQueue, int studentID, const char* studentName) 
{
    STUDENT* newStudent = (STUDENT*)malloc(sizeof(STUDENT));
    if(newStudent==NULL)
    {
        printf("Memory allocation error\n");
        return;
    }
    
        newStudent->studentID = studentID;
        strcpy(newStudent->studentName, studentName);
        newStudent->next = *students;
        *students = newStudent;
        FILE *file=fopen("Student.csv","a");
        if(file!=NULL)
        {
            fprintf(file,"%d,%s\n",newStudent->studentID,newStudent->studentName);
            fclose(file);
        }
        else
        {
            printf("could not open file for writing\n");
        }
        printf("Student added successfully.\n");
    
}

void displayStudents(STUDENT* students) 
{
    printf("Students in the library system:\n");
    printf(">--------------------------<\n");
    FILE *file;
    file=fopen("Student.csv","r");
    if(file==NULL)
    {
        printf("could not open file\n");
        return;
    }
    char line[1024];
    while (fgets(line,sizeof(line),file)!=NULL) 
    {
        int studentID;
        char studentName[100];
        sscanf(line,"%d,%99[^,]",&studentID,studentName);
        printf("ID: %d\n", studentID);
        printf("Student Name: %s\n", studentName);
        printf(">--------------------------<\n");
    }
    fclose(file);
}

BOOK* findBook(BOOK* library, int bookID) 
{
    while (library != NULL) 
    {
        if (library->bookID == bookID) 
        {
            return library;
        }
        library = library->next;
    }
    return NULL; // Book not found
}

STUDENT* findStudent(STUDENT* students, int studentID) 
{
    while (students != NULL) 
    {
        if (students->studentID == studentID) 
        {
            return students;
        }
        students = students->next;
    }
    return NULL; // Student not found
}



void deleteRowAndDisplay(int studentID) {
    // Open the file in read mode
    FILE *file = fopen("issuebook.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Open a temporary file in write mode
    FILE *tempFile = fopen("temp.csv", "w");
    if (tempFile == NULL) {
        perror("Error creating temporary file");
        exit(EXIT_FAILURE);
    }

    // Read each line from the file
    char line[MAX_STRING_LENGTH];
    ISSUE_BOOK book;
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        // Parse the CSV line into book structure
        sscanf(line, "%d,%[^,],%[^,],%[^,],%d,%[^,],%[^\n]",
               &book.bookId, book.title, book.author, book.genre,
               &book.studentId, book.studentName, book.dateTime);

        // Check if the studentID matches
        if (book.studentId == studentID) {
            // Display the deleted row
            printf(">--------------------------------<");
            printf("\nBook ID:%d\nBook Name:%s\nAuthor Name:%s\nGenre:%s\nStudentID:%d\nStudent Name:%s\nDate-Time%s\n",
                   book.bookId, book.title, book.author, book.genre,
                   book.studentId, book.studentName, book.dateTime);
            printf("Student returned book successfully\n");

            found = 1;
        } else {
            // Write the line to the temporary file
            fprintf(tempFile, "%d,%s,%s,%s,%d,%s,%s\n",
                    book.bookId, book.title, book.author, book.genre,
                    book.studentId, book.studentName, book.dateTime);
        }
    }

    // Close the files
    fclose(file);
    fclose(tempFile);

    // Remove the original file
    remove("issuebook.csv");

    // Rename the temporary file to the original file
    rename("temp.csv", "issuebook.csv");

    // If studentID was not found, display a message
    if (!found) {
        printf("StudentID not found in issuebook.csv\n");
    }
}

int main() 
{
    BOOK* library = NULL;
    STUDENT* students = NULL;
    ISSUE_BOOK book_issued[1233];
    
    BOOK issue_book_array[MAX_BOOKS];
    STUDENT issue_student_array[MAX_STUDENTS];
    readBookCSV("Book.csv",issue_book_array,MAX_BOOKS);
    readStudentCSV("Student.csv",issue_student_array,MAX_STUDENTS);

    int choice, studentID, bookID,student;
    char bookName[100], authorName[100], studentName[100],genre[100];
    PriorityQueue* bookNameQueue = createPriorityQueue();
    PriorityQueue* studentNameQueue = createPriorityQueue();

    do {
        printf("\nMenu:\n");
        printf("1. Display Books\n");
        printf("2. Display Students\n");
        printf("3. Add Book\n");
        printf("4. Add Student\n");
        printf("5. Issue Book\n");
        printf("6. Return Book\n");
        printf("7. Display Book Names in Alphabetical Order\n");
        printf("8. Display Student Names in Alphabetical Order\n");
        printf("9. Display Books Issued to Students \n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                displayBooks(library);
                break;
            case 2:
                displayStudents(students);
                break;
            case 3:
                printf("Enter Book ID: ");
                scanf("%d", &bookID);
                getchar();
                printf("Enter Book Title: ");
                scanf("%[^\n]s", bookName);
                getchar();
                printf("Enter Book Author: ");
                scanf("%[^\n]s", authorName);
                printf("Enter genre: ");
                scanf("%s",genre);
                getchar();
                addBook(&library, bookNameQueue, bookID, bookName, authorName,genre);
                break;
            case 4:
                printf("Enter Student ID: ");
                scanf("%d", &studentID);
                getchar();
                printf("Enter Student Name: ");
                scanf("%[^\n]s", studentName);
                getchar();
                addStudent(&students, studentNameQueue, studentID, studentName);
                break;
            case 5:
               issueBook(issue_book_array,issue_student_array,1230);
                break;
            case 6:
                int studentId;
                printf("Enter the student details\n");
                scanf("%d",&studentId);
                deleteRowAndDisplay(studentId);
                break;
            case 7:
                enqueue_books_from_file(bookNameQueue);
                break;
            case 8:
                enqueue_student_from_file(studentNameQueue);
                break;
            case 9:
                issued_book_display("issuebook.csv",book_issued,MAX_BOOKS);
                break;
            
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    // Freeing allocated memory (optional at the end of the program)
    while (library != NULL) 
    {
        BOOK* temp = library;
        library = library->next;
        free(temp);
    }

    while (students != NULL) 
    {
        STUDENT* temp = students;
        students = students->next;
        free(temp);
    }
    
    return 0;
}
