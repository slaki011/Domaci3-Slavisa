#ifndef FILESYSTEMFACADE_H
#define FILESYSTEMFACADE_H
#include"FilesystemOperations.h"
#include"FilesystemException.h"
class FSObject;
class Folder;
class File;

class Filesystem {
private:
	Filesystem();
	static Filesystem* instance;
	
	long size,used;
public:
	Folder* rootFolder;
	static Filesystem* getInstance();
	File* createFile(Text* fName, Folder* destFolder);
	Folder* createFolder(Text* fName, Folder* parentFolder=nullptr);
	std::vector<FSObject*> listFolder(Folder* folder);
	Byte* readFile(File* file);
	void writeFile(Byte* content,File* f);
	void grantAccess(FSObject* fObj, Text* opName = nullptr);
	void revokeAccess(FSObject* fObj, Text* opName = nullptr);
	Folder* openFolder(Text* folderPath);
	std::vector<FSObject*> search(Text* objName);
	void copyPaste(FSObject* objToCopy, Folder* destFolder, Text* name);
	void move(FSObject* objToMove, Folder* destFolder);
	bool deleteF(FSObject* objToDelete);
	void setSize(long a);
	long freeSpace();
	~Filesystem() {
		delete rootFolder;
	}
};


#endif