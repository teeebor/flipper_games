#include <gui/gui.h>
#include <stdlib.h>
#include <dolphin/dolphin.h>
#include <math.h>
#include "util.h"
#include "defines.h"
#include "card.h"
#include "util.h"

#define APP_NAME "Texas hold'em"
#define STARTING_MONEY 200
#define DEALER_MAX 17

static void render_callback(Canvas *const canvas, void *ctx) {

}

void tick(GameState *game_state) {

}

void start_round(GameState *game_state) {

}

void init(GameState *game_state) {

}

static void input_callback(InputEvent *input_event, FuriMessageQueue *event_queue) {
    furi_assert(event_queue);
    AppEvent event = {.type = EventTypeKey, .input = *input_event};
    furi_message_queue_put(event_queue, &event, FuriWaitForever);
}

static void update_timer_callback(FuriMessageQueue *event_queue) {
    furi_assert(event_queue);
    AppEvent event = {.type = EventTypeTick};
    furi_message_queue_put(event_queue, &event, 0);
}

int32_t blackjack_app(void *p) {
    UNUSED(p);

    int32_t return_code = 0;

    FuriMessageQueue *event_queue = furi_message_queue_alloc(8, sizeof(AppEvent));

    GameState *game_state = malloc(sizeof(GameState));
    init(game_state);
    game_state->state = GameStateStart;

    ValueMutex state_mutex;
    if (!init_mutex(&state_mutex, game_state, sizeof(GameState))) {
        FURI_LOG_E(APP_NAME, "cannot create mutex\r\n");
        return_code = 255;
        goto free_and_exit;
    }

    ViewPort *view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, render_callback, &state_mutex);
    view_port_input_callback_set(view_port, input_callback, event_queue);

    FuriTimer *timer =
            furi_timer_alloc(update_timer_callback, FuriTimerTypePeriodic, event_queue);
    furi_timer_start(timer, furi_kernel_get_tick_frequency() / 25);

    Gui *gui = furi_record_open("gui");
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    AppEvent event;

    for (bool processing = true; processing;) {
        FuriStatus event_status = furi_message_queue_get(event_queue, &event, 100);
        GameState *game_state = (GameState *) acquire_mutex_block(&state_mutex);

        if (event_status == FuriStatusOk) {
            if (event.type == EventTypeKey) {

                if (event.input.type == InputTypePress) {
                    switch (event.input.key) {

                        case InputKeyUp:
                            game_state->selectDirection = DirectionUp;
                            break;
                        case InputKeyDown:
                            game_state->selectDirection = DirectionDown;
                            break;
                        case InputKeyRight:
                            game_state->selectDirection = DirectionRight;
                            break;
                        case InputKeyLeft:
                            game_state->selectDirection = DirectionLeft;
                            break;
                        case InputKeyBack:
                            processing = false;
                            break;

                        case InputKeyOk:
                            if (game_state->state == GameStateGameOver || game_state->state == GameStateStart) {
                                init(game_state);
                            } else {
                                game_state->selectDirection = Select;
                            }
                            break;
                    }
                }
            } else if (event.type == EventTypeTick) {
                tick(game_state);
            }
        } else {
            FURI_LOG_D(APP_NAME, "osMessageQueue: event timeout");
            // event timeout
        }
        view_port_update(view_port);
        release_mutex(&state_mutex, game_state);
    }


    furi_timer_free(timer);
    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);
    delete_mutex(&state_mutex);

    free_and_exit:
    queue_clear();
    free(game_state);
    furi_message_queue_free(event_queue);

    return return_code;
}
