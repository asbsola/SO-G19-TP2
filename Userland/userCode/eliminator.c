#include <eliminator.h>

static uint32_t screen_width;
static uint32_t screen_height;

static uint8_t players = 1;
static char player_names[2][MAX_PLAYER_NAME] = {0};

static uint8_t speed = 1;

typedef struct {
    uint32_t x;
    uint32_t y;
    uint8_t direction; // 0: up, 1: right, 2: down, 3: left
    uint32_t color;
    uint8_t score;
    uint8_t alive;
} PlayerData;

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

    while(1) {
        char key = sys_get_character_pressed();
        if(key == ' ' || key == '\n') {
            return key;
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
    
    while(x <= screen_width - SQUARE_SIZE * 2){
        sys_draw_square(0x00FF0000, x, SQUARE_SIZE, SQUARE_SIZE);
        sys_draw_square(0x00FF0000, x, screen_height - SQUARE_SIZE * 2, SQUARE_SIZE);
        // sys_delay(60);
        x+=SQUARE_SIZE;
    }

    x-=SQUARE_SIZE;

    while(y <= screen_height - SQUARE_SIZE * 2){
        sys_draw_square(0x00FF0000, SQUARE_SIZE, y, SQUARE_SIZE);
        sys_draw_square(0x00FF0000, x, y, SQUARE_SIZE);
        // sys_delay(60);
        y+=SQUARE_SIZE;
    }

    y-=SQUARE_SIZE;

    uint32_t bufferWidth = x/SQUARE_SIZE - 2;
    uint32_t bufferHeight = y/SQUARE_SIZE - 2;
    uint32_t buffer[bufferHeight][bufferWidth];

    sys_delay(500);

    PlayerData player1Data = {bufferWidth / 2, 5, 2, 0x00FF0000, 0, 1};
    PlayerData player2Data = {bufferWidth / 2, bufferHeight - 6, 0, 0x0000FF00, 0, 1};

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

        if(player1Data.alive == 0 || player2Data.alive == 0) {
            endGame();
            break;
        }

        

    }
    
}

void play_eliminator() {
    
    screen_width = sys_get_screen_width();
    screen_height = sys_get_screen_height();
    
    init_presentation();

    char menu_result = init_menu();

    if(menu_result == ' ') {
        play();
    } else if (menu_result == '\n') {
        change_settings_and_play();
    }
    
    while(1);
}