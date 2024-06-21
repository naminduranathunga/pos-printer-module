#include <blocks/page.h>


using namespace std;
using namespace SimpleDoc;
using namespace rapidxml;

Text::Text(xml_node<>* text_node): Block(text_node){
    if (string(text_node->name()) != "text"){
        throw runtime_error("Invalid text node");
    }
    text = text_node->value();
    if (text_node->first_attribute("fname") != 0){
        fontName = text_node->first_attribute("fname")->value();
    } else {
        fontName = "Arial";
    }

    if (text_node->first_attribute("fsize") != 0){
        fontSize = text_node->first_attribute("fsize")->value();
    } else {
        fontSize = "2mm";
    }

    if (text_node->first_attribute("color") != 0){
        color = text_node->first_attribute("color")->value();
    } else {
        color = "black";
    }

    // falign
    if (text_node->first_attribute("falign") != 0){
        string align_s = text_node->first_attribute("falign")->value();
        // validate align - start, center, end or left, center, right
        if (align_s == "start" || align_s == "left"){
            align = DT_LEFT;
        } else if (align_s == "center"){
            align = DT_CENTER;
        } else if (align_s == "end" || align_s == "right"){
            align = DT_RIGHT;
        } else {
            throw runtime_error("Invalid align value");
        }

    } else {
        align = DT_LEFT;
    }

    // font weight 100, 200, 300, 400, 500, 600, 700, 800, 900
    if (text_node->first_attribute("fweight") != 0){
        string weight_S = text_node->first_attribute("fweight")->value();
        const string valid_weights[] = {"100", "200", "300", "400", "500", "600", "700", "800", "900"};
        bool valid = false;
        for (int i = 0; i < 9; i++){
            if (weight_S == valid_weights[i]){
                valid = true;
                break;
            }
        }
        if (!valid){
            throw runtime_error("Invalid font weight");
        }
        fontWeight = stoi(weight_S);
    } else {
        fontWeight = 400;
    }

    // italic
    if (text_node->first_attribute("italic") != 0){
        string italic_S = text_node->first_attribute("italic")->value();
        if (italic_S == "true"){
            italic = true;
        } else if (italic_S == "false"){
            italic = false;
        } else {
            throw runtime_error("Invalid italic value");
        }
    } else {
        italic = false;
    }

    // underline
    if (text_node->first_attribute("underline") != 0){
        string underline_S = text_node->first_attribute("underline")->value();
        if (underline_S == "true"){
            underline = true;
        } else if (underline_S == "false"){
            underline = false;
        } else {
            throw runtime_error("Invalid underline value");
        }
    } else {
        underline = false;
    }
    
}

/**
 * To make simple, let's reduce support of padding and margin
*/
void Text::CalculateRect(LayoutReferenceToolbox toolbox){
    Block::CalculateRect(toolbox);
    font_height = parse_unit(fontSize, &toolbox);
    int font_width = font_height * 0.5;
    int text_length = text.length();

    int max_width = parse_unit(width, &toolbox);
    if (width == "auto"){
        // set for width of parent
        max_width = toolbox.parentWidth;
    }
    if (max_width <= 0){
        max_width = toolbox.pageWidth;
    }
    int max_chars = max_width / font_width;
    int lines = 1; //text_length / max_chars;
    /*if (text_length % max_chars != 0){
        lines++;
    }*/
    layoutRect.height = lines * font_height;
    layoutRect.width = max_width;
}

void Text::RenderBlock(HDC hdc){
    RECT rect;
    rect.left = layoutRect.x;
    rect.top = layoutRect.y;
    rect.right = layoutRect.x + layoutRect.width;
    rect.bottom = layoutRect.y + layoutRect.height;
    int fontSizeInt = font_height;
    HFONT hFont = CreateFont(
        fontSizeInt, 
        0, 0, 0, 
        fontWeight, 
        italic, underline, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, fontName.c_str());
    // color
    SetTextColor(hdc, RGB(0, 0, 0));
    SelectObject(hdc, hFont);
    DrawText(hdc, text.c_str(), text.length(), &rect, align);
    DeleteObject(hFont);
}