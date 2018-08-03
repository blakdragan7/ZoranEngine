# ZoranEngine

ZoranEngine Is a game engine designed with Multiplatform capabilities
Is contains a main class ZoranEngine that Is what everything else funnels through

Each *Engine class is an abstraction from implementation. So RenderEngine is an abstraction from rendering specifics
Every *Engine class should be created from ZoranEngine itself and not by the user of the engine

The Only class's that can be created directly by the user are scene object but they should never be destroyed by the user
instead they call destroy() on the class and the engine will take care of it.

## Coding Standards
When creating class, All Variables and Functions are Camel Case. All Variables start with lower case letters and all Functions start with Uper case letters
Each section (public vs private vs protected) should be preceeded by a space (except the first) and function and variables should not be mixed

ex.
~~~
class SomeClass : public SceneObject
{
private:
	bool somePrivateBool;
	int  somePrivateInt;

protected:
	int someProtectedInt;

public:
	int somePublicInt;

private:
	void SomePrivateFunc();

protected:
	void SomeProtectedFunc();

public:
	SomeClass();
	~SomeClass();

	void SomPublicFunc();
}
~~~ 