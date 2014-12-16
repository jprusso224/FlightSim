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
	public:
		Camera();
		virtual ~Camera();
		Vec3f getPosition();
		void reset();
		void update();
		void roll(int th);
		void pitch(int th);
		void yaw(int th);
		void thrust();
		void brake();
};
#endif
