//------------------------------------------------------------------------------------*
//                                                                                    *
//  This is COMP 410 - 510 Assigment 3                                                *
//                                                                                    *
//  Author: Ahmet Can Turgut - 40616                                                  *
//  Bouncing Ball With Texture and Lightning                                          *
//  Spring 2017 - Koc Unviversity - CE                                                *
//                                                                                    *
//  Skeleton code from spinning cube from Angel's Book...                             *
//  Sphere code taken from angels book and edited according to needs...               *
//  This code works in macOS 10.8 and Below otherwise some methods depracated...      *
//                                                                                    *
//--------------------------------------------------------------------------------------

// Specialities

// Two diffrent meterial: Metallic, Plastic --> Meterial Shiness Decresse or Incrase
// Two diffrent texture : Earth, Basketball
// Colors               : Green, Red, Blue, White --> When You Select Lost Texture !
// Lights               : ON / OFF
// Perspective view     : Done
// Shading Mode         : Phong - Gourad

// REFERANCES: SHPARE NORMALIZATION HINT TAKEN FROM THIS WEB PAGE
// --> REF https://en.wikibooks.org/wiki/GLSL_Programming/GLUT/Textured_Spheres
// IMAGE READING TAKEN FROM LECTURES NOTES

// IN GOURAD VIEW YOU CAN SEE BOTH LIGHT SOURCES  !! IMPORTANT NOTE



//--------------------------------------------------------------------------------------

#include <iostream>
#include "Angel.h" // Go to this web page "https://www.cs.unm.edu/~angel/BOOK/INTERACTIVE_COMPUTER_GRAPHICS/SIXTH_EDITION/CODE/include/Angel.h"

using namespace std;

// Initilizing Variables -----------------------------------------------------------------------------------------------------------------------

typedef vec4  color4;
typedef vec4  point4;

// Movement Related Vectors Defined  -----------------------------------------------------------------------------------------------------------

bool lightMove = false;

vec3 location(0.0,0.8,-2.8);
vec3 velocity(0.0,0.0000001,0.0);
vec3 accelaration(0.0,0.00000001,0.0);
vec4 myWhite(1.0, 1.0, 1.0, 1.0);
vec4 myColor = myWhite;

// Menu Related Inits  -----------------------------------------------------------------------------------------------------------------------

static int window;
static int menu_id;
static int submenu_id;
static int submenu_id2;
static int submenu_id3;
static int submenu_id4;
static int submenu_id5;
static int submenu_id6;
static int submenu_id7;
static int submenu_id8;

static int valueMenu = 0;

// Color Selector "KEY SELECT HOLDER" ---------------------------------------------------------------------------------------------------------

int colorSelector = 0;

// Light Set Up -----------------------------------------------------------------------------------------------------------------------

point4 light_position( 10.0, 20.0, -5.0, 1.0 );
color4 light_ambient( 0.1, 0.1, 0.1, 1.0 );
color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

color4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
color4 material_diffuse( 1.0, 0.8, 0.4, 1.0 );
color4 material_specular( 1.0, 0.8, 0.0, 1.0 );

color4 ambient_product2;
color4 diffuse_product2;
color4 specular_product2;

float  material_shininess = 1.0;

// View related inits -----------------------------------------------------------------------------------------------------------------------

enum { Xaxis   = 0,
    Yaxis   = 1,
    Zaxis   = 2,
    NumAxes = 3 };

int Axis = Xaxis;
int Zmeter = Zaxis;

GLfloat Theta[NumAxes] = { 0.0, -20, 20};
GLfloat aspect;
GLuint  ModelView, Projection;
GLuint  program;

// Color method initilized -----------------------------------------------------------------------------------------------------------------------

void colorChanger(std::string);

GLuint textures[2];

GLubyte *image;
GLubyte *image2;

// ---- READ TEXTURE

// basketball.ppm read -----------------------------------------------------------------------------------------------------------------------

void readPPMofBasketball(){
    
    FILE *fd;
    int k,n,m;
    char c;
    int i;
    char b[]= "basketball.ppm";
    int red,blue,green;
    
    fd = fopen(b,"r");
    fscanf(fd,"%[^\n] ",b);
    
    if(b[0]!='P'|| b[1] != '3'){
        printf("Not a ppm file.");
        exit(1);
    }
    printf("Valid file.");
    fscanf(fd, "%c",&c);
    
    while(c == '#'){
        fscanf(fd,"%[^\n] ",b);
        printf("%s\n",b);
        fscanf(fd, "%c",&c);
    }
    
    ungetc(c,fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);
    //printf("%d rows %d columns max value= %d\n",n,m,k);
    
    int nm = n*m;
    
    image = (GLubyte*)malloc(3*sizeof(GLubyte)*nm);
    
    for(i=nm;i>0;i--){
        
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        image[3*nm-3*i]=red;
        image[3*nm-3*i+1]=green;
        image[3*nm-3*i+2]=blue;
        
    }
}

// earth.ppm read -----------------------------------------------------------------------------------------------------------------------

void readPPMofWorldMap(){
    
    FILE *fd;
    int k,n,m;
    char c;
    int i;
    char b[]= "earth.ppm";
    int red,blue,green;
    
    fd = fopen(b,"r");
    fscanf(fd,"%[^\n] ",b);
    
    if(b[0]!='P'|| b[1] != '3'){
        printf("Not a ppm file.");
        exit(1);
    }
    printf("Valid file.");
    fscanf(fd, "%c",&c);
    
    while(c == '#'){
        fscanf(fd,"%[^\n] ",b);
        printf("%s\n",b);
        fscanf(fd, "%c",&c);
    }
    
    ungetc(c,fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);
    //printf("%d rows %d columns max value= %d\n",n,m,k);
    
    int nm = n*m;
    
    image2 = (GLubyte*)malloc(3*sizeof(GLubyte)*nm);
    
    for(i=nm;i>0;i--){
        
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        image2[3*nm-3*i]=red;
        image2[3*nm-3*i+1]=green;
        image2[3*nm-3*i+2]=blue;
        
    }
}

// SPHARE Creation
// Sphere Variables

const int NumTriangles        = 1024;
const int NumVerticesSphare   = 3 * NumTriangles;

point4 pointsSp[NumVerticesSphare];
vec3   normalsSp[NumVerticesSphare];
color4 colorSp[NumVerticesSphare];
vec3   texture[NumVerticesSphare];

int IndexSphere = 0;

// Creating Triangle and Color Assign ---------------------------------------------------------------------------------------------------------------------------------

// I slightly understand this code, it is hard to understand
// But triangle class recursivly called and 1024*3 verticies were createde

// Additionally Texture position arranged here... +++


void triangle( const point4& a, const point4& b, const point4& c ){
    
    vec3  normal = normalize(cross(b - a, c - b));
    
    normalsSp[IndexSphere] = normal;
    colorSp[IndexSphere] = myColor;
    pointsSp[IndexSphere] = a;
    texture[IndexSphere] = vec3(a.x,a.y,a.z); // TEXTURE POINTS for a
    IndexSphere++;
    
    normalsSp[IndexSphere] = normal;
    colorSp[IndexSphere] = myColor;
    pointsSp[IndexSphere] = b;
    texture[IndexSphere] = vec3(b.x,b.y,b.z); // TEXTURE POINTS for b
    IndexSphere++;
    
    normalsSp[IndexSphere] = normal;
    colorSp[IndexSphere] = myColor;
    pointsSp[IndexSphere] = c;
    texture[IndexSphere] = vec3(c.x,c.y,c.z); // TEXTURE POINTS for c
    IndexSphere++;
}

double sizeSP = 5.0;

point4 unit( const point4& p ){
    
    float len = p.x*p.x + p.y*p.y + p.z*p.z;
    point4 t;
    
    if ( len > DivideByZeroTolerance ) {
        t = p / sqrt(len);
        t.w = sizeSP; // Optimum value was assigned
    }
    
    return t;
}

void divide_triangle( const point4& a, const point4& b, const point4& c, int count ){
    
    if ( count > 0 ) {
        
        point4 v1 = unit( a + b );
        point4 v2 = unit( a + c );
        point4 v3 = unit( b + c );
        divide_triangle(  a, v1, v2, count - 1 );
        divide_triangle(  c, v2, v3, count - 1 );
        divide_triangle(  b, v3, v1, count - 1 );
        divide_triangle( v1, v3, v2, count - 1 );
        
    }else{
        triangle( a, b, c );
    }
}

void drawSphare(int count){
    
    point4 v[4] = {
        
        vec4( 0.0, 0.0, 1.0, 100.0 ), // w index were changed other wise werd triangles occured best option selected "100.0"
        vec4( 0.0, 0.942809, -0.333333, 100.0 ),
        vec4( -0.816497, -0.471405, -0.333333, 100.0 ),
        vec4( 0.816497, -0.471405, -0.333333, 100.0 )
    };
    
    divide_triangle( v[0], v[1], v[2], count );
    divide_triangle( v[3], v[2], v[1], count );
    divide_triangle( v[0], v[3], v[1], count );
    divide_triangle( v[0], v[2], v[3], count );
}

// INIT CODE

// Starts with Cube
// Shader connection

GLuint buffer;

color4 ambient_product;
color4 diffuse_product;
color4 specular_product;

void init(){
    
    readPPMofBasketball();
    readPPMofWorldMap();
    
    IndexSphere = 0;
    drawSphare(4);
    
    glGenTextures( 2, textures );
    
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 512, 256, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 2048, 1024, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image2 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    // Initial Texture
    
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    
    GLuint vao;
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE( vao );
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointsSp)+sizeof(colorSp)+sizeof(normalsSp)+sizeof(texture), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsSp), pointsSp);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsSp), sizeof(colorSp), colorSp);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsSp)+sizeof(colorSp), sizeof(normalsSp), normalsSp);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsSp)+sizeof(colorSp)+sizeof(normalsSp), sizeof(texture), texture);
    
    program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );
    
    glUniform4fv( glGetUniformLocation(program, "colorDefiner"), 1, myColor );
    
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsSp)));
    
    GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsSp)+sizeof(colorSp)));
    
    GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsSp)+sizeof(colorSp)+sizeof(normalsSp)));
    
    glUniform1f(glGetUniformLocation(program, "Flag"), 0);
    glUniform1i(glGetUniformLocation(program, "phong"), 1);

    ambient_product2 = light_ambient * material_ambient;
    diffuse_product2 = light_diffuse * material_diffuse;
    specular_product2 = light_specular * material_specular;
    
    point4 light_position( 0.1, 0.1, -5.8, 1.0 );
    color4 light_ambient( 0.1, 0.1, 0.1, 1.0 );

    color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    color4 light_specular( 1.0, 1.0, 1.0, 1.0 );
    
    color4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
    color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
    color4 material_specular( 1.0, 0.8, 0.0, 1.0 );
    float  material_shininess = 100.0;
    
    ambient_product = light_ambient * material_ambient;
    diffuse_product = light_diffuse * material_diffuse;
    specular_product = light_specular * material_specular;
    
    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"), 1, specular_product );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, light_position );
    glUniform1f( glGetUniformLocation(program, "Shininess"), material_shininess );

    ModelView = glGetUniformLocation(program, "ModelView");
    Projection = glGetUniformLocation(program, "Projection");
    
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_CULL_FACE);
    glEnable(GL_STENCIL_TEST);
    
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
}

// DISPLAY ACTIONS --------------------------------------------------------------------------------------------------------------------------------

void display(void) {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4  model_view = (Scale(1.0, 1.0, 1.0) * Translate(location) *
                        RotateX(Theta[Xaxis]) *
                        RotateY(Theta[Yaxis]) *
                        RotateZ(Theta[Zaxis]));
    
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    glDrawArrays(GL_TRIANGLES, 0, NumVerticesSphare);
    glutSwapBuffers();
}

// TIMER ACTIONS --------------------------------------------------------------------------------------------------------------------------------

static void Timer(int n) { // Gravity Related Codes...
    
    if(location.y <= -0.8 ){ // Border Bottom
        velocity = vec3 (0.0,-velocity.y + 0.00000001,0.0);
    }
    
    if(location.y >= 0.8 ){ // Border Top
        velocity = vec3 (0.0, -velocity.y,0.0);
    }
    if(location.y <-0.8){ // Bottom Stop
        location.y = -0.8;
    }
    
    velocity.operator+=(accelaration); // Gravity Handled
    accelaration.y = 0.001;
    
    
    if (lightMove){
        
        vec4 newspecular(1.0, 1.0, 1.0, 1.0);
        glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, newspecular);
        light_position = vec4(light_position.x + location.x, light_position.y + location.y, light_position.z, 1.0);
        glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
        
    }else{
        glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
        
        vec4 light_position2 = vec4(1.0, 1.0, -1.0, 1.0);
        
        glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position2);
    }
    
    
    
    
    location.operator-=(velocity); // Velocity Handled
    
    glutTimerFunc(1, Timer, 0);
    glutPostRedisplay();
}


void colorChanger(std::string k);

// KEYBOARD ACTIONS --------------------------------------------------------------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y) { // Keyboard Related Operations PRESS H During to Exevution
    
    if (key == 'Q' | key == 'q') {
        exit(0);
    }
    
    if (key == 'w' |key =='W') {
        
        if(velocity.y > 0){
            velocity = vec3 (0.0,velocity.y + 0.01,0.0);
            
        }else if(velocity.y < 0){
            velocity = vec3 (0.0,velocity.y - 0.01,0.0);
        }
    }
    
    if (key == 'H' |key =='h') {
        
        printf(" Two diffrent meterial: Metallic, Plastic --> Meterial Shiness Decresse or Incrase \n Two diffrent texture : Earth, Basketball \n Colors               : Green, Red, Blue, White --> When You Select Lost Texture ! \n Lights               : ON / OFF \n Perspective view     : Done \n Shading Mode         : Phong - Gourad \n REFERANCES: SHPARE NORMALIZATION HINT TAKEN FROM THIS WEB PAGE \n IMAGE READING TAKEN FROM LECTURES NOTES \n IN GOURAD VIEW YOU CAN SEE BOTH LIGHT SOURCES  !! IMPORTANT NOTE \n ");
        
        
        printf("Instruction \n");
        printf("Left Click Open Menu \n");
        printf("w faster \n");
        printf("s slower  \n");
        printf("q exit from program \n");
        printf("a - d rotate (EXTRA EVENT) \n");
        printf("m - n fill and line mode \n");
    
    }
    
    if (key == 'S'|key=='s') {
        
        if(velocity.y > 0){
            velocity = vec3 (0.0,velocity.y - 0.01,0.0);
            
        }else if (velocity.y < 0){
            velocity = vec3 (0.0,velocity.y + 0.01,0.0);
        }
    }
    
    if (key == 'O'|key=='o') {
        location = vec3(0.0,0.8, -2.8);
        velocity = vec3(0.0,0.01, 0.0);
    }
    
    if (key == 'A'|key=='a') {
        Theta[Axis] += 4.0;
    }
    
    if (key == 'D'|key=='d') {
        Theta[Axis] -= 4.0;
    }
    
    if (key == '3') {
        Theta[Zmeter] += 4.0;
    }
    
    if (key == '1') {
        Theta[Zmeter] -= 4.0;
    }
    
    if (key == 'M'|key=='m') {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
    if (key == 'N'|key=='n') {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    if (key == 'b'|key=='B') {
        sizeSP = sizeSP + 0.1;
    }
    
    if (key == 'v'|key=='V') {
        sizeSP = sizeSP - 0.1;
    }
    
    if (key == 'r'|key=='R') {
        
        if(colorSelector == 0){
            colorChanger("blue");
            colorSelector++;
            
        }else if(colorSelector == 1){
            colorChanger("red");
            colorSelector++;
            
        }else if(colorSelector == 2){
            colorChanger("green");
            colorSelector++;
            
        }else if(colorSelector == 3){
            colorChanger("yellow");
            colorSelector++;
            
        }else if(colorSelector == 4){
            colorChanger("black");
            colorSelector = 0;
        }
        
        glutPostRedisplay();
    }
}

// MENU MENU ACTIONS ----------------------------------------------------------------------------

void menu(int num){ // Menu Options Handled
    
    if(num == 0){
        glutDestroyWindow(window);
        exit(0);
        
    }else if(num == 5){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Line Drawing "Keyboard M"
        
    }else if(num == 6){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill Drawing "Keyboard N"
        glUniform1i(glGetUniformLocation(program, "flag"), 2);

        
    }else if(num == 3){
        glUniform1i(glGetUniformLocation(program, "flag"), 1);
        colorChanger("blue");
        
    }else if(num == 4){
        glUniform1i(glGetUniformLocation(program, "flag"), 1);
        colorChanger("red");
        
    }else if(num == 7){
        glUniform1i(glGetUniformLocation(program, "flag"), 1);
        colorChanger("green");
        
    }else if(num == 8){
        glUniform1i(glGetUniformLocation(program, "flag"), 1);
        colorChanger("yellow");
        
    }else if(num == 9){
        glUniform1i(glGetUniformLocation(program, "flag"), 1);
        colorChanger("white");
        
    }else if(num == 10){
        
        // DELETE IT
        
    }else if(num == 11){ // METALIC
        
        material_shininess = 5;
        glUniform1f( glGetUniformLocation(program, "Shininess"), material_shininess );
        
    }else if(num == 2){  // PLASTIC
        
        material_shininess = 1000000;
        glUniform1f( glGetUniformLocation(program, "Shininess"), material_shininess );
        
    }else if(num == 21){ // Light Type 1
        
        glUniform4fv( glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product2 );
        glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product2 );
        glUniform4fv( glGetUniformLocation(program, "SpecularProduct"), 1, specular_product2 );
        glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, light_position);

    }else if(num == 22){ // Light Type 2
        
        glUniform4fv( glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product );
        glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product );
        glUniform4fv( glGetUniformLocation(program, "SpecularProduct"), 1, specular_product );
        glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, light_position );

    }else if(num == 31){ // Light On
        
        ambient_product = light_ambient * material_ambient; // k_a * L_a
        diffuse_product = light_diffuse * material_diffuse; // k_d * L_d
        specular_product = light_specular * material_specular; // k_s * L_s
        
        glUniform4fv( glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product );
        glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product );
        glUniform4fv( glGetUniformLocation(program, "SpecularProduct"), 1, specular_product );
        glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, light_position );

    }else if(num == 32){ // Light Off
        
        ambient_product = 0; // k_a * L_a
        diffuse_product = 0; // k_d * L_d
        specular_product = 0; // k_s * L_s
        
        glUniform4fv( glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product );
        glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product );
        glUniform4fv( glGetUniformLocation(program, "SpecularProduct"), 1, specular_product );
        glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, light_position );

    }else if(num == 51){ // EARTH
        
        colorChanger("white");
        glUniform1i(glGetUniformLocation(program, "flag"), 0);
        glBindTexture( GL_TEXTURE_2D, textures[1] );
        
    }else if(num == 52){ // BASKETBALL
        
        colorChanger("white");
        glUniform1i(glGetUniformLocation(program, "flag"), 0);
        glBindTexture( GL_TEXTURE_2D, textures[0] );
    
    }else if(num == 52){
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill Drawing "Keyboard N"
        glUniform1i(glGetUniformLocation(program, "flag"), 0);
    
    }else if(num == 41){ // PHONG
        
        glUniform1i(glGetUniformLocation(program, "phong"), 1);

        
    }else if(num == 42){ // GOURAD

        glUniform1i(glGetUniformLocation(program, "phong"), 0);

    }else if(num == 61){
        

        lightMove = false;
        
        
        
    }else if(num == 62){
        
        
        lightMove = true;

        
        
    }else{ valueMenu = num; }
    
    glutPostRedisplay(); 
}

void colorChanger(std::string k){ // Color Conditions...
    
    if(k == "blue"){
        
        vec4 myBlue(0.0, 0.0, 1.0, 1.0);
        myColor = myBlue;
        glUniform4fv( glGetUniformLocation(program, "colorDefiner"), 1, myColor );

    }else if(k == "red"){
        
        vec4 myRed(1.0, 0.0, 0.0, 1.0);
        myColor = myRed;
        glUniform4fv( glGetUniformLocation(program, "colorDefiner"), 1, myColor );
        
    }else if(k == "yellow"){
        
        vec4 myYellow(1.0, 1.0, 0.0, 1.0);
        myColor = myYellow;
        glUniform4fv( glGetUniformLocation(program, "colorDefiner"), 1, myColor );
        
    }else if(k == "green"){
        
        vec4 myGreen(0.0, 1.0, 0.0, 1.0);
        myColor = myGreen;
        glUniform4fv( glGetUniformLocation(program, "colorDefiner"), 1, myColor );
        
    }else if(k == "white"){
        
        vec4 myWhite2(1.0, 1.0, 1.0, 1.0);
        myColor = myWhite2;
        glUniform4fv( glGetUniformLocation(program, "colorDefiner"), 1, myColor );
        
    }else if(k == "black"){
        
        vec4 myBlack(0.0, 0.0, 0.0, 1.0);
        myColor = myBlack;
        glUniform4fv( glGetUniformLocation(program, "colorDefiner"), 1, myColor );
        
    }
}

void createMenu(void){ // Creating Menu, Class Slides were used...
    
    submenu_id3 = glutCreateMenu(menu); // DRAWING MODE
   
    glutAddMenuEntry("wireframe", 5);
    glutAddMenuEntry("solid", 6);
    glutAddMenuEntry("shading", 61);

    
    submenu_id2 = glutCreateMenu(menu); // COLOR SELECT
    
    glutAddMenuEntry("Blue", 3);
    glutAddMenuEntry("Red", 4);
    glutAddMenuEntry("Green",7);
    glutAddMenuEntry("Yellow",8);
    glutAddMenuEntry("White", 9);
    glutAddMenuEntry("Black", 10);
    
    submenu_id = glutCreateMenu(menu); // METERIAL
    
    glutAddMenuEntry("plastic", 2);
    glutAddMenuEntry("metallic", 11);
    
    submenu_id4 = glutCreateMenu(menu); // LIGHT TYPE
    
    glutAddMenuEntry("directional", 21);
    glutAddMenuEntry("point", 22);
    
    submenu_id5 = glutCreateMenu(menu); // OPEN CLOSE LIGHT
    
    glutAddMenuEntry("lights On", 31);
    glutAddMenuEntry("lights Off", 32);
    
    submenu_id6 = glutCreateMenu(menu); // SHADING TYPE
    
    glutAddMenuEntry("phong", 41);
    glutAddMenuEntry("gourad", 42);
    
    submenu_id7 = glutCreateMenu(menu); // TEXTURE TYPE
    
    glutAddMenuEntry("earth", 51);
    glutAddMenuEntry("basketball", 52);
    
    submenu_id8 = glutCreateMenu(menu); // LIGHT POSITION
    
    glutAddMenuEntry("fixed position", 61);
    glutAddMenuEntry("with sphere", 62);

    menu_id = glutCreateMenu(menu);
    
    glutAddSubMenu("Object Meterial", submenu_id);
    glutAddSubMenu("Drawing Mode", submenu_id3);
    glutAddSubMenu("Light Mode", submenu_id4);
    glutAddSubMenu("Light ON/OFF", submenu_id5);
    glutAddSubMenu("Light Position", submenu_id8);
    glutAddSubMenu("Shading Mode", submenu_id6);
    glutAddSubMenu("Texture Type", submenu_id7);
    glutAddSubMenu("Color", submenu_id2);
    glutAddMenuEntry("Quit", 0);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// RESHAPE ------------------------------------------------------------------------------------------

void reshape( int width, int height ){ // Taken from angels sphere example Appendix emxaple 7.

    glViewport( 0, 0, width, height );
    GLfloat aspect = GLfloat(width)/height;
    mat4  projection = Perspective( 45.0, aspect, 0.001, 100 );
    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
}

// MAIN ---------------------------------------------------------------------------------------------

int main(int argc, char **argv){ // Taken From Class Exapmle
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize( 512, 512 );
    glutCreateWindow("COMP 410 - Ass 3 - 0040616");
    init();
    createMenu();
    glutDisplayFunc(display);
    glutReshapeFunc( reshape );
    glutKeyboardFunc(keyboard);
    Timer(1);
    glutMainLoop();
    return 0;
}
