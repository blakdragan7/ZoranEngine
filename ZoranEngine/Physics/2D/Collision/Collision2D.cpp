#include "stdafx.h"
#include "Collision2D.h"
#include <Core/2D/DebugSceneObject2D.h>
#include <Rendering/RenderedObjectBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Math/MathLib.h>
#include <OpenGL/DebugShader2D.h>

void Collision2D::AddDebugObject(Vec2D pos)
{
	if (shouldRender == false)return;
	DebugSceneObject2D* debugObject = new DebugSceneObject2D("collision point");
	debugObject->SetColor(Vec3D(0.0,1.0,0.0));

	debugObject->GetRenderedObject()->MakeFullScreenQuad();
	debugObject->SetScale(5, 5);
	debugObject->SetPosition(pos);

	zEngine->AddSceneObject(debugObject);

	debugObjects.push_back(debugObject);
}

void Collision2D::AddCollisionPoint(CollisionPoint & point)
{
	collisionPoints.push_back(point);
	AddDebugObject(point.pos);
}

Collision2D::Collision2D()
{
	collided = false;
	wasUpdated = false;
	collidedObjects[0] = 0;
	collidedObjects[1] = 0;
	objectBounds[0] = 0;
	objectBounds[1] = 0;
	friction = 0;
	ID = sID++;
	frame = 0;
	shouldRender = false;
}

Collision2D::~Collision2D()
{
	for(auto debugObject : debugObjects)
	{
		debugObject->Destroy();
		debugObject = 0;
	}

	debugObjects.clear();
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
	if (shouldRender == false)
	{
		shouldRender = true;
		for (auto point : collisionPoints)
		{
			AddDebugObject(point.pos);
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
		Vec2D dv = aV.velocity[bIndex] + c.r2.crossLeft(aV.angularVelocity[bIndex]) - aV.velocity[aIndex] - c.r1.crossLeft(aV.angularVelocity[aIndex]);

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
	std::vector<CollisionPoint>& points = other->collisionPoints;

	wasUpdated = true;

	CollisionPoint mergedContacts[2];

	for (int i = 0; i < points.size(); ++i)
	{
		CollisionPoint& cNew = points[i];
		int k = -1;
		for (int j = 0; j < collisionPoints.size(); ++j)
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
	}

	 // slow ... optimize later

	int diff = collisionPoints.size() - points.size();

	if (diff > 0)
	{
		for (int i = 0; i < diff; i++)
		{
			auto object = debugObjects.back();
			object->Destroy();
			debugObjects.pop_back();

			collisionPoints.pop_back();
		}
	}

	for (int i = 0; i < points.size(); ++i)
	{
		if (i >= collisionPoints.size())
			collisionPoints.push_back(mergedContacts[i]);
		else
			collisionPoints[i] = mergedContacts[i];
		if (i >= debugObjects.size())
			AddDebugObject(mergedContacts[i].pos);
		else
			debugObjects[i]->SetPosition(mergedContacts[i].pos);
	}
}

unsigned Accumulated2DVelocities::AddObject(PhysicsObject2DBase * object)
{
	for (unsigned i = 0; i < numObjects; i++)
	{
		if (objects[i] == object)
		{
			return i;
		}
	}

	unsigned index = numObjects++;

	objects[index] = object;
	velocity[index] = object->GetVelocity();
	angularVelocity[index] = object->GetAngularVelocity();

	return index;
}
