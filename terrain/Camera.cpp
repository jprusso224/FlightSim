
    #include "Camera.h"
    using namespace std;

	
	//Constructor
	Camera::Camera(){
		reset();
	}
	
	//Destructor
	Camera::~Camera(){
	}
	
	Vec3f Camera::getPosition(){
		return position;
	}
	
	void Camera::reset(){
		position = Vec3f(0.0,2.0,0.0);
		along = Vec3f(1.0,0.0,0.0);
		forward = Vec3f(0.0,0.0,-1.0);
		up = Vec3f(0.0,1.0,0.0);
		update();
	}
	void Camera :: update(){
		float x = along.dot(position);
		float y = up.dot(position);
		float z = forward.dot(position);
		
		float ViewMatrix[4][4]; 
		ViewMatrix[0][0] = along[0];
		ViewMatrix[0][1] = up[0];
		ViewMatrix[0][2] = forward[0];
		ViewMatrix[0][3] = 0.0;
		 
		ViewMatrix[1][0] = along[1];
		ViewMatrix[1][1] = up[1];
		ViewMatrix[1][2] = forward[1];
		ViewMatrix[1][3] = 0.0;
	
		ViewMatrix[2][0] = along[2];
		ViewMatrix[2][1] = up[2];
		ViewMatrix[2][2] = forward[2];
		ViewMatrix[2][3] = 0.0;
		
		ViewMatrix[3][0] = -x;
		ViewMatrix[3][1] = y;
		ViewMatrix[3][2] = z;
		ViewMatrix[3][3] = 1.0;
		
		
		glLoadMatrixf((float *)&ViewMatrix);
		//glTranslated(position[0],position[1],position[2]);
		//gluLookAt(position[0],position[1],position[2],forward[0],forward[1],forward[2], up[0],up[1],up[2]);
		
	}
	
	void Camera::roll(int th){
		
		up = up*Cos(th) - along*Sin(th);
		up.normalize();
		along = forward.cross(up);
		update();
	}
	void Camera::pitch(int th){
		
		forward = forward*Cos(th) + up*Sin(th);
		forward.normalize();
		up = forward.cross(along)*-1.0;
		update();
	}
	void Camera::yaw(int th){
		
		along = along*Cos(th) + forward*Sin(th);
		along.normalize();
		forward = along.cross(up)*-1.0;
		update();
	}
	void Camera::thrust(){
		int du = 1;
		position += du*forward;
		update();
	}
	
	void Camera::brake(){
		int du = 1;
		position -= du*forward;
		update();
	}
	

