
#include <blocks/page.h>
/**
 * Base class definitions
*/

using namespace SimpleDoc;
using namespace std;

Block::Block(string name){
    this->name = name;
}

Block::Block(rapidxml::xml_node<>* block_node){
    this->name = block_node->name();
    children = vector<Block*>();
    
    if (block_node->first_attribute("border")) {
        this->border = block_node->first_attribute("border")->value();
    }else {
        this->border = "0 0 0 0";
    }
    
    if (block_node->first_attribute("padding")) {
        this->padding = block_node->first_attribute("padding")->value();
    } else {
        this->padding = "0 0 0 0";
    }
    
    if (block_node->first_attribute("margin")) {
        this->margin = block_node->first_attribute("margin")->value();
    } else {
        this->margin = "0 0 0 0";
    }
    

    if (block_node->first_attribute("width")) {
        this->width = block_node->first_attribute("width")->value();
    } else {
        this->width = "100%";
    }
    
    if (block_node->first_attribute("height")) {
        this->height = block_node->first_attribute("height")->value();
    } else {
        this->height = "auto";
    }

    
}

/**
 * Allowed units => mm - calculated based on dpi 
 *               => pt - absolute point
 *               => % - percentage of parent with respect to direction
*/
int Block::parse_unit(string value, LayoutReferenceToolbox* toolbox, RelativeDirection direction){
    if (value == "auto") {
        return MEASEUERMENT_UNIT_AUTO;
    }
    if (value.find("mm") != string::npos) {
        return toolbox->dpi * stof(value.substr(0, value.length() - 2)) / 25.4;
    } else if (value.find("pt") != string::npos) {
        return stof(value.substr(0, value.length() - 2));
    } else if (value.find("%") != string::npos) {
        if (direction == HORIZONTAL) {
            return toolbox->parentWidth * stof(value.substr(0, value.length() - 1)) / 100;
        } else {
            return toolbox->parentHeight * stof(value.substr(0, value.length() - 1)) / 100;
        }
    } else {
        return stof(value);
    }
}


void Block::fill_children(rapidxml::xml_node<>* block_node){
    for (rapidxml::xml_node<>* child_node = block_node->first_node(); child_node; child_node = child_node->next_sibling()) {
        string name = child_node->name();
        Block* block = nullptr;
        if (name == "row") {
            block = new Row(child_node);
        } else if (name == "column") {
            block = new Column(child_node);
        } else if (name == "page") {
            block = new Page(child_node);
        } else if (name == "text") {
            block = new Text(child_node);
        } else {
            cout << "Unknown block type" << endl;
        }

        if (block != nullptr) {
            block->parent = this;
            children.push_back(block);
        }
    }
}

Block* Block::get_parent(){
    return parent;
}

vector<string> split_string(string str, char delimiter){
    vector<string> result;
    string current = "";
    for (int i = 0; i < str.length(); i++){
        if (str[i] == delimiter){
            result.push_back(current);
            current = "";
        } else {
            current += str[i];
        }
    }
    result.push_back(current);
    return result;
}


void Block::CalculateRect(LayoutReferenceToolbox toolbox){
    // initial rect calculation - calculate basic inner info
    // parse Padding
    vector<string> padding_values_str = split_string(padding, ' ');
    padding_values.top = parse_unit(padding_values_str[0], &toolbox, VERTICAL);
    padding_values.right = parse_unit(padding_values_str[1], &toolbox, HORIZONTAL);
    padding_values.bottom = parse_unit(padding_values_str[2], &toolbox, VERTICAL);
    padding_values.left = parse_unit(padding_values_str[3], &toolbox, HORIZONTAL);


    // parse border
    vector<string> border_values_str = split_string(border, ' ');
    border_values.top = parse_unit(border_values_str[0], &toolbox, VERTICAL);
    border_values.right = parse_unit(border_values_str[1], &toolbox, HORIZONTAL);
    border_values.bottom = parse_unit(border_values_str[2], &toolbox, VERTICAL);
    border_values.left = parse_unit(border_values_str[3], &toolbox, HORIZONTAL);
    
    /*
    // set basic rect info
    layoutRect.x = padding_values.left;
    layoutRect.y = padding_values.top;
    layoutRect.width = parse_unit(width, &toolbox, HORIZONTAL) - padding_values.left - padding_values.right;
    layoutRect.height = parse_unit(height, &toolbox, VERTICAL) - padding_values.top - padding_values.bottom;

    // set inner rect info
    layoutRect.innerWidth = layoutRect.width;
    layoutRect.innerHeight = layoutRect.height;

    // set max rect info
    layoutRect.maxWidth = toolbox.parentWidth; // default to parent width
    layoutRect.maxHeight = toolbox.parentHeight; // default to parent height


    // new toolbox for children
    LayoutReferenceToolbox childToolbox = {toolbox.dpi, layoutRect.width, layoutRect.height, layoutRect.width, layoutRect.height};

    // render children
    int child_max_width = 0;
    int child_max_height = 0;

    for (int i = 0; i < children.size(); i++){
        children[i]->CalculateRect(childToolbox);
        if (children[i]->layoutRect.width > child_max_width){
            child_max_width = children[i]->layoutRect.width;
        }
        if (children[i]->layoutRect.height > child_max_height){
            child_max_height = children[i]->layoutRect.height;
        }
    }

    // set max rect info
    // modify the max width and height based on children
    if (layoutRect.width == MEASEUERMENT_UNIT_AUTO){
        layoutRect.width = child_max_width + padding_values.left + padding_values.right;
    }
    if (layoutRect.height == MEASEUERMENT_UNIT_AUTO){
        layoutRect.height = child_max_height + padding_values.top + padding_values.bottom;
    }*/
}

void Block::CalculateRectPosition(LayoutReferenceToolbox toolbox){
    if (parent != nullptr){
        layoutRect.x += parent->layoutRect.x + parent->padding_values.left;
        layoutRect.y += parent->layoutRect.y + parent->padding_values.top;
    }

    cout << name << " is x:" << layoutRect.x << " y:" << layoutRect.y << " w:" << layoutRect.width << " h:" << layoutRect.height << endl;
    for (int i = 0; i < children.size(); i++){
        children[i]->CalculateRectPosition(toolbox);
    }

}

void Block::RenderBlock(HDC hdc){
    // calculate my position using parent if 0
    if (layoutRect.x == 0 && layoutRect.y == 0){
        if (parent != nullptr){
            layoutRect.x = parent->layoutRect.x;
            layoutRect.y = parent->layoutRect.y;
        }
    }

    for (int i = 0; i < children.size(); i++){
        children[i]->RenderBlock(hdc);
    }

    // draw a box
    RECT rect = {layoutRect.x, layoutRect.y, layoutRect.x + layoutRect.width, layoutRect.y + layoutRect.height};
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
    // stroke
    HPEN pen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    //Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    LineTo(hdc, rect.right, rect.bottom);
    LineTo(hdc, rect.left, rect.bottom);
    LineTo(hdc, rect.left, rect.top);
    LineTo(hdc, rect.right, rect.top);
    // draw border lines 


}