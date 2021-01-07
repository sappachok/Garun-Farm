#ifndef _CAMERA_H
#define _CAMERA_H

#include "vector.h"

//////////////////////////////////////
//The CCamera Class
//////////////////////////////////////
class CCamera 
{
	public:

		tVector3 mPos;	
		tVector3 mView;		
		tVector3 mUp;	
		
		float m_AngleY;
		float m_AngleZ;
		float m_Pitch;

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
		// This function let you control the camera with the mouse
		void Mouse_Move(int wndWidth, int wndHeight);
		void Mouse_Move_1P(int wndWidth, int wndHeight);

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
		void Move_Camera(float speed);
		void Slide_Camera(float speed);
		void Rotate_View(float speed);
		void Rotate_Position(float speed);
		void Position_Camera(float pos_x, float pos_y,float pos_z,
			 				 float view_x, float view_y, float view_z,
							 float up_x,   float up_y,   float up_z);

		tVector3 GetViewSlide(float speed);
		tVector3 GetViewMove(float speed);

		float GetDirect()
		{
			float direct,theta;
			CVector diff;       // the vector from the enemy to the player
		    diff.x = mPos.x - mView.x;
			diff.y = mPos.y - mView.y;
			diff.z = mPos.z - mView.z;
		    diff.Normalize();

			if(diff.x <= 0)
			{
				theta = (float)atan( diff.z / diff.x );
				direct = theta;
			}
			else
			{
				theta = (float)atan( diff.z / diff.x );
				direct = DEG2RAD(90)+(theta+DEG2RAD(90));
			}
				
			return direct;
		}

		float GetPitch()
		{
			CVector pos,view;
			
			pos.x = mPos.x;
			pos.y = 0;
			pos.z = mPos.z;

			view.x = mView.x;
			view.y = 0;
			view.z = mView.z;

			float dist = (pos - view).Length();

			float theta = (float)atan( (mPos.y - mView.y) / dist );
			float direct = theta;

			return direct;
		}
};

#endif
