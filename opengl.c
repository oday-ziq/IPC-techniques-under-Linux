#include "header.h"
#include "functions.h"
#include "constants.h"
//------------------------------------------------[OpenGL variables]-------------------------------------------------------------//
void startOpenGl();
void display();
void init();
void reshape(int w, int h);
void timer(int value);
void rectangle(int x1, int y1, int x2, int y2, float r, float g, float b);
void writeIntegerAtPosition(int number, float x, float y);
void drawSkyAndGround();
void drawCircle(float cx, float cy, float r, int num_segments ,float red, float green, float blue);
void drawPlane(float x, float y, float scale, float red, float green, float blue);
void drawContainersReached(int plane_number, int deceplacement, int containerY);
void drawWorkersCommittee( int startX, int startY, int workerWidth, int workerHeight, int spacingX, int spacingY, float r, float g, float b , int counter) ;
void drawWorker(int x, int y, int width, int height, float r, float g, float b);
void drawCamp(int x, int y, int numTents, int baseWidth, int height, int spacing);
void drawTent(int x, int y, int baseWidth, int height);
void drawText(float x, float y, const char *text , float r, float g, float b);
float *speeds;  
float *positions;
int counter =0;
int indexOfWorkers = 0;
int number_of_collected_containers =0 ;
int number_of_distributed_containers =0;
int number_of_split_containers =0;
int number_of_whet_bags_reached_for_families =0;
typedef struct {
	float x;
	float y;
	float color[3];
	float speed;
} PLANE;
// array of planes
PLANE *planes ;
//-------------------------------------------------[    Main    ]----------------------------------------------------------------//
void kill_handler(int signum) {
	// change the color of plane 0
	CARGO_PLANES--;
}
int main(int argc, char **argv)
{ 	 
	 counter=0;
	readFile("args.txt");
	speeds = (float*)malloc(CARGO_PLANES * sizeof(float));
	positions = (float*)malloc(CARGO_PLANES * sizeof(float));
	// dynamic alloctaion for planes
	planes = (PLANE*)malloc(CARGO_PLANES * sizeof(PLANE));
	for (int i = 0 ; i < CARGO_PLANES ; i++ ){
		positions[i] = 300.0;
	} 
	for(int i=0 ; i < CARGO_PLANES ; i++){
		speeds[i] = i+1;
		planes[i].speed = speeds[i];
	}
	glutInit(&argc, argv);
	startOpenGl();
    return 0;
}
//------------------------------------------------[OpenGL functions]-------------------------------------------------------------//
void startOpenGl(){
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Moving Rectangle");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    init();
    glutMainLoop();
}

//--------------------------------------------------------------------

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 600, 0, 600);
	glMatrixMode(GL_MODELVIEW);
}
//--------------------------------------------------------------------

void init()
{
	counter=0;
	glClearColor(1, 1, 1, 1);
}
//--------------------------------------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
	drawSkyAndGround();
	drawContainersReached(0, 0, 125);
	drawText(10, 80, "COLLECTING COMMITTEES", 0.0, 0.0, 0.0);
    writeIntegerAtPosition(number_of_collected_containers, 157, 80);
	for (int i = 0; i < COLLECTING_WORKERS_COMMITTEES; i++) {
		drawWorkersCommittee(10, 21*(i+1), 8, 10, 20, 1, 1, 1, 0, COLLECTING_WORKERS_PER_COMMITTEE);
   
	}

	drawText(10, 200, "SPLITTING COMMITTEES", 0.0, 0.0, 0.0);
	writeIntegerAtPosition(number_of_split_containers, 157, 200);


	for (int i = 0; i < SPLITTING_WORKERS_COMMITTEES; i++) {
		drawWorkersCommittee(10, 130+(22*i), 8, 10, 30, 1, 1, 0, 1,SPLITTING_WORKERS_PER_COMMITTEE);
	}
	drawText(8, 300, "DISTRIBUTING COMMITTEES", 0.0, 0.0, 0.0);

	for (int i = 0; i < DISTRIBUTING_WORKERS_COMMITTEES; i++) {
		drawWorkersCommittee(10, 230+(22*i), 8, 10, 30, 1, 0, 0, 1,DISTRIBUTING_WORKERS_PER_COMMITTEE);
   
	}
	drawCamp(500, 200, NUM_OF_FAMILIES, 25, 30, 10); // Draw a camp with 3 tents at coordinates (200, 200) with tent parameters (baseWidth: 150, height: 100) and spacing 100 pixels apart
		
	drawText(440, 150, "FAMILIES WIEGHT BAGS", 0.0, 0.0, 0.0);
	writeIntegerAtPosition(number_of_whet_bags_reached_for_families, 550, 100);

    int diff = 50;
    int deplacement = 0 ;
    for (int i = 0; i < CARGO_PLANES; i++) {

		drawPlane(positions[i], 600 - diff, 1 ,0, 0, 0);
	     planes[i].x = 300;
	     planes[i].y =  600 - diff;
		 diff += 50;
	}
	char *msg= receive_a_message(); // drop x
   if(msg !=NULL){
    char* token = strtok(msg, " ");
    if (token != NULL && strcmp(token, "drop") == 0) {
        // Split the message to get the plane number
        char* number = strtok(NULL, " ");
        if (number != NULL) {
            int container_count = atoi(number);
                for ( int i= 0 ; i < container_count ; i++ ){
                          counter++;  } 	
        } 

	}else if(token != NULL && strcmp(token, "collect") == 0){
	  if(counter>0){
	     counter -- ;
	     number_of_collected_containers++;
	  }

	}
	else if(token != NULL && strcmp(token, "split") == 0){
		if(number_of_collected_containers>0){
        number_of_split_containers +=WHEAT_CONTAINER_MASS;
		number_of_collected_containers--;
		}

	}
	else if(token != NULL && strcmp(token, "send") == 0){
          if(number_of_split_containers>0){
      number_of_distributed_containers +=DISTRIBUTION_COMMITTIEE_CARRY_BAGS;
	  number_of_split_containers -= DISTRIBUTION_COMMITTIEE_CARRY_BAGS;
	  number_of_whet_bags_reached_for_families += DISTRIBUTION_COMMITTIEE_CARRY_BAGS;
	  }
	}
   }
   
    drawText(10, 400, " ", 0.0, 0.0, 0.0);
	writeIntegerAtPosition(counter, 300, 150 );
    glFlush();


}
//--------------------------------------------------------------------
void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);


    // Update the x position of the rectangle
    for (int i = 0; i < CARGO_PLANES; i++) {
        positions[i] += speeds[i];  // Update each square's position based on its speed
        if (positions[i] > 550) {  // Reset position if out of bounds
            positions[i] = -50;
        }
    }
}


//--------------------------------------------------------------------

void rectangle(int x1, int y1, int x2, int y2, float r, float g, float b)
{
	glBegin(GL_POLYGON);
	glColor3f(r, g, b);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
}

//--------------------------------------------------------------------

void drawSquare(int centerX, int centerY, int sideLength, float red, float green, float blue)
{
	int halfSide = sideLength / 2; // Calculate half of the side length

	int vertices[] = {
		centerX - halfSide, centerY - halfSide, // Bottom-left vertex
		centerX + halfSide, centerY - halfSide, // Bottom-right vertex
		centerX + halfSide, centerY + halfSide, // Top-right vertex
		centerX - halfSide, centerY + halfSide	// Top-left vertex
	};

	glColor3f(red, green, blue); // Set the color of the square

	glBegin(GL_QUADS); // Draw the square
	for (int i = 0; i < 4; i++)
		glVertex2f(vertices[i * 2], vertices[i * 2 + 1]);
	glEnd();
}
void writeIntegerAtPosition(int number, float x, float y)
{
	glRasterPos2f(x, y);
	char str[10];
	sprintf(str, "%d", number);
	for (int i = 0; str[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}
//--------------------------------------------------------------------
void drawSkyAndGround() {

    glColor3f(0.53, 0.81, 0.92); // Light blue color for the sky
    glBegin(GL_QUADS);
    glVertex2f(0, 150);  // Start from the middle of the window to the top
    glVertex2f(600, 150);
    glVertex2f(600, 600);
    glVertex2f(0, 600);
    glEnd();
    // Draw Grass
    glColor3f(0.0, 0.39, 0.0); // Dark green color for the grass
    glBegin(GL_QUADS);
    glVertex2f(0, 0);   // Start from the bottom of the window to the middle
    glVertex2f(600, 0);
    glVertex2f(600, 350);
    glVertex2f(0, 350);
    glEnd();
    glColor3f(0.8, 0.8, 0.8); // Light grey for the container
	drawSquare(300, 100, 250, 0.8, 0.8, 0.8);
	//rectangle(100, 75, 500, 125, 0.8, 0.8, 0.8);

	
}
void drawCircle(float cx, float cy, float r, int num_segments , float red, float green, float blue) {
    GLenum err;
    glBegin(GL_TRIANGLE_FAN);
   // glColor3f(1.0, 0.0, 0.0); // Red color for the circle
   glColor3f(red, green, blue); // Set the color of the square

    glVertex2f(cx, cy); // center of circle
    for (int i = 0; i <= num_segments; i++) {
		float theta = 2.0f * 3.1415926f * (float)i / (float)num_segments; // get the current angle
        float x = r * cosf(theta); // calculate the x component
        float y = r * sinf(theta); // calculate the y component
        glVertex2f(x + cx, y + cy); // output vertex
    }
    glEnd();

    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error: %d\n", err);
    }
}

void drawPlane(float x, float y, float scale, float red, float green, float blue) {
	// design the plane 
	 // body of the plane is black
	 glColor3b(0, 0, 0);
	rectangle(x, y, x + 50 * scale, y + 10 * scale, red, green, blue);
	// Draw the wings of the plane
	rectangle(x + 10 * scale, y + 10 * scale, x + 40 * scale, y + 20 * scale, red, green, blue);
	// Draw the tail of the plane
	rectangle(x - 10 * scale, y + 5 * scale, x, y + 15 * scale, red, green, blue);
	// Draw the windows of the plane
	glColor3f(1, 1, 1); // Set the color of the windows
	rectangle(x + 45 * scale, y + 5 * scale, x + 47 * scale, y + 7 * scale, 1, 1, 1);
	rectangle(x + 47 * scale, y + 5 * scale, x + 49 * scale, y + 7 * scale, 1, 1, 1);
	rectangle(x + 45 * scale, y + 3 * scale, x + 47 * scale, y + 5 * scale, 1, 1, 1);
	rectangle(x + 47 * scale, y + 3 * scale, x + 49 * scale, y + 5 * scale, 1, 1, 1);
    
}
void drawContainersReached(int plane_number, int deceplacement, int containerY) {
	int containerHeight = 30; // Height of each container square
    int containerSpacing = 5; // Spacing between containers
	int containerWidth = 20; // Width of each container square

    int containersPerRow = 10; // Number of containers per row
    int startX = 190; // Starting X-coordinate for the first container
    int startY = containerY-100; // Starting Y-coordinate for the first row of containers

    for (int i = 0; i <counter ; i++) {
        int row = i % containersPerRow; // Calculate the current row
        int col = i / containersPerRow; // Calculate the current column

        // Calculate the coordinates of the current container
        int containerX = startX + row * (containerWidth + containerSpacing);
        int containerY = startY + col * (containerHeight + containerSpacing); // Adjusted calculation

        // Draw the square at the calculated coordinates
        drawSquare(containerX, containerY, containerWidth, 0.36, 0.25, 0.20);
    }

}
void drawWorkersCommittee( int startX, int startY, int workerWidth, int workerHeight, int spacingX, int spacingY, float r, float g, float b ,int counter) {
    // Loop through each worker
	int numberOfWorkers = counter;

    for (int i = 0; i < numberOfWorkers; i++) {
        // Calculate the position of the current worker
        int workerX = startX + (i * (workerWidth + spacingX)) ;
        int workerY = startY;
        // Draw the worker
        drawWorker(workerX, workerY, workerWidth, workerHeight, r, g, b);
    }
}

void drawWorker(int x, int y, int width, int height, float r, float g, float b) {
	drawCircle(x + width / 2, y + height, width / 4, 100, r, g, b);
	rectangle(x, y, x + width, y + height, r, g, b);
	// Draw the arms of the worker
	rectangle(x - width / 2, y + height / 2, x + width / 2, y + height / 2 + height / 4, r, g, b);
	// Draw the legs of the worker
	rectangle(x, y, x + width / 4, y - height, r, g, b);
	rectangle(x + width, y, x + width - width / 4, y - height, r, g, b);

}
void drawCamp(int x, int y, int numTents, int baseWidth, int height, int spacing) {
  int endX = x + ((numTents - 1) * (baseWidth + spacing)) / 2; // Calculate the ending X-coordinate for the last tent
  for (int i = numTents - 1; i >= 0; i--) {
    drawTent(endX - i * (baseWidth + spacing), y, baseWidth, height); // Draw each tent with appropriate spacing, in reverse order
  }

}

void drawTent(int x, int y, int baseWidth, int height) {
  glColor3f(1.0, 0.0, 0.0); // Set the color of the square
  rectangle(x, y, x + baseWidth, y + height, 0.8, 0.8, 0.8);
  glColor3f(0.36, 0.25, 0.20); // Brown color for the tent roof
  triangle(x - 10, y + height, x + baseWidth / 2, y + height + 20, x + baseWidth + 10, y + height);

}
void triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  glBegin(GL_TRIANGLES);
  glVertex2i(x1, y1);
  glVertex2i(x2, y2);
  glVertex2i(x3, y3);
  glEnd();
}

// draw text method
void drawText(float x, float y, const char *text , float r, float g, float b) {
	glColor3f(r, g, b); // Set the color of the text
	glRasterPos2f(x, y); // Set the position for the text
	while (*text) {
		// font size is 18
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *text); // Render the character using GLUT's built-in font
		++text;
	}
}



 