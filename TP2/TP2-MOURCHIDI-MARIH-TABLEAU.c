#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "tp2.h"



char lireCase(Grille g,int l,int c){
    return g[l][c];
}

void initialiseGrille(Grille g ){
    for (int i=0;i<HAUTEUR;i++){
        for (int j=0;j<LARGEUR;j++){
            g[i][j]=' ';
        }
    }
}


void afficheGrille(Grille grille){
    for (int i=HAUTEUR-1; i>-1; i--){
        printf("||");
        for (int j=0; j<LARGEUR; j++){
             printf("%c", lireCase(grille,i,j));
        }
        printf("||\n");
    }
    
    for (int j=0; j<LARGEUR+4; j++){
            printf("|");
    }
    printf("\n");
    printf("  0");
    for (int j=0; j<LARGEUR; j++){
             if (j==9){
                 printf("1");
             }
             else{
                 printf(" ");
             }
    }
    printf("\n  ");
    for (int j=0; j<LARGEUR; j++){
            printf("%d", j%10);
    }
    printf("\n");
}


void genererPieces(Table_Piece tabPiece){



    tabPiece[1].hauteur = 2;
    tabPiece[1].largeur = 2;
    tabPiece[1].forme[1] = "@@";
    tabPiece[1].forme[0] = "@@";

    tabPiece[2].hauteur = 4;
    tabPiece[2].largeur = 1;
    tabPiece[2].forme[3] = "l";
    tabPiece[2].forme[2] = "l";
    tabPiece[2].forme[1] = "l";
    tabPiece[2].forme[0] = "l";

    tabPiece[3].hauteur = 1;
    tabPiece[3].largeur = 4;
    tabPiece[3].forme[0] = "llll";
   
    tabPiece[4].hauteur = 2;
    tabPiece[4].largeur = 3;
    tabPiece[4].forme[1] = "  #";
    tabPiece[4].forme[0] = "###";

    tabPiece[5].hauteur = 3;
    tabPiece[5].largeur = 2;
    tabPiece[5].forme[2] = "##";
    tabPiece[5].forme[1] = " #";
    tabPiece[5].forme[0] = " #";

    tabPiece[6].hauteur = 2;
    tabPiece[6].largeur = 3;
    tabPiece[6].forme[1] = "###";
    tabPiece[6].forme[0] = "#  ";

    tabPiece[7].hauteur = 3;
    tabPiece[7].largeur = 2;
    tabPiece[7].forme[2] = "# ";
    tabPiece[7].forme[1] = "# ";
    tabPiece[7].forme[0] = "##";


    tabPiece[8].hauteur = 2;
    tabPiece[8].largeur = 3;
    tabPiece[8].forme[1] = "ooo";
    tabPiece[8].forme[0] = " o ";

    tabPiece[9].hauteur = 3;
    tabPiece[9].largeur = 2;
    tabPiece[9].forme[2] = " o";
    tabPiece[9].forme[1] = "oo";
    tabPiece[9].forme[0] = " o";

    tabPiece[10].hauteur = 2;
    tabPiece[10].largeur = 3;
    tabPiece[10].forme[1] = " o ";
    tabPiece[10].forme[0] = "ooo";


    tabPiece[0].hauteur = 3;
    tabPiece[0].largeur = 2;
    tabPiece[0].forme[2] = "o ";
    tabPiece[0].forme[1] = "oo";
    tabPiece[0].forme[0] = "o ";
}


void affichePiece(Piece p){
    
    for (int i= p.hauteur-1; i>=0;i--){
        printf("\n        ");
        for (int j = 0; j<p.largeur;j++ ){
             printf("%c",p.forme[i][j]);
        }
    }
    printf("\n");
};


void ecrireCase(Grille g,int i,int j,char c){
    if (lireCase(g, i, j)==' ') g[i][j]=c;
}



void ecrirePiece(Grille g,Piece *p , int hauteur,int colonne){
    for (int i=0;i<p->hauteur;i++ ){
        for(int j=0;j<p->largeur;j++){
            if ((p->forme)[i][j]!=' ') ecrireCase(g,hauteur+i,colonne+j,(p->forme)[i][j]);
        }
    }
}



int nombre_aleatoire(){
    srand(time(NULL)); 
    int alea = (int)(((double)rand()/((double)RAND_MAX)) * (NB_PIECES));
    return alea;
}


//fonction qui renvoie le maximum entre 2 nombres
int maximum(int a,int b){
    if (a>b) return a;
    else return b;
}


//fonction qui donne la hauteur de l'emplacement a jouer dans une colonne
int hauteurColonne(int colonne,Grille g){
    int i;
    for ( i= HAUTEUR-1; i >-1 ; i--){
        if (lireCase(g,i,colonne)!=' ') return i+1;
    }
    return i;

}

//fonction qui calcul la hauteur à jouer d'une grille 
int hauteurAJouer(Grille g,Piece *piece,int colonne){
    int res = 0;
    for (int i = 0 ;i< (piece->largeur);i++){
        res = maximum(res , hauteurColonne(colonne+i,g));
    }
    return res;
}

//fonction qui supprime une ligne 
void supprimeLigne(Grille g ,int ligne){
    for (int i=ligne ; i<HAUTEUR ; i++){
        for (int j = 0 ; j < LARGEUR ; j++){

            //on teste si on se trouve en derniere ligne et on la rempli avec des ' ' 
            if (i==HAUTEUR-1) g[i][j]=' ';

            //on recopie la ligne de i+1 a i
            else g[i][j]=g[i+1][j];
            
        }
    }
}


//fonction qui calcule si une ligne est pleine
int LignePlein(Grille g , int ligne){
    for (int i=0  ; i<LARGEUR ; i++){
        if (lireCase( g ,ligne,i) == ' ') return 0;
    }
    return 1;
}

//fonction qui nettoie les la grille en supprimant les lignes pleines
void Nettoyage(Grille grille){
    for (int i=0;i<HAUTEUR;i++){
        if (LignePlein(grille,i)==1) supprimeLigne(grille,i);
    }
}

//fonction qui calcule la fin de jeu , 1 pour la fin de jeu et 0 dans le cas contraire
int finDeJeu(Piece *piece , int hauteurAJouer){
    if ((HAUTEUR-hauteurAJouer)>= (piece->hauteur) ) return 1;
    else return 0;
}



//fonction qui verifie si le schéma de la piece peut etre emboiter a une position (i,j) de la grille
int peut_Se_Poser (Grille g , Piece *piece, int hauteur,int colonne){
    for (int i=0 ; i<piece->hauteur ; i++){
        for (int j=0 ;j <piece->largeur ; j++){
            if ((piece->forme[i][j]!=' ') && g[hauteur+i][colonne+j]!=' ') return 0;
        }
    }
    return 1;
}


//fonction qui permet de calculer la hauteur exacte a jouer dans la grille
int hauteurExacte( Grille g, int col_gauche, Piece* piece ){
    int hauteur =hauteurAJouer(g,piece,col_gauche);
    while(peut_Se_Poser(g,piece,hauteur,col_gauche)==1){
        hauteur--;
    }
    return hauteur+1;
}



 

int tournerGauche( int position){
   switch (position){
            
            case 1:
                return 1;
            case 2:
                return 3;
            case 3:
                return 2;
            case 4:
                return 5;
            case 5:
                return 6;
            case 6:
                return 7;
            case 7:
                return 4;
            case 8:
                return 0;
            case 9:
                return 8;
            case 10:
                return 9;
            case 0:
                return 10;

            default :
                return -1;
    }
}


int tournerDroite( int position){
   switch (position){
            
            case 1:
                return 1;
            case 2:
                return 3;
            case 3:
                return 2;
            case 4:
                return 7;
            case 5:
                return 4;
            case 6:
                return 5;
            case 7:
                return 6;
            case 8:
                return 9;
            case 9:
                return 10;
            case 10:
                return 0;
            case 0:
                return 8;

            default :
                return -1;
            }
}






int main(int argc, char *argv[])
{

    
    Table_Piece tabPiece;
    Grille g;
    Piece piece;
    int colonne=0;
    int hauteur;
    int piece_jouer = 0;
    int position_piece;
    char str[ 8 ];
    initialiseGrille(g );
    


    while(colonne!=-1){
        genererPieces(tabPiece);
        position_piece=nombre_aleatoire();
        piece = tabPiece[position_piece];
        afficheGrille(g);
        affichePiece(piece);



        //boucle pour tourner une piece avant de la jouer
        while ( 1 ) {
            printf( "(g)auche, (d)roite ou (0-14) colonne: " );
            if ( scanf( "%7s", str ) == 1 ) {
                if ( str[ 0 ] == 'g' ) { 
                    position_piece = tournerGauche(position_piece);
                    piece = tabPiece[position_piece];
                }
                else if ( str[ 0 ] == 'd' ) { 
                    position_piece = tournerDroite(position_piece);
                    piece = tabPiece[position_piece];
                     }
                else {
                    colonne = atoi( str );
                    break;
                }
          
            affichePiece(piece);
            }
        }




        //--tant que la colonne est invalide on entre une nouvelle valeur
        while((colonne<-1)||(colonne>LARGEUR-piece.largeur)){
             printf(" \n Veuillez entrez une colonne entre 0 et %d : ",(LARGEUR-piece.largeur));
             scanf("%d", &colonne);
        }


        //on calcule la position a jouer
        //hauteur=hauteurAJouer(g,&piece,colonne);
        hauteur = hauteurExacte(g,colonne,&piece);

        //on teste si on est en fin de partie
        if (finDeJeu(&piece,hauteur)==1) {
            ecrirePiece(g,&piece,hauteur,colonne);
            Nettoyage(g);
            piece_jouer++;
        }

        //--on quitte le jeu
        else {
            printf(" \n Vous avez Perdu ! \n Vous avez reussi a placer %d piece \n",piece_jouer);
            piece_jouer=0;
            initialiseGrille(g);  
        }
        

    }
    
    return 0;
}
