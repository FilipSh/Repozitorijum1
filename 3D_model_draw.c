/*Kod koji iscrtava .obj 3d modele,
 * verovatno ne radi za sve .obj fajlove
 * jer sam ga napravio da radi samo za verzije
 * dobijene exportom iz programa meshLab*/
 
/*Funkcija draw_3d_model prima 3 argumenta: .obj fajl, broj cvorova i broj
 * trouglova */

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include "3D_model_draw.h"

//Maksimalna duzina linije koju ucitava fgets iz .obj fajla
#define MAX_INPUT_LINE_LENGTH 50

//F-ja error koja prijavljuje gresku prilikom neuspesnog otvaranja .obj fajla
static void error(){
	fprintf(stderr, "Error reading the 3D model file\n");
	exit(EXIT_FAILURE);
}

void draw_3d_model(char* file,int no_of_vertices,int no_of_triangles){

	//Alociranje 3 niza
	//prvi cuva tacke
	//drugi normale
	//treci trouglove, svaka 3 clana niza predstavljaju jedan trougao
	vertex* vertices=malloc(sizeof(vertex)*(no_of_vertices+2));
	vertex* vertices_normals=malloc(sizeof(vertex)*(no_of_vertices+2));
	triangle* triangles=malloc(sizeof(vertex)*no_of_triangles);
	
	//niska gde se cuva ucitana linija iz .obj
	char line[MAX_INPUT_LINE_LENGTH];
	
	//otvaranje .obj falja
	FILE* model_file = fopen(file, "r");
	
	//pozivanje f-je greska ukoliko nije dobro otvoren fajl
	if(model_file==NULL){
		error();
	}
	
	// tri brojaca
	// prvi je za normale
	//drugi za tacke
	//treci za trouglove
	int i=1;
	int j=1;
	int k=1;
	
	//Ucitava se linija iz fajla .obj
	//Ako pocinje sa 'v' znaci da je tacka,
	//Ako sa vn onda je normala 
	//AKo sa f onda je (face) trougao
	while(fgets(line,MAX_INPUT_LINE_LENGTH, model_file) != NULL){
		if(line[0]=='v'){
			char tmp[1];
			if(line[1]=='n'){
				//iz niske line izvlacimo potrebne informacije
				sscanf(line,"%s %lf %lf %lf",&tmp[0],&vertices_normals[i].x, &vertices_normals[i].y, &vertices_normals[i].z);
				i++;
			}
			else{
			
				sscanf(line,"%s %lf %lf %lf",&tmp[0],&vertices[j].x, &vertices[j].y, &vertices[j].z);
				j++;

			}
		}else if(line[0]=='f'){
			char tmp[1];
			char tmp1[MAX_INPUT_LINE_LENGTH/3+1];
			char tmp2[MAX_INPUT_LINE_LENGTH/3+1];
			char tmp3[MAX_INPUT_LINE_LENGTH/3+1];
			//iz niske line izvlacimo potrebne informacije
			sscanf(line,"%s %s %s %s",tmp,tmp1,tmp2,tmp3);
			char* a=strtok(tmp1,"/");
			char* b=strtok(tmp2,"/");
			char* c=strtok(tmp3,"/");
			triangles[k].x=atoi(a);
			triangles[k].y=atoi(b);
			triangles[k].z=atoi(c);
			k++;
		} else{
			//Ignorisemo sve ostale linije koje ne pocinju sa v,vt ili f
		}
	}
	//zatvaramo fajl
	fclose(model_file);
	
	//Iscrtavamo figuru koristeci prethodno dobijene informacije
	//izvucene iz .obj fajla
    glPushMatrix();
		glScalef(3,3,3);
		glRotatef(-90,1,0,0);
		glRotatef(45,0,0,1);
		glRotatef(180,0,0,1);
		
		//Ukljucujemo popunjavanje poligona
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for(int i = 1;i<=no_of_triangles;i++){
			glBegin(GL_TRIANGLES);
				
				//Delimo sa 500 zbog razmera
				glNormal3f(vertices_normals[triangles[i].x].x, vertices_normals[triangles[i].x].y, vertices_normals[triangles[i].x].z);
				glVertex3f(vertices[triangles[i].x].x/500.0, vertices[triangles[i].x].y/500.0, vertices[triangles[i].x].z/500.0);

				
				glNormal3f(vertices_normals[triangles[i].y].x,vertices_normals[triangles[i].y].y,vertices_normals[triangles[i].y].z);
				glVertex3f(vertices[triangles[i].y].x/500.0, vertices[triangles[i].y].y/500.0, vertices[triangles[i].y].z/500.0);
				
				
				glNormal3f(vertices_normals[triangles[i].x].x,vertices_normals[triangles[i].y].y,vertices_normals[triangles[i].y].z);
				glVertex3f(vertices[triangles[i].z].x/500.0, vertices[triangles[i].z].y/500.0, vertices[triangles[i].z].z/500.0);


			glEnd();
		}
   
	glPopMatrix();
	

	
}

