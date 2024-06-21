#include <blocks/page.h>
#include <base64.hpp>
#include <fstream>
#include <Shlwapi.h>

using namespace SimpleDoc;
using namespace Gdiplus;

// Initialize GDI+
void InitGDIPlus(ULONG_PTR &gdiplusToken) {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

// Shutdown GDI+
void ShutdownGDIPlus(ULONG_PTR gdiplusToken) {
    GdiplusShutdown(gdiplusToken);
}

string create_random_file_name(string extension){
    int length = 10;
    string temp_file = "temp_image";
    for (int i = 0; i < length; i++){
        temp_file += (char)('a' + rand() % 26);
    }
    return temp_file + "." + extension;
}

string SimpleDoc::Image::parse_base64Images(string src){
    if (src.find("data:image") != 0){
        return src;
    }

    //data:image/png;base64,
    // get file type - png, jpeg, etc 
    size_t start = src.find("data:image/");
    size_t end = src.find(";", start);
    string file_type = src.substr(start + 11, end - start - 11);
    if (file_type == "jpeg"){
        file_type = "jpg";
    } else if (file_type != "png" && file_type != "jpg" && file_type != "bmp"){
        throw std::runtime_error("Image type not supported");
    }

    // find the start of the base64 string
    start = src.find("base64,") + 7;

    // get the base64 string
    string base64 = src.substr(start);
    // decode the base64 string
    string decoded = base64::from_base64(base64);
    TCHAR temp_path[MAX_PATH];
    GetTempPath(MAX_PATH, temp_path);
    
    // create a tempory file to store the image
    string temp_file = create_random_file_name(file_type);
    temp_file = temp_path + temp_file;

    //return decoded;
    // create a temporary file to store the image
    ofstream file(temp_file, ios::out | ios::binary);
    file.write(decoded.c_str(), decoded.size());
    file.close();
    temp_file_name = temp_file; // this file will be deleted when the object is destroyed
    return temp_file;

}

// Function to convert std::string to std::wstring
std::wstring StringToWString(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}

SimpleDoc::Image::Image(rapidxml::xml_node<>* image_node): Block(image_node){
    if (image_node->first_attribute("src") == NULL){
        throw std::runtime_error("Image node must have src attribute");
    }
    src = image_node->first_attribute("src")->value();
    src = parse_base64Images(src);

    if (image_node->first_attribute("width")){
        width = image_node->first_attribute("width")->value();
    } else {
        width = "100%";
    }
    
    if (image_node->first_attribute("height")){
        height = image_node->first_attribute("height")->value();
    } else {
        height = "auto";
    }

    // load the image file
    gdiplusToken = 0;
    InitGDIPlus(gdiplusToken);

    std::wstring wsrc = StringToWString(src);
    bitmap = new Gdiplus::Bitmap(wsrc.c_str(), TRUE);
    Gdiplus::Status status = bitmap->GetLastStatus();
    if (status != Gdiplus::Status::Ok) {
        cout << src <<" " << status <<" " << Gdiplus::Status::Ok << endl;
        delete bitmap;
        throw std::runtime_error("Image file not found");
    }
    
    image_width = bitmap->GetWidth();
    image_height = bitmap->GetHeight();
    aspect_ratio = (float)image_width / (float)image_height;

    temp_file_name = "";
    
}

SimpleDoc::Image::~Image(){
    //DeleteObject(hBitmap);
    if (temp_file_name != ""){
        DeleteFile(temp_file_name.c_str());
        cout << "Deleted " << temp_file_name << endl;
    }
    delete bitmap;
    ShutdownGDIPlus(gdiplusToken);
}

void SimpleDoc::Image::CalculateRect(LayoutReferenceToolbox toolbox){
    Block::CalculateRect(toolbox);
    int max_width = toolbox.parentWidth;
    int max_height = toolbox.parentHeight;

    if (width == "auto"){
        layoutRect.width = max_width;
    } else {
        layoutRect.width = parse_unit(width, &toolbox, RelativeDirection::HORIZONTAL);
    }

    if (height == "auto"){
        // determine the aspect ratio
        layoutRect.height = (float)layoutRect.width / aspect_ratio;
    } else {
        layoutRect.height = parse_unit(height, &toolbox, RelativeDirection::VERTICAL);
        if (width == "auto"){
            layoutRect.width = layoutRect.height * aspect_ratio;
        }
    }
}

void SimpleDoc::Image::RenderBlock(HDC hdc){

    Gdiplus::Graphics graphics(hdc);
    graphics.SetPageUnit(Gdiplus::Unit::UnitPixel);

    cout << bitmap->GetHeight() << " " << bitmap->GetWidth() << endl;
    Rect destRect(layoutRect.x, layoutRect.y, layoutRect.width, layoutRect.height);
    graphics.DrawImage(bitmap, destRect);

    Block::RenderBlock(hdc);
}