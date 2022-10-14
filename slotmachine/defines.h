#pragma once

typedef enum {
    GameStateGameOver,
    GameStateStart,
    GameStatePlay
} PlayState;

typedef struct {
    PlayState state;
    uint32_t money;
    uint8_t bet;
} GameState;

typedef enum {
    EventTypeTick,
    EventTypeKey,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} AppEvent;

typedef enum {
    DirectionUp,
    DirectionRight,
    DirectionDown,
    DirectionLeft,
    Select,
    None
} Direction;