#pragma once
#include <furi.h>
#include <input/input.h>
#include <gui/elements.h>
#include "card.h"

#define ANIMATION_TIME furi_ms_to_ticks(1500)
#define ANIMATION_END_MARGIN furi_ms_to_ticks(200)
#define BLIND 10

typedef enum {
    EventTypeTick,
    EventTypeKey,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} AppEvent;

typedef enum {
    GameStateGameOver,
    GameStateStart,
    GameStatePlay
} PlayState;

typedef enum {
    DirectionUp,
    DirectionRight,
    DirectionDown,
    DirectionLeft,
    Select,
    None
} Direction;

typedef struct {
    unsigned int last_tick;
    unsigned int animationStart;
    bool animating;
    Deck deck;
    Card player_card[2];
    Card opponent_card[3][2];
    uint8_t player_card_count;
    PlayState state;

    Direction selectDirection;

    uint32_t stake;
} GameState;