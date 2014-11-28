#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "vec3f.h"
#include "CSCIx229.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
class Camera {
	private:
		Vec3f position;
		Vec3f forward;
		Vec3f up;
		Vec3f along;
		float droll;
		float dpitch;
		float dyaw;
		float speed;
	public:
		Camera();
		virtual ~Camera();
		Vec3f getPosition();
		void reset();
		void update();
		void roll(float th);
		void pitch(float th);
		void yaw(float th);
		void deltaRoll(float dth);
		void deltaPitch(float dth);
		void deltaYaw(float dth);
		void move();
		void thrust();
		void brake();
};
#endif
