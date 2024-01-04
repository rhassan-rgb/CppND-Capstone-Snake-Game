#include "util.h"

TextScreenItem::TextScreenItem(Coordinates cord, std::string content,
                               bool selectable)
    : ScreenItem(cord, ScreenItemType::ITEM_TEXT),
      _isSelectable(selectable),
      _content(content) {
    _colors = UNSELECTED_COLOR;
}
TextScreenItem::TextScreenItem(Coordinates cord, std::string content)
    : TextScreenItem(cord, content, false) {}
bool TextScreenItem::IsSelectable() const { return _isSelectable; };
std::string TextScreenItem::ToString() const { return _content; }

/*Screen Items Implementation*/
ScreenItem::ScreenItem(Coordinates cord)
    : ScreenItem(cord, ScreenItemType::ITEM_BLOCK) {}
ScreenItem::ScreenItem(Coordinates cord, ScreenItemType type)
    : _coordinates(cord), _itemType(type) {}
ScreenItemType ScreenItem::GetType() const { return _itemType; }
Colors ScreenItem::GetColor() const { return _colors; }
Coordinates ScreenItem::GetCoordinates() const { return _coordinates; }
void ScreenItem::SetColors(Colors clrs) {
    _colors.a = clrs.a;
    _colors.r = clrs.r;
    _colors.g = clrs.g;
    _colors.b = clrs.b;
}
void ScreenItem::SetCoordinates(Coordinates coords) {
    _coordinates.x = coords.x;
    _coordinates.y = coords.y;
}