#include "stdafx.h"
#include "Collision2D.h"
#include <Core/2D/DebugSceneObject2D.h>
#include <Rendering/RenderedObjectBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Math/MathLib.h>
#include <Rendering/OpenGL/2D/DebugShader2D.h>

void Collision2D::AddCollisionPoint(CollisionPoint & point)
{
	assert(numCollisionPoints < 2);

	if (shouldRender)
	{
		DebugSceneObject2D* db = new DebugSceneObject2D("Debug Collision Point");
		db->SetScale(5, 5);
		db->SetColor({ 0.0, 1.0, 0.0 });
		db->SetPosition(point.pos);

		debugObjects[numCollisionPoints] = db;
		zEngine->AddSceneObject(db);
	}
	collisionPoints[numCollisionPoints++] =  point;
}

Collision2D::Collision2D()
{
	numCollisionPoints = 0;
	collided = false;
	wasUpdated = false;
	shouldRender = false;
	collidedObjects[0] = 0;
	collidedObjects[1] = 0;
	debugObjects[0] = 0;
	debugObjects[1] = 0;
	objectBounds[0] = 0;
	objectBounds[1] = 0;
	friction = 0;
	ID = sID++;
	frame = 0;
}

Collision2D::~Collision2D()
{

	if (debugObjects[0])debugObjects[0]->Destroy();
	if (debugObjects[1])debugObjects[1]->Destroy();
	
}

Collision2D* Collision2D::Reflection()
{
	Collision2D *res = new Collision2D();

	res->ID = ID;
	res->collided = collided;
	res->collidedObjects[0] = collidedObjects[1];
	res->collidedObjects[1] = collidedObjects[0];
	res->objectBounds[0] = objectBounds[1];
	res->objectBounds[1] = objectBounds[0];

	return res;
};

bool Collision2D::operator==(const Collision2D& other)
{
	return ID == other.ID;
}

void Collision2D::PreUpdate(float inv_dt, Accumulated2DVelocities& aV)
{
	if (shouldRender == false && ZoranEngine::canRenderDebug)
	{
		shouldRender = true;
		
		for (unsigned i = 0; i < numCollisionPoints; i++)
		{
			DebugSceneObject2D* db = new DebugSceneObject2D("Debug Collision Point");
			db->SetScale(5, 5);
			db->SetColor({ 0.0, 1.0, 0.0 });
			db->SetPosition(collisionPoints[i].pos);

			debugObjects[i] = db;
			zEngine->AddSceneObject(db);
		}
	}
	static const float k_allowedPenetration = 0.1f;
	static float k_biasFactor = 0.2f;

	for (CollisionPoint& c : collisionPoints)
	{
		Vec2D r1 = c.pos - collidedObjects[0]->GetScenePos();
		Vec2D r2 = c.pos - collidedObjects[1]->GetScenePos();

		// Precompute normal mass, tangent mass, and bias.
		float rn1 = r1.dot(c.normal);
		float rn2 = r2.dot(c.normal);
		float kNormal = collidedObjects[0]->GetInvMass() + collidedObjects[1]->GetInvMass();
		kNormal += collidedObjects[0]->GetInvInertia() * (r1.dot(r1) - rn1 * rn1) + collidedObjects[1]->GetInvInertia() * (r2.dot(r2) - rn2 * rn2);
		c.massNormal = 1.0f / kNormal;

		Vec2D tangent = c.normal.crossRight(1.0f);
		float rt1 = r1.dot(tangent);
		float rt2 = r2.dot(tangent);
		float kTangent = collidedObjects[0]->GetInvMass() + collidedObjects[1]->GetInvMass();
		kTangent += collidedObjects[0]->GetInvInertia() * (r1.dot(r1) - rn1 * rn1) + collidedObjects[1]->GetInvInertia() * (r2.dot(r2) - rn2 * rn2);
		c.massTangent = 1.0f / kTangent;

		c.bias = -k_biasFactor * inv_dt * min(0.0f, c.separation + k_allowedPenetration);

		// Apply normal + friction impulse
		Vec2D P = c.normalImpulse * c.normal + c.normalTangetImpulses * tangent;

		//collidedObjects[0]->ApplyImpulseToVelocity(-P);
		//collidedObjects[0]->ApplyImpulseToAngularVelocity(-c.r1.cross(P));
		aV.velocity[aIndex] -= collidedObjects[0]->GetInvMass() * P;
		aV.angularVelocity[aIndex] -= collidedObjects[0]->GetInvInertia() * r1.cross(P);

		//collidedObjects[1]->ApplyImpulseToVelocity(P);
		//collidedObjects[1]->ApplyImpulseToAngularVelocity(c.r2.cross(P));
		aV.velocity[bIndex] += collidedObjects[1]->GetInvMass() * P;
		aV.angularVelocity[bIndex] += collidedObjects[1]->GetInvInertia() * r2.cross(P);
		
	}
}

void Collision2D::UpdateForces(Accumulated2DVelocities& aV)
{
	wasUpdated = false;

	PhysicsObject2DBase* b1 = collidedObjects[0];
	PhysicsObject2DBase* b2 = collidedObjects[1];

	for (CollisionPoint& c : collisionPoints)
	{
		c.r1 = c.pos - b1->GetScenePos();
		c.r2 = c.pos - b2->GetScenePos();

		// Relative velocity at contact
		//Vec2D dv = b2->GetVelocity() + c.r2.crossLeft(b2->GetAngularVelocity()) - b1->GetVelocity() - c.r1.crossLeft(b1->GetAngularVelocity());
		Vector2D dv = aV.velocity[bIndex] + c.r2.crossLeft(aV.angularVelocity[bIndex]) - aV.velocity[aIndex] - c.r1.crossLeft(aV.angularVelocity[aIndex]);

		// Compute normal impulse
		float vn = dv.dot(c.normal);

		float dPn = c.massNormal * (-vn + c.bias);
		
		// Clamp the accumulated impulse
		float Pn0 = c.normalImpulse;
		c.normalImpulse = max(Pn0 + dPn, 0.0f);
		dPn = c.normalImpulse - Pn0;

		// Apply contact impulse
		Vec2D Pn = dPn * c.normal;

		//b1->ApplyImpulseToVelocity(-Pn);
		//b1->ApplyImpulseToAngularVelocity(-c.r1.cross(Pn));
		aV.velocity[aIndex] -= b1->GetInvMass() * Pn;
		aV.angularVelocity[aIndex] -= b1->GetInvInertia() * c.r1.cross(Pn);

		//b1->ApplyImpulseToVelocity(Pn);
		//b1->ApplyImpulseToAngularVelocity(c.r2.cross(Pn));
		aV.velocity[bIndex] += b2->GetInvMass() * Pn;
		aV.angularVelocity[bIndex] += b2->GetInvInertia() * c.r2.cross(Pn);

		// Relative velocity at contact
		//dv = b2->GetVelocity() + c.r2.crossLeft(b2->GetAngularVelocity()) - b1->GetVelocity() - c.r1.crossLeft(b1->GetAngularVelocity());
		dv = aV.velocity[bIndex] + c.r2.crossLeft(aV.angularVelocity[bIndex]) - aV.velocity[aIndex] - c.r1.crossLeft(aV.angularVelocity[aIndex]);

		Vec2D tangent = c.normal.crossRight(1.0f);
		float vt = dv.dot(tangent);
		float dPt = c.massTangent * (-vt);

		// Compute friction impulse
		float maxPt = friction * c.normalImpulse;

		// Clamp friction
		float oldTangentImpulse = c.normalTangetImpulses;
		c.normalTangetImpulses = MathLib::Clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
		dPt = c.normalTangetImpulses - oldTangentImpulse;

		// Apply contact impulse
		Vec2D Pt = dPt * tangent;

		//b1->ApplyImpulseToVelocity(-Pt);
		//b1->ApplyImpulseToAngularVelocity(-c.r1.cross(Pt));
		aV.velocity[aIndex] -= b1->GetInvMass() * Pt;
		aV.angularVelocity[aIndex] -= b1->GetInvInertia() * c.r1.cross(Pt);

		//b1->ApplyImpulseToVelocity(Pt);
		//b1->ApplyImpulseToAngularVelocity(c.r2.cross(Pt));
		aV.velocity[bIndex] += b2->GetInvMass() * Pt;
		aV.angularVelocity[bIndex] += b2->GetInvInertia() * c.r2.cross(Pt);

	}
}

void Collision2D::Update(Collision2D* other)
{
	CollisionPoint* points = other->collisionPoints;

	wasUpdated = true;

	CollisionPoint mergedContacts[2];

	for (unsigned i = 0; i < other->numCollisionPoints; ++i)
	{
		CollisionPoint& cNew = points[i];
		int k = -1;
		for (unsigned j = 0; j < numCollisionPoints; ++j)
		{
			CollisionPoint& cOld = collisionPoints[j];
			if (cNew.edges.value == cOld.edges.value)
			{
				k = j;
				break;
			}
		}

		if (k > -1)
		{
			CollisionPoint& c = mergedContacts[i];
			CollisionPoint& cOld = collisionPoints[k];
			c = cNew;
			
			c.normalImpulse = cOld.normalImpulse;
			c.normalTangetImpulses = cOld.normalTangetImpulses;
			c.noramlBiasImpulses = cOld.noramlBiasImpulses;
			
		}
		else
		{
			mergedContacts[i] = points[i];
		}

		if (numCollisionPoints > other->numCollisionPoints)
		{
			if (shouldRender)
			{
				for (unsigned i = other->numCollisionPoints; i < numCollisionPoints; i++)
				{
					debugObjects[i]->Destroy();
					debugObjects[i] = 0;
				}
			}
		}
		else if (numCollisionPoints < other->numCollisionPoints)
		{
			if (shouldRender)
			{
				for (unsigned i = numCollisionPoints; i < other->numCollisionPoints; i++)
				{
					DebugSceneObject2D* db = new DebugSceneObject2D("Debug Collision Point");
					db->SetScale(5, 5);
					db->SetColor({ 0.0, 1.0, 0.0 });
					db->SetPosition(collisionPoints[i].pos);

					debugObjects[i] = db;
					zEngine->AddSceneObject(db);
				}
			}
		}

		numCollisionPoints = other->numCollisionPoints;
	}
}

unsigned int Accumulated2DVelocities::AddObject(PhysicsObject2DBase * object)
{
	for (unsigned int i = 0; i < numObjects; i++)
	{
		if (objects[i] == object)
		{
			return i;
		}
	}

	unsigned int index = numObjects++;

	objects[index] = object;
	velocity[index] = object->GetVelocity();
	angularVelocity[index] = object->GetAngularVelocity();

	return index;
}
