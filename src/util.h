#ifndef UTIL_H
#define UTIL_H

#include <string>
enum class KeyStroke {
    KEY_UP = 0,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ENTER,
    KEY_ESC,
    KEY_EXT
};

class MenuItem {
   public:
    MenuItem(std::string content, bool selectable, bool isSelected = false);
    MenuItem(std::string content);
    bool IsSelectable();
    bool IsSelected();
    void Select();
    void Unselect();
    std::string ToString();

   private:
    bool _isSelectable;
    bool _isSelected;
    std::string _content;
};

#endif /*UTIL_H*/