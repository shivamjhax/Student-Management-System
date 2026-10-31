#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <limits.h>

#define MAX_STUDENTS 100
#define NAME_LENGTH 50

struct Student
{
    int id;
    char name[NAME_LENGTH];
    float marks;
};

// INPUT VALIDATION: INTEGER
int readInt(const char prompt[], int *value)
{
    char input[100];
    char *end;

    printf("%s", prompt);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 0;

    errno = 0;
    long num = strtol(input, &end, 10);

    while (*end == ' ' || *end == '\t' ||
           *end == '\n' || *end == '\r')
        end++;

    if (end == input || *end != '\0' ||
        errno == ERANGE || num < INT_MIN || num > INT_MAX)
    {
        return -1;
    }

    *value = (int)num;
    return 1;
}

// INPUT VALIDATION: FLOAT
int readFloat(const char prompt[], float *value)
{
    char input[100];
    char *end;

    printf("%s", prompt);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 0;

    errno = 0;
    float num = strtof(input, &end);

    while (*end == ' ' || *end == '\t' ||
           *end == '\n' || *end == '\r')
        end++;

    if (end == input || *end != '\0' ||
        errno == ERANGE || !isfinite(num))
    {
        return -1;
    }

    *value = num;
    return 1;
}

// INPUT VALIDATION: NAME
int readName(const char prompt[], char name[])
{
    printf("%s", prompt);

    if (fgets(name, NAME_LENGTH, stdin) == NULL)
        return 0;

    size_t len = strlen(name);

    if (len > 0 && name[len - 1] == '\n')
    {
        name[len - 1] = '\0';
    }
   else
{
    int ch = getchar();

    if (ch != '\n' && ch != EOF)
    {
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
        }

        printf("Name is too long! Maximum 49 characters allowed.\n");
        return -1;
    }
}

    if (strlen(name) == 0)
    {
        printf("Name cannot be empty!\n");
        return -1;
    }

    return 1;
}

// CHECK DUPLICATE ID
int isDuplicate(struct Student students[], int count, int id)
{
    for (int i = 0; i < count; i++)
    {
        if (students[i].id == id)
            return 1;
    }

    return 0;
}

// SAVE STUDENTS TO FILE
int saveStudents(struct Student students[], int count)
{
    FILE *fp = fopen("students.txt", "w");

    if (fp == NULL)
    {
        printf("File opening failed! Data could not be saved.\n");
        return 0;
    }

    for (int i = 0; i < count; i++)
    {
        if (fprintf(fp, "%d\n%s\n%.2f\n",
                    students[i].id,
                    students[i].name,
                    students[i].marks) < 0)
        {
            printf("Error writing data to file!\n");
            fclose(fp);
            return 0;
        }
    }

    if (fclose(fp) != 0)
    {
        printf("Error closing file! Data may not be saved properly.\n");
        return 0;
    }

    printf("Data saved successfully!\n");
    return 1;
}

// LOAD STUDENTS FROM FILE
void loadStudents(struct Student students[], int *count)
{
    FILE *fp = fopen("students.txt", "r");

    if (fp == NULL)
    {
        printf("No previous data file found. Starting fresh.\n");
        return;
    }

    while (*count < MAX_STUDENTS)
    {
        int id;
        float marks;
        char name[NAME_LENGTH];

       if (fscanf(fp, "%d", &id) != 1)
    break;

int ch;

while ((ch = fgetc(fp)) != '\n' && ch != EOF)
{
}

if (ch == EOF)
    break;

        if (fgets(name, sizeof(name), fp) == NULL)
            break;

        name[strcspn(name, "\n")] = '\0';

        if (fscanf(fp, "%f\n", &marks) != 1)
            break;

        if (id <= 0 || strlen(name) == 0 ||
            marks < 0 || marks > 100 || !isfinite(marks))
        {
            printf("Invalid record found in file. Skipping record.\n");
            continue;
        }

        if (isDuplicate(students, *count, id))
        {
            printf("Duplicate ID found in file. Skipping record.\n");
            continue;
        }

        students[*count].id = id;
        strcpy(students[*count].name, name);
        students[*count].marks = marks;

        (*count)++;
    }

    if (ferror(fp))
        printf("Error reading student file!\n");

    fclose(fp);
}

// Existing functions...

void sortStudentsByMarks(struct Student students[], int count)
{
    struct Student temp;

for (int i = 0; i < count - 1; i++)
{
    for (int j = 0; j < count - i - 1; j++)
    {
        if (students[j].marks < students[j + 1].marks)
        {
            temp = students[j];
            students[j] = students[j + 1];
            students[j + 1] = temp;
        }
    }
}
}

void displayTopper(struct Student students[], int count)
{
    if (count == 0)
    {
        printf("No students available!\n");
        return;
    }

    int topper = 0;

    for (int i = 1; i < count; i++)
    {
        if (students[i].marks > students[topper].marks)
        {
            topper = i;
        }
    }

    printf("\n========== CLASS TOPPER ==========\n");
    printf("Student ID: %d\n", students[topper].id);
    printf("Name: %s\n", students[topper].name);
    printf("Marks: %.2f\n", students[topper].marks);
    printf("==================================\n");
}

void displayClassAverage(struct Student students[], int count)
{
    if (count == 0)
    {
        printf("No students available!\n");
        return;
    }

    float total = 0;

    for (int i = 0; i < count; i++)
    {
        total += students[i].marks;
    }

    float average = total / count;

    printf("\n========== CLASS AVERAGE ==========\n");
    printf("Total Students: %d\n", count);
    printf("Total Marks: %.2f\n", total);
    printf("Class Average: %.2f\n", average);
    printf("===================================\n");
}

int main()
{
    struct Student students[MAX_STUDENTS];

    int count = 0;
    int choice;
    int result;

    // LOAD PREVIOUS DATA
    loadStudents(students, &count);

    printf("\nLoaded %d students from file.\n", count);

    do
    {
        printf("\n================================\n");
        printf("     STUDENT MANAGEMENT SYSTEM\n");
        printf("================================\n");

        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Sort Students by Marks\n");
        printf("7. Display Class Topper\n");
        printf("8. Display Class Average\n");
        printf("0. Exit\n");

        // MENU INPUT
        result = readInt("\nEnter your choice: ", &choice);

        if (result == 0)
        {
            printf("\nInput closed. Exiting program.\n");
            break;
        }

        if (result == -1)
        {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        switch (choice)
        {
            // 1. ADD STUDENT
            case 1:
            {
                int id;
                float marks;
                char name[NAME_LENGTH];

                if (count >= MAX_STUDENTS)
                {
                    printf("Student list is full!\n");
                    break;
                }

                result = readInt("Enter Student ID: ", &id);

                if (result == 0)
                    break;

                if (result == -1 || id <= 0)
                {
                    printf("Invalid ID! Enter a positive number.\n");
                    break;
                }

                if (isDuplicate(students, count, id))
                {
                    printf("Student ID already exists!\n");
                    break;
                }

                result = readName("Enter Student Name: ", name);

                if (result == 0)
                    break;

                if (result == -1)
                    break;

                result = readFloat("Enter Student Marks: ", &marks);

                if (result == 0)
                    break;

                if (result == -1 || marks < 0 || marks > 100)
                {
                    printf("Invalid marks! Enter a number between 0 and 100.\n");
                    break;
                }

                students[count].id = id;
                strcpy(students[count].name, name);
                students[count].marks = marks;

               count++;

if (saveStudents(students, count))
{
    printf("\nStudent added successfully!\n");
    printf("ID: %d\n", id);
    printf("Name: %s\n", name);
    printf("Marks: %.2f\n", marks);
}
else
{
    count--;
    printf("Error! Student could not be saved.\n");
}

break;
            }

            // 2. VIEW STUDENTS
            case 2:
            {
                if (count == 0)
                {
                    printf("\nNo students found!\n");
                }
                else
                {
                    printf("\n========== ALL STUDENTS ==========\n");

                    for (int i = 0; i < count; i++)
                    {
                        printf("\nStudent %d\n", i + 1);
                        printf("ID: %d\n", students[i].id);
                        printf("Name: %s\n", students[i].name);
                        printf("Marks: %.2f\n", students[i].marks);
                    }
                }

                break;
            }

            // 3. SEARCH STUDENT
            case 3:
            {
                int searchId;
                int found = 0;

                result = readInt("Enter Student ID to search: ", &searchId);

                if (result == 0)
                    break;

                if (result == -1 || searchId <= 0)
                {
                    printf("Invalid Student ID!\n");
                    break;
                }

                for (int i = 0; i < count; i++)
                {
                    if (students[i].id == searchId)
                    {
                        printf("\nStudent Found!\n");
                        printf("ID: %d\n", students[i].id);
                        printf("Name: %s\n", students[i].name);
                        printf("Marks: %.2f\n", students[i].marks);

                        found = 1;
                        break;
                    }
                }

                if (!found)
                    printf("Student not found!\n");

                break;
            }

            // 4. UPDATE STUDENT
            case 4:
            {
                int updateId;
                int found = 0;
                char newName[NAME_LENGTH];
                float newMarks;

                result = readInt("Enter Student ID to update: ", &updateId);

                if (result == 0)
                    break;

                if (result == -1 || updateId <= 0)
                {
                    printf("Invalid Student ID!\n");
                    break;
                }

                for (int i = 0; i < count; i++)
                {
                    if (students[i].id == updateId)
                    {
                        result = readName("Enter new name: ", newName);

                        if (result == 0)
                            break;

                        if (result == -1)
                        {
                            found = 1;
                            break;
                        }

                        result = readFloat("Enter new marks: ", &newMarks);

                        if (result == 0)
                            break;

                        if (result == -1 ||
                            newMarks < 0 || newMarks > 100)
                        {
                            printf("Invalid marks! Enter 0 to 100.\n");
                            found = 1;
                            break;
                        }

                        struct Student oldStudent = students[i];

strcpy(students[i].name, newName);
students[i].marks = newMarks;

if (saveStudents(students, count))
{
    printf("\nStudent updated successfully!\n");
    printf("ID: %d\n", students[i].id);
    printf("Name: %s\n", students[i].name);
    printf("Marks: %.2f\n", students[i].marks);
}
else
{
    students[i] = oldStudent;
    printf("Error! Student update could not be saved.\n");
}

found = 1;
break;
                        found = 1;
                        break;
                    }
                }

                if (!found)
                    printf("Student not found!\n");

                break;
            }

            // 5. DELETE STUDENT
            case 5:
            {
                int deleteId;
                int found = 0;

                result = readInt("Enter Student ID to delete: ", &deleteId);

                if (result == 0)
                    break;

                if (result == -1 || deleteId <= 0)
                {
                    printf("Invalid Student ID!\n");
                    break;
                }

                for (int i = 0; i < count; i++)
                {
                    if (students[i].id == deleteId)
                    {
                     struct Student deletedStudent = students[i];

for (int j = i; j < count - 1; j++)
{
    students[j] = students[j + 1];
}

count--;

if (saveStudents(students, count))
{
    printf("\nStudent deleted successfully!\n");
}
else
{
    for (int j = count; j > i; j--)
    {
        students[j] = students[j - 1];
    }

    students[i] = deletedStudent;
    count++;

    printf("Error! Student could not be deleted.\n");
}

found = 1;
break;   
                    }
                }

                if (!found)
                    printf("Student not found!\n");

                break;
            }

            // 0. EXIT
            case 0:
                printf("Exiting program. Goodbye!\n");
                break;


                // 6. SORT STUDENTS
case 6:
{
    if (count == 0)
    {
        printf("No students to sort!\n");
        break;
    }

    sortStudentsByMarks(students, count);
    saveStudents(students, count);

    printf("\nStudents sorted by marks (High to Low)!\n");
    break;
}



case 7:
{
    displayTopper(students, count);
    break;
}

// 8. DISPLAY CLASS AVERAGE
case 8:
{
    if (count == 0)
    {
        printf("No students available!\n");
        break;
    }

    float total = 0;

    for (int i = 0; i < count; i++)
    {
        total += students[i].marks;
    }

    printf("\nClass Average: %.2f\n", total / count);

    break;
}

            default:
                printf("Invalid choice! Select 0 to 8.\n");
                break;
        }

    } while (choice != 0);

    return 0;
}
