// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <eliminator.h>

static uint32_t screen_width;
static uint32_t screen_height;

static uint8_t players = 1;

static PlayerData player1Data = {"PLAYER 1", 8, 0, 0, 0, HEX_RED, 0, 1, 1};
static PlayerData player2Data = {"PLAYER 1", 8, 0, 0, 0, HEX_GREEN, 0, 1, 1};

static uint8_t speed = 1;

void init_presentation() {
    sys_clear_text_buffer(0);
    sys_set_font_size(3);
    sys_put_text("ELIMINATOR", 10, HEX_RED, 100, 100);
    sys_put_text("by los egrep", 12, HEX_RED, 100, 170);
    sys_put_text("Press any key to start", 23, HEX_RED, 100, 240);

    while(sys_get_key_pressed() == 0);
}

void init_menu() {
    sys_clear_text_buffer(0);

    print_setting("PLAYERS: ", 10, players, HEX_RED, 100, 100);
    print_setting("SPEED: ", 8, speed, HEX_RED, 100, 150);

    sys_put_text("[SPACE] to begin game", 21, HEX_RED, 100, 250);
    sys_put_text("[ENTER] to change", 17, HEX_RED, 100, 300);
    sys_put_text("[X] to exit game", 16, HEX_RED, 100, 350);
}

void change_settings_and_play() {

    sys_clear_text_buffer(0);

    player1Data.score = 0;
    player2Data.score = 0;

    sys_put_text("SPEED (1-5): ", 13, HEX_RED, 100, 100);
    char key = 0;
    while(key < '1' || key > '5') {
        key = sys_get_character_pressed();
    }
    speed = key - '0';
    
    print_setting("SPEED (1-5): ", 14, speed, HEX_RED, 100, 100);

    sys_put_text("PLAYERS (1-2): ", 15, HEX_RED, 100, 150);
    key = 0;
    while(key != '1' && key != '2') {
        key = sys_get_character_pressed();
    }
    players = key - '0';
    print_setting("PLAYERS (1-2): ", 16, players, HEX_RED, 100, 150);

    scan_player_name("PLAYER 1: ", 10, 1, &player1Data);

    if(players == 2) {
        scan_player_name("PLAYER 2: ", 10, 2, &player2Data);
    }

    sys_put_text("Press any key to start", 22, HEX_RED, 100, 400);

    while(sys_get_character_pressed() == 0);

    play();
}

void scan_player_name(char * text, int len, uint8_t player_number, PlayerData* player) {
    char dest[len + MAX_LENGTH_PLAYER_NAME + 1];
    sys_put_text(text, len, HEX_RED, 100, 250 + 50 * (player_number - 1));
    char key = 0;
    int i;
    for(i = 0; i < MAX_LENGTH_PLAYER_NAME && key != '\n'; i++) {
        key = 0;
        while(!isAlpha(key) && key != '\n') {
            key = sys_get_character_pressed();
        }
        if(key != '\n') player->name[i] = key;
        else player->name[i] = '\0';
        strcat(dest, text, player->name);
        sys_put_text(dest, len + i + 1, HEX_RED, 100, 250 + 50 * (player_number - 1));
    }
    player->name[i] = '\0';
    player->name_len = i;
}

void print_setting(char * text, uint8_t max_text_size, uint8_t value, uint32_t hexColor, uint32_t posX, uint32_t posY) {
    char dest[max_text_size + 1];
    char number[2] = {value + '0', '\0'};
    strcat(dest, text, number);
    sys_put_text(dest, max_text_size, hexColor, posX, posY);
}

void changeDirection(char key, int* p1Dir, int* p2Dir){
    switch (key)
    {
    case 0:
        break;
    case W_KEY_CODE_PRESSED:
        if(player1Data.direction != DOWN_DIRECTION) *p1Dir = UP_DIRECTION;
        break;
    case D_KEY_CODE_PRESSED:
        if(player1Data.direction != LEFT_DIRECTION) *p1Dir = RIGHT_DIRECTION;
        break;
    case S_KEY_CODE_PRESSED:
        if(player1Data.direction != UP_DIRECTION) *p1Dir = DOWN_DIRECTION;
        break;
    case A_KEY_CODE_PRESSED:
        if(player1Data.direction != RIGHT_DIRECTION) *p1Dir = LEFT_DIRECTION;
        break;
    case UP_ARROW_CODE_PRESSED:
        if(player2Data.direction != DOWN_DIRECTION) *p2Dir = UP_DIRECTION;
        break;
    case RIGHT_ARROW_CODE_PRESSED:
        if(player2Data.direction != LEFT_DIRECTION) *p2Dir = RIGHT_DIRECTION;
        break;
    case DOWN_ARROW_CODE_PRESSED:
        if(player2Data.direction != UP_DIRECTION) *p2Dir = DOWN_DIRECTION;
        break;
    case LEFT_ARROW_CODE_PRESSED:
        if(player2Data.direction != RIGHT_DIRECTION) *p2Dir = LEFT_DIRECTION;
        break;
    default:
        break;
    }
}

void play() {
    int keep_playing = 1;
    while(keep_playing == 1){
        sys_clear_screen(0);

        uint32_t normalized_screen_width = screen_width - screen_width % SQUARE_SIZE;
        uint32_t normalized_screen_height = screen_height - screen_height % SQUARE_SIZE;

        for(uint32_t x = SQUARE_SIZE; x < normalized_screen_width - SQUARE_SIZE; x += SQUARE_SIZE){
            sys_draw_square(HEX_RED, x, SQUARE_SIZE, SQUARE_SIZE);
            sys_draw_square(HEX_RED, x, normalized_screen_height - SQUARE_SIZE * 2, SQUARE_SIZE);
        }

        for(uint32_t y = SQUARE_SIZE; y < normalized_screen_height - SQUARE_SIZE; y += SQUARE_SIZE){
            sys_draw_square(HEX_RED, SQUARE_SIZE, y, SQUARE_SIZE);
            sys_draw_square(HEX_RED, normalized_screen_width - SQUARE_SIZE * 2, y, SQUARE_SIZE);
        }

        uint32_t bufferWidth = normalized_screen_width/SQUARE_SIZE - 4;
        uint32_t bufferHeight = normalized_screen_height/SQUARE_SIZE - 4;
        uint32_t buffer[bufferHeight][bufferWidth];
        for(int i=0; i<bufferHeight; i++){
            for(int j=0; j<bufferWidth; j++){
                buffer[i][j] = 0;
            }
        }

        sys_beep(400, 300);

        player1Data.x = bufferWidth / 2;
        player1Data.y = STARTING_HEIGHT;
        player1Data.direction = DOWN_DIRECTION;
        player1Data.alive = 1;

        player2Data.x = bufferWidth / 2;
        player2Data.y = bufferHeight - 1 - STARTING_HEIGHT;
        player2Data.direction = UP_DIRECTION;
        player2Data.alive = 1;

        while(player1Data.alive == 1 && (players != 2 || player2Data.alive == 1)) {
            char key;
            int player1Dir = player1Data.direction;
            int player2Dir = player2Data.direction;
            while((key = sys_get_key_pressed()) != 0){
                changeDirection(key, &player1Dir, &player2Dir);
            }
            player1Data.direction = player1Dir;
            player2Data.direction = player2Dir;

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

            if(player1Data.alive){
                buffer[player1Data.y][player1Data.x] = 1;
                sys_draw_square(player1Data.color, (player1Data.x + 2) * SQUARE_SIZE, (player1Data.y + 2) * SQUARE_SIZE, SQUARE_SIZE);
            }

            if(players == 2 && player2Data.alive) {
                buffer[player2Data.y][player2Data.x] = 1;
                sys_draw_square(player2Data.color, (player2Data.x + 2) * SQUARE_SIZE, (player2Data.y + 2) * SQUARE_SIZE, SQUARE_SIZE);
            }

            sys_delay(MAX_DELAY / speed);
        }
        keep_playing = endGame();
    }
    init_menu();
}

int endGame() {
    sys_beep(400, 300);

    if (players == 2)
    {
        if(player1Data.alive == 0 && player2Data.alive == 1) {
            player2Data.score += 1;
            int n = 1;
            int i;
            for(i=1; n*10 <= player2Data.score && i<MAX_DIGITS_SCORE; i++) n*=10;
            player2Data.score_digits = i;
        } else if (player1Data.alive == 1 && player2Data.alive == 0) {
            player1Data.score += 1;
            int n = 1;
            int i;
            for(i=1; n*10 <= player1Data.score && i<MAX_DIGITS_SCORE; i++) n*=10;
            player1Data.score_digits = i;
        }
    }
    
    char dest[MAX_LENGTH_PLAYER_NAME + 2 + MAX_DIGITS_SCORE + 1] = {0};
    char number[MAX_DIGITS_SCORE + 1] = {0};
    strcat(dest, player1Data.name, ": ");
    strcat(dest, dest, itoa(player1Data.score, number, player1Data.score_digits));
    sys_put_text(dest, player1Data.name_len + 2 + player1Data.score_digits, player1Data.color, 100, 100);

    if(players == 2) {
        strcat(dest, player2Data.name, ": ");
        strcat(dest, dest, itoa(player2Data.score, number, player2Data.score_digits));
        sys_put_text(dest, player2Data.name_len + 2 + player2Data.score_digits, player2Data.color, 100, 150);
    }

    sys_put_text("Press [SPACE BAR] to play again", 31, HEX_WHITE, 50, 250);
    sys_put_text("Or [ENTER] to go back to menu", 29, HEX_WHITE, 70, 300);

    while(1) {
        char key = sys_get_character_pressed();
        if(key == ' ') {
            return 1;
        } else if (key == '\n') {
            return 0;
        }
    }
}

void play_eliminator() {
    
    screen_width = sys_get_screen_width();
    screen_height = sys_get_screen_height();
    
    init_presentation();
    
    init_menu();
    char key = sys_get_character_pressed();

    while(key != 'x' && key != 'X') {
        if(key == ' ') {
            play();
        } else if (key == '\n') {
            change_settings_and_play();
        }
        key = sys_get_character_pressed();
    }
}
