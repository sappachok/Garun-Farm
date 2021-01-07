#ifndef _GARDEN_H
#define _GARDEN_H

#include "camera.h"
#include "object.h"
#include "CGfxopenGl.h"
#include "3dsLoader.h"

class CGarden:public CObject
{
private:

protected:
	// terrain doesn't move, so no physics animations
	void OnAnimate(scalar_t deltaTime) {}

	void OnDraw(CCamera *camera);
	void OnCollision(CObject *collisionObject);

public:
	CGarden();
	~CGarden();

	C3dsLoader obj3dsLoader;
	void Load(char* filename, char* tex_filename);
	void Unload3ds();
	void Render();
};

#endif