
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
		position = Vec3f(0.0,-1.0,0.0);
		along = Vec3f(1.0,0.0,0.0);
		forward = Vec3f(0.0,0.0,-1.0);
		up = Vec3f(0.0,1.0,0.0);
		dyaw = 0;
		dpitch = 0;
		droll = 0;
		speed = 0.05;
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
		
		ViewMatrix[3][0] = x;
		ViewMatrix[3][1] = y;
		ViewMatrix[3][2] = z;
		ViewMatrix[3][3] = 1.0;
		
		
		glLoadMatrixf((float *)&ViewMatrix);
		//glTranslated(position[0],position[1],position[2]);
		//gluLookAt(position[0],position[1],position[2],forward[0],forward[1],forward[2], up[0],up[1],up[2]);
		
	}
	
	void Camera::roll(float th){
		
		if (th == 0) return;
		up = up*Cos(th) - along*Sin(th);
		up.normalize();
		along = forward.cross(up);
		//update();
	}
	void Camera::pitch(float th){
		
		if (th == 0) return;
		forward = forward*Cos(th) + up*Sin(th);
		forward.normalize();
		up = forward.cross(along)*-1.0;
		//update();
	}
	void Camera::yaw(float th){
		
		if (th == 0) return;
		along = along*Cos(th) + forward*Sin(th);
		along.normalize();
		forward = along.cross(up)*-1.0;
		//update();
	}
	
	void Camera::deltaRoll(float dth){
		droll=dth;
	}
		
	void Camera::deltaPitch(float dth){
		dpitch=dth;
	}
		
	void Camera::deltaYaw(float dth){
		dyaw=dth;
	}
	
	
	void Camera::move(){
		position += speed*forward;
	    roll(droll);
		pitch(dpitch);
		yaw(dyaw);
		update();
	}
	
	void Camera::thrust(){
		speed = speed + 0.02;
		if (speed > 1) speed = 1;
	}
	
	void Camera::brake(){
		speed = speed - 0.02;
		if (speed < 0) speed = 0;
	}
	

