#include <blocks/page.h>
#include <string>
#include <iostream>

using namespace std;
using namespace SimpleDoc;
using namespace rapidxml;



Column::Column(xml_node<>* page_node): Block(page_node){
    gap = "0";
    if (page_node->first_attribute("gap") != 0){
        gap = page_node->first_attribute("gap")->value();
    }

    if (page_node->first_attribute("halign") != 0){
        justifyContent = page_node->first_attribute("halign")->value();
    }else{
       justifyContent = "left";
    }
    
    fill_children(page_node);
}


/**
 * Calculate the rect for the column
 */
void Column::CalculateRect(LayoutReferenceToolbox toolbox){
    Block::CalculateRect(toolbox);
    
    int max_width = get_parent()->layoutRect.width;
    int max_height = MEASEUERMENT_UNIT_AUTO;

    int gap_x = parse_unit(gap, &toolbox, RelativeDirection::HORIZONTAL);
    int gap_y = parse_unit(gap, &toolbox, RelativeDirection::VERTICAL);

    if (width == "auto"){
        layoutRect.width = max_width;
        // max width for childrn is calculated based on number of children
        int total_gap = gap_x * (children.size() - 1);
    } else {
        layoutRect.width = parse_unit(width, &toolbox, RelativeDirection::HORIZONTAL);
    }

    if (height == "auto"){
        layoutRect.height = max_height;
    } else {
        layoutRect.height = parse_unit(height, &toolbox, RelativeDirection::VERTICAL);
    }

    // set inner width and height
    int inner_width = layoutRect.width - padding_values.left - padding_values.right;
    int inner_height = layoutRect.height - padding_values.top - padding_values.bottom;

    LayoutReferenceToolbox child_toolbox;
    child_toolbox.dpi = toolbox.dpi;
    child_toolbox.pageWidth = toolbox.pageWidth;
    child_toolbox.pageHeight = toolbox.pageHeight;
    child_toolbox.parentWidth = inner_width;
    child_toolbox.parentHeight = inner_height;

    int cursor_y = 0;
    for (int i = 0; i < children.size(); i++){
        Block* child = children[i];
        child->CalculateRect(child_toolbox);
        
        child->layoutRect.y = cursor_y + layoutRect.y + padding_values.top;
        cursor_y = cursor_y + child->layoutRect.height + gap_x;

        if (justifyContent == "center"){
            child->layoutRect.x = (layoutRect.width - child->layoutRect.width) / 2;
        } else if (justifyContent == "end" || justifyContent == "right"){
            child->layoutRect.x = layoutRect.width - child->layoutRect.width;
        }else {
            child->layoutRect.x = 0;
        }
    }

    // set the height of the column based on children
    int max_child_height = 0;
    for (int i = 0; i < children.size(); i++){
        Block* child = children[i];
        max_child_height += child->layoutRect.height;
    }
    layoutRect.height = max_child_height + padding_values.top + padding_values.bottom;
}
