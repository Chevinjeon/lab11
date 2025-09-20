// COMSC-210 | Lab 11 | Chevin Jeon 
// IDE used: VSC
// Date: 2025-09-18

#include <iostream>   // std::cout, std::endl
#include <string>     // std::string

//==============================
// Appointment struct
//==============================

// Represents one clinic appointment.
// This struct includes a DYNAMIC ARRAY (symptoms) plus simple fields.
struct Appointment {
    std::string patientName;   // a normal (non-dynamic) field: patient's name
    int ticketNo;              // a normal field: queue/ticket number
    std::string* symptoms;     // POINTER to a dynamic array of symptom strings
    size_t symptomCount;       // how many elements are currently in 'symptoms'
};


//==============================
// Functions that operate on ONE Appointment
//==============================

// Initialize an Appointment with no symptoms yet.
void initAppointment(Appointment &a, const std::string &name, int tno) {
    a.patientName   = name;      // set patient name
    a.ticketNo      = tno;       // set ticket number
    a.symptoms      = nullptr;   // no symptom array yet
    a.symptomCount  = 0;         // count = 0
}

// Add one symptom by resizing the inner dynamic array by +1.
void addSymptom(Appointment &a, const std::string &sym) {
    // New total size for the inner array
    size_t newCount = a.symptomCount + 1;

    // Allocate a NEW array with the increased size
    std::string *newArr = new std::string[newCount];

    // Copy old symptoms (if any) into the new array
    for (size_t i = 0; i < a.symptomCount; ++i) {
        newArr[i] = a.symptoms[i];
    }

    // Put the new symptom at the end
    newArr[newCount - 1] = sym;

    // Free the old array memory
    delete[] a.symptoms;

    // Point the struct to the new array and update the count
    a.symptoms = newArr;
    a.symptomCount = newCount;
}

// Remove a symptom at index `idx` (0-based). Safely ignore bad indices.
void removeSymptomAt(Appointment &a, size_t idx) {
    if (idx >= a.symptomCount) {
        std::cout << "[removeSymptomAt] Ignored invalid index " << idx << "\n";
        return;
    }

    // If removing the last remaining symptom, end up with an empty array.
    size_t newCount = a.symptomCount - 1;

    if (newCount == 0) {
        delete[] a.symptoms;     // free old (size 1) array
        a.symptoms = nullptr;    // no array left
        a.symptomCount = 0;      // count is zero
        return;
    }

    // Otherwise, allocate a NEW array with size-1
    std::string *newArr = new std::string[newCount];

    // Copy everything except the element at idx
    size_t w = 0;
    for (size_t r = 0; r < a.symptomCount; ++r) {
        if (r == idx) continue;   // skip the removed index
        newArr[w++] = a.symptoms[r];
    }

    // Free old array and update pointer/count
    delete[] a.symptoms;
    a.symptoms = newArr;
    a.symptomCount = newCount;
}

// Free the inner dynamic array of symptoms for one Appointment.
void freeAppointment(Appointment &a) {
    delete[] a.symptoms;   // safe if nullptr
    a.symptoms = nullptr;
    a.symptomCount = 0;
}

// Pretty-print one Appointment.
void printAppointment(const Appointment &a) {
    std::cout << "Ticket #" << a.ticketNo << " — " << a.patientName << "\n";
    std::cout << "  Symptoms (" << a.symptomCount << "): ";
    if (a.symptomCount == 0) {
        std::cout << "(none)\n";
        return;
    }
    for (size_t i = 0; i < a.symptomCount; ++i) {
        std::cout << (i == 0 ? "" : ", ") << a.symptoms[i];
    }
    std::cout << "\n";
}

//==============================
// Outer dynamic array of Appointments
//==============================

// Append an Appointment to the OUTER dynamic array.
// We pass the pointer and size by reference so we can replace them.
void appendAppointment(Appointment *&arr, size_t &count, const Appointment &toAdd) {
    // Compute new outer size
    size_t newCount = count + 1;

    // Allocate new array of that size
    Appointment *newArr = new Appointment[newCount];

    // Copy old appointments over (memberwise copy)
    for (size_t i = 0; i < count; ++i) {
        newArr[i] = arr[i];
        // NOTE: strings deep-copy themselves; `symptoms` pointer is copied (shallow),
        // which is OK because we’re moving ownership forward and will free once.
    }

    // Copy the new appointment into the last slot
    newArr[newCount - 1] = toAdd;

    // Free old outer array and update pointer + count
    delete[] arr;
    arr = newArr;
    count = newCount;
}

// Print the whole outer array
void printAll(const Appointment *arr, size_t count) {
    if (count == 0) {
        std::cout << "[No appointments]\n";
        return;
    }
    std::cout << "=== Current Appointments (" << count << ") ===\n";
    for (size_t i = 0; i < count; ++i) {
        std::cout << "------------------------------\n";
        printAppointment(arr[i]);
    }
    std::cout << "------------------------------\n";
}

// Free ALL memory: first inner arrays for each Appointment, then the outer array.
void freeAll(Appointment *&arr, size_t &count) {
    if (arr) {
        for (size_t i = 0; i < count; ++i) {
            freeAppointment(arr[i]); // frees per-appointment symptoms
        }
        delete[] arr;                // free the outer array
    }
    arr = nullptr;
    count = 0;
}

//==============================
// main(): exercise everything
//==============================

int main() {
    // OUTER dynamic array start: empty
    Appointment *queue = nullptr; // pointer to dynamic array of Appointment
    size_t qCount = 0;            // how many appointments in the array

    // 1) Create an appointment with several symptoms
    Appointment a;
    initAppointment(a, "Alice Kim", 17);
    addSymptom(a, "Headache");
    addSymptom(a, "Nausea");
    addSymptom(a, "Fever");
    appendAppointment(queue, qCount, a);

    // 2) Appointment with zero symptoms (edge case)
    Appointment b;
    initAppointment(b, "Bob Singh", 18);
    appendAppointment(queue, qCount, b);

    // 3) Add & remove symptoms to test resizing + boundary handling
    Appointment c;
    initAppointment(c, "Carla Diaz", 19);
    addSymptom(c, "Cough");
    addSymptom(c, "Sore throat");
    addSymptom(c, "Fatigue");
    removeSymptomAt(c, 1);       // remove "Sore throat"
    removeSymptomAt(c, 42);      // ignored (out of range)
    appendAppointment(queue, qCount, c);

    // 4) Larger one with several additions
    Appointment d;
    initAppointment(d, "Diego Park", 20);
    addSymptom(d, "Back pain");
    addSymptom(d, "Dizziness");
    addSymptom(d, "Shortness of breath");
    addSymptom(d, "Chest discomfort");
    appendAppointment(queue, qCount, d);

    // Show all current data
    printAll(queue, qCount);

    // Modify first patient (remove first symptom) and reprint to show changes
    std::cout << "\n[Update] Removing symptom 0 from ticket #" << queue[0].ticketNo << ":\n";
    removeSymptomAt(queue[0], 0);
    printAll(queue, qCount);

    // Clean up all dynamic memory
    freeAll(queue, qCount);

    // Prove cleanup worked
    std::cout << "\nAfter freeAll: qCount=" << qCount
              << ", queue=" << (queue == nullptr ? "nullptr" : "not-null") << "\n";
    return 0;
}
