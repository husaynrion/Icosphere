#include <Gl/gl.h>
#include <Gl/glu.h>
#include <Gl/glut.h>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <time.h>

void display();
void reshape(int,int);
void timer(int);

#define num_of_triangles 5120

// num_of_triangles = 20 * ( 4^n );


float pi = 3.14159;


unsigned int int_pow(unsigned short num, unsigned short p){
    unsigned int result = 1;
    if(p!=0){
        for(unsigned short i=0; i<p; i++){
            result*=num;
        }
    }
    return result;
}

unsigned short log4(unsigned int num){
    unsigned short log=0;
    while(num!=1){
        num/=4;
        log++;
    }
    return log;
}

float to_rad(float degree){
    return degree*pi/180;
}




void hClearColor(int r, int g, int b,float a){
    float r1,g1,b1;
    r1 = r/255.0;
    g1 = g/255.0;
    b1 = b/255.0;
    glClearColor(r1,g1,b1,1.0);
}
void hColor3f(int r, int g, int b){
    float r1,g1,b1;
    r1 = r/255.0;
    g1 = g/255.0;
    b1 = b/255.0;
    glColor3f(r1,g1,b1);
}
void init(){
    hClearColor(1,1,1,1.0);
    glEnable(GL_DEPTH_TEST);
}




float radius=4;

float vertices[12][3];

float triangle[num_of_triangles][3][3];

unsigned int spacing = num_of_triangles/20;

void copy_vertices(float vertex[3], float *destination){
    for(unsigned short i=0; i<3; i++){
        *(destination + i) = vertex[i];
    }
}


void generate_vertices(){

    vertices[0][0] = 0;
    vertices[0][1] = 0;
    vertices[0][2] = radius;

    vertices[1][0] = 0;
    vertices[1][1] = 0;
    vertices[1][2] = -radius;

    float theta = atan(0.5);


    // Making vertices
    for(unsigned short i=0; i<5; i++){
        vertices[2+i][0] = radius*cos(theta)*cos( to_rad(72) * i );
        vertices[2+i][1] = radius*cos(theta)*sin( to_rad(72) * i );
        vertices[2+i][2] = radius*sin(theta);
    }
    for(unsigned short i=0; i<5; i++){
        vertices[7+i][0] = radius*cos(theta)*cos( to_rad(72) * i + to_rad(36) );
        vertices[7+i][1] = radius*cos(theta)*sin( to_rad(72) * i + to_rad(36) );
        vertices[7+i][2] = -radius*sin(theta);
    }
}

void generate_sphere(){

    // Making triangles

    // Upper 5

    for(unsigned short i=0; i<5; i++){


        copy_vertices(vertices[0],triangle[i*spacing][0]);

        for(int j=0; j<3; j++){
            copy_vertices(vertices[i+2],triangle[i*spacing][1]);
        }

        if(i!=4){
            copy_vertices(vertices[i+3],triangle[i*spacing][2]);
        }else{
            copy_vertices(vertices[2],triangle[i*spacing][2]);
        }
    }


    // Lower 5

    for(unsigned short i=0; i<5; i++){

        for(unsigned short j=0; j<3; j++){
            copy_vertices(vertices[1],triangle[(5+i)*spacing][0]);
        }

        for(unsigned short j=0; j<3; j++){
            copy_vertices(vertices[11-i],triangle[(5+i)*spacing][1]);
        }

        for(unsigned short j=0; j<3; j++){
            if(i!=4){
                copy_vertices(vertices[11-1-i],triangle[(5+i)*spacing][2]);
            }else{
                copy_vertices(vertices[11],triangle[(5+i)*spacing][2]);
            }
        }
    }


    // Upper Middle 5

    for(unsigned short i=0; i<5; i++){

        for(unsigned short j=0; j<3; j++){
            copy_vertices(vertices[i+2],triangle[(10+i)*spacing][0]);
        }

        for(unsigned short j=0; j<3; j++){
            if(i!=0){
                copy_vertices(vertices[i+7-1],triangle[(10+i)*spacing][1]);
            }else{
                copy_vertices(vertices[11],triangle[(10+i)*spacing][1]);
            }
        }

        for(unsigned short j=0; j<3; j++){
           copy_vertices(vertices[i+7],triangle[(10+i)*spacing][2]);
        }

    }


    // Lower Middle 5

    for(unsigned short i=0; i<5; i++){

        for(unsigned short j=0; j<3; j++){
            copy_vertices(vertices[11-i],triangle[(15+i)*spacing][0]);
        }

        for(unsigned short j=0; j<3; j++){
            if(i!=0){
                copy_vertices(vertices[7-i],triangle[(15+i)*spacing][1]);
            }else{
                copy_vertices(vertices[2],triangle[(15+i)*spacing][1]);
            }
        }

        for(unsigned short j=0; j<3; j++){
           copy_vertices(vertices[7-1-i], triangle[(15+i)*spacing][2]);
        }

    }

}





float newv[3][3];

unsigned short sub_div_num = log4(num_of_triangles/20);

void sub_divide(){
    float scale;
    for(unsigned short sdn=0; sdn<sub_div_num; sdn++){
        unsigned int n = 20 * int_pow(4,sdn);
        for(unsigned int i=0; i<n; i++){
                for(unsigned short k=0; k<3; k++){

                    newv[0][k] = triangle[i*spacing][0][k] + triangle[i*spacing][1][k];
                    newv[1][k] = triangle[i*spacing][1][k] + triangle[i*spacing][2][k];
                    newv[2][k] = triangle[i*spacing][2][k] + triangle[i*spacing][0][k];
                }




                for(unsigned short j=0; j<3; j++){
                    scale = radius/sqrt( newv[j][0]*newv[j][0]+ newv[j][1]*newv[j][1]+ newv[j][2]* newv[j][2]);
                    for(unsigned short k=0; k<3; k++){
                        newv[j][k] *= scale;
                    }
                }


                //        v0
                //       / \
                // newv0*   *newv2
                //     /     \
                //   v1---*---v2
                //      newv1


                unsigned int sub_spacing = spacing/4;


                // Spacing v1 and v2  and leaving v0 at its place

                copy_vertices( triangle[i*spacing][1],triangle[i*spacing + 1*sub_spacing][0] );


                copy_vertices( triangle[i*spacing][2],triangle[i*spacing + 2*sub_spacing][0] );


                // Adding other to points in anti-clockwise order after v0, v1 and v2 each

                copy_vertices( newv[0], triangle[i*spacing + 0*sub_spacing][1] );
                copy_vertices( newv[2], triangle[i*spacing + 0*sub_spacing][2] );

                copy_vertices( newv[1], triangle[i*spacing + 1*sub_spacing][1] );
                copy_vertices( newv[0], triangle[i*spacing + 1*sub_spacing][2] );

                copy_vertices( newv[2], triangle[i*spacing + 2*sub_spacing][1] );
                copy_vertices( newv[1], triangle[i*spacing + 2*sub_spacing][2] );

                // Adding the 4th and central triangle at last in anti-clockwise order

                copy_vertices( newv[0], triangle[i*spacing + 3*sub_spacing][0] );
                copy_vertices( newv[1], triangle[i*spacing + 3*sub_spacing][1] );
                copy_vertices( newv[2], triangle[i*spacing + 3*sub_spacing][2] );
        }


        spacing/=4;
    }
}



unsigned int color[num_of_triangles];

void set_color(){
    for(unsigned int i=0; i<num_of_triangles; i++){
            color[i] = 50+(rand()%200);
    }
}



int main(int argc, char* argv[]){
    srand(time(0));
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowPosition(200,100);
    glutInitWindowSize(500,500);

    glutCreateWindow("ICOSPHERE");

    generate_vertices();

    generate_sphere();

    sub_divide();

    set_color();

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);
    glutTimerFunc(1000,timer,0);
    init();

    glutMainLoop();

}

void sphereSurf(){
    for(unsigned int i=0; i<num_of_triangles; i++){

        if(i<num_of_triangles-2){hColor3f(color[i],color[i+1],color[i+2]);}else{hColor3f(color[i],color[i-1],color[i-2]);}

        for(unsigned int j=0; j<3; j++){
            glVertex3f(triangle[i][j][0],triangle[i][j][1],triangle[i][j][2]);
        }
    }
}

// Below is Not Important

float angle = 0.0;

void timer(int){
    glutPostRedisplay();
    glutTimerFunc(1000.0/10,timer,0);
    angle += 1;
    if(angle>2*pi){
        angle = angle-360.0;
    }

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0,0.0,-8.5);
    glRotatef(angle,1.0,0.0,0.0);
    glRotatef(angle,0.0,0.5,0.0);
    glBegin(GL_TRIANGLES);
    //draw
    sphereSurf();

    glEnd();
    glutSwapBuffers();

}

void reshape(int w, int h){
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,2.0,50.0);
    glMatrixMode(GL_MODELVIEW);
}

