#include "stdafx.h"
#include "Collision2D.h"
#include <Core/2D/SceneObject2D.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Math/MathLib.h>

Collision2D::Collision2D()
{
	collided = false;
	wasUpdated = false;
	collidedObjects[0] = 0;
	collidedObjects[1] = 0;
	objectBounds[0] = 0;
	objectBounds[1] = 0;
	velocitySnapshot[0] = 0;
	velocitySnapshot[1] = 0;
	friction = 0;
	ID = sID++;
	frame = 0;
}

Collision2D Collision2D::Reflection()
{
	Collision2D res;

	res.ID = ID;
	res.collided = collided;
	res.collidedObjects[0] = collidedObjects[1];
	res.collidedObjects[1] = collidedObjects[0];
	res.objectBounds[0] = objectBounds[1];
	res.objectBounds[1] = objectBounds[0];
	res.velocitySnapshot[0] = velocitySnapshot[1];
	res.velocitySnapshot[1] = velocitySnapshot[0];

	return res;
};

bool Collision2D::operator==(const Collision2D& other)
{
	return ID == other.ID;
}

void Collision2D::PreUpdate(float inv_dt)
{
	const float k_allowedPenetration = 0.01f;
	static const float k_biasFactor = 0.2f;

	for (CollisionPoint& collisionPoint : collisionPoints)
	{
		PhysicsObject2DBase* objectA = collidedObjects[0];
		PhysicsObject2DBase* objectB = collidedObjects[1];

		Vector2D r1 = collisionPoint.pos - objects[0]->GetPosition();
		Vector2D r2 = collisionPoint.pos - objects[1]->GetPosition();

		// Precompute normal mass, tangent mass, and bias.
		float rn1 = r1.dot(collisionPoint.normal);
		float rn2 = r2.dot(collisionPoint.normal);

		float kNormal = objectA->invMass + objectB->invMass;
		kNormal += objectA->invMass * (r1.dot(r1) - rn1 * rn1) + objectB->invInertia * (r2.dot(r2) - rn2 * rn2);
		collisionPoint.massNormal = 1.0f / kNormal;

		Vector2D tangent = collisionPoint.normal.crossRight(1.0f);
		float rt1 = r1.dot(tangent);
		float rt2 = r2.dot(tangent);
		float kTangent = objectA->invMass + objectB->invMass;
		kTangent += objectA->invInertia * (r1.dot(r1) - rt1 * rt1) + objectB->invInertia * (r2.dot(r2) - rt2 * rt2);
		collisionPoint.massTangent = 1.0f / kTangent;

		collisionPoint.bias = -k_biasFactor * inv_dt * min(0.0f, collisionPoint.separation + k_allowedPenetration);

		// Apply normal + friction impulse
		Vector2D impulse = collisionPoint.normalImpulse * collisionPoint.normal + collisionPoint.normalTangetImpulses * tangent;

		objectA->ApplyImpulseToVelocity(-impulse);
		objectA->ApplyImpulseToAngularVelocity(-r1.cross(impulse));

		objectB->ApplyImpulseToVelocity(impulse);
		objectB->ApplyImpulseToAngularVelocity(r2.cross(impulse));

	}
}

void Collision2D::UpdateForces()
{
	wasUpdated = false;

	PhysicsObject2DBase* b1 = collidedObjects[0];
	PhysicsObject2DBase* b2 = collidedObjects[1];

	for (CollisionPoint& collisionPoint : collisionPoints)
	{
		Vector2D r1 = collisionPoint.pos - objects[0]->GetPosition();
		Vector2D r2 = collisionPoint.pos - objects[1]->GetPosition();

		// Relative velocity at contact
		Vector2D dv = b2->GetVelocity() + collisionPoint.r2.crossLeft(b2->GetAngularVelocity()) - b1->GetVelocity() - collisionPoint.r1.crossLeft(b1->GetAngularVelocity());

		// Compute normal impulse
		float vn = dv.dot(collisionPoint.normal);

		float dPn = collisionPoint.massNormal * (-vn + collisionPoint.bias);

		// Clamp the accumulated impulse
		float Pn0 = collisionPoint.normalImpulse;
		collisionPoint.normalImpulse = max(Pn0 + dPn, 0.0f);
		dPn = collisionPoint.normalImpulse - Pn0;

		// Apply contact impulse
		Vector2D Pn = dPn * collisionPoint.normal;

		b1->ApplyImpulseToVelocity(-Pn);
		b1->ApplyImpulseToAngularVelocity(-collisionPoint.r1.cross(Pn));

		b2->ApplyImpulseToVelocity(Pn);
		b2->ApplyImpulseToAngularVelocity(collisionPoint.r2.cross(Pn));

		// Relative velocity at contact
		dv = b2->GetVelocity() + collisionPoint.r2.crossLeft(b2->GetAngularVelocity()) - b1->GetVelocity() - collisionPoint.r1.crossLeft(b1->GetAngularVelocity());

		Vector2D tangent = collisionPoint.normal.crossRight(1.0f);
		float vt = dv.dot(tangent);
		float dPt = collisionPoint.massTangent * (-vt);

		// Compute friction impulse
		float maxPt = friction * collisionPoint.normalImpulse;

		// Clamp friction
		float oldTangentImpulse = collisionPoint.normalTangetImpulses;
		collisionPoint.normalTangetImpulses = MathLib::Clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
		dPt = collisionPoint.normalTangetImpulses - oldTangentImpulse;


		// Apply contact impulse
		Vector2D Pt = dPt * tangent;

		b1->ApplyImpulseToVelocity(-Pt);
		b1->ApplyImpulseToAngularVelocity(-collisionPoint.r1.cross(Pt));

		b2->ApplyImpulseToVelocity(Pt);
		b2->ApplyImpulseToAngularVelocity(collisionPoint.r2.cross(Pt));
	}
}

void Collision2D::Update(std::vector<CollisionPoint> points)
{
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

	for (int i = 0; i < points.size(); ++i)
		collisionPoints[i] = mergedContacts[i];

}