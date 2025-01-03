# Inventory Management:

```cpp
struct Item {
  char* id;
  int amount;

  // Use Localization
  char* name() { return "ItemName"; } 

  // Remove
  void decreaseAmount(int amountToDecrease) { amount -= amountToDecrease }
}

struct WorldItem {
    Item item;
    Position position;

    bool withinPickupRange(Position pos) {
        if(distance(position, pos) < 1) { 
            return true;
        }
        return false;
    }
}

struct Inventory {
  int gold;
  std::unordered_map<int, Item> items;

  int count() { return items.size() }
  bool hasItem(char* itemId, int amount) {

  }
  Item getSlot(int slotId) {
    return items[slotId];
  }
  void clearSlot(int slotId) {
    items[slotId] = NULL;
  }
  void clearAll() {}
  bool removeItem(char* itemId, int amount) {
    if(!hasItem(itemId, amount)) {
      return false;
    }
    // remove items
    return true;
  }
}
```

# Player

# Townie

# Block

