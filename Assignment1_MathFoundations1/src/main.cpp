// Includes for the assignment
#include "Vector4f.h"
#include "Matrix4f.h"
#include <iostream>

// Tests for comparing our library
// You may compare your operations against the glm library
// which is a well tested library.
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Sample unit test comparing against GLM.
bool unitTest0(){
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Matrix4f myIdentity( 1.0f,0,0,0,
        			     0,1.0f,0,0,
		        	     0,0,1.0f,0,
			             0,0,0,1.0f);

    if(
        glmIdentityMatrix[0][0]==myIdentity[0][0] &&
        glmIdentityMatrix[0][1]==myIdentity[0][1] &&
        glmIdentityMatrix[0][2]==myIdentity[0][2] &&
        glmIdentityMatrix[0][3]==myIdentity[0][3] &&
        glmIdentityMatrix[1][0]==myIdentity[1][0] &&
        glmIdentityMatrix[1][1]==myIdentity[1][1] &&
        glmIdentityMatrix[1][2]==myIdentity[1][2] &&
        glmIdentityMatrix[1][3]==myIdentity[1][3] &&
        glmIdentityMatrix[2][0]==myIdentity[2][0] &&
        glmIdentityMatrix[2][1]==myIdentity[2][1] &&
        glmIdentityMatrix[2][2]==myIdentity[2][2] &&
        glmIdentityMatrix[2][3]==myIdentity[2][3] &&
        glmIdentityMatrix[3][0]==myIdentity[3][0] &&
        glmIdentityMatrix[3][1]==myIdentity[3][1] &&
        glmIdentityMatrix[3][2]==myIdentity[3][2] &&
        glmIdentityMatrix[3][3]==myIdentity[3][3]){
            return true;
    }
	
    return false;	
}

bool unitTest1(){
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Matrix4f myIdentity( 1.0f,0,0,0,
        			     0,1.0f,0,0,
		        	     0,0,1.0f,0,
			             0,0,0,1.0f);

    if(
        glmIdentityMatrix[0][0]==myIdentity(0,0) &&
        glmIdentityMatrix[0][1]==myIdentity(0,1) &&
        glmIdentityMatrix[0][2]==myIdentity(0,2) &&
        glmIdentityMatrix[0][3]==myIdentity(0,3) &&
        glmIdentityMatrix[1][0]==myIdentity(1,0) &&
        glmIdentityMatrix[1][1]==myIdentity(1,1) &&
        glmIdentityMatrix[1][2]==myIdentity(1,2) &&
        glmIdentityMatrix[1][3]==myIdentity(1,3) &&
        glmIdentityMatrix[2][0]==myIdentity(2,0) &&
        glmIdentityMatrix[2][1]==myIdentity(2,1) &&
        glmIdentityMatrix[2][2]==myIdentity(2,2) &&
        glmIdentityMatrix[2][3]==myIdentity(2,3) &&
        glmIdentityMatrix[3][0]==myIdentity(3,0) &&
        glmIdentityMatrix[3][1]==myIdentity(3,1) &&
        glmIdentityMatrix[3][2]==myIdentity(3,2) &&
        glmIdentityMatrix[3][3]==myIdentity(3,3)){
            return true;
    }
	
    return false;	
}

// Sample unit test comparing against GLM.
bool unitTest2(){
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Vector4f a(1.0f ,0.0f,  0.0f,   0.0f);
	Vector4f b(0.0f ,1.0f,  0.0f,   0.0f);
	Vector4f c(0.0f ,0.0f,  1.0f,   0.0f);
	Vector4f d(0.0f ,0.0f,  0.0f,   1.0f);
	Matrix4f myIdentity(a,b,c,d);

    if(
        glmIdentityMatrix[0][0]==myIdentity[0][0] &&
        glmIdentityMatrix[0][1]==myIdentity[0][1] &&
        glmIdentityMatrix[0][2]==myIdentity[0][2] &&
        glmIdentityMatrix[0][3]==myIdentity[0][3] &&
        glmIdentityMatrix[1][0]==myIdentity[1][0] &&
        glmIdentityMatrix[1][1]==myIdentity[1][1] &&
        glmIdentityMatrix[1][2]==myIdentity[1][2] &&
        glmIdentityMatrix[1][3]==myIdentity[1][3] &&
        glmIdentityMatrix[2][0]==myIdentity[2][0] &&
        glmIdentityMatrix[2][1]==myIdentity[2][1] &&
        glmIdentityMatrix[2][2]==myIdentity[2][2] &&
        glmIdentityMatrix[2][3]==myIdentity[2][3] &&
        glmIdentityMatrix[3][0]==myIdentity[3][0] &&
        glmIdentityMatrix[3][1]==myIdentity[3][1] &&
        glmIdentityMatrix[3][2]==myIdentity[3][2] &&
        glmIdentityMatrix[3][3]==myIdentity[3][3]){
            return true;
    }
	
    return false;	
}

// Sample unit test comparing against GLM.
bool unitTest3(){
	glm::mat4 glmScale = glm::scale(glm::vec3(2.0f));
	Vector4f a(1.0f,0,0,0);
	Vector4f b(0,1.0f,0,0);
	Vector4f c(0,0,1.0f,0);
	Vector4f d(0,0,0,1.0f);
	Matrix4f myScaled(a,b,c,d);
    myScaled = myScaled * myScaled.MakeScale(2.0f,2.0f,2.0f);

    if(
        glmScale[0].x==myScaled[0].x &&
        glmScale[0].y==myScaled[0].y &&
        glmScale[0].z==myScaled[0].z &&
        glmScale[0].w==myScaled[0].w &&
        glmScale[1].x == myScaled[1].x &&
        glmScale[1].y == myScaled[1].y &&
        glmScale[1].z == myScaled[1].z &&
        glmScale[1].w == myScaled[1].w &&
        glmScale[2].x == myScaled[2].x &&
        glmScale[2].y == myScaled[2].y &&
        glmScale[2].z == myScaled[2].z &&
        glmScale[2].w == myScaled[2].w &&
        glmScale[3].x == myScaled[3].x &&
        glmScale[3].y == myScaled[3].y &&
        glmScale[3].z == myScaled[3].z &&
        glmScale[3].w == myScaled[3].w){
            return true;
    }
	
    return false;	
}

// Sample unit test comparing against GLM.
// Testing operator
bool unitTest4(){
	glm::mat4 glmTest = glm::mat4(1.0f);
    glmTest[1][3] = 72.0f;
    glmTest[2][3] = 2.1f;

	Matrix4f myMatrix(0,0,0,0,
                      0,0,0,0,
                      0,0,0,0,
                      0,0,0,0);

    myMatrix[1][3] = 72.0f;
    myMatrix[2][3] = 2.1f;

    if( glmTest[1][3]==myMatrix[1][3] &&
        glmTest[2][3]==myMatrix[2][3] ){
            return true;
    }
	
    return false;	
}

// Sample unit test testing your library
bool unitTest5(){
  Vector4f a(1,1,1,1);
  Vector4f b(0,0,0,0);
  Vector4f c = a + b;

  if(c.x == 1 && c.y == 1 && c.z ==1 && c.w==1){
    return true;
  }
    return false;
}

// Sample unit test comparing against GLM.
bool unitTest6() {
    glm::mat4 glmScale = glm::rotate(2.0f, glm::vec3(1.0f, 0, 0));
    Vector4f a(1.0f, 0, 0, 0);
    Vector4f b(0, 1.0f, 0, 0);
    Vector4f c(0, 0, 1.0f, 0);
    Vector4f d(0, 0, 0, 1.0f);
    Matrix4f myRotated(a, b, c, d);
    myRotated = myRotated * myRotated.MakeRotationX(2.0f);

    if (
        glmScale[0].x == myRotated[0].x &&
        glmScale[0].y == myRotated[0].y &&
        glmScale[0].z == myRotated[0].z &&
        glmScale[0].w == myRotated[0].w &&
        glmScale[1].x == myRotated[1].x &&
        glmScale[1].y == myRotated[1].y &&
        glmScale[1].z == myRotated[1].z &&
        glmScale[1].w == myRotated[1].w &&
        glmScale[2].x == myRotated[2].x &&
        glmScale[2].y == myRotated[2].y &&
        glmScale[2].z == myRotated[2].z &&
        glmScale[2].w == myRotated[2].w &&
        glmScale[3].x == myRotated[3].x &&
        glmScale[3].y == myRotated[3].y &&
        glmScale[3].z == myRotated[3].z &&
        glmScale[3].w == myRotated[3].w) {
        return true;
    }

    return false;
}

// Sample unit test comparing against GLM.
bool unitTest7() {
    glm::mat4 glmScale = glm::rotate(2.0f, glm::vec3(0, 1.0f, 0));
    Vector4f a(1.0f, 0, 0, 0);
    Vector4f b(0, 1.0f, 0, 0);
    Vector4f c(0, 0, 1.0f, 0);
    Vector4f d(0, 0, 0, 1.0f);
    Matrix4f myRotated(a, b, c, d);
    myRotated = myRotated * myRotated.MakeRotationY(2.0f);

    if (
        glmScale[0].x == myRotated[0].x &&
        glmScale[0].y == myRotated[0].y &&
        glmScale[0].z == myRotated[0].z &&
        glmScale[0].w == myRotated[0].w &&
        glmScale[1].x == myRotated[1].x &&
        glmScale[1].y == myRotated[1].y &&
        glmScale[1].z == myRotated[1].z &&
        glmScale[1].w == myRotated[1].w &&
        glmScale[2].x == myRotated[2].x &&
        glmScale[2].y == myRotated[2].y &&
        glmScale[2].z == myRotated[2].z &&
        glmScale[2].w == myRotated[2].w &&
        glmScale[3].x == myRotated[3].x &&
        glmScale[3].y == myRotated[3].y &&
        glmScale[3].z == myRotated[3].z &&
        glmScale[3].w == myRotated[3].w) {
        return true;
    }

    return false;
}

// Sample unit test comparing against GLM.
bool unitTest8() {
    glm::mat4 glmScale = glm::rotate(2.0f, glm::vec3(0, 0, 1.0f));
    Vector4f a(1.0f, 0, 0, 0);
    Vector4f b(0, 1.0f, 0, 0);
    Vector4f c(0, 0, 1.0f, 0);
    Vector4f d(0, 0, 0, 1.0f);
    Matrix4f myRotated(a, b, c, d);
    myRotated = myRotated * myRotated.MakeRotationZ(2.0f);

    if (
        glmScale[0].x == myRotated[0].x &&
        glmScale[0].y == myRotated[0].y &&
        glmScale[0].z == myRotated[0].z &&
        glmScale[0].w == myRotated[0].w &&
        glmScale[1].x == myRotated[1].x &&
        glmScale[1].y == myRotated[1].y &&
        glmScale[1].z == myRotated[1].z &&
        glmScale[1].w == myRotated[1].w &&
        glmScale[2].x == myRotated[2].x &&
        glmScale[2].y == myRotated[2].y &&
        glmScale[2].z == myRotated[2].z &&
        glmScale[2].w == myRotated[2].w &&
        glmScale[3].x == myRotated[3].x &&
        glmScale[3].y == myRotated[3].y &&
        glmScale[3].z == myRotated[3].z &&
        glmScale[3].w == myRotated[3].w) {
        return true;
    }

    return false;
}

int main(){
    // Keep track of the tests passed
    unsigned int testsPassed = 0;

    // Run 'unit tests'
    std::cout << "Passed 0: " << unitTest0() << " \n";
    std::cout << "Passed 1: " << unitTest1() << " \n";
    std::cout << "Passed 2: " << unitTest2() << " \n";
    std::cout << "Passed 3: " << unitTest3() << " \n";
    std::cout << "Passed 4: " << unitTest4() << " \n";
    std::cout << "Passed 5: " << unitTest5() << " \n";
    std::cout << "Passed 6: " << unitTest6() << " \n";
    std::cout << "Passed 7: " << unitTest7() << " \n";
    std::cout << "Passed 8: " << unitTest8() << " \n";

    return 0;
}
