#include <main.h>
#include <fstream>

#include <print_document.h>
#include <argparse/argparse.hpp>

using namespace std;

void loadxml_content(string filename, string& content){
    ifstream file(filename, ios::in);
    if (file.is_open()){
        string line;
        while (getline(file, line)){
            content += line;
        }
        file.close();
    }
}


int main(int argc, char* argv[]){
    // parsing arguments
    argparse::ArgumentParser program("PosPrinterModule");

    program.add_argument("command")
        .help("Command to execute")
        .action([](const string& value) { return value; });
    program.add_argument("-p", "--printer")
        .help("Printer name")
        .default_value(string(""));
    program.add_argument("-f", "--file")
        .help("File to print. Default is standard input")
        .default_value(string("stdin"));
    program.add_argument("-r", "--resolution")
        .help("Resolution")
        .default_value(300)
        .action([](const string& value) { return stoi(value); });

    try{
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err){
        cout << err.what() << endl;
        cout << program;
        return 0;
    }

    string command = program.get<string>("command");
    if (command == "list"){
        show_printer_list();
    } else if (command == "print"){
        string printer_name = program.get<string>("--printer");
        string file_name = program.get<string>("--file");
        int resolution = program.get<int>("--resolution");
        string content;
        if (file_name == "stdin"){
            string line;
            while (getline(cin, line)){
                content += line;
            }
        } else {
            loadxml_content(file_name, content);
        }
        PrintDocument* doc = new PrintDocument(content, 300, 380, 0);
        doc->printer_name = printer_name;
        doc->dpi = resolution;
        doc->print();
    } else {
        cout << program;
    }
    return 0;
    if (argc == 2 && string(argv[1]) == "list"){
        show_printer_list();
    }
    //show_printer_list();
    string content;
    loadxml_content("../sample.xml", content);
    PrintDocument* doc = new PrintDocument(content, 300, 380, 0);
    doc->print();

    std::string printer_name = "Canon G1010 series";
    return 0;
}


