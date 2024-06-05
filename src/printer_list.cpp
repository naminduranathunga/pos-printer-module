#include <main.h>
#include <vector>

using namespace std;


vector<string> get_printer_list(){
    vector<string> printers;
    printers.push_back("Printer 1");
    //PRINTER_INFO_1* printer_info;
    // allocate memory for printer_info
    //printer_info = (PRINTER_INFO_1*)malloc(sizeof(PRINTER_INFO_1)*10);

    DWORD printer_info_size = sizeof(PRINTER_INFO_1)*10; // Allocate memory for printer_info_size
    DWORD printer_count = 0; // Initialize printer_count

    bool results = EnumPrinters(
        PRINTER_ENUM_LOCAL,
        NULL,
        1L,
        (LPBYTE)NULL,
        0L,
        &printer_info_size,
        &printer_count);

    if (printer_info_size > 0){
        PRINTER_INFO_1* printer_info = (PRINTER_INFO_1*)malloc(printer_info_size);
        results = EnumPrinters(
            PRINTER_ENUM_LOCAL,
            NULL,
            1L,
            (LPBYTE)printer_info,
            printer_info_size,
            &printer_info_size,
            &printer_count);
        for (int i = 0; i < printer_count; i++){
            printers.push_back(printer_info[i].pName);
        }
        free(printer_info);
    }
    return printers;
}

void show_printer_list(){
    vector<string> printers = get_printer_list();
    for (int i = 0; i < printers.size(); i++){
        cout << printers[i] << endl;
    }
}