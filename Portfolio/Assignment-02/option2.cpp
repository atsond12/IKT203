#include "option2.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>

// Convert enum to string
std::string DeptToString(EDepartment d) {
    switch (d) {
        case EDepartment::Finance: return "Finance";
        case EDepartment::HR:      return "HR";
        case EDepartment::IT:      return "IT";
        default:                   return "Unknown";
    }
}

// Generate department randomly based on index
static EDepartment RandomDepartmentFromIndex(int i) {
    switch (i % 3) {
        case 0: return EDepartment::Finance;
        case 1: return EDepartment::HR;
        default: return EDepartment::IT;
    }
}

// Read names from file and call callback for each
void readNamesFromFile(const std::string& filename, NameCallback callback) {
    std::ifstream ifs(filename);
    if (!ifs) return;

    std::string first, last;
    int index = 0;
    while (ifs >> first >> last) {
        callback(first, last, index++);
    }
}

// Static global containers for callback
static std::list<TEmployee*>* gMasterList = nullptr;
static std::vector<TEmployee*>* gMasterArray = nullptr;

// Static callback function (no captures!)
static void OnNameReadStatic(const std::string& first,
                             const std::string& last,
                             int idx) {
    TEmployee* e = new TEmployee(first, last, RandomDepartmentFromIndex(idx));
    gMasterList->push_back(e);      // owning container
    gMasterArray->push_back(e);     // non-owning vector
}

// Load data (from file or sample data)
static void LoadData(std::list<TEmployee*>& masterList,
                     std::vector<TEmployee*>& masterArray) {
    gMasterList = &masterList;
    gMasterArray = &masterArray;

    readNamesFromFile("names.txt", OnNameReadStatic);

    if (masterList.empty()) {
        std::vector<std::pair<std::string,std::string>> sample = {
            {"Alice","Smith"},{"Bob","Brown"},{"Charlie","Johnson"},{"Diana","Evans"},
            {"Eve","Miller"},{"Frank","Wilson"},{"Grace","Taylor"},{"Hank","Anderson"},
            {"Ivy","Thomas"},{"Jack","Moore"},{"Kate","Martin"},{"Leo","Jackson"},
            {"Mona","White"},{"Ned","Harris"},{"Olive","Clark"},{"Paul","Lewis"},
            {"Quinn","Robinson"},{"Rita","Walker"},{"Sam","Young"},{"Tina","King"}
        };
        for (size_t i = 0; i < sample.size(); ++i) {
            OnNameReadStatic(sample[i].first, sample[i].second, static_cast<int>(i));
        }
    }
}

// QuickSort template
template<typename Compare>
static int Partition(std::vector<TEmployee*>& arr, int low, int high, Compare comp) {
    TEmployee* pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (comp(arr[j], pivot)) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

template<typename Compare>
static void QuickSortArray(std::vector<TEmployee*>& arr, int low, int high, Compare comp) {
    if (low < high) {
        int pi = Partition(arr, low, high, comp);
        QuickSortArray(arr, low, pi - 1, comp);
        QuickSortArray(arr, pi + 1, high, comp);
    }
}

// Binary Search using lower_bound
static TEmployee* BinarySearchEmployee(const std::vector<TEmployee*>& arr,
                                       const std::string& lastname,
                                       const std::string& firstname) {
    TEmployee target(firstname, lastname, EDepartment::Finance); // department ignored
    auto it = std::lower_bound(arr.begin(), arr.end(), &target,
        [](TEmployee* a, TEmployee* b) {
            if (a->lastName != b->lastName) return a->lastName < b->lastName;
            return a->firstName < b->firstName;
        });
    if (it != arr.end() && (*it)->lastName == lastname && (*it)->firstName == firstname)
        return *it;
    return nullptr;
}

// Main application
int RunApp() {
    std::list<TEmployee*> masterList;      // owns the memory
    std::vector<TEmployee*> masterArray;   // non-owning pointers

    // 1. Load employees
    LoadData(masterList, masterArray);
    std::cout << "Loaded " << masterArray.size() << " employees.\n";

    // 2. Merge Sort: alphabetical
    std::cout << "\n=== Merge Sort: alphabetical (last, first) ===\n";
    masterList.sort([](TEmployee* a, TEmployee* b){ return *a < *b; });
    for (auto e : masterList) {
        std::cout << DeptToString(e->department) << " | "
                  << e->lastName << ", " << e->firstName << "\n";
    }

    // 3. Quick Sort: multi-key
    std::cout << "\n=== Quick Sort: department + name ===\n";
    auto comp = [](TEmployee* a, TEmployee* b){
        std::string da = DeptToString(a->department);
        std::string db = DeptToString(b->department);
        if (da != db) return da < db;
        if (a->lastName != b->lastName) return a->lastName < b->lastName;
        return a->firstName < b->firstName;
    };
    QuickSortArray(masterArray, 0, static_cast<int>(masterArray.size())-1, comp);
    for (auto e : masterArray) {
        std::cout << DeptToString(e->department) << " | "
                  << e->lastName << ", " << e->firstName << "\n";
    }

    // 4. Binary Search
    std::vector<TEmployee*> alphaArray(masterList.begin(), masterList.end());

    std::cout << "\n=== Binary Search ===\n";
    std::cout << "Enter <LastName FirstName> (or press Enter to skip): ";
    std::string last, first;
    if (!(std::cin >> last >> first))
        std::cout << "Skipping search.\n";
    else {
        TEmployee* found = BinarySearchEmployee(alphaArray, last, first);
        if (found)
            std::cout << "Found: " << DeptToString(found->department)
                      << " | " << found->lastName << ", " << found->firstName << "\n";
        else
            std::cout << "Employee not found.\n";
    }

    // 5. Cleanup memory
    for (auto ptr : masterList) { delete ptr; ptr = nullptr; }
    masterList.clear();
    masterArray.clear();
    alphaArray.clear();

    std::cout << "\nCleanup done.\n";
    return 0;
}
