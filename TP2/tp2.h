

#define HAUTEUR 10
#define LARGEUR 15
#define NB_PIECES 11
#define HAUTEUR_MAX_DES_PIECES 4

typedef  char Grille[HAUTEUR][LARGEUR];
 typedef struct{
    int hauteur;
    int largeur;
    char* forme[HAUTEUR_MAX_DES_PIECES];
}Piece;

typedef Piece Table_Piece[NB_PIECES];
//--grille
void initialiseGrille(Grille g);
void afficheGrille(Grille g );

//--case
void ecrireCase(Grille g,int i,int j,char c);
char lireCase(Grille g , int l , int c );

//--piece
void genererPieces(Table_Piece tabPiece);
void affichePiece (Piece p);
void ecrirePiece(Grille g,Piece *p , int hauteur,int colonne);
Piece pieceAleatoire( Table_Piece tabPiece);
int tournerGauche( int position);
int tournerDroite( int position);

//--other
int maximum(int a,int b);
int hauteurColonne(int colonne,Grille g);
int hauteurAJouer(Grille g,Piece *piece,int colonne);

//--Ligne
void supprimeLigne(Grille grille ,int ligne);
int finDeJeu(Piece *piece , int hauteurAJouer);
int LignePlein(Grille g , int ligne);
int peut_Se_Poser (Grille g , Piece *piece, int hauteur,int colonne);





//#endif
