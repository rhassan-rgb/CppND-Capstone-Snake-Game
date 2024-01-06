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
    KEY_EXT,
    KEY_NONE
};

enum class WelcomeItems {
    ITEM_WELCOME = 0,
    ITEM_NEW_GAME,
    ITEM_LEADER_BOARD,
    ITEM_EXIT,
    TOTAL_ITEMS
};

enum class GameItems {
    ITEM_GAME = 0,
    ITEM_GAME_OVER,
    ITEM_PAUSE_GAME,
    TOTAL_ITEMS
};

enum class LeaderBoardItems { ITEM_LEADERBOARD = 0, ITEM_EXIT, TOTAL_ITEMS };

enum class ScreenItemType { ITEM_BLOCK = 0, ITEM_TEXT };
struct Colors {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    bool operator==(const Colors &other) {
        return (r == other.r && g == other.g && b == other.b && a == other.a);
    }
};

constexpr Colors SELECTED_COLOR = {.r = 255, .g = 0, .b = 0, .a = 0};
constexpr Colors UNSELECTED_COLOR = {.r = 255, .g = 255, .b = 255, .a = 0};

struct Coordinates {
    int x;
    int y;
};

class ScreenItem {
   public:
    ScreenItem() = delete;
    ScreenItem(Coordinates cord);
    ScreenItem(Coordinates cord, ScreenItemType type);
    ScreenItemType GetType() const;
    Colors GetColor() const;
    Coordinates GetCoordinates() const;
    void SetColors(Colors clrs);
    void SetCoordinates(Coordinates coords);

   protected:
    ScreenItemType _itemType;
    Colors _colors;
    Coordinates _coordinates;
};

class TextScreenItem : public ScreenItem {
   public:
    TextScreenItem(Coordinates cord, std::string content, bool selectable);
    TextScreenItem(Coordinates cord, std::string content);
    bool IsSelectable() const;
    std::string ToString() const;
    void UpdateContent(std::string newContent);

   private:
    bool _isSelectable;
    std::string _content;
};

#endif /*UTIL_H*/