//header koji sadrzi strukture potrebne za iscrtavanje 3d modela,
//tacka i trougao kao i dekleraciju f-je

typedef struct{
	double x;
	double y;
	double z;
}vertex;

typedef struct{
	int x;
	int y;
	int z;
}triangle;


void draw_3d_model(char* file,int no_of_vertices,int no_of_triangles);
