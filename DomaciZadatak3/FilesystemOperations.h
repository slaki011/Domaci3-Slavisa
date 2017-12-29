#ifndef FILESYSTEMOPERATIONS_H
#define FILESYSTEMOPERATIONS_H
#include"FilesystemObject.h"
#include<vector>
class FSObject;
class Folder;
class File;
typedef char Text;
typedef char* Byte;

class FSOperation {
private:
	Text* opName;
public:
	FSOperation(Text* t):opName(t){}
	virtual void execute() = 0;
	Text* getName();
};
//done!
class ReadFile :public FSOperation{
private:
	
public:
	File* f;
	Byte* stanje;
	ReadFile(File* fn) :FSOperation("ReadFile"),f(fn) {}
	void execute();
};
//done!
class WriteFile :public FSOperation {
public:
	File* f;
	Byte* content;
	WriteFile(Byte* content,File* f) :FSOperation("WriteFile"),f(f),content(content) {}
	void execute();
};
class CreateFile :public FSOperation {
private:
	Text* fName;
	Folder* destFolder;
public:
	File* f;
	CreateFile(Text* fName,Folder* destFolder) :FSOperation("CreateFile"),fName(fName),destFolder(destFolder) {}
	void execute();
};
//done!
class DeleteObject :public FSOperation {
public:
	FSObject* objToDelete;
	DeleteObject(FSObject *objToDelete) :FSOperation("DeleteObject"),objToDelete(objToDelete) {}
	void execute();
};
class CreateFolder :public FSOperation {
public:
	Folder* f;
	Folder* parentFolder;
	Text* fName;
	CreateFolder(Text* fName,Folder* parentFolder) :FSOperation("CreateFolder"),fName(fName),parentFolder(parentFolder) {}
	void execute();
};
//done!
class CopyPaste :public FSOperation {
public:
	FSObject *objToCopy, *rez;
	Folder* destFolder;
	Text* name;
	CopyPaste(FSObject* objToCopy,Folder* destFolder,Text* name) :FSOperation("CopyPaste"),name(name),objToCopy(objToCopy),destFolder(destFolder) {}
	void execute();
};
//done!
class ListDirectory :public FSOperation {
public:
	std::vector<FSObject*> rez;
	Folder* folder;
	ListDirectory(Folder* f) :FSOperation("ListDirectory"),folder(f) {}
	void execute();
};
//done!
class Search :public FSOperation {
public:
	Text* folderToSearch;
	Folder* root;
	std::vector<FSObject*> found;
	Search(Text *folderToSearch,Folder* root) :FSOperation("Search"), folderToSearch(folderToSearch){}
	void execute();
};
class ProtectedOperation :public FSOperation {
private:
	FSObject *ff,*ff2;
public:
	FSOperation* wrappedOperation;
	ProtectedOperation(FSOperation* operacija, FSObject* ff1, FSObject* ff2 = nullptr) :FSOperation("protectedOperation"), wrappedOperation(operacija), ff(ff1), ff2(ff2) {}
	void execute();
	bool checkPrecondition();
};
class Move :public FSOperation {
public:
	FSObject *objToMove;
	Folder* destFolder;
	Move(FSObject * objToMove, Folder * destFolder) :FSOperation("Move"),objToMove(objToMove),destFolder(destFolder) {}
	void execute();
};
#endif