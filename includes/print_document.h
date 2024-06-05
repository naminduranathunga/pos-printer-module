

#ifndef PRINT_DOCUMENT_H
#define PRINT_DOCUMENT_H 1
#include <string>
#include <iostream>
#include <windows.h>
#include <vector>
#include <blocks/page.h>

using namespace std;

class PrintDocument {
    private: 
        int width_px;
        int height_px;
        vector<SimpleDoc::Page*> pages;

        void parse_document(string xml);

    public:
        PrintDocument(string xml, int dpi, int width_px, int height_px);
        void print();

        string printer_name;
        int dpi;

};

#endif