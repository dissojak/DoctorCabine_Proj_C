#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // For getting today's date
#include <conio.h> // For hash of password ( security reasons )

// Patient structure
typedef struct Patient {
    int id;
    char name[50];
    char problem[100];
    struct Patient* prev;
    struct Patient* next;
} Patient;

// Rendezvous structure
typedef struct Rendezvous {
    int patientId;
    char date[20];
    char time[10];
    struct Rendezvous* next;
} Rendezvous;

// Global vars
Patient* first = NULL;       // First of the patient list
Patient* last = NULL;        // Last of the patient list
Rendezvous* appointments = NULL; // pointer of the appointments list

// Function to clear the screen
void clearScreen() {
    system("cls");
}

// Function to print the header
void printHeader() {
    printf("========================================\n");
    printf("   Doctor Cabin Management System\n");
    printf("========================================\n");
}

// Function to print the footer
void printFooter() {
    printf("========================================\n");
}

// Function to add a patient
void addPatient(int id, char name[], char problem[]) {
    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    newPatient->id = id;
    strcpy(newPatient->name, name);
    strcpy(newPatient->problem, problem);
    newPatient->prev = NULL;
    newPatient->next = NULL;

    if (first == NULL) {
        first = last = newPatient;
    } else {
        last->next = newPatient;
        newPatient->prev = last;
        last = newPatient;
    }
}

// Function to delete a patient
void deletePatient(int id) {
    Patient* temp = first;
    while (temp != NULL) {
        if (temp->id == id) {
            if (temp->prev != NULL) {
                temp->prev->next = temp->next;
            } else {
                first = temp->next;
            }
            if (temp->next != NULL) {
                temp->next->prev = temp->prev;
            } else {
                last = temp->prev;
            }
            free(temp);
            printf("Patient deleted successfully!\n");
            return;
        }
        temp = temp->next;
    }
    printf("Patient not found!\n");
}

// Function to search for a patient
Patient* searchPatient(int id) {
    Patient* temp = first;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Function to display all patients
void displayPatients() {
    Patient* temp = first;
    while (temp != NULL) {
        printf("ID: %d, Name: %s, Problem: %s\n", temp->id, temp->name, temp->problem);
        temp = temp->next;
    }
}

// Function to schedule an appointment
void scheduleRendezvous(int patientId, char date[], char time[]) {
    // Check if the patient exists
    Patient* patient = searchPatient(patientId);
    if (patient == NULL) {
        printf("Patient not found!\n");
        return;
    }

    // Create a new appointment
    Rendezvous* newAppointment = (Rendezvous*)malloc(sizeof(Rendezvous));
    newAppointment->patientId = patientId;
    strcpy(newAppointment->date, date);
    strcpy(newAppointment->time, time);
    newAppointment->next = NULL;

    // Add to the appointments list
    if (appointments == NULL) {
        appointments = newAppointment;
    } else {
        Rendezvous* temp = appointments;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newAppointment;
    }
}

// Function to edit an appointment
void editAppointment(int patientId, char newDate[], char newTime[]) {
    Rendezvous* temp = appointments;
    while (temp != NULL) {
        if (temp->patientId == patientId) {
            strcpy(temp->date, newDate);
            strcpy(temp->time, newTime);
            printf("Appointment updated successfully!\n");
            return;
        }
        temp = temp->next;
    }
    printf("Appointment not found!\n");
}

// Function to delete an appointment
void deleteAppointment(int patientId) {
    Rendezvous* temp = appointments;
    Rendezvous* prev = NULL;

    while (temp != NULL) {
        if (temp->patientId == patientId) {
            if (prev == NULL) {
                appointments = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            printf("Appointment deleted successfully!\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Appointment not found!\n");
}

// Function to display all appointments
void displayRendezvous() {
    Rendezvous* temp = appointments;
    while (temp != NULL) {
        printf("Patient ID: %d, Date: %s, Time: %s\n", temp->patientId, temp->date, temp->time);
        temp = temp->next;
    }
}

// Function to search appointments by date
void searchAppointmentsByDate(char date[]) {
    Rendezvous* temp = appointments;
    int found = 0;
    while (temp != NULL) {
        if (strcmp(temp->date, date) == 0) {
            printf("Patient ID: %d, Time: %s\n", temp->patientId, temp->time);
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) {
        printf("No appointments found for the given date.\n");
    }
}

// Function to display today's appointments
void displayTodaysAppointments() {
    // Get today's date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char today[20];
    sprintf(today, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    Rendezvous* temp = appointments;
    int found = 0;
    while (temp != NULL) {
        if (strcmp(temp->date, today) == 0) {
            printf("Patient ID: %d, Time: %s\n", temp->patientId, temp->time);
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) {
        printf("No appointments found for today.\n");
    }
}

// Function to save patients to a file
void savePatientsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening patients file!\n");
        return;
    }

    // Save patients
    Patient* tempPatient = first;
    while (tempPatient != NULL) {
        fprintf(file, "%d %s %s\n", tempPatient->id, tempPatient->name, tempPatient->problem);
        tempPatient = tempPatient->next;
    }

    fclose(file);
    printf("Patients saved to file successfully!\n");
}

// Function to load patients from a file
void loadPatientsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No existing patients file found. Starting with an empty patient database.\n");
        return;
    }

    int id;
    char name[50], problem[100];

    while (fscanf(file, "%d %s %s", &id, name, problem) != EOF) {
        addPatient(id, name, problem);
    }

    fclose(file);
    printf("Patients loaded from file successfully!\n");
}

// Function to save appointments to a file
void saveAppointmentsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening appointments file!\n");
        return;
    }

    // Save appointments
    Rendezvous* tempAppointment = appointments;
    while (tempAppointment != NULL) {
        fprintf(file, "%d %s %s\n", tempAppointment->patientId, tempAppointment->date, tempAppointment->time);
        tempAppointment = tempAppointment->next;
    }

    fclose(file);
    printf("Appointments saved to file successfully!\n");
}

// Function to load appointments from a file
void loadAppointmentsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No existing appointments file found. Starting with an empty appointments database.\n");
        return;
    }

    int patientId;
    char date[20], time[10];

    while (fscanf(file, "%d %s %s", &patientId, date, time) != EOF) {
        scheduleRendezvous(patientId, date, time);
    }

    fclose(file);
    printf("Appointments loaded from file successfully!\n");
}

// Function to handle program exit
void onExit() {
    printf("Saving data before exiting...\n");
    savePatientsToFile("patients.txt"); // Save patients to file
    saveAppointmentsToFile("appointments.txt"); // Save appointments to file
    printf("Data saved. Exiting the program. Goodbye!\n");
}

// Function to hide password input
void hidePassword(char password[]) {
    char ch;
    int i = 0;
    while ((ch = getch()) != '\r') { // '\r' is the Enter key
        if (ch == '\b' && i > 0) { // Handle backspace
            printf("\b \b");
            i--;
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0'; // Null-terminate the password
    printf("\n");
}

// Function to authenticate user
int authenticate() {
    char password[20];
    printf("Enter password: ");
    hidePassword(password);

    // Hardcoded password for security process
    if (strcmp(password, "ademAdmin") == 0 || strcmp(password, "mahaAdmin") == 0) {
        return 1; // Authentication successful
    } else {
        printf("Incorrect password. Access denied.\n");
        return 0; // Authentication failed
    }
}

// Main function
int main() {
    int choice, id;
    char name[50], problem[100], date[20], time[10];

    // Authenticate user
    if (!authenticate()) {
        return 0; // Exit if authentication fails
    }

    // Load data automatically when the program starts
    printf("Logged in successfully !\n");
    printf("\n");
    printf("Loading data...\n");
    loadPatientsFromFile("patients.txt"); // Load patients from file
    loadAppointmentsFromFile("appointments.txt"); // Load appointments from file
    getchar(); // Wait for Enter

    // Register the exit function
    atexit(onExit);

    while (1) {
        // Clear the screen and display the menu
        clearScreen();
        printHeader();
        printf("1. Add Patient\n");
        printf("2. Delete Patient\n");
        printf("3. Search Patient\n");
        printf("4. Display All Patients\n");
        printf("5. Schedule Rendezvous\n");
        printf("6. Display Rendezvous\n");
        printf("7. Edit Appointment\n");
        printf("8. Delete Appointment\n");
        printf("9. Search Appointments by Date\n");
        printf("10. Display Today's Appointments\n");
        printf("11. Exit\n");
        printFooter();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Add Patient
                clearScreen();
                printHeader();
                printf("========== Add Patient ==========\n");
                printf("Enter ID: ");
                scanf("%d", &id);
                printf("Enter Name: ");
                scanf("%s", name);
                printf("Enter Problem: ");
                scanf("%s", problem);
                addPatient(id, name, problem);
                printf("Patient added successfully!\n");
                break;

            case 2: // Delete Patient
                clearScreen();
                printHeader();
                printf("========== Delete Patient ==========\n");
                printf("Enter ID to delete: ");
                scanf("%d", &id);
                deletePatient(id);
                break;

            case 3: { // Search Patient
                clearScreen();
                printHeader();
                printf("========== Search Patient ==========\n");
                printf("Enter ID to search: ");
                scanf("%d", &id);
                Patient* patient = searchPatient(id);
                if (patient != NULL) {
                    printf("Patient Found:\n");
                    printf("ID: %d, Name: %s, Problem: %s\n", patient->id, patient->name, patient->problem);
                } else {
                    printf("Patient not found!\n");
                }
                break;
            }

            case 4: // Display All Patients
                clearScreen();
                printHeader();
                printf("========== All Patients ==========\n");
                displayPatients();
                break;

            case 5: // Schedule Rendezvous
                clearScreen();
                printHeader();
                printf("========== Schedule Rendezvous ==========\n");
                printf("Enter Patient ID: ");
                scanf("%d", &id);
                printf("Enter Date (DD/MM/YYYY): ");
                scanf("%s", date);
                printf("Enter Time (HH:MM): ");
                scanf("%s", time);
                scheduleRendezvous(id, date, time);
                printf("Appointment scheduled successfully!\n");
                break;

            case 6: // Display Rendezvous
                clearScreen();
                printHeader();
                printf("========== All Rendezvous ==========\n");
                displayRendezvous();
                break;

            case 7: // Edit Appointment
                clearScreen();
                printHeader();
                printf("========== Edit Appointment ==========\n");
                printf("Enter Patient ID: ");
                scanf("%d", &id);
                printf("Enter New Date (DD/MM/YYYY): ");
                scanf("%s", date);
                printf("Enter New Time (HH:MM): ");
                scanf("%s", time);
                editAppointment(id, date, time);
                break;

            case 8: // Delete Appointment
                clearScreen();
                printHeader();
                printf("========== Delete Appointment ==========\n");
                printf("Enter Patient ID: ");
                scanf("%d", &id);
                deleteAppointment(id);
                break;

            case 9: // Search Appointments by Date
                clearScreen();
                printHeader();
                printf("========== Search Appointments by Date ==========\n");
                printf("Enter Date (DD/MM/YYYY): ");
                scanf("%s", date);
                searchAppointmentsByDate(date);
                break;

            case 10: // Display Today's Appointments
                clearScreen();
                printHeader();
                printf("========== Today's Appointments ==========\n");
                displayTodaysAppointments();
                break;

            case 11: // Exit
                printf("Exiting the program. Goodbye!\n");
                exit(0);

            default: // Invalid choice
                printf("Invalid choice! Please try again.\n");
        }

        // Pause to let the user read the output
        printf("\nPress Enter to continue...");
        getchar(); // Wait for Enter
        getchar(); // Clear
    }

    return 0;
}
