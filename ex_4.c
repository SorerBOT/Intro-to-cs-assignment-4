#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define INVALID_INPUT "INVALID INPUT"
#define PLAYER_NAME_DELIMITER "#"

#define BOARD_DIM_INDEX 1
#define NUM_PLAYERS_INDEX 2
#define PLAYER_NAMES_INDEX 3
#define FIRST_OBJECT_INDEX 4

#define MAX_BOARD_DIM 30
#define MAX_PLAYERS 10
#define MAX_OBJECTS ( ( MAX_BOARD_DIM * MAX_BOARD_DIM ) / 2 )
#define MAX_PLAYER_NAME_LENGTH 20
#define MAX_OBJECT_NAME_LENGTH 7

typedef enum { false, true } bool;

typedef struct {
    char name[MAX_PLAYER_NAME_LENGTH];
    int score;
} Player;
typedef struct {
    int firstInstance;
    int secondInstance;
} Position;
typedef struct {
    char name[MAX_OBJECT_NAME_LENGTH];
    Position position;
} Object;
typedef struct {
    bool isSuccess;
    int dim;
    int numPlayers;
    Player players[MAX_PLAYERS];
    int numObjects;
    Object objects[MAX_OBJECTS];
} Board;
/**
 * Test for validity of the input.
 * 
 * @return {Board*} Returns a pointer to a Board or NULL if the input was invalid
*/
Board initialiseBoard(int argc, char** argv);
/**
 * Prints the initial state of the board
 * 
 * @return {void}
*/
void printBoard(Board board);
/**
 * Initialises the board using initialiseBoard
 * Prints the board using printBoard
 *  
 *
*/
int main(int argc, char** argv) {
    Board board = initialiseBoard( argc, argv );
    if ( !board.isSuccess ) return 0;
    printBoard(board);
    return 0;
}

Board initialiseBoard(int argc, char** argv) {
    Board board;
    board.isSuccess = false;
    if (argc < 3) {
        printf( "%s\n", INVALID_INPUT );
        return board;
    }
    
    int boardDim = atoi( argv[BOARD_DIM_INDEX] );
    int numPlayers = atoi( argv[NUM_PLAYERS_INDEX] );
    
    if ( boardDim > MAX_BOARD_DIM ) {
        printf( "%s\n", INVALID_INPUT );
        return board;        
    }
    if ( numPlayers > MAX_PLAYERS ) {
        printf( "%s\n", INVALID_INPUT );
        return board;
    }
    // Input size (argc) validations
    int board_length = boardDim * boardDim;
    int num_of_objects = ( board_length ) / 2;
    int num_of_pos = board_length;
    int desired_argc = 4 + num_of_objects + num_of_pos;
    if (argc != desired_argc) {
        printf( "%s\n", INVALID_INPUT );
        return board;
    }
    board.dim = boardDim;
    char copy_of_player_names_string[MAX_PLAYERS * (MAX_PLAYER_NAME_LENGTH + 1)]; // 20 chars per player + # + \0
    strcpy( copy_of_player_names_string, argv[PLAYER_NAMES_INDEX] );
    char* playerName = strtok( copy_of_player_names_string, PLAYER_NAME_DELIMITER );
    int i = 0;
    while ( playerName != NULL ) {
        Player player;
        strcpy( player.name, playerName );
        player.score = 0;
        board.players[i] = player;
        board.numPlayers++;
        i++;
        playerName = strtok( NULL, PLAYER_NAME_DELIMITER );
    }
    // Validating the amount of players entered as input
    if ( board.numPlayers != numPlayers ) {
        printf( "%s\n", INVALID_INPUT );
        return board;
    }
    // Parsing the objects
    int j = 0;
    for (; j < num_of_objects; j++) {
        Object object;
        strcpy( object.name, argv[FIRST_OBJECT_INDEX + j] );
        board.objects[i] = object;
        board.numObjects++;
    }
    int FIRST_POSITION_INDEX = FIRST_OBJECT_INDEX + j;
    // Checking for repeating positions
    for (int k = 0; k < num_of_pos; k++) {
        for (int p = k + 1; p < num_of_pos; p++) {
            if ( atoi( argv[FIRST_POSITION_INDEX + k] ) == atoi( argv[FIRST_POSITION_INDEX + p] ) ) {
                printf( "%s\n", INVALID_INPUT );
                return board;
            }
        }
    }
    // Parsing the positions
    for (int k = 0; k < num_of_pos; k += 2) {
        Position position;
        position.firstInstance = atoi( argv[FIRST_POSITION_INDEX + k] );
        position.secondInstance = atoi( argv[FIRST_POSITION_INDEX + k + 1] );
        if (position.firstInstance > board_length || position.secondInstance > board_length) {
            printf( "%s\n", INVALID_INPUT );
            return board;
        }
        board.objects[k].position = position;
    }
    board.isSuccess = true;
    return board;
}

void printBoard(Board board) {
    int dividerLength = ( 6 * board.dim ) + 3;
    printf("  |");
    for (int i = 0; i < board.dim; i++) {
        printf("  %d  |", i);
    }
    printf("\n");
    for (int i = 0; i < dividerLength; i++) printf("-");
    printf("\n");
    for (int i = 0; i < board.dim; i++) {
        // Using x,y to simulate matrix notation
        int x = i;
        int y = 0;
        printf("%d |", x);
        for (; y < board.dim; y++) {
            printf("(%d,%d)|", x, y);
        }
        printf("\n");
        for (int i = 0; i < dividerLength; i++) printf("-");
        printf("\n");
    }
    printf("\n");
}