//name: Kelly Tan Kai Ling
//student id: 20310184

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 49

struct book {
    char *title;
    char *author;
    char *subject;
};

struct library {
    struct book collection;
    int num_books;
    struct library *next;
};

//prototypes
void copybook(struct book* dest, struct book* source);
void deletebook(struct library** thislib, int num);
void addBook(struct library** thislib, char title[], char author[], char subject[]);
void deleteTitle(struct library* thislib, char title[]);
void deleteAuthor(struct library* thislib, char author[]);
void searchTitle(struct library* thislib, char title[]);
void searchAuthor(struct library* thislib, char author[]);
void listAuthor(struct library* thislib, char author[]);
void listSubject(struct library* thislib, char subject[]);
void allContent();

//main function
int main()
{
    char line[100], tit[30], aut[30], sub[30];
    int choice;
    struct book mainbook;
    struct library *mainlib = NULL; //initialize pointer mainlib to NULL
    struct library *temp;

    FILE *libPtr; //libPtr = library.txt file pointer

    if((libPtr = fopen("library.txt","r")) == NULL){ //error opening file
        puts("File could not be opened");

    }else{
        while (fgets(line,100, libPtr)){ //get 1 line from library.txt and save in line array
            puts("");
            choice = atoi(line); //convert line from string to integer to determine choice

            switch(choice){
                case 1:
                    sscanf(line, "%d %s %s %s",&choice,tit,aut,sub); //scan the line and save them into tit, aut, sub
                    addBook(&mainlib,tit,aut,sub); //proceed to function 1
                    break;

                case 2:
                    sscanf(line, "%d %s",&choice,tit);
                    deleteTitle(mainlib,tit); //proceed to function 2
                    break;

                case 3:
                    sscanf(line, "%d %s",&choice,aut);
                    int found = 0;
                    temp = mainlib; //save the first pointer in temp

                    //check how many books by the author
                    while(mainlib != NULL){ //if thislib == NULL, it reaches the end of collection
                        strcpy(line,mainlib->collection.author);

                        if(strcmp(strupr(line),strupr(aut))==0){
                            found++; //num of books by the author
                        }
                        mainlib = mainlib->next; //continue points to next element till NULL
                    }

                    //repeat if there's more than 1 books by the author
                    do{
                        mainlib = temp; //reset mainlib pointer
                        deleteAuthor(mainlib,aut); //proceed to function 3
                        found--;
                    }while(found > 0);

                    break;

                case 4:
                    sscanf(line, "%d %s",&choice,tit); //scan the line and save them into tit
                    searchTitle(mainlib,tit); //proceed to function 4
                    break;

                case 5:
                    sscanf(line, "%d %s",&choice,aut); //scan the line and save them into aut
                    searchAuthor(mainlib,aut); //proceed to function 5
                    break;

                case 6:
                    sscanf(line, "%d %s",&choice,aut); //scan the line and save them into aut
                    listAuthor(mainlib,aut); //proceed to function 6
                    break;

                case 7:
                    sscanf(line, "%d %s",&choice,sub); //scan the line and save them into sub
                    listSubject(mainlib,sub);//proceed to function 7
                    break;

                case 8:
                    allContent(mainlib); //proceed to function 8
                    break;

                default:
                    break; //ignore choices other than 1-8
            }
        }
    }
    fclose(libPtr); //close the file


    return 0;
}

//copy function
void copybook(struct book* dest, struct book* source){
    *dest = *source; //copy by reference
}

//delete function
void deletebook(struct library** thislib, int num){
    int lastnum;
    struct library *templib;
    templib = *thislib;

    //determine num of books in the collection
    while((*thislib)!=NULL){

        if((*thislib)->next == NULL){ //last element in the collection
            lastnum = (*thislib)->num_books; //last element points to newlib
            break;

        }else{ //continue to point to next collection until found NULL
            (*thislib) = (*thislib)->next;
        }
    }
    *thislib = templib;

    //here starts deleting
    if(lastnum == 1 && num == 1){ //if there's only 1 book left in collection
        free(*thislib); //deletes
        *thislib = NULL; //points to NULL

    }else if(lastnum == 2){ //if there's only 2 books left in collection

        if(num == 1){ //if books to be deleted is first book
            templib = (*thislib)->next;
            copybook(&((*thislib)->collection), &(templib->collection)); //copy 2nd (last book) to the first book (book to be deleted)
            free(templib); //deletes the 2nd (last book)
            (*thislib)->next = NULL; //points next to NULL

        }else if(num == 2){ //if books to be deleted is second book (last book)
            templib = (*thislib)->next;
            free(templib); //delete the book
            (*thislib)->next = NULL; //points next to NULL
        }

    }else if(lastnum > 2){ //if there's more than 2 books in the collection

        if(num == lastnum-1){ //if books to be deleted is 2nd last book
            while((*thislib)!=NULL){

                if((*thislib)->num_books == num){ //if found the book
                    templib = (*thislib)->next; //templib points to the last book while thislib points to book to be deleted
                    break;
                }else{ //continue to point to next collection until found num
                    (*thislib) = (*thislib)->next;
                }
            }
            copybook(&((*thislib)->collection), &(templib->collection)); //copy last book to 2nd last book
            free(templib); //deletes last book
            (*thislib)->next = NULL; //points next to NULL

        }else if(num == lastnum){ //if books to be deleted is last book
            while((*thislib)!=NULL){

                if((*thislib)->num_books == num-1){ //if found 2nd last book
                    templib = (*thislib)->next; //templib points to the last book
                    free(templib); //deletes the last book
                    (*thislib)->next = NULL; //points next to NULL
                    break;
                }else{ //continue to point to next collection until found num
                    (*thislib) = (*thislib)->next;
                }
            }

        }else{ //if books to be deleted is not 2nd last or last book
            while((*thislib)!=NULL){

                if((*thislib)->num_books == num){ //if found the book to be deleted
                    templib = *thislib; //templib points to the book to be deleted
                }else if((*thislib)->num_books == lastnum-1){ //if found the 2nd last book
                    copybook(&(templib->collection), &((*thislib)->collection)); //copy 2nd last book to the book to be deleted
                    templib = (*thislib)->next; //templib points to the last book
                    copybook(&((*thislib)->collection), &(templib->collection)); //copy last book to the 2nd last book
                    free(templib); //deletes the last book
                    (*thislib)->next = NULL; //points next to NULL
                }
                (*thislib) = (*thislib)->next; //continue to point to next collection until found num
            }

        }

    }
}

//function 1: add book
void addBook(struct library** thislib, char *title, char *author, char *subject){
    struct library *newlib = (struct library*)malloc(sizeof(struct library)); //allocate new memory pointed by newlib
    struct library *templib;

    printf("The book %s author %s subject %s has been added to the library.\n",title,author,subject); //print to terminal output

    FILE *ptr; //ptr = output.txt file pointer

    if((ptr = fopen("output.txt","a+")) == NULL){ //error opening file
        puts("File could not be opened");
    }else{

        fprintf(ptr, "The book %s author %s subject %s has been added to the library.\n\n",title,author,subject); //write into output.txt
    }
    fclose(ptr); //close the file

    if(newlib != NULL){
        //copy title, author and subject to collection
        newlib->collection.title = malloc(strlen(title) + 1);
        strcpy(newlib->collection.title, title);
        newlib->collection.author = malloc(strlen(author) + 1);
        strcpy(newlib->collection.author, author);
        newlib->collection.subject = malloc(strlen(subject) + 1);
        strcpy(newlib->collection.subject, subject);

        newlib->next = NULL; //newlib points to NULL

        if(*thislib != NULL){ //add the book at the end of the collection
            templib = *thislib; //save the list header in templib

            while((*thislib)!=NULL){

                if((*thislib)->next == NULL){ //last element in the collection
                    newlib->num_books = (*thislib)->num_books + 1; //update num of books
                    (*thislib)->next = newlib; //last element points to newlib
                    break;

                }else{ //continue to point to next collection untill found NULL
                    (*thislib) = (*thislib)->next;
                }
            }
            *thislib = templib; //reset the thislib pointer as the header

        }else{ //if collection has no book yet
            *thislib = newlib; //thislib points to newlib
            newlib->num_books = 1;
        }

    }else{
        printf("Memory not allocated.\n"); //failed to allocate memory
    }

}

//function 2: delete by title
void deleteTitle(struct library* thislib, char *title){
    int found = 0;
    struct library *deletelib;
    deletelib = thislib;

    FILE *ptr; //ptr = output.txt file pointer

    if((ptr = fopen("output.txt","a+")) == NULL){ //error opening file
        puts("File could not be opened");

    }else{
        if(thislib == NULL){ //no books in library
            printf("Deletion cannot be performed as the requested item does not exist.\n"); //print to terminal output
            fprintf(ptr,"Deletion cannot be performed as the requested item does not exist.\n"); //write to output.txt

        }else{
            while(thislib != NULL){
                if(strcmp(thislib->collection.title,title)==0){ //compare the input with collection
                    printf("The book %s has been removed from the library.\n",thislib->collection.title); //print to terminal output
                    fprintf(ptr,"The book %s has been removed from the library.\n",thislib->collection.title); //write to output.txt
                    deletebook(&deletelib, thislib->num_books);
                    found = 1; //set the found flag to 1
                    break;

                }else{
                    thislib = thislib->next; //continue points to next element till NULL
                }
            }
        }

        if(found == 0){ //not in the collection
            printf("Deletion cannot be performed as the requested item does not exist.\n"); //print to terminal output
            fprintf(ptr,"Deletion cannot be performed as the requested item does not exist.\n"); //write to output.txt
        }
    }
    fclose(ptr); //close the file
}

//function 3: delete by author
void deleteAuthor(struct library* thislib, char *author){
    char name[30];
    int found = 0;
    struct library *deletelib;
    deletelib = thislib;

    FILE *ptr; //ptr = output.txt file pointer

    if((ptr = fopen("output.txt","a+")) == NULL){ //error opening output.txt
        puts("File could not be opened");

    }else{
        if(thislib == NULL){
            printf("Deletion cannot be performed as the requested author does not exist.\n"); //print to terminal output
            fprintf(ptr,"Deletion cannot be performed as the requested author does not exist.\n"); //write to output.txt

        }else{
            while(thislib != NULL){ //if thislib == NULL, it reaches the end of collection
                strcpy(name,thislib->collection.author);
                if(strcmp(strupr(name),strupr(author))==0){
                    printf("The book %s authored by %s has been removed from the library.\n",thislib->collection.title,author); //print to terminal output
                    fprintf(ptr,"The book %s authored by %s has been removed from the library.\n",thislib->collection.title,author); //write to output.txt
                    deletebook(&deletelib, thislib->num_books);
                    found = 1; //set the found flag to 1
                    break;
                }
                thislib = thislib->next; //continue points to next element till NULL
            }
        }

        if(found == 0){ //not in the collection
            printf("Deletion cannot be performed as the requested author does not exist.\n"); //print to terminal output
            fprintf(ptr,"Deletion cannot be performed as the requested author does not exist.\n"); //write to output.txt
        }
    }
    fprintf(ptr,"\n");
    fclose(ptr); //close the file
}

//function 4: search by title
void searchTitle(struct library* thislib, char *title){
    int found = 0;

    FILE *ptr; //ptr = output.txt file pointer

    if((ptr = fopen("output.txt","a+")) == NULL){ //error opening file
        puts("File could not be opened");

    }else{
        if(thislib == NULL){ //no books in library
            printf("The book %s is NOT currently in the library.\n",title); //print to terminal output
            fprintf(ptr,"The book %s is NOT currently in the library.\n\n",title); //write to output.txt

        }else{
            while(thislib != NULL){
                if(strcmp(thislib->collection.title,title)==0){ //compare the input with collection
                    printf("The book %s is currently in the library.\n",title); //print to terminal output
                    fprintf(ptr,"The book %s is currently in the library.\n\n",title); //write to output.txt
                    found = 1; //set the found flag to 1
                    break;

                }else{
                    thislib = thislib->next; //continue points to next element till NULL
                }
            }
        }

        if(found == 0){ //not in the collection
            printf("The book %s is NOT currently in the library.\n",title); //print to terminal output
            fprintf(ptr,"The book %s is NOT currently in the library.\n\n",title); //write to output.txt
        }
    }
    fclose(ptr); //close the file
}

//function 5: search by author
void searchAuthor(struct library* thislib, char *author){
    int found = 0;
    char name[39];

    FILE *ptr; //ptr = output.txt file pointer

    if((ptr = fopen("output.txt","a+")) == NULL){ //error opening output.txt
        puts("File could not be opened");

    }else{
        if(thislib == NULL){
            printf("The book authored by %s is NOT currently in the library.\n",author); //print to terminal output
            fprintf(ptr,"The book authored by %s is NOT currently in the library.\n",author); //write to output.txt

        }else{
            printf("The book authored by %s\n",author); //print to terminal output
            fprintf(ptr,"The book authored by %s\n",author); //write to output.txt

            while(thislib != NULL){ //if thislib == NULL, it reaches the end of collection
                strcpy(name,thislib->collection.author);

                if(strcmp(strupr(name),strupr(author))==0){
                    printf("%s\n",thislib->collection.title); //print to terminal output
                    fprintf(ptr,"%s\n",thislib->collection.title); //write to output.txt
                    found = 1; //set the found flag to 1
                }
                thislib = thislib->next; //continue points to next element till NULL
            }
        }

        if(found == 0){ //not in the collection
            printf("The book authored by %s is NOT currently in the library.\n",author); //print to terminal output
            fprintf(ptr,"The book authored by %s is NOT currently in the library.\n",author); //write to output.txt
        }
    }
    fprintf(ptr,"\n");
    fclose(ptr); //close the file
}

//function 6: list by author
void listAuthor(struct library* thislib, char *author){
    char array[30][MAX],temp[30], name[30];
    int index = 0;

    FILE *ptr; //ptr = output.txt file pointer

    if((ptr = fopen("output.txt","a+")) == NULL){ //error opening output.txt
        puts("File could not be opened");

    }else{
        if(thislib == NULL){
            printf("No book found from author %s.\n",author); //print to terminal output
            fprintf(ptr,"No book found from author %s.\n",author); //write to output.txt

        }else{
            printf("List of all books by %s\n",author); //print to terminal output
            fprintf(ptr,"List of all books by %s\n",author); //write to output.txt

            while(thislib != NULL){
                strcpy(name,thislib->collection.author);

                if(strcmp(strupr(name),strupr(author))==0){ //convert to uppercase and compare input with collection
                    strcpy(array[index],thislib->collection.title); //if found, save its title into array
                    index++;
                }
                thislib = thislib->next; //continue points to next element till NULL
            }
        }

        if(index == 0){ //not in the collection
            printf("No book found from author %s.\n",author); //print to terminal output
            fprintf(ptr,"No book found from author %s.\n",author); //write to output.txt

        }else{
            //sort the array in ascending order
            for(int i=0;i<index;i++){
              for(int j=i+1;j<index;j++){
                 if(strcmp(array[i],array[j])>0){ //if next element larger current element
                    strcpy(temp,array[i]); //store current element to temp
                    strcpy(array[i],array[j]); //swap the 2 elements
                    strcpy(array[j],temp); //store temp into next element
                 }
              }
            }

            //prints the sorted array
            for(int i=0;i<index;i++){
                puts(array[i]); //print to terminal output
                fprintf(ptr,"%s\n",array[i]); //write to output.txt
            }
        }
    }
    fprintf(ptr,"\n");
    fclose(ptr); //close the file
}

//function 7: list by subject
void listSubject(struct library* thislib, char *subject){
    char array[30][MAX],temp[30],name[30];
    int index = 0;

    FILE *ptr; //ptr = output.txt file pointer

    if((ptr = fopen("output.txt","a+")) == NULL){ //error opening output.txt
        puts("File could not be opened");

    }else{
        if(thislib == NULL){
            printf("No book found from subject %s.\n",subject); //print to terminal output
            fprintf(ptr,"No book found from subject %s.\n",subject); //write to output.txt

        }else{
            printf("List of all books on %s\n",subject); //print to terminal output
            fprintf(ptr,"List of all books on %s\n",subject); //write to output.txt

            while(thislib != NULL){
                strcpy(name,thislib->collection.subject);

                if(strcmp(strupr(name),strupr(subject))==0){ //convert to uppercase and compare input with collection
                    strcpy(array[index],thislib->collection.title); //if found, save its title into array
                    index++;
                }
                thislib = thislib->next; //continue points to next element till NULL
            }
        }

        if(index == 0){ //not in the collection
            printf("No book found from subject %s.\n",subject); //print to terminal output
            fprintf(ptr,"No book found from subject %s.\n",subject); //write to output.txt

        }else{

            //sort the array in descending order
            for(int i=0;i<index;i++){
              for(int j=i+1;j<index;j++){
                 if(strcmp(array[i],array[j])<0){ //if next element smaller current element
                    strcpy(temp,array[i]); //store current element to temp
                    strcpy(array[i],array[j]); //swap the 2 elements
                    strcpy(array[j],temp); //store temp into next element
                 }
              }
            }

            //prints the sorted array
            for(int i=0;i<index;i++){
                puts(array[i]); //print to terminal output
                fprintf(ptr,"%s\n",array[i]); //write to output.txt
            }
        }
    }
    fprintf(ptr,"\n");
    fclose(ptr); //close the file
}

//function 8: show all collection
void allContent(struct library* thislib){

    FILE *ptr; //ptr = output.txt file pointer

    if((ptr = fopen("output.txt","a+")) == NULL){ //error opening output.txt
        puts("File could not be opened");

    }else{
        if(thislib == NULL){
            printf("Library is empty.\n"); //print to terminal output
            fprintf(ptr,"Library is empty.\n\n"); //write to output.txt

        }else{
            printf("Content of all books\n"); //print to terminal output
            fprintf(ptr,"Content of all books\n"); //write to output.txt

            while(thislib != NULL){
                printf("%s %s %s\n",thislib->collection.title,thislib->collection.author,thislib->collection.subject);
                fprintf(ptr,"%s %s %s\n",thislib->collection.title,thislib->collection.author,thislib->collection.subject); //write to output.txt
                thislib = thislib->next; //continue points to next element till NULL
            }
        }
    }
    fprintf(ptr,"\n");
    fclose(ptr); //close the file
}
