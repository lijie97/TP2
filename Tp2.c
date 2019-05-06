#include<stdio.h>
#include<string.h>
#include<malloc.h>
//#include <climits>
//######## Definir les Noms #################
#define ESPACE ' '  //Escpace               #
#define PBX 'X'     //Pièce Grand X         #
#define PSX 'x'     //Pièce Petit x         #
#define PBO 'O'     //Pièce Grand O         #
#define PSO 'o'     //Pièce Petit o         #
#define PD '#'      //"#"                   #
#define PLAYER_O 0  //                      #
#define PLAYER_X 1  //                      #
#define FALSE 0     //                      #
#define TRUE 1      //                      #
#define POSTOMASK(y,x) (1<<(24-(y*5+x))) // # 
#define MAX(x,y) (x<y?y:x)               // #
#define MIN(x,y) (x>y?y:x)               // #
#define INT_MAX 2147483647               // #
#define INT_MIN -2147483648              // #
//#define MAXLEVEL 4                     // #
#define ISPAIR(x) (!(x%2))               // #
#define TURN_TO_PIECE(x) ((x==0)?79:88)  // #
#define LEN sizeof(struct LinkedLists)   // #
//###########################################

typedef struct IntsBoards{
    int pBO;
    int pBX;
    int pSO;
    int pSX; 
    int pD;
} IntsBoard;

typedef struct Cords {
	int x;
	int y;
    int value;
} Cord;

struct LinkedLists {
    IntsBoard value;
    struct LinkedLists *next;
    struct LinkedLists *pre;
};

typedef struct LinkedLists LinkedList;

const int LR[] = {0,-1,0,1};
const int UD[] = {-1,0,1,0};//Tableau de direction
const int Pieces[] = {PBO,PSO,PBX,PSX,PD};


int in(int y,int x){
    /*--------------------------------------------------------*
     * Une fonction juge si la position est bordée            *
     * Entrees : une paire de int de coordonné                *
     * Retourne : int qui dit si la position est bordée       *
     *--------------------------------------------------------*/
    return (0<=x && x<=4 && 0<=y && y<=4);   
}

int judgeGroup(int p){
    /*--------------------------------------------------------*
     * Une fonction retournant le groupe d'une pièce vecteurs *
     * Entrees : int qui représente le code de pièce          *
     * Retourne : int qui représente le groupe de pièce       *
     *--------------------------------------------------------*/
    if (p==PSO || p==PBO)
        return PLAYER_O; 
    if (p==PSX || p==PBX)
        return PLAYER_X;
    return PD;
}

int countPoints(int board[][5],int y, int x,int turn){
    /*--------------------------------------------------------*
     * Une fonction retournant points gagnée pour une postion *
     * Entrees : le tableau de board, le coordonné            *
     * Entrees : int qui représente le tourne                 *
     * Retourne : int qui représente le point gagnée          *
     *--------------------------------------------------------*/
    int sy,sx;
    int points = 5;
    if (board[y][x]!=ESPACE) points --;
    for (int i = 0; i < 4; i++){
        sy = y + UD[i];
        sx = x + LR[i];
        if ((! in(sy,sx)) || 
            (in(sy,sx) && (judgeGroup(board[sy][sx]) == turn || 
                          (board[sy][sx] == PD))))
            points --;
    }
    return points;
}

int available(int board[][5], int y, int x, int turn){
    /*--------------------------------------------------------*
     * Une fonction juge si la position est accessible        *
     * Entrees : le tableau de board, le coordonné            *
     * Entrees : int qui représente le tourne                 *
     * Retourne : int qui dit si la position est accessible   *
     *--------------------------------------------------------*/
    return (board[y][x]==ESPACE                    ||
            (board[y][x]==PSO && turn == PLAYER_O) ||
            (board[y][x]==PSX && turn == PLAYER_X));
}

void initialisationBoard(int board[][5]){
    /*--------------------------------------------------------*
     * Une fonction de l'initialisation de tableau vide       *
     * Entrees : le tableau de board                          *
     *--------------------------------------------------------*/
    for (int i = 0;i<5;i++)
        for (int j = 0; j<5; j++)
            board[i][j] = ESPACE;
}

void printBoard(int board[][5],int y, int x){
    /*--------------------------------------------------------*
     * Une fonction de l'affichage de tableau vide            *
     * Entrees : le tableau de board                          *
     *--------------------------------------------------------*/
    printf("  ");
    int i,j;
    for (i = 1; i < 6; i++)
        printf("%4d",i);
    for (i = 0; i < 5; i++){
        printf("\n   +---+---+---+---+---+\n");
        printf(" %c |",'A' + i);
        for (j = 0; j < 5; j++){
            if (i==y && j==x)
                printf("[%c]|", board[i][j]);
            else 
                printf(" %c |",board[i][j]);
        }
    }
    printf("\n   +---+---+---+---+---+\n");
}


void putPiece(int board[][5],int y, int x,int turn){
    /*--------------------------------------------------------*
     * Une fonction de mettre un pièce sur le tableau         *
     * Entrees : le tableau de board, le coordonné            *
     * Entrees : int qui représente le tourne                 *
     *--------------------------------------------------------*/
    board[y][x] = (turn==PLAYER_O)?  PBO
                                  :  PBX;
    int sy,sx;
    for (int i = 0; i < 4; i++){
        sy = y + UD[i];
        sx = x + LR[i];
        if (in(sy,sx))
            switch (board[sy][sx])
            {
                case ESPACE :
                    board[sy][sx] = (turn==PLAYER_O)?  PSO
                                                    :  PSX;
                    break;
                case PSO :
                    board[sy][sx] = (turn==PLAYER_X)?  PD
                                                    :  PSO;
                    break;
                case PSX :
                    board[sy][sx] = (turn==PLAYER_O)?  PD
                                                    :  PSX;
                    break;
            }
    }
}


void score(int board[][5]){
    /*--------------------------------------------------------*
     * Une fonction de l'affichage de résultat du jeu         *
     * Entrees : le tableau de board                          *
     *--------------------------------------------------------*/
    int scoreO,scoreX;
    scoreO = scoreX = 0;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            switch (board[i][j]){
                case PBO:
                    scoreO += 4;
                    break;
                case PSO:
                    scoreO ++;
                    break;
                case PBX:
                    scoreX += 4;
                    break;
                case PSX:
                    scoreX ++;
                    break;
            }
    printf("Le jeu est fini,\n pour le joueur X la note est %d\n",scoreX);
    printf("pour le joueur O la note est %d\n",scoreO);
    printBoard(board,-1,-1);
}
int finished(int board[][5],int turn){
    /*--------------------------------------------------------*
     * Une fonction juge si le jeu est fini                   *
     * Entrees : le tableau de board                          *
     * Entrees : int qui représente le tourne                 *
     * Retourne : int qui dit si le jeu est fini              *
     *--------------------------------------------------------*/
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (available(board,i,j,turn))
                return FALSE;
    return TRUE;
}


void aiGreedy(int board[][5],int turn) {
    /*--------------------------------------------------------*
     * Une fonction joue le rôle de I.A. par le glouton       *
     * Entrees : le tableau de board                          *
     * Entrees : int qui représente le tourne                 *
     *--------------------------------------------------------*/
    int max = -1;
    int maxi, maxj, points;
    for (int i=0;i<5;i++)
        for (int j=0;j<5;j++)
            if (available(board,i,j,turn)){
                points = countPoints(board,i,j,turn);
                if (max<points){
                    max = points;
                    maxi = i; 
                    maxj = j;
                }
            }
    if (max != -1 && available(board,maxi,maxj,turn)) putPiece(board,maxi,maxj,turn);
    printf("L'ordinateur a mi sa piece %c dans la case %c%c\n",TURN_TO_PIECE(turn),maxi+'A',maxj + '1');
    printBoard(board, maxi, maxj);
}
int getPieceFromInt(int singleBorad,int y, int x){
    /*--------------------------------------------------------*
     * Une fonction prenant une pièce d'un coordonné          *
     * Entrees : la carte de seule pièce                      *
     * Entrees : la coordonné                                 *
     * Sortie : la pièce prise                                *
     *--------------------------------------------------------*/
    int mask;
    mask = POSTOMASK(y,x);
    return mask & singleBorad;
} 

char pickFromInts(IntsBoard intsBoard, int y, int x){
    /*--------------------------------------------------------*
     * Une fonction prenant une pièce d'un coordonné          *
     * Entrees : la struture de board                         *
     * Entrees : la coordonné                                 *
     * Sortie : la pièce prise                                *
     *--------------------------------------------------------*/
    if (getPieceFromInt(intsBoard.pBO, y, x)) return PBO;
    if (getPieceFromInt(intsBoard.pBX, y, x)) return PBX;
    if (getPieceFromInt(intsBoard.pSO, y, x)) return PSO;
    if (getPieceFromInt(intsBoard.pSX, y, x)) return PSX;
    if (getPieceFromInt(intsBoard.pD, y, x)) return PD;
    return ESPACE;
}
int availableForInts(IntsBoard intsBoard, int y, int x, int turn) {
    /*--------------------------------------------------------*
     * Une fonction juge si la position est accessible        *
     * Entrees : la structure de board, le coordonné          *
     * Entrees : int qui représente le tourne                 *
     * Retourne : int qui dit si la position est accessible   *
     *--------------------------------------------------------*/
	return (pickFromInts(intsBoard, y, x) == ESPACE ||
		(pickFromInts(intsBoard, y, x) == PSO && turn == PLAYER_O) ||
		(pickFromInts(intsBoard, y, x) == PSX && turn == PLAYER_X));
}
int scoreForInts(IntsBoard intsBoard, int turn) {
    /*--------------------------------------------------------*
     * Une fonction de calcul de valeur d'un état             *
     * Entrees : la struture de board                         *
     * Entrees : la tourne                                    *
     * Sortie : la valeur                                     *
     *--------------------------------------------------------*/
	int scoreO, scoreX;
	scoreO = scoreX = 0;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			switch (pickFromInts(intsBoard,i,j)) {
				case PBO:
					scoreO += 4;
					break;
				case PSO:
					scoreO++;
					break;
				case PBX:
					scoreX += 4;
					break;
				case PSX:
					scoreX++;
					break;
			}
	return ((turn == PLAYER_O) ? scoreO - scoreX : scoreX - scoreO);
}

int isFinishedForInts(IntsBoard intsBoard,int turn){
    /*--------------------------------------------------------*
     * Une fonction juge si le jeu est fini                   *
     * Entrees : le tableau de board                          *
     * Entrees : int qui représente le tourne                 *
     * Retourne : int qui dit si le jeu est fini              *
     *--------------------------------------------------------*/
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (availableForInts(intsBoard,i,j,turn))
                return FALSE;
    return TRUE;
}

IntsBoard putPieceToGrid(IntsBoard intsBoard, int y, int x, int piece){
    IntsBoard rslt;
    rslt = intsBoard;
    int mask, maskReversed;
    mask = POSTOMASK(y,x);
    maskReversed = (1 << 25)-1 - mask;
    switch (piece){
        case PBO:
            rslt.pBO = mask | rslt.pBO;
            rslt.pBX = rslt.pBX & maskReversed;
            rslt.pSX = rslt.pSX & maskReversed;
            rslt.pSO = rslt.pSO & maskReversed;
            rslt.pD = rslt.pD & maskReversed;
            break;
        case PBX:
            rslt.pBX = mask | rslt.pBX;
            rslt.pBO = rslt.pBO & maskReversed;
            rslt.pSX = rslt.pSX & maskReversed;
            rslt.pSO = rslt.pSO & maskReversed;
            rslt.pD = rslt.pD & maskReversed;
            break;
        case PSO:
            rslt.pSO = mask | rslt.pSO;
            rslt.pBO = rslt.pBO & maskReversed;
            rslt.pSX = rslt.pSX & maskReversed;
            rslt.pBX = rslt.pBX & maskReversed;
            rslt.pD = rslt.pD & maskReversed;
            break;
        case PSX:
            rslt.pSX = mask | rslt.pSX;
            rslt.pBX = rslt.pBX & maskReversed;
            rslt.pBO = rslt.pBO & maskReversed;
            rslt.pSO = rslt.pSO & maskReversed;
            rslt.pD = rslt.pD & maskReversed;
            break;
        case PD:
            rslt.pD = mask | rslt.pD;
            rslt.pBX = rslt.pBX & maskReversed;
            rslt.pSX = rslt.pSX & maskReversed;
            rslt.pSO = rslt.pSO & maskReversed;
            rslt.pBO = rslt.pBO & maskReversed;
            break;
        default:
            break;
    }
    return rslt;
}

IntsBoard putPieceToInts(IntsBoard intsBoard,int y, int x,int turn){
    /*--------------------------------------------------------*
     * Une fonction de mettre un pièce sur le tableau         *
     * Entrees : le tableau de board, le coordonné            *
     * Entrees : int qui représente le tourne                 *
     *--------------------------------------------------------*/
	intsBoard = putPieceToGrid(intsBoard, y, x, (turn == PLAYER_O) ? PBO : PBX);
	
    int sy,sx;
    for (int i = 0; i < 4; i++){
        sy = y + UD[i];
        sx = x + LR[i];
        if (in(sy,sx))
            switch (pickFromInts(intsBoard, sy, sx))
            {
                case ESPACE :
					intsBoard = putPieceToGrid(intsBoard, sy, sx, (turn == PLAYER_O) ? PSO: PSX);
                    break;
                case PSO :
					intsBoard = putPieceToGrid(intsBoard, sy, sx, (turn == PLAYER_X) ? PD : PSO);
                    break;
                case PSX :
					intsBoard = putPieceToGrid(intsBoard, sy, sx, (turn == PLAYER_O) ? PD : PSX);
                    break;
            }
    }
	return intsBoard;
}
int toIntsForPiece(int board[][5], int piece){
    /*--------------------------------------------------------*
     * Une fonction de transfert en structure du board        *
     * Entrees : le tableau de board                          *
     * Entrees : int qui présente un type de pièce            *
     * Sortie : int qui présente une carte d'une pièce        *
     *--------------------------------------------------------*/
    int count = 0;
    for (int i = 0;i < 5;i ++){
        for (int j = 0;j < 5;j ++){
            count<<=1;
            if (board[i][j]==piece)
                count ++;
        }
    }
    return count;
}

IntsBoard toInts(int board[][5]){
    /*--------------------------------------------------------*
     * Une fonction de transfert en structure du board        *
     * Entrees : le tableau de board                          *
     * Sortie : la structure de Board                         *
     *--------------------------------------------------------*/
    IntsBoard rslt;
    rslt.pBO = toIntsForPiece(board, PBO);
    rslt.pBX = toIntsForPiece(board, PBX);
    rslt.pSO = toIntsForPiece(board, PSO);
    rslt.pSX = toIntsForPiece(board, PSX);
    rslt.pD = toIntsForPiece(board, PD);
    return rslt;
}

void toBoard(int board[][5],IntsBoard intsBoard){
    /*--------------------------------------------------------*
     * Une fonction de transfert en tableau du board          *
     * Entrees : le tableau de board                          *
     * Entrees : la structure de Board                        *
     *--------------------------------------------------------*/
    for (int i = 4;i > -1;i --){
        for (int j = 4;j > -1;j --){
            if (intsBoard.pBO & 1)  board[i][j] = PBO;
            else if (intsBoard.pBX & 1)  board[i][j] = PBX;
            else if (intsBoard.pSO & 1)  board[i][j] = PSO;
            else if (intsBoard.pSX & 1)  board[i][j] = PSX;
            else if (intsBoard.pD & 1)  board[i][j] = PD;
            else board[i][j] = ESPACE;
            intsBoard.pBO>>=1;
            intsBoard.pBX>>=1;
            intsBoard.pSX>>=1;
            intsBoard.pSO>>=1;
            intsBoard.pD>>=1;
        }
    }
}

void printBoardForInts(IntsBoard intsBoard, int y, int x) {
    /*--------------------------------------------------------*
     * Une fonction de l'affichage de tableau vide            *
     * Entrees : la struture du board                         *
     * Entrees : la coordonné du pièce mi                     *
     *--------------------------------------------------------*/
    printf("  ");
    int i, j;
    for (i = 1; i < 6; i++)
        printf("%4d", i);
    for (i = 0; i < 5; i++) {
        printf("\n   +---+---+---+---+---+\n");
        printf(" %c |", 'A' + i);
        for (j = 0; j < 5; j++) {
            if (y==i && x==j)
                printf("(%c)|", pickFromInts(intsBoard,i,j));
            else
                printf(" %c |", pickFromInts(intsBoard, i, j));
        }
    }
    printf("\n   +---+---+---+---+---+\n");
}

Cord minmax(IntsBoard intsBoard, int turn, int recursiveDepth,int maxLevel) {
    /*--------------------------------------------------------*
     * Une fonction kernel de trouver la solution d'arbre Min-*
     * Max                                                    *
     * Entrees : la struture du board                         *
     * Entrees : int qui représente la tourne                 *
     * Entrees : le niveau actuel                             *
     * Entrees : le niveau maximal                            *
     * Sortie  : le choix optimal en coordonné et sa valeur   *
     *--------------------------------------------------------*/
	Cord cord,rslt;

	IntsBoard newIntBoard;
    int valueMinMax;
    int finished = 1;
    valueMinMax = ISPAIR(recursiveDepth) ? INT_MIN : INT_MAX;

    //system("pause");
    if (recursiveDepth >= maxLevel) {
        rslt.x = -1;
        rslt.y = -1;
        rslt.value = scoreForInts(intsBoard, turn  ==  ISPAIR(recursiveDepth));
        return rslt;
    }
    //Trouver le Maximum lorsqu'il est pair
	for (int i = 0;i<5; i++)
		for (int j = 0; j < 5; j++){
            if (availableForInts(intsBoard,i,j,turn)) {
                finished = 0;
			    newIntBoard = putPieceToInts(intsBoard,i,j,turn);
                cord = minmax(newIntBoard, !turn, recursiveDepth + 1, maxLevel);
                if (ISPAIR(recursiveDepth)) {
                    if (cord.value>=valueMinMax){
                        valueMinMax = cord.value;
                        rslt.y = i;
                        rslt.x = j;
                    }
                }
                else{ 
                    if (cord.value<=valueMinMax){
                        valueMinMax = cord.value;
                        rslt.y = i;
                        rslt.x = j;
                    }
                }
            }
        }
    if (finished) {
        rslt.value = scoreForInts(intsBoard, turn == ISPAIR(recursiveDepth));
        if (rslt.value > 0) rslt.value = INT_MAX;
        else rslt.value = INT_MIN;
    };
    rslt.value = valueMinMax;
    return rslt;
}

void menu(int board[][5], int turn, LinkedList **p) {
    /*--------------------------------------------------------*
     * Une fonction de UI                                     *
     * Entrees : le tableau de board                          *
     * Entrees : int qui représente la tourne                 *
     *--------------------------------------------------------*/
    char txt[10];
    int y, x;
    LinkedList *pNew,*pDel;
    while (1) {
        printf("Vous etes le joueur de piece %c\n", TURN_TO_PIECE(turn));
        printf("Votre coup : ");
        scanf("%s", txt);
        if (strlen(txt) == 2 && txt[0] == 'U' && txt[1] == 'D') {
            if ((*p)->pre) {
                (*p) = (*p)->pre;
                toBoard(board, (*p)->value);
                //printBoardForInts(p->value, -1, -1);
                printBoard(board, -1, -1);
            }
            else printf("Il est le plus ancien\n");
            continue;
        }
        if (strlen(txt) == 2 && txt[0] == 'R' && txt[1] == 'D') {
            if ((*p)->next) {
                (*p) = (*p)->next;
                toBoard(board, (*p)->value);
                printBoard(board, -1, -1);
            }
            else printf("Il est le plus recent\n");
            continue;
        }
        while (strlen(txt) != 2 || !(in((int)(txt[0] - 'A'), (int)(txt[1] - '1')))) {
            printf("\n Excusez moi. Verifier votre saisie s'il vous plait,");
            printf("\n la saisie illegale \nVotre coup : ");
            scanf("%s", txt);
        }
        y = txt[0] - 'A';
        x = txt[1] - '1';
        if (available(board, y, x, turn)) {
            putPiece(board, y, x, turn);
            pNew = (LinkedList*) malloc(LEN);
            pNew->pre = (*p);
            pNew->value = toInts(board);
            pNew->next = NULL;
            if ((*p)->next){
                pDel = (*p)->next;
                while (pDel->next) {
                    pDel = pDel->next;
                    free(pDel->pre);
                }
                free(pDel);
            }
            
            (*p)->next = pNew;
            break;
        }
        printf("\n Excusez moi. Verifier votre saisie s'il vous plait,");
        printf("\n Le position ne valide pas \n");
    }
    printBoard(board, y, x);
}

void aiMinMax(int board[][5], int turn,int maxLevel){
    /*--------------------------------------------------------*
     * Une fonction joue le rôle de I.A. par l'arbre Min-max  *
     * Entrees : le tableau de board                          *
     * Entrees : int qui représente le tourne                 *
     * Entrees : int qui représente le niveau maximal         *
     *--------------------------------------------------------*/
    Cord cord;
    cord = minmax(toInts(board),turn,0, maxLevel);
    putPiece(board,cord.y,cord.x,turn);
    printf("L'ordinateur de haut niveau a mi sa piece %c dans la case %c%c\n", TURN_TO_PIECE(turn), cord.y + 'A', cord.x + '1');
    printBoard(board,cord.y,cord.x);
}

int main(int argc,char *argv[ ]){
    int board[5][5];
    int turn = PLAYER_X;
    int cpuPlays;
    int numTurn = 0;
    LinkedList *head,*p;
    cpuPlays = 1;
    if (argc>1) cpuPlays = argv[1][0] - 48;
    initialisationBoard(board);

    //#######init de liste chainé######
    head = (LinkedList*)malloc(LEN);
    head->next = NULL;
    head->value = toInts(board);
    head->pre = NULL;
    p = head;
    //###Fin de init de liste chainé###

    while (!finished(board, turn)) {
        if (cpuPlays && turn){
            if (numTurn < 2)
                aiGreedy(board, turn);
            else aiMinMax(board, turn, 4);
            p->value = toInts(board);
        }
        else{
            menu(board,turn,&p);
            p = p->next;
        }
        turn = ! turn; //change le tourne;
        numTurn ++;
    }
    score(board);
    system("pause");
    return 0;
}