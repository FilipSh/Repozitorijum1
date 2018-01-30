/*Projekat iz racunarske grafike - no stone unturned
 *Filip Sasic 135/2015
 * 
 * Ovaj prjekat je igra koja se igra u dva igraca, igra se na tabli
 * slicnoj sahovskoj
 
 *Neki komentari za neke delove koda su uzeti sa vezbi iz racunarske grafike 2017/2018 skolska
  godina.
 
  *URADITI DANAS(OVO OBRISATI POSLE, OVO JE PORUKA ZA MENE):
  *	- napraviti main, inicijalizovati glut, napisati neke callback funkcije,pogled,matrica...
  * - iscrtati tablu za igru
  * - uraditi osvetljenje(samo povrsno, doterati kasnije)
  * - napisati kod koji ucitava .obj 3d modele skinute sa interneta
  * - ucitati modele i postaviti ih na svoja mesta(samo par cisto da vidim dal sve lepo rado)
  * - napisati funkciju za pisanje teksta po ekranu(trebace ksanije)
  * - napistai funkciju koja preslikava koordinate prozora u koordinate sveta(tebace da se koordinate kilka misa pretvore u koordinate sveta)
  */

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include "lighting.h"
#include "3D_model_draw.h"
#include <string.h>

/* Deklaracije callback funkcija*/
static void on_reshape(int width, int height);
static void on_display(void);
static void on_mouse (int button, int state, int x, int y);

/*Dekleracije ostalih funkcija */
/*TODO: Naprviti header fajli i funkcije lepo razvrstati po fajlovima*/ 
void draw_board(void);
void write_text_on_screen(char* input_string,int x,int y);
void GetOGLPos(int mouse_x, int mouse_y,double* x,double* y,double* z);


int main(int argc, char **argv){
	
	/* Inicijalizuje se GLUT*/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    
	/* Kreira se prozor*/
    glutInitWindowSize(1300, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

	/* Registruju se callback funkcije*/
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutMouseFunc(on_mouse);
    
	/* Obavlja se OpenGL inicijalizacija*/
	
	/*Postavlja se boja kojom ce se cistiti bufer*/
    glClearColor(0, 0, 0, 0);
    /* Ukljucuje se testiranje z-koordinate piksela*/
    glEnable(GL_DEPTH_TEST);
    /*Ukljucuje se skaliranje vektora normala*/
    glEnable(GL_NORMALIZE);
	
	/* Program ulazi u glavnu petlju*/
    glutMainLoop();

    return 0;
}


static void on_reshape(int width, int height){
    /* Postavlja se viewport*/
    glViewport(0, 0, width, height);

    /* Postavljaju se parametri projekcije*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 500);
}

static void on_display(){
	
	/* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*Poziva se funkcija lighting koja podesava osvetljenje
	 * i definisana je u odvojenom falju*/
	
	/*NOTE : Osvetljenje ja zasada samo kopirano sa vezbi da bi isprobali
	 * da li sve radi, uraditi lepo soveljenje kasnije*/
	
	lighting();
	
	/* Inicijalizuje se matrica transformacije. */
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(7, 10, 10, 7, 0, 0, 0, 1, 0);
    

    /* Ova figura ima previse troulgova i trosi previse vremena da bi se iscrtala*/
    //~ glPushMatrix();
		//~ glTranslatef(1,1,-1);
		//~ glScalef(0.7,0.7,0.7);
		//~ glRotatef(180,0,1,0);
		//~ draw_3d_model("ogre.obj",892072,1757186);
	//~ glPopMatrix();
	
	/*Iscrtavaju se figure vuk i konj pomocu f-je draw_3d_model
	 * koja je definisana u odvojenom falju*/
	 
	glPushMatrix();
		glTranslatef(3,1,-1);
		glScalef(0.3,0.3,0.3);
		glRotatef(-135,0,1,0);
		draw_3d_model("wolf.obj",4496,1784);

	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(5,1,-1);
		glScalef(0.3,0.3,0.3);
		glRotatef(-135,0,1,0);
		draw_3d_model("horse.obj",30258,30961);

	glPopMatrix();
	
	
	/*iscrtava se tabla*/
	draw_board();    
    
	glutSwapBuffers();
	
}

/* Definicija Funkcije za crtanje table*/
void draw_board(void){      
	
	glPushMatrix();
		glTranslatef(1,0,-1);
		glTranslatef(14,0,2);
	
		for(int i=0;i<7;i++){
			glTranslatef(-14,0,-2);
			
			for(int j=0;j<7;j++){
				if((j+i)%2==0){
					GLfloat ambient_coeffs[] = { 1, 1, 1, 1 };
					GLfloat diffuse_coeffs[] = { 1, 1, 1, 1 };

					glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		

				}else{
					GLfloat ambient_coeffs[] = { 0.098, 0.074, 0.443, 1 };
					GLfloat diffuse_coeffs[] = { 0.098, 0.074, 0.443, 1 };

					glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);

				}
				glutSolidCube(2);
				glTranslatef(2,0,0);
			}
		}
		
		
	glPopMatrix();
	
}

/* Funkicja za ispis teksta na ekran, preuzeto iz man strana, izmenjeno i
 * prilagodjeno potrebama projekta, trenutno nisam siguran sta rade
 * f-je glRasterPos2f i glutBitmapCharacter, proucicu kasnije, jer nije
 * okej stavljati tudji kod a da ne znam tacno kako funkcionise */
 
void write_text_on_screen(char* input_string,int x,int y){ 
		
	int len, i;
	glRasterPos2f(x, y);
	len = (int) strlen(input_string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, input_string[i]);
	 }  
}

/* Funkicija koja detektuje klik misem */

void on_mouse (int button, int state, int x, int y){
	double p_x,p_y,p_z;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		/* Poziv funkcije koja pretvara koordinate  prozora u koordinate sveta*/
		GetOGLPos(x,y,&p_x,&p_y,&p_z);
		/* Testiram dal lepo radi */
		printf("%lf %lf %lf\n",p_x,p_y,p_z);
		
  }
}
/* Funkcija koja pretvara koordinate  prozora u koordinate sveta, 
 * preuzeto sa http://nehe.gamedev.net/article/using_gluunproject/16013/
 * izmenjeno za potrebe kursa, napisati kasnije kako tacno radi*/
void GetOGLPos(int mouse_x, int mouse_y,double* x,double* y,double* z){
    int viewport[4];
    double modelview[16];
    double projection[16];
    float winX, winY, winZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)mouse_x;
    winY = (float)viewport[3] - (float)mouse_y;
    glReadPixels( mouse_x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, x, y, z);
}


