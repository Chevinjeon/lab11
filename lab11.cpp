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
