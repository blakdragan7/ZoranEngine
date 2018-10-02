#include "stdafx.h"

#include <Physics/2D/Collision/QuadTreeCollisionBucket.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>
#include <Physics/3D/Collision/SphereCollisionObject.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>
#include <Physics/3D/Collision/AABBoxCollisionObject.h>
#include <Physics/2D/Collision/QuadTreeCollision.h>
#include <Physics/2D/Collision/CollisionFrame2D.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Core/2D/SceneObject2D.h>
#include <Utils/VectorAddons.hpp>
#include <Core/SceneObject.h>
#include <Core/ZoranEngine.h>
#include <Core/ThreadBase.h>
#include <assert.h>
#include <string>

#include <Utils/Statistics.h>

#include <ThirdParty/imgui/imgui.h>

static unsigned long long numBuckets = 0;

QuadTreeCollisionBucket::QuadTreeCollisionBucket(std::string name, Vec2D pos, Vec2D size, unsigned maxObjects,QuadTreeCollisionBucket * parent) : CollisionBucket2DBase(name,pos,maxObjects)
{
	children[0] = 0;
	children[1] = 0;
	children[2] = 0;
	children[3] = 0;

	hasSubdivided = false;
	this->parent = parent;
	numBuckets++;

	Vec2D halfSize = size / 2.0;

	collision = new QuadTreeCollision(pos - halfSize,pos + halfSize, sceneObject->Get2DRoot());

	Log(LogLevel_Verbose,"Buckets: %i", numBuckets);
}


QuadTreeCollisionBucket::~QuadTreeCollisionBucket()
{
	if (hasSubdivided)
	{
		delete children[0];
		delete children[1];
		delete children[2];
		delete children[3];
	}

	numBuckets--;
}

bool QuadTreeCollisionBucket::AddObject(CollisionObject2DBase * object)
{
	assert(FindObject(object) == -1);

	bool wasAdded = false;

	while (mutex.try_lock() == false)ThreadBase::Sleep(0.001F);
	if ((collisionObjects.size() < maxObjects && !hasSubdivided))
	{
		if (ObjectIsWithinBucket(object))
		{
			collisionObjects.push_back(object);
			wasAdded = true;

			Log(LogLevel_Debug,"Object %s Add To Bucket %s\n", object->GetAffectedComponent()->GetSceneName(), sceneObject->readableName->c_str());
		}
	}
	else 
	{
		if (hasSubdivided == false)Subdivide();
		
		for (unsigned i = 0; i < 4; ++i)
		{
			if (children[i]->AddObject(object))
			{
				wasAdded = true;
				break;
			}
		}
		if (wasAdded == false)
		{
			bool objectIsinf = ObjectIsWithinBucket(object);
			if (objectIsinf || parent == 0)
			{
				collisionObjects.push_back(object);
				wasAdded = true;
			}
		}
	}

	mutex.unlock();
	return wasAdded;
}

CollisionObject2DBase * QuadTreeCollisionBucket::RemoveObject(CollisionObject2DBase * object)
{
	while (mutex.try_lock() == false)ThreadBase::Sleep(0.001F);
	auto iter = std::find(collisionObjects.begin(), collisionObjects.end(), object);
	if (iter != collisionObjects.end())
	{
		size_t index = iter - collisionObjects.begin();
		remove(collisionObjects,index);
		mutex.unlock();
		return object;
	}
	if(hasSubdivided)
	{
		for (unsigned i = 0; i < 4; ++i)
		{
			if (children[i]->RemoveObject(object))
			{
				mutex.unlock();
				return object;
			}
		}
	}
	mutex.unlock();
	return nullptr;
}

bool QuadTreeCollisionBucket::UpdateObject(CollisionObject2DBase * object)
{
	// Static collision objects don't need to be updated
	//if (object->GetDynamics() == CD_Static)return false;
	object->SetBoundsBySceneObject();
	object->isDirty = false;
	int index = FindObject(object);
	if (index != -1)
	{
		bool wasAdded = false;
		if (hasSubdivided)
		{
			for (unsigned i = 0; i < 4; ++i)
			{
				if (children[i]->AddObject(object))
				{
					remove(collisionObjects, index);
					wasAdded = true;
					return true;
				}
			}
		}

		if (wasAdded == false)
		{
			if (ObjectIsWithinBucket(object) == false)
			{
				if (parent)
				{
					Log(LogLevel_Debug, "Object %s No Longer in Bucket %s\n", object->GetAffectedComponent()->GetSceneName(), sceneObject->readableName->c_str());

					remove(collisionObjects,index);
					return parent->TraverseUpAddObject(object);
				}
			}
		}
	}
	else if(hasSubdivided)
	{
		for (unsigned i = 0; i < 4; ++i)
		{
			if (children[i]->UpdateObject(object))
			{
				return true;
			}
		}
	}

	return false;
}



void QuadTreeCollisionBucket::UpdateAllObjects()
{
	for (auto object : collisionObjects)
	{
		//if (object->isDirty)
		{
			UpdateObject(object);
		}
	}

	if (hasSubdivided)
	{
		children[0]->UpdateAllObjects();
		children[1]->UpdateAllObjects();
		children[2]->UpdateAllObjects();
		children[3]->UpdateAllObjects();
	}
}

void QuadTreeCollisionBucket::CheckAllCollision(struct CollisionFrame2D& frame)
{
 	if (hasSubdivided)
	{
		children[0]->CheckAllCollision(frame);
		children[1]->CheckAllCollision(frame);
		children[2]->CheckAllCollision(frame);
		children[3]->CheckAllCollision(frame);
	}

	for (unsigned i = 0;i<collisionObjects.size();++i)
	{
		CollisionObject2DBase* object = collisionObjects[i];
		
		if (object->GetPhysicsObject()->GetIsSweptCollision() || object->GetDynamics() == CD_Static)continue;

		if (i <= collisionObjects.size() - 1)
		{
			// TODO remove bandade and actually fix like below suggestion 
			// This is more of a bandade then anything
			// Instead of this, static objects need there own array for efficiency
			unsigned limit = i;
			for (unsigned j = 0; j < collisionObjects.size(); ++j)
			{
				if ((j < limit && collisionObjects[j]->GetDynamics() != CD_Static) || j == i)continue;
				if (auto collision = object->CollidesWith(collisionObjects[j]))
				{
					frame.UpdateCollisionWithKey(Collision2DKey(collision->objects[0], collision->objects[1]), collision);
				}
			}
		}

		if (hasSubdivided)
		{
			children[0]->CheckCollisionForObject(object, frame);
			children[1]->CheckCollisionForObject(object, frame);
			children[2]->CheckCollisionForObject(object, frame);
			children[3]->CheckCollisionForObject(object, frame);
		}

		if (parent)parent->CheckCollisionForObjectTraverseUp(object, &frame);
	}

}

bool QuadTreeCollisionBucket::ObjectIsWithinBucket(CollisionObject2DBase * object)
{
	// always add to the root tree if it hasnt subdivided yet
	return collision->CollidesWithNoCollision(object) || (hasSubdivided == false && parent == 0);
}

void QuadTreeCollisionBucket::Subdivide()
{
	if (hasSubdivided)
	{
		std::cerr << "QuadTreeCollisionBucket::Subdivide() Already Subdivided !\n";
		return;
	}

	Vec2D subSize = collision->GetSize() / 2.0f;
	Vec2D size = collision->GetSize();

	Vec2D pos = collision->GetScenePos();

	Vector2D TL = Vector2D(pos.x - (size.x / 4.0f), pos.y + (size.y / 4.0f));
	Vector2D BL = Vector2D(pos.x - (size.x / 4.0f), pos.y - (size.y / 4.0f));
	Vector2D TR = Vector2D(pos.x + (size.x / 4.0f), pos.y + (size.y / 4.0f));
	Vector2D BR = Vector2D(pos.x + (size.x / 4.0f), pos.y - (size.y / 4.0f));

	std::string sname = *sceneObject->readableName;

	children[TL_BUCKET] = new QuadTreeCollisionBucket((sname + ".topleft").c_str(),TL, subSize, maxObjects, this);
	children[TR_BUCKET] = new QuadTreeCollisionBucket((sname + ".topright").c_str(),TR, subSize, maxObjects, this);
	children[BL_BUCKET] = new QuadTreeCollisionBucket((sname + ".bottomleft").c_str(),BL, subSize, maxObjects, this);
	children[BR_BUCKET] = new QuadTreeCollisionBucket((sname + ".bottomright").c_str(),BR, subSize, maxObjects, this);

	std::vector<CollisionObject2DBase*> keptObjects;

	for (auto object : collisionObjects)
	{
		bool wasAdded = false;
		for (unsigned i = 0; i < 4; ++i)
		{
			if (children[i]->AddObject(object))
			{
				wasAdded = true;
				break;
			}
		}
		if (wasAdded == false)
		{
			keptObjects.push_back(object);
		}
	}

	collisionObjects.clear();

	collisionObjects = keptObjects;

	hasSubdivided = true;
}

Collision2D* QuadTreeCollisionBucket::CheckCollisionForObjectNoFrame(CollisionObject2DBase * object)
{
	for (unsigned i = 0; i < collisionObjects.size(); ++i)
	{
		if (auto collision = object->CollidesWith(collisionObjects[i]))
		{
			return collision;
		}
	}

	if (hasSubdivided)
	{
		if (auto collision = children[0]->CheckCollisionForObjectNoFrame(object))return collision;
		if (auto collision = children[1]->CheckCollisionForObjectNoFrame(object))return collision;
		if (auto collision = children[2]->CheckCollisionForObjectNoFrame(object))return collision;
		if (auto collision = children[3]->CheckCollisionForObjectNoFrame(object))return collision;
	}

	return false;
}

void QuadTreeCollisionBucket::CheckCollisionForObject(CollisionObject2DBase * object, CollisionFrame2D& frame)
{
	for (unsigned i = 0; i < collisionObjects.size(); ++i)
	{
		if (auto collision = object->CollidesWith(collisionObjects[i]))
		{
			frame.UpdateCollisionWithKey(Collision2DKey(collision->objects[0], collision->objects[1]), collision);
		}
	}

	if (hasSubdivided)
	{
		children[0]->CheckCollisionForObject(object, frame);
		children[1]->CheckCollisionForObject(object, frame);
		children[2]->CheckCollisionForObject(object, frame);
		children[3]->CheckCollisionForObject(object, frame);
	}
}

void QuadTreeCollisionBucket::CheckCollisionForObjectTraverseUp(CollisionObject2DBase * object, CollisionFrame2D * frame)
{
	for (unsigned i = 0; i < collisionObjects.size(); ++i)
	{
		if (auto collision = object->CollidesWith(collisionObjects[i]))
		{
			frame->UpdateCollisionWithKey(Collision2DKey(collision->objects[0], collision->objects[1]), collision);
		}
	}

	if (parent)
	{
		parent->CheckCollisionForObjectTraverseUp(object,frame);
	}
}

bool QuadTreeCollisionBucket::SweepCollision(CollisionObject2DBase * object, Vec2D newPosition, SweepCollision2D & response)
{
	int index = FindObject(object);
	if (index == -1)
	{
		if (children)
		{
			if (children[0]->SweepCollision(object, newPosition, response))return true;
			if (children[1]->SweepCollision(object, newPosition, response))return true;
			if (children[2]->SweepCollision(object, newPosition, response))return true;
			if (children[3]->SweepCollision(object, newPosition, response))return true;
		}
	}
	else
	{
		if (SweepCollisionHitTest(object, newPosition, response))return true;

		if (hasSubdivided)
		{
			if (children[0]->SweepCollisionHitTest(object, newPosition, response))return true;
			if (children[1]->SweepCollisionHitTest(object, newPosition, response))return true;
			if (children[2]->SweepCollisionHitTest(object, newPosition, response))return true;
			if (children[3]->SweepCollisionHitTest(object, newPosition, response))return true;
		}
		return true;
	}
	return false;
}

bool QuadTreeCollisionBucket::SweepCollisionHitTest(CollisionObject2DBase * object, Vec2D newPosition, SweepCollision2D & response)
{
	Vector2D origin = object->GetScenePos();
	Vector2D deltaPos = newPosition - origin;
	Vector2D size = object->GetSize();
	
	Vector2D InvEntry;
	Vector2D InvExit;

	for (unsigned i = 0; i < collisionObjects.size(); i++)
	{
		CollisionObject2DBase* other = collisionObjects[i];
		if (other == object)continue;

		if (object->SweepCollidesWith(other, newPosition, response))return true;

	}

	return false;
}

Collision2D* QuadTreeCollisionBucket::CheckObjectAgainstStaic(CollisionObject2DBase * object)
{
	for (unsigned i = 0; i < collisionObjects.size(); i++)
	{
		if (collisionObjects[i]->GetDynamics() != CD_Static)continue;

		if (auto collision = object->CollidesWith(collisionObjects[i]))
		{
			return collision;
		}
	}

	if (hasSubdivided)
	{
		if (auto collision = children[0]->CheckCollisionForObjectNoFrame(object))return collision;
		if (auto collision = children[1]->CheckCollisionForObjectNoFrame(object))return collision;
		if (auto collision = children[2]->CheckCollisionForObjectNoFrame(object))return collision;
		if (auto collision = children[3]->CheckCollisionForObjectNoFrame(object))return collision;
	}

	return false;
}

bool QuadTreeCollisionBucket::TraverseUpAddObject(CollisionObject2DBase * object)
{
	if (AddObject(object) == false)
		if(parent)
			return parent->TraverseUpAddObject(object);

	return true;
}

// Debug Prints

void QuadTreeCollisionBucket::PrintAllContents(unsigned depth)
{
	for (unsigned int i = 0; i < depth; ++i)Log(LogLevel_None, "\t");
	Log(LogLevel_None, "%s Contains: \n", sceneObject->readableName->c_str());
	for (auto object : collisionObjects)
	{
		for (unsigned int j = 0; j < depth; ++j)Log(LogLevel_None, "\t");
		Log(LogLevel_None, "\t%s\n", object->GetAffectedComponent()->GetSceneName());
	}
	if (hasSubdivided)
	{
		int new_depth = depth + 1;
		children[0]->PrintAllContents(new_depth);
		children[1]->PrintAllContents(new_depth);
		children[2]->PrintAllContents(new_depth);
		children[3]->PrintAllContents(new_depth);
	}
}

void QuadTreeCollisionBucket::PrintCollisionForObject(CollisionObject2DBase * object)
{
	for (unsigned i = 0; i < collisionObjects.size(); i++)
	{
		Log(LogLevel_None, "\tCheck Collision %s => %s\n", object->GetAffectedComponent()->GetSceneName(), collisionObjects[i]->GetAffectedComponent()->GetSceneName());
		if (object->CollidesWithNoCollision(collisionObjects[i]))
		{
			Log(LogLevel_None, "Found Collision\n");
		}
	}

	if (hasSubdivided)
	{
		children[0]->PrintCollisionForObject(object);
		children[1]->PrintCollisionForObject(object);
		children[2]->PrintCollisionForObject(object);
		children[3]->PrintCollisionForObject(object);
	}
}

void QuadTreeCollisionBucket::PrintCollisionForObjectTraverseUp(CollisionObject2DBase * object)
{
	for (unsigned i = 0; i < collisionObjects.size(); i++)
	{
		Log(LogLevel_None, "\tCheck Collision %s => %s\n", object->GetAffectedComponent()->GetSceneName(), collisionObjects[i]->GetAffectedComponent()->GetSceneName());
		if (object->CollidesWithNoCollision(collisionObjects[i]))
		{
			Log(LogLevel_None, "Found Collision\n");
		}
	}

	if (parent)parent->PrintCollisionForObjectTraverseUp(object);
}

void QuadTreeCollisionBucket::ImGuiDraw()
{
	ImGui::Begin("QuadTreeCollisionBucket Contents");

	DrawImGuiDown();

	ImGui::End();
}

void QuadTreeCollisionBucket::DrawImGuiDown(unsigned depth)
{
	if (ImGui::TreeNode(sceneObject->readableName->c_str()))
	{
		for (auto object : collisionObjects)
		{
			ImGui::Text((object->GetAffectedComponent()->GetSceneName()));
		}

		if (hasSubdivided)
		{
			children[0]->DrawImGuiDown(depth + 1);
			children[1]->DrawImGuiDown(depth + 1);
			children[2]->DrawImGuiDown(depth + 1);
			children[3]->DrawImGuiDown(depth + 1);
		}

		ImGui::TreePop();
	}
}

void QuadTreeCollisionBucket::PrintAllCollisions()
{
	if (hasSubdivided)
	{
		children[0]->PrintAllCollisions();
		children[1]->PrintAllCollisions();
		children[2]->PrintAllCollisions();
		children[3]->PrintAllCollisions();
	}

	for (unsigned i = 0; i<collisionObjects.size(); i++)
	{
		CollisionObject2DBase* object = collisionObjects[i];

		if (object->GetDynamics() != CD_Dynamic)continue;

		if (i < collisionObjects.size() - 1)
		{
			for (unsigned j = i + 1; j < collisionObjects.size(); j++)
			{
				Log(LogLevel_None, "\tCheck Collision %s => %s\n", object->GetAffectedComponent()->GetSceneName(), collisionObjects[j]->GetAffectedComponent()->GetSceneName());

				if (object->CollidesWithNoCollision(collisionObjects[j]))
				{
					Log(LogLevel_None, "Found Collision\n");
				}
			}
		}

		if (hasSubdivided)
		{
			children[0]->PrintCollisionForObject(object);
			children[1]->PrintCollisionForObject(object);
			children[2]->PrintCollisionForObject(object);
			children[3]->PrintCollisionForObject(object);
		}

		if (parent)parent->PrintCollisionForObject(object);
	}
}
