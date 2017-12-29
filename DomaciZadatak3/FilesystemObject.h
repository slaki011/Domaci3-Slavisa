#ifndef FILESYSTEMOBJECT_H
#define FILESYSTEMOBJECT_H
#include"FilesystemException.h"
#include<string>
#include<vector>
using namespace std;


typedef char Text;
typedef char* Byte;

class AccessDescriptor;
class Folder;
class FilesystemVisitor;
class FSObject {
protected:
	AccessDescriptor *accessDescriptor;
	Text* name;
public:
	FSObject(Text* nm, Folder* fn = nullptr);
	Folder* parent;                                                               //0..1 ?????
	Text* getName();
	void setName(Text* name);
	AccessDescriptor* getAccessDescriptor() {
		if (accessDescriptor == nullptr) return nullptr;
		return accessDescriptor;
	}
	virtual void accept(FilesystemVisitor* v) = 0;
	virtual long size() = 0;
	virtual FSObject* copy() = 0;
	virtual ~FSObject();
};
class Folder :public FSObject {
private:
	
public:
	Folder(Text* a, Folder* fn);
	void accept(FilesystemVisitor* v);
	void add(FSObject* o);
	long size() { return 0; }
	std::vector<FSObject*> getObjects();
	void remove(FSObject* obj);
	std::vector<FSObject*> containedObjects;
	FSObject* copy();
	~Folder();
};


class File :public FSObject {
private:
	Byte* content;
public:
	File(Text* a, Folder* fn);
	void accept(FilesystemVisitor* v);
	void write(Byte* content);
	FSObject* copy();
	long size();
	Byte* read();
	~File();
};
class FilesystemVisitor {
public:
	//FilesystemVisitor();
	virtual void visitFile(File* f) = 0;
	virtual void visitFolder(Folder* f) = 0;
};
class SearchVisitor :public FilesystemVisitor {
private:
	Text* filename;
public:
	Folder* folder;
	SearchVisitor(Text*);
	void visitFile(File* f);
	void visitFolder(Folder* f);
	vector<FSObject*> foundObjects;
};
class AccessDescriptor {
private:
	FSObject* protectedObject;
	vector<Text*> *allowedOperations;
public:
	AccessDescriptor(FSObject*);
	void add(Text* operationName);
	void remove(Text* operationName);
	vector<Text*> getAllowedOperations();
	bool checkAccess(Text* operationName);
	~AccessDescriptor();
};

class CheckVisitor :public FilesystemVisitor {
private:
	Text* access;
public:
	bool check;
	CheckVisitor(Text*);
	void visitFile(File* f);
	void visitFolder(Folder* f);
};
class SizeVisitor :public FilesystemVisitor {
public:
	long size;
	SizeVisitor() { size = 0; }
	void visitFile(File* f);
	void visitFolder(Folder* f);
};




#endif