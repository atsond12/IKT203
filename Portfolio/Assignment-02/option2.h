#pragma once
#include <string>
#include <vector>
#include <list>

// Enumeration for employee departments
enum class EDepartment {
    Finance,
    HR,
    IT
};

// Employee structure
// Ownership: TEmployee* objects are owned by masterList
struct TEmployee {
    std::string firstName;
    std::string lastName;
    EDepartment department;

    TEmployee(const std::string& f, const std::string& l, EDepartment d)
        : firstName(f), lastName(l), department(d) {}

    // Overload operator< for alphabetical sort (lastName, firstName)
    bool operator<(const TEmployee& other) const {
        if (lastName != other.lastName) return lastName < other.lastName;
        return firstName < other.firstName;
    }
};

// Public entry point
int RunApp();

// Helper: Convert enum to string
std::string DeptToString(EDepartment d);

// Callback type for reading names
// Pass strings by const reference to match OnNameReadStatic
using NameCallback = void(*)(const std::string& first,
                             const std::string& last,
                             int index);
