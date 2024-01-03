#include "util.h"

MenuItem::MenuItem(std::string content, bool selectable, bool isSelected)
    : _isSelectable(selectable), _content(content), _isSelected(isSelected) {}
MenuItem::MenuItem(std::string content)
    : _isSelectable(false), _content(content), _isSelected(false) {}
bool MenuItem::IsSelected() { return _isSelected; }
void MenuItem::Select() { _isSelected = true; }
void MenuItem::Unselect() { _isSelected = false; }
bool MenuItem::IsSelectable() { return _isSelectable; };
std::string MenuItem::ToString() { return _content; }