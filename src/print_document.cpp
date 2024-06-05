#include <print_document.h>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <fstream>

using namespace std;
using namespace rapidxml;

PrintDocument::PrintDocument(string xml, int dpi, int width_px, int height_px){
    this->width_px = width_px;
    this->height_px = height_px;
    this->dpi = dpi;
    this->parse_document(xml);
}


void PrintDocument::parse_document(string xml){
    xml_document<> doc;
    doc.parse<0>(&xml[0]);
    xml_node<>* root = doc.first_node();
    
    // extract printer settings
    if (root->first_attribute("printer") != NULL){
        this->printer_name = root->first_attribute("printer")->value();
    } else {
        this->printer_name = "Canon G1010 series";
    }

    // dpi
    if (root->first_attribute("dpi") != NULL){
        this->dpi = atoi(root->first_attribute("dpi")->value());
    } else {
        this->dpi = 600;
    }
    root->first_attribute();
    xml_node<>* page = root->first_node("page");
    while (page != NULL){
        SimpleDoc::Page* new_page = new SimpleDoc::Page(page);
        SimpleDoc::LayoutReferenceToolbox toolbox;
        toolbox.pageWidth = this->width_px;
        toolbox.pageHeight = this->height_px;
        toolbox.dpi = this->dpi;
        
        new_page->CalculateRect(toolbox);
        new_page->CalculateRectPosition(toolbox);

        //new_page->renderLayout(null); // dpi is 201

        this->pages.push_back(new_page);
        page = page->next_sibling("page");
    } 
    
}

void PrintDocument::print(){
    // let's print hello world
    //  ignore the hdc and let's create new printerDC
    HDC printerDC = CreateDC("WINSPOOL", this->printer_name.c_str(), NULL, NULL);
    if (printerDC == NULL){
        cout << "Failed to create printerDC" << endl;
        return;
    }

    // Set the printerDC to the printer
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = 0 + this->width_px;
    if (this->height_px == 0){
        rect.bottom = 100;
    }else{
        rect.bottom = 0 + this->height_px;
    }

    // Start printing
    DOCINFO docInfo;
    docInfo.cbSize = sizeof(DOCINFO);
    docInfo.lpszDocName = "Hello World";
    docInfo.lpszOutput = NULL;
    docInfo.lpszDatatype = NULL;
    docInfo.fwType = 0;

    int result = StartDoc(printerDC, &docInfo);
    if (result < 0){
        cout << "Failed to start document" << endl;
        return;
    }

    for (int i = 0; i < this->pages.size(); i++){
        SimpleDoc::Page* page = this->pages[i];
        // Start printing
        result = StartPage(printerDC);
        if (result < 0){
            cout << "Failed to start page" << endl;
            return;
        }

        page->RenderBlock(printerDC);

        // End printing
        result = EndPage(printerDC);
        if (result < 0){
            cout << "Failed to end page" << endl;
            return;
        }
    }
    /*// Start printing
    result = StartPage(printerDC);
    if (result < 0){
        cout << "Failed to start page" << endl;
        return;
    }

    // Print the text
    result = DrawText(printerDC, "Hello World2\nThis is a test for eclipses and owerflow. To see how it will work.", -1, &rect, DT_CENTER);
    if (result < 0){
        cout << "Failed to draw text" << endl;
        return;
    }

    // End printing
    result = EndPage(printerDC);
    if (result < 0){
        cout << "Failed to end page" << endl;
        return;
    }*/

    result = EndDoc(printerDC);
    if (result < 0){
        cout << "Failed to end document" << endl;
        return;
    }
}