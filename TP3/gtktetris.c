//////////////////////////////////////////////////////////////////////////////////

////////              MOURCHIDI & MARIH TP3 TETRIS                          //////

//////////////////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include "tetris.h"


///                            713dd495cb8bf12322d22964aa84898e



typedef struct  {
    Grille grille;
    Piece tab[ NB_PIECES ];
    int colonne_a_jouer;
    int hauteur_a_jouer;
    int position_piece;
    int score;
    int delai;
    int fin;
    Piece piece;
    GtkWidget* drawing_area; //le canvas
    GtkWidget* label_delay; //pour mettre à jour le label avec le delai
    GtkWidget* label_score; //pour mettre à jour le label avec le score
    GtkWidget* window;
} Jeu;



void initJeu(Jeu *jeu);




// Cette fonction est à mettre en dehors du main, comme toute fonction.
GtkWidget *create_arrow_button( GtkArrowType  arrow_type )
{ // Les boutons sont en fait des conteneurs à un seul élément.
  GtkWidget* button = gtk_button_new ();
  GtkWidget* arrow = gtk_arrow_new (arrow_type, GTK_SHADOW_OUT);
  gtk_container_add (GTK_CONTAINER (button), arrow);
  gtk_widget_show_all( button );
  return button;
}






void dessineCarre( cairo_t* cr, int ligne, int colonne, char c ){
    int colg = (colonne+2)*TAILLE_CARRE;
    int ligneg = (ligne+4)*TAILLE_CARRE;
    switch (c){
        case '@':
            cairo_set_source_rgb (cr, 0.16, 0.92, 0.02); //Couleur fond: vert clair
            cairo_rectangle (cr, colg, ligneg, TAILLE_CARRE, TAILLE_CARRE ); // x, y, largeur, hauteur
            cairo_fill_preserve( cr ); // remplit la forme actuelle (un rectangle)
            // => "_preserve" garde la forme (le rectangle) pour la suite
            cairo_set_line_width(cr, 3);
            cairo_set_source_rgb (cr, 0.20, 0.5, 0.14); //Couleur border: vert dark
            cairo_stroke( cr );
            break;
        case 'o':
            cairo_set_source_rgb (cr, 0.85, 0.92, 0.05); //Couleur fond: jaune clair
            cairo_rectangle (cr, colg, ligneg, TAILLE_CARRE, TAILLE_CARRE ); // x, y, largeur, hauteur
            cairo_fill_preserve( cr ); // remplit la forme actuelle (un rectangle)
            // => "_preserve" garde la forme (le rectangle) pour la suite
            cairo_set_line_width(cr, 3);
            cairo_set_source_rgb (cr, 0.53, 0.56, 0.12); //Couleur border: jaune dark
            cairo_stroke( cr );
            break;
        case '#':
            cairo_set_source_rgb (cr, 0.96, 0.13, 0.06); //Couleur fond: rouge clair
            cairo_rectangle (cr, colg, ligneg, TAILLE_CARRE, TAILLE_CARRE ); // x, y, largeur, hauteur
            cairo_fill_preserve( cr ); // remplit la forme actuelle (un rectangle)
            // => "_preserve" garde la forme (le rectangle) pour la suite
            cairo_set_line_width(cr, 3);
            cairo_set_source_rgb (cr, 0.62, 0.18, 0.15); //Couleur border: rouge dark
            cairo_stroke( cr );
            break;
        case 'l':
            cairo_set_source_rgb (cr, 0.08, 0.96, 0.88); //Couleur fond: bleu_ciel clair
            cairo_rectangle (cr, colg, ligneg, TAILLE_CARRE, TAILLE_CARRE ); // x, y, largeur, hauteur
            cairo_fill_preserve( cr ); // remplit la forme actuelle (un rectangle)
            // => "_preserve" garde la forme (le rectangle) pour la suite
            cairo_set_line_width(cr, 3);
            cairo_set_source_rgb (cr, 0.11, 0.60, 0.56); //Couleur border: bleu_ciel dark
            cairo_stroke( cr );
            break;
        
        default:
            break;
    }
}



//DESSINE PIECE
void dessinePiece( cairo_t* cr, Jeu* Jeu ){
    Piece piece = Jeu->tab[Jeu->position_piece];
    for (int i=piece.hauteur-1; i>=0; i--){
        for (int j=0; j<piece.largeur; j++){ 
            if (piece.forme[i][j]!=' '){
                dessineCarre(cr, -i-1+(Jeu->hauteur_a_jouer), j+Jeu->colonne_a_jouer, piece.forme[i][j]);
            }         
        }
    }
}


void pieceAleatoire(Jeu *jeu){
//fonction permettant de selectionner une pièce aléatoire
  int nb=nombre_aleatoire();
  jeu->position_piece=nb;
  jeu->piece = jeu->tab[nb];
}


void mis_a_jour(Jeu *jeu){
  int comparer =hauteurColonne(0,jeu->grille);//variable permettant de comparer si on a nettoyé la grille pour ajouter un bonus au score
  Nettoyage(jeu->grille); //on efface les lignes pleines
  if (comparer !=hauteurColonne(0,jeu->grille)) jeu->score = jeu ->score +10 ;
  jeu->score ++;

  afficheGrille(jeu->grille); //test avec la grille en 2D
  pieceAleatoire(jeu);
  jeu->hauteur_a_jouer =0; // on reinitialise la position de la piece
  jeu->colonne_a_jouer =LARGEUR/2; // on reinitialise la position de la piece

}





//DESSIN LA GRILLE
void dessineGrille( cairo_t* cr, Grille g ){
   cairo_set_source_rgb (cr, 0.25, 0.22, 0.90); // choisit le fond bleu
   cairo_paint( cr ); // remplit tout dans la couleur choisie.

   cairo_set_source_rgb (cr, 1, 1, 1); // choisit le blanc
   cairo_rectangle (cr, 2*TAILLE_CARRE, 0, TAILLE_CARRE*(LARGEUR+4)-4*TAILLE_CARRE, TAILLE_CARRE*(HAUTEUR+7)-2*TAILLE_CARRE); // x, y, largeur, hauteur.
   cairo_fill( cr ); // remplit la forme actuelle (un rectangle) avec une couleur Blanc
   for (int i=0; i<HAUTEUR; i++){
        for (int j=0; j<LARGEUR; j++){
                   if (lireCase(g, i, j)!=' '){ //Si on a une case qui contient quelque chose
                           dessineCarre(cr, HAUTEUR-i, j, lireCase(g, i, j)); //On déssine un carré, si l'emplacement est libre  
                   }
           }
   }
   //Dessin rectangle gris
   cairo_set_source_rgb (cr, 0.6, 0.6, 0.67); // choisit le gris
   cairo_rectangle(cr, 2*TAILLE_CARRE, 0, TAILLE_CARRE*(LARGEUR+4)-4*TAILLE_CARRE, 5*TAILLE_CARRE);
   cairo_fill(cr);
}



gboolean realize_evt_reaction( GtkWidget *widget, gpointer data ){ // force un événement "expose" juste derrière.
   gtk_widget_queue_draw( widget ); 
   return TRUE;
 }

 gboolean Gauche(GtkWidget *widget, gpointer data ){ 
    Jeu *jeu =(Jeu*)data;
    if (jeu->colonne_a_jouer>0) jeu->colonne_a_jouer--;
    gtk_widget_queue_draw( jeu->window); 
    return TRUE;
 }

  gboolean Droite(GtkWidget *widget, gpointer data ){ // force un événement "expose" juste derrière.
    Jeu *jeu =(Jeu*)data;
    int largeur_piece = jeu->tab[jeu->position_piece].largeur;
    if (jeu->colonne_a_jouer<14+1-largeur_piece) jeu->colonne_a_jouer++;
    gtk_widget_queue_draw( jeu->window); 
    return TRUE;
 }

  gboolean Bas(GtkWidget *widget, gpointer data ){ // force un événement "expose" juste derrière.
    Jeu *jeu =(Jeu*)data;
    int hauteur =hauteurExacte( jeu->grille, jeu->colonne_a_jouer, &(jeu->piece) );
    ecrirePiece(jeu->grille,&jeu->piece,hauteur,jeu->colonne_a_jouer);
    mis_a_jour(jeu);
    gtk_widget_queue_draw( jeu->window);   
    return TRUE;
 }




int PeutTourner(Jeu *jeu){
  //fonction permettant de savoir si one peut effectuer une rotation selon la configuration de la grille 

  if (((jeu->colonne_a_jouer) + (jeu->piece).hauteur )  >= LARGEUR ){
    return 0;
  }


  for(int i=0;i<(jeu->piece).hauteur ;i++){
    //if (lireCase(jeu->grille, jeu->hauteur_a_jouer, (jeu->colonne_a_jouer)+ i)!=' ') return 0; 
  }

  return 1;
}


  gboolean RotGauche(GtkWidget *widget, gpointer data ){ // force un événement "expose" juste derrière.
    Jeu *jeu =(Jeu*)data;
    if (PeutTourner(jeu)==1){
      jeu-> position_piece = tournerGauche(jeu->position_piece);
      jeu->piece = jeu->tab [jeu-> position_piece ];
    }
    gtk_widget_queue_draw( jeu->window);   //redessine la zone de dessin
    return TRUE;
 }



   gboolean RotDroite(GtkWidget *widget, gpointer data ){ // force un événement "expose" juste derrière.
    Jeu *jeu =(Jeu*)data;
    if (PeutTourner(jeu)==1){
    jeu-> position_piece = tournerDroite(jeu->position_piece);
    jeu->piece = jeu->tab [jeu-> position_piece ];
   
  }
  gtk_widget_queue_draw( jeu->window);    //redessine la zone de dessin
  return TRUE;
 }

void dessendrePiece(Jeu * jeu){
//fonction qui permet de dessendre une piece


  int hauteur =hauteurExacte( jeu->grille, jeu->colonne_a_jouer, &(jeu->piece) );

  //on dessend la piece sans l inscrire dans la grille
  if ((jeu->hauteur_a_jouer <= 10-hauteur)||jeu->hauteur_a_jouer<=2) {
    jeu->hauteur_a_jouer++; //on deplace la pice vers le bas
  }
  //on inscrit la piece dans la grille
  else {

    //on teste si on peut jouer
    if(10-hauteur >=(jeu->piece).hauteur ){
      ecrirePiece(jeu->grille,&jeu->piece,hauteur,jeu->colonne_a_jouer);
      mis_a_jour(jeu);
    }

    //fin de jeu
    else{
      printf("Fin du jeu \n");
      GtkWidget *pAbout;
   
    // Definition DIALOG
    pAbout = gtk_message_dialog_new(GTK_WINDOW(jeu->window), //INITIALISATION DIALOGUE
                      GTK_DIALOG_MODAL, // DIALOGUE
                      GTK_MESSAGE_INFO, // TYPE INFORMATION
                      GTK_BUTTONS_OK, //BOUTTON OK
                      "Fin du jeu \n"
                      " score : %d \n"
                      "temps : %d \n" 
                      "\nINFO504 TETRIS- TP3\n2019/2020\nL3 INFO S5" ,jeu->score,jeu->delai);
    gtk_dialog_run(GTK_DIALOG(pAbout)); //Exécution du dialogue
    gtk_widget_destroy(pAbout); //Destruction quand ok cliquer

    jeu->fin = 1;
    }
  }
  gtk_widget_queue_draw( jeu->window);   


}

 // c'est la réaction principale qui va redessiner tout.
 gboolean expose_evt_reaction( GtkWidget *widget, GdkEventExpose *event, gpointer data )
 {
   // c'est la structure qui permet d'afficher dans une zone de dessin
   // via Cairo 
    Jeu *jeu = (Jeu*)data ;
   cairo_t* cr = gdk_cairo_create (widget->window);
   dessineGrille(cr,jeu->grille);
   dessinePiece(cr,jeu);
   cairo_destroy (cr);
   return TRUE;
 }


//timer
gint tic( gpointer data )
{
  Jeu* pJeu = (Jeu*) data;
  int delai ;
  char valeur[16] ;
  if (pJeu->fin ==0){
    delai = atoi(gtk_label_get_text( GTK_LABEL( pJeu->label_delay )));
    delai ++;
    sprintf(valeur,"%d",delai++);
    pJeu->delai = delai-1 ;
    gtk_label_set_text( GTK_LABEL( pJeu-> label_delay), valeur );
    sprintf(valeur,"%d",(pJeu->score));
    gtk_label_set_text( GTK_LABEL( pJeu-> label_score), valeur );
    dessendrePiece(pJeu);

  }
else{

  gtk_label_set_text( GTK_LABEL( pJeu-> label_delay), "0" );
}
  g_timeout_add (1000, tic, (gpointer) pJeu ); // réenclenche le timer.
  return 0;
}


void initJeu(Jeu *jeu){
    initialiseGrille(jeu->grille) ;
    genererPieces(jeu->tab);
    jeu->colonne_a_jouer=LARGEUR/2; //on place la piece au milieu de la ligne de départ
    jeu->hauteur_a_jouer = 0;
    pieceAleatoire(jeu);
    jeu->fin = 0;
    jeu->score =0 ;
    jeu->delai =0;

}


gboolean new(GtkWidget *widget, gpointer data ){
    Jeu *jeu =(Jeu*)data;
    initJeu(jeu);
    gtk_widget_queue_draw( jeu->window);   
    return TRUE;
}


void creerIHM( Jeu* jeu ){

  
    /* Crée une fenêtre. */
    jeu-> window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    // Crée les boutons
    GtkWidget* button_quit;
    GtkWidget* button_new;
    GtkWidget* left = create_arrow_button( GTK_ARROW_LEFT );
    GtkWidget* right = create_arrow_button( GTK_ARROW_RIGHT );
    GtkWidget* down = create_arrow_button( GTK_ARROW_DOWN );
    GtkWidget* rotationdroit;
    GtkWidget* rotationgauche ;



    //crée les labels
    GtkWidget* delay = gtk_label_new( "delay" );
    jeu->label_delay = gtk_label_new( "0" );

    GtkWidget* score = gtk_label_new( "score" );
    jeu->label_score = gtk_label_new( "0" );


    button_quit = gtk_button_new_with_label ( "Quit" );
    button_new = gtk_button_new_with_label ( "New" );
    rotationgauche = gtk_button_new_with_label ( "RotGauche" );
    rotationdroit = gtk_button_new_with_label ( "RotDroit" );
    
    g_signal_connect( button_quit, "clicked", G_CALLBACK( gtk_main_quit ), NULL);
    g_signal_connect( button_new, "clicked", G_CALLBACK( new ), jeu);
    g_signal_connect( rotationdroit, "clicked", G_CALLBACK( RotDroite ), jeu);
     g_signal_connect( rotationgauche, "clicked", G_CALLBACK( RotGauche ), jeu);
    g_signal_connect( left, "clicked", G_CALLBACK( Gauche), jeu);
    g_signal_connect( right, "clicked", G_CALLBACK( Droite), jeu);
    g_signal_connect( down, "clicked", G_CALLBACK( Bas), jeu);

    jeu->drawing_area = gtk_drawing_area_new ();
    // largeur=150 pixels, hauteur = 100 pixels.

    gtk_widget_set_size_request (jeu->drawing_area, TAILLE_CARRE*(LARGEUR+4), TAILLE_CARRE*(HAUTEUR+7));


    g_signal_connect( G_OBJECT(jeu->drawing_area), "realize", //Appel à l'initialisation
    G_CALLBACK(realize_evt_reaction), jeu );
      g_signal_connect( G_OBJECT (jeu->drawing_area), "expose_event", //Appeler à chaque changement
    G_CALLBACK (expose_evt_reaction), jeu );





    // FALSE: les composants internes n'auront pas forcément la même largeur. 
    // TRUE: les composants internes auront forcément la même largeur. 
    // 10: bords autour des composants.
    GtkWidget* hbox1 = gtk_hbox_new (FALSE, 10);
    GtkWidget* hbox2 = gtk_hbox_new (TRUE, 10);
    GtkWidget* hbox3 = gtk_hbox_new (TRUE, 10);
    GtkWidget* hbox4 = gtk_hbox_new (TRUE, 10);
    GtkWidget* hbox5 = gtk_hbox_new (TRUE, 10);
    GtkWidget* hbox6 = gtk_hbox_new (TRUE, 10);
    GtkWidget* vbox1 = gtk_vbox_new (FALSE, 10);
    GtkWidget* vbox2 = gtk_vbox_new (TRUE, 10);
    

    gtk_container_add ( GTK_CONTAINER (hbox2), button_new );
    gtk_container_add ( GTK_CONTAINER (hbox2), button_quit );


    gtk_container_add ( GTK_CONTAINER (hbox3), delay );
    gtk_container_add ( GTK_CONTAINER (hbox3), jeu->label_delay );


    gtk_container_add ( GTK_CONTAINER (hbox4), score );
    gtk_container_add ( GTK_CONTAINER (hbox4), jeu->label_score );

    //button arrow
    gtk_container_add ( GTK_CONTAINER (hbox5), left );
    gtk_container_add ( GTK_CONTAINER (hbox5), down );
    gtk_container_add ( GTK_CONTAINER (hbox5), right );

    //boutton rotation
    gtk_container_add ( GTK_CONTAINER (hbox6), rotationgauche );
    gtk_container_add ( GTK_CONTAINER (hbox6), rotationdroit );

    gtk_container_add ( GTK_CONTAINER (vbox1), jeu->drawing_area );
    gtk_container_add ( GTK_CONTAINER (vbox1), hbox6 );
    gtk_container_add ( GTK_CONTAINER (vbox1), hbox5 );
    gtk_container_add ( GTK_CONTAINER (vbox2), hbox2 );
    gtk_container_add ( GTK_CONTAINER (vbox2), hbox3 );
    gtk_container_add ( GTK_CONTAINER (vbox2), hbox4 );


    gtk_container_add ( GTK_CONTAINER (hbox1), vbox1 );
    gtk_container_add ( GTK_CONTAINER (hbox1), vbox2 );


    //ajout du container dans windows
    gtk_container_add( GTK_CONTAINER( jeu->window ), hbox1 ); 

    //on affiche tous les composants
    gtk_widget_show_all( jeu->window );

///timer
    g_timeout_add (100, tic, jeu );


}




int main( int   argc,
          char *argv[] )
{

    /* Passe les arguments à GTK, pour qu'il extrait ceux qui le concernent. */
    gtk_init (&argc, &argv);
    
    Jeu jeu;
    initJeu(&jeu);
    creerIHM(&jeu);
    gtk_main ();
    return 0;
}

