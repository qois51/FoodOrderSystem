#ifndef ORDER_ACTION_H
#define ORDER_ACTION_H

#include <string>
#include <unordered_map>

enum class ActionType {
    ADD_ITEM,
    REMOVE_ITEM,
    CLEAR_ORDER
};

struct OrderAction {
    ActionType action;
    std::string itemName;
    int quantity;
    std::unordered_map<std::string, int> previousOrderState;
    
    OrderAction(ActionType act, const std::string& item = "", int qty = 0, 
                const std::unordered_map<std::string, int>& prevState = {})
        : action(act), itemName(item), quantity(qty), previousOrderState(prevState) {}
    
    // For display purposes
    std::string toString() const {
        switch(action) {
            case ActionType::ADD_ITEM:
                return "Added " + std::to_string(quantity) + "x " + itemName;
            case ActionType::REMOVE_ITEM:
                return "Removed " + std::to_string(quantity) + "x " + itemName;
            case ActionType::CLEAR_ORDER:
                return "Cleared entire order";
            default:
                return "Unknown action";
        }
    }
};

// For std::cout support
inline std::ostream& operator<<(std::ostream& os, const OrderAction& action) {
    return os << action.toString();
}

#endif // ORDER_ACTION_H