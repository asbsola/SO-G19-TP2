#include <eliminator.h>

typedef struct {
    uint32_t x;
    uint32_t y;
    uint8_t direction; // 0: up, 1: right, 2: down, 3: left
    uint32_t color;
    uint8_t score;
    uint8_t alive;
} PlayerData;

static uint32_t screen_width;
static uint32_t screen_height;

static uint8_t players = 1;
static char player_names[2][MAX_PLAYER_NAME] = {"a", "b"};

static PlayerData player1Data = {0, 0, 0, 0x00FF0000, 0, 1};
static PlayerData player2Data = {0, 0, 0, 0x0000FF00, 0, 1};

static uint8_t speed = 1;

void init_presentation() {
    sys_clear_text_buffer(0);
    sys_set_font_size(3);
    sys_put_text("ELIMINATOR", 10, 0x00FF0000, 100, 100);
    sys_put_text("by los egrep", 12, 0x00FF0000, 100, 170);
    sys_put_text("Press any key to start", 23, 0x00FF0000, 100, 240);

    while(sys_get_key_pressed() == 0);
}

char init_menu() {

    sys_clear_text_buffer(0);

    print_setting("PLAYERS: ", 10, players, 0x00FF0000, 100, 100);
    print_setting("SPEED: ", 8, speed, 0x00FF0000, 100, 150);

    sys_put_text("[SPACE] to begin game", 21, 0x00FF0000, 100, 250);
    sys_put_text("[ENTER] to change", 17, 0x00FF0000, 100, 300);
    sys_put_text("[X] to exit game", 16, 0x00FF0000, 100, 350);

    while(1) {
        char key = sys_get_character_pressed();
        if(key == ' ') {
            play();
        } else if (key == '\n') {
            change_settings_and_play();
        } else if (key == 'X') {
            return 0;
        }
    }

}

void change_settings_and_play() {

    sys_clear_text_buffer(0);

    sys_put_text("SPEED (1-5): ", 13, 0x00FF0000, 100, 100);
    while(1) {
        char key = sys_get_character_pressed();
        if(key >= '1' && key <= '5') {
            speed = key - '0';
            break;
        }
    }
    print_setting("SPEED (1-5): ", 14, speed, 0x00FF0000, 100, 100);

    while(sys_get_character_pressed() != '\n');

    sys_put_text("PLAYERS (1-2): ", 15, 0x00FF0000, 100, 150);
    while(1) {
        char key = sys_get_character_pressed();
        if(key == '1' || key == '2') {
            players = key - '0';
            break;
        }
    }
    print_setting("PLAYERS (1-2): ", 16, players, 0x00FF0000, 100, 150);

    while(sys_get_character_pressed() != '\n');

    scan_player_name("PLAYER 1'S: ", 1);

    if(players == 2) {
        scan_player_name("PLAYER 2'S: ", 2);
    }

    sys_put_text("Press any key to start", 22, 0x00FF0000, 100, 400);

    while(sys_get_character_pressed() == 0);

    play();
}

void scan_player_name(char * text, uint8_t player_number) {
    char dest[12 + MAX_PLAYER_NAME + 1];
    sys_put_text(text, 12, 0x00FF0000, 100, 250 + 50 * (player_number - 1));
    for(int i = 0; i < 20; i++) {
        char key;
        while(1) {
            key = sys_get_character_pressed();
            if(key != 0) break;
        }
        if(key == '\n' || i == 19) {
            player_names[player_number - 1][i] = '\0';
            break;
        }
        player_names[player_number - 1][i] = key;

        strcat(dest, text, player_names[player_number - 1]);
        sys_put_text(dest, 12 + 1 + i, 0x00FF0000, 100, 250 + 50 * (player_number - 1));
    }
}

void print_setting(char * text, uint8_t max_text_size, uint8_t value, uint32_t hexColor, uint32_t posX, uint32_t posY) {
    char dest[max_text_size + 1];
    char number[2] = {value + '0', '\0'};
    strcat(dest, text, number);
    sys_put_text(dest, max_text_size, hexColor, posX, posY);
}

void play() {

    sys_clear_screen(0);

    uint32_t x = SQUARE_SIZE;
    uint32_t y = SQUARE_SIZE;

    uint32_t normalized_screen_width = screen_width - screen_width % SQUARE_SIZE;
    uint32_t normalized_screen_height = screen_height - screen_height % SQUARE_SIZE;
    
    while(x <= normalized_screen_width - SQUARE_SIZE * 2){
        sys_draw_square(0x00FF0000, x, SQUARE_SIZE, SQUARE_SIZE);
        sys_draw_square(0x00FF0000, x, normalized_screen_height - SQUARE_SIZE * 2, SQUARE_SIZE);
        // sys_delay(60);
        x+=SQUARE_SIZE;
    }

    while(y <= normalized_screen_height - SQUARE_SIZE * 2){
        sys_draw_square(0x00FF0000, SQUARE_SIZE, y, SQUARE_SIZE);
        sys_draw_square(0x00FF0000, normalized_screen_width - SQUARE_SIZE * 2, y, SQUARE_SIZE);
        // sys_delay(60);
        y+=SQUARE_SIZE;
    }

    uint32_t bufferWidth = x/SQUARE_SIZE - 3;
    uint32_t bufferHeight = y/SQUARE_SIZE - 3;
    uint32_t buffer[bufferHeight][bufferWidth];

    sys_delay(500);

    // TODO: verificar si funciona el beep
    sys_beep(400, 300);

    player1Data.x = bufferWidth / 2;
    player1Data.y = 5;
    player1Data.direction = 2;
    player1Data.alive = 1;

    player2Data.x = bufferWidth / 2;
    player2Data.y = bufferHeight - 6;
    player2Data.direction = 0;
    player2Data.alive = 1;

    while(1) {
        char key = sys_get_key_pressed();
        if(key != 0) {
            if(key == W_KEY_CODE_PRESSED && player1Data.direction != 2) {
                player1Data.direction = 0;
            } else if(key == D_KEY_CODE_PRESSED && player1Data.direction != 3) {
                player1Data.direction = 1;
            } else if(key == S_KEY_CODE_PRESSED && player1Data.direction != 0) {
                player1Data.direction = 2;
            } else if(key == A_KEY_CODE_PRESSED && player1Data.direction != 1) {
                player1Data.direction = 3;
            } else if(key == UP_ARROW_CODE_PRESSED && player2Data.direction != 2) {
                player2Data.direction = 0;
            } else if(key == RIGHT_ARROW_CODE_PRESSED && player2Data.direction != 3) {
                player2Data.direction = 1;
            } else if(key == DOWN_ARROW_CODE_PRESSED && player2Data.direction != 0) {
                player2Data.direction = 2;
            } else if(key == LEFT_ARROW_CODE_PRESSED && player2Data.direction != 1) {
                player2Data.direction = 3;
            }
        }

        // 0 up, 1 right, 2 down, 3 left

        player1Data.x += (player1Data.direction == 1) - (player1Data.direction == 3);
        player1Data.y += (player1Data.direction == 2) - (player1Data.direction == 0);

        player2Data.x += (player2Data.direction == 1) - (player2Data.direction == 3);
        player2Data.y += (player2Data.direction == 2) - (player2Data.direction == 0);

        if(player1Data.x < 0 || player1Data.x > bufferWidth - 1 || player1Data.y < 0 || player1Data.y > bufferHeight - 1 || buffer[player1Data.y][player1Data.x] == 1) {
            player1Data.alive = 0;
        }

        if(player2Data.x < 0 || player2Data.x > bufferWidth - 1 || player2Data.y < 0 || player2Data.y > bufferHeight - 1 || buffer[player2Data.y][player2Data.x] == 1) {
            player2Data.alive = 0;
        }

        if(player1Data.x == player2Data.x && player1Data.y == player2Data.y) {
            player1Data.alive = 0;
            player2Data.alive = 0;
        }

        buffer[player1Data.y][player1Data.x] = 1;
        sys_draw_square(player1Data.color, (player1Data.x + 2) * SQUARE_SIZE, (player1Data.y + 2) * SQUARE_SIZE, SQUARE_SIZE);

        if(players == 2) {
            buffer[player2Data.y][player2Data.x] = 1;
            sys_draw_square(player2Data.color, (player2Data.x + 2) * SQUARE_SIZE, (player2Data.y + 2) * SQUARE_SIZE, SQUARE_SIZE);
        }
        
        if(player1Data.alive == 0 || (players == 2 && player2Data.alive == 0)) {
            endGame();
            break;
        }
        
        sys_delay(300 / speed);
    }
    
}

void endGame() {

    // TODO: verificar si funciona el beep
    sys_beep(400, 300);

    if (players == 2)
    {
        if(player1Data.alive == 0 && player2Data.alive == 1) {
            player2Data.score += 1;
        } else if (player1Data.alive == 1 && player2Data.alive == 0) {
            player1Data.score += 1;
        }
    }
    
    char dest[MAX_PLAYER_NAME + 5] = {0};
    char number[4];
    strcat(dest, player_names[0], ": ");
    strcat(dest, dest, itoa(player1Data.score, number, 3));
    sys_put_text(dest, MAX_PLAYER_NAME + 5, player1Data.color, 100, 100);

    if(players == 2) {
        strcat(dest, player_names[1], ": ");
        strcat(dest, dest, itoa(player2Data.score, number, 3));
        sys_put_text(dest, MAX_PLAYER_NAME + 5, player2Data.color, 100, 150);
    }

    sys_put_text("Press [SPACE BAR] to play again", 31, 0x00FFFFFF, 50, 250);
    sys_put_text("Or [ENTER] to go back to menu", 29, 0x00FFFFFF, 70, 300);

    while(1) {
        char key = sys_get_character_pressed();
        if(key == ' ') {
            play();
            return;
        } else if (key == '\n') {
            init_menu();
            return;
        }
    }

}

void play_eliminator() {
    
    screen_width = sys_get_screen_width();
    screen_height = sys_get_screen_height();
    
    init_presentation();

    init_menu();
}