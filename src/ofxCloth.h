#include "ofMain.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cloth_glm.h"

using namespace glm;

class Clothing
{
	public:
	Cloth* cloth1;
	vector<glm::vec3> ball_pos; 
	vector<float> ball_radius; 

	void setup( int w, int h, int numPartWidth, int numPartHeight );
	vector<ofVec3f> getPositionSphere();
	int getPlaneWidth();
	int getPlaneHeight();
	vector<float> getRadiusSphere();

	void update();
	void setPositionSphere(vector<ofVec3f> pos);
	void setRadiusSphere( vector<float> radius );
        void addForce( ofVec3f dir, float time  );
	void windForce( ofVec3f dir, float time );

	void sphereCollision( vector<ofVec3f> pos, vector<float> radius );
	void pointCollision( ofVec3f center, ofVec3f coord, float surface );
	void constrainPoints( int index, ofVec3f offSetPos );

	void drawCloth( bool wire );
};
