/**
 * NAME: Alon Filler
 * ID: 216872374
 *
*/

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
#define LENGTH_OF_CELL 15

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
    bool isFound;
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
 * Runs the game
*/
void playGame(Board* board);
/**
 * Prints the initial state of the board
 * 
 * @return {void}
*/
/**
 * Plays one turn of the game
 * 
 * @return {void}
*/
void playTurn(Board* board, Player* player);
void printInitialBoard(Board board);
/**
 * Prints the current state of the board
 * 
 * @return {void}
*/
void printBoardState(Board* board);
/**
 * Prints winning miscellaneous
 * 
 * @return {void}
*/
void printWinner(Board board);
/**
 * Checks if there are objects which have not yet been found
 * 
 * @return {bool}
*/
bool handleCheckIsGameFinished(Board board);
/**
 * Finds the object located in the nth position
*/
Object* handleGetObjectAtPosition(Board* board, int n);
/**
 * Initialises the board using initialiseBoard
 * Prints the board using printInitialBoard
 *  
 *
*/
int main(int argc, char** argv) {
    Board board = initialiseBoard( argc, argv );
    if ( !board.isSuccess ) return 0;
    playGame( &board );
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
    for ( ; j < num_of_objects; j++ ) {
        Object object;
        object.isFound = false;
        strcpy( object.name, argv[FIRST_OBJECT_INDEX + j] );
        board.objects[j] = object;
        board.numObjects++;
    }
    int FIRST_POSITION_INDEX = FIRST_OBJECT_INDEX + j;
    // Checking for repeating positions
    for ( int k = 0; k < num_of_pos; k++ ) {
        for ( int p = k + 1; p < num_of_pos; p++ ) {
            if ( atoi( argv[FIRST_POSITION_INDEX + k] ) == atoi( argv[FIRST_POSITION_INDEX + p] ) ) {
                printf( "%s\n", INVALID_INPUT );
                return board;
            }
        }
    }
    // Parsing the positions
    for ( int k = 0; k < num_of_pos; k += 2 ) {
        Position position;
        position.firstInstance = atoi( argv[FIRST_POSITION_INDEX + k] );
        position.secondInstance = atoi( argv[FIRST_POSITION_INDEX + k + 1] );
        if (position.firstInstance > board_length || position.secondInstance > board_length) {
            printf( "%s\n", INVALID_INPUT );
            return board;
        }
        board.objects[k / 2].position = position;
    }
    board.isSuccess = true;
    return board;
}
void playGame( Board* board ) {
    int playerIndex = 0;
    printInitialBoard( *board );
    printf( "Let the game begin!\n\n" );
    printBoardState( board );
    while ( !handleCheckIsGameFinished( *board ) ) {
        playTurn( board, &( board->players[playerIndex] ) );
        playerIndex = ( ( playerIndex + 1 ) % board->numPlayers );
    }
    printWinner( *board );
}
void playTurn(Board* board, Player* player) {
    int first_x, first_y;
    int second_x, second_y;
    int first_index;
    int second_index;
    Object* firstObject;
    Object* secondObject;
    bool isFinishedTurn = false;
    int boardLength = board->dim * board->dim;

    while ( !isFinishedTurn ) {
        printf( "It is %s's turn.\n", ( *player ).name );
        bool enteredValidCoordinates = false;
        // Get coordinates
        while ( !enteredValidCoordinates ) {
            printf( "Enter the coordinates of the first card (row col): " );
            scanf(" %d %d", &first_x, &first_y);
            printf( "Enter the coordinates of the second card (row col): " );
            scanf(" %d %d", &second_x, &second_y);
            // Transform coordinates to index notation
            first_index = ( first_x * board->dim ) + first_y;
            second_index = ( second_x * board->dim ) + second_y;
            // Get objects at said positions
            firstObject = handleGetObjectAtPosition( board, first_index );
            secondObject = handleGetObjectAtPosition( board, second_index );
            // Validate coordinates
            if (
                first_x < 0 ||
                first_x >= board->dim ||
                first_y < 0 ||
                first_y >= board->dim ||
                second_x < 0 ||
                second_x >= board->dim ||
                second_y < 0 ||
                second_y >= board->dim ||
                ( first_x == second_x && first_y == second_y ) ||
                firstObject->isFound ||
                secondObject->isFound
            ) {
                printf( "Invalid move. Try again.\n" );
                continue;
            }
            else enteredValidCoordinates = true;
        }
        // Get objects at said positions
        if ( ( *firstObject ).position.firstInstance == second_index || ( *firstObject ).position.secondInstance == second_index ) {
            ( *firstObject ).isFound = true;
            ( *player ).score++;
            printf( "Match!\n" );
        }
        else {
            isFinishedTurn = true;
            printf( "No match. Try again.\n" );
        }
        printf( "\nThe scores are:\n" );
        for ( int i = 0; i < board->numPlayers; i++ ) {
            printf( "%s: %d", board->players[i].name, board->players[i].score );
            if ( i != ( board->numPlayers - 1 ) ) printf( ", " );
        }
        printf( "\n" );
        if ( handleCheckIsGameFinished( *board ) ) return;
        printBoardState( board );
    }
}
void printInitialBoard( Board board ) {
    int dividerLength = ( 6 * board.dim ) + 3;
    printf( "Welcome to the Memory Card Game!\n" );
    printf( "The game will be played on the following board:\n\n" );
    printf( "  |" );
    for ( int i = 0; i < board.dim; i++ ) {
        printf( "  %d  |", i );
    }
    printf( "\n" );
    for ( int i = 0; i < dividerLength; i++ ) printf( "-" );
    printf( "\n" );
    for ( int i = 0; i < board.dim; i++ ) {
        // Using x,y to simulate matrix notation
        int x = i;
        int y = 0;
        printf( "%d |", x );
        for ( ; y < board.dim; y++ ) {
            printf( "(%d,%d)|", x, y );
        }
        printf( "\n" );
        for ( int i = 0; i < dividerLength; i++ ) printf( "-" );
        printf( "\n" );
    }
    printf( "\n" );
    printf( "And the following objects:\n\n" );
    for ( int i = 0; i < board.numObjects; i++ ) {
        printf( "%d. %s\n", i + 1, board.objects[i].name );
    }
    printf( "\n" );
}
void printBoardState( Board* board ) {
    int boardLength = ( board->dim * board->dim );
    int dividerLength = ( 16 * board->dim ) + 1;
    printf( "Current board state:\n" );
    for ( int i = 0; i < dividerLength; i++ ) printf( "-" );
    printf( "\n" );
    printf( "|" );        
    for ( int i = 0; i < boardLength; i++ ) {
        Object* object = handleGetObjectAtPosition( board, i );
        // actualName will enable us to hide cards which are yet to have been found
        char actualName[MAX_OBJECT_NAME_LENGTH];
        strcpy( actualName, ( *object ).isFound ? ( *object ).name : "X" );
        int objectNameLength = strlen( actualName );
        int amountOfSpacesNeeded = LENGTH_OF_CELL - objectNameLength;
        int amountOfSpacesNeededBefore = ( amountOfSpacesNeeded / 2 ) + ( amountOfSpacesNeededBefore % 2 );
        int amountOfSpacesNeededAfter = ( amountOfSpacesNeeded - amountOfSpacesNeededBefore );

        for ( int b = 0; b < amountOfSpacesNeededBefore; b++ ) printf(" ");
        printf("%s", actualName);
        for ( int b = 0; b < amountOfSpacesNeededAfter; b++ ) printf(" ");
        printf("|");
        if ( !( ( i + 1 ) % board->dim ) ) {
            printf( "\n" );
            for ( int b = 0; b < dividerLength; b++ ) printf( "-" );
            printf( "\n" );
            if ( ( i + 1 ) != boardLength ) printf( "|" );
        }
    }
    printf( "\n" );
}
bool handleCheckIsGameFinished(Board board) {
    for (int i = 0; i < board.numObjects; i++) {
        if ( !( board.objects[i].isFound ) ) return false;
    }
    return true;
}
Object* handleGetObjectAtPosition( Board* board, int n ) {
    for ( int i = 0; i < board->numObjects; i++ ) {
        if ( board->objects[i].position.firstInstance == n || board->objects[i].position.secondInstance == n ) return &( board->objects[i] );
    }
}
void printWinner(Board board) {
    Player topPlayer = board.players[0];
    bool isTie = false;

    for (int i = 1; i < board.numPlayers; i++) {
        if (board.players[i].score > topPlayer.score) {
            topPlayer = board.players[i];
            isTie = false;
        }
        if (board.players[i].score == topPlayer.score) isTie = true;
    }
    if (!isTie) {
        printf( "Congratulations %s! You won!\n", topPlayer.name );
    }
    else {
        printf( "It's a tie!\n" );
    }
}