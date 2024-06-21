#include <blocks/page.h>

using namespace std;
using namespace SimpleDoc;
using namespace rapidxml;

Row::Row(xml_node<>* page_node): Block(page_node){
    gap = "0";
    if (page_node->first_attribute("gap") != 0){
        gap = page_node->first_attribute("gap")->value();
    }
    
    fill_children(page_node);
}


/**
 * Calculate the rect for the row
*/
void Row::CalculateRect(LayoutReferenceToolbox toolbox){
    Block::CalculateRect(toolbox);
    
    int max_width = get_parent()->layoutRect.width;
    int max_height = MEASEUERMENT_UNIT_AUTO;

    int gap_x = parse_unit(gap, &toolbox, RelativeDirection::HORIZONTAL);
    int gap_y = parse_unit(gap, &toolbox, RelativeDirection::VERTICAL);

    if (width == "auto"){
        layoutRect.width = max_width;
        int len_childern = children.size();
        layoutRect.width -= (len_childern - 1) * gap_x;
        layoutRect.width /= (len_childern);


        //
    } else {
        layoutRect.width = parse_unit(width, &toolbox, RelativeDirection::HORIZONTAL);
    }

    if (height == "auto"){
        layoutRect.height = max_height;
    } else {
        layoutRect.height = parse_unit(height, &toolbox, RelativeDirection::VERTICAL);
    }

    LayoutReferenceToolbox child_toolbox = toolbox;
    child_toolbox.parentWidth = layoutRect.width;
    child_toolbox.parentHeight = layoutRect.height;

    
    int cursor_x = 0;
    for (int i = 0; i < children.size(); i++){
        Block* child = children[i];
        child->CalculateRect(child_toolbox);
        
        // now set it's position y = cursor_y
        child->layoutRect.x = cursor_x;
        cursor_x = cursor_x + child->layoutRect.width + gap_x;

        // set vertical position
        child->layoutRect.y = layoutRect.y + padding_values.top;
    }

    // get the max height of children
    int max_child_height = 0;
    for (int i = 0; i < children.size(); i++){
        Block* child = children[i];
        if (child->layoutRect.height > max_child_height){
            max_child_height = child->layoutRect.height;
        }
    }

    layoutRect.height = max_child_height + padding_values.top + padding_values.bottom;
}