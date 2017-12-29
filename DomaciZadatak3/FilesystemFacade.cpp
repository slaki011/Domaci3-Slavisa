#include "FilesystemFacade.h"
#include "FilesystemObject.h"
#include "FilesystemOperations.h"


Filesystem* Filesystem::instance = nullptr;

Filesystem::Filesystem() {
	rootFolder = new Folder("rootFolder", nullptr);
}

Filesystem * Filesystem::getInstance()
{
	if (instance == nullptr)
		instance = new Filesystem();
	return instance;
}

File* Filesystem::createFile(Text * fName, Folder * destFolder)
{
	CreateFile cf(fName, destFolder);
	ProtectedOperation po(&cf, destFolder);
	if (!po.checkPrecondition()) {
		throw OperationFailedException("Greska pri izvrsavanju operacije CreateFile");
		return nullptr;
	}
	else
		return cf.f;
}
Folder* Filesystem::createFolder(Text * fName, Folder * parentFolder)
{
	if (parentFolder == nullptr)
		parentFolder = rootFolder;
	CreateFolder cf(fName, parentFolder);
	ProtectedOperation po(&cf, parentFolder);
	if (!po.checkPrecondition()) { 
		throw OperationFailedException("Greska pri izvrsavanju operacije CreateFolder"); 
		return nullptr;
	}
	else
	return cf.f;
}
std::vector<FSObject*> Filesystem::listFolder(Folder * folder)
{
	ListDirectory ld(folder);
	ProtectedOperation po(&ld, folder);
	if (!po.checkPrecondition()) {
		throw OperationFailedException("Greska u operaciji listFolder!");
	}
	return ld.rez;
}
Byte* Filesystem::readFile(File * file)
{
	ReadFile rf(file);
	ProtectedOperation po(&rf, file);
	if (!po.checkPrecondition()) {
		throw OperationFailedException("Greska u operaciji writeFile!");
	}
	else
	return rf.stanje;
}
void Filesystem::writeFile(Byte * content, File* f)
{
	if (freeSpace()<sizeof(content))throw OperationFailedException("Nema dovoljno memorije na disku!");
	WriteFile wf(content, f);
	ProtectedOperation po(&wf,f);
	if (!po.checkPrecondition()) {
		throw OperationFailedException("Greska u operaciji writeFile!");
	}
}
void Filesystem::grantAccess(FSObject * fObj, Text * opName)
{
	AccessDescriptor* a = fObj->getAccessDescriptor();
	if (a->checkAccess(opName)) {
		return;
	}
	else {
		a->add(opName);
	}
}
void Filesystem::revokeAccess(FSObject * fObj, Text * opName)
{
	AccessDescriptor* a = fObj->getAccessDescriptor();
	if (a->checkAccess(opName)) {
		a->remove(opName);
	}
	else {
		return;
	}
}
Folder * Filesystem::openFolder(Text * folderPath)
{
	std::vector<char*> putanja;
	Text *ime = new Text();
	for (int i = 0; i < sizeof(folderPath) / sizeof(folderPath[0]); i++) {
		if (folderPath[i] == '/' || folderPath[i] == '\0') {
			putanja.push_back(ime);
			ime = '\0';
			continue;
		}
		else {
			Text* pomoc = new Text[strlen(ime) + 1];
		
			strcpy(ime, pomoc);
		}
	}
	Folder* root = rootFolder;
	for (unsigned int i = 0; i < putanja.size(); i++) {
		Text* key = putanja[i];
		SearchVisitor v(key);
		root->accept(&v);
		Folder* novi = v.folder;
		root = novi;
	}
	return root;
}
std::vector<FSObject*> Filesystem::search(Text * objName)
{
	Search s(objName, rootFolder);
	ProtectedOperation po(&s, rootFolder);
	if (!po.checkPrecondition()) {
		throw OperationFailedException("Greska u operaciji delete!");
		return s.found;
	}
	else
	return s.found;
}
void Filesystem::copyPaste(FSObject * objToCopy, Folder * destFolder, Text * name)
{
	CopyPaste cp(objToCopy, destFolder, name);
	cp.execute();
}

void Filesystem::move(FSObject * objToMove, Folder * destFolder)
{
	Move m(objToMove, destFolder);
	ProtectedOperation po(&m, objToMove, destFolder);
	if (!po.checkPrecondition()) {
		throw OperationFailedException("Greska u operaciji move!");
	}
}

bool Filesystem::deleteF(FSObject * objToDelete)
{
	DeleteObject del(objToDelete);
	ProtectedOperation po(&del, objToDelete);
	if (!po.checkPrecondition()) {
		throw OperationFailedException("Greska u operaciji delete!");
		return false;
	}
	else
		return true;
}
void Filesystem::setSize(long a)
{
	this->size = a;
}

long Filesystem::freeSpace()
{
	SizeVisitor sv;
	rootFolder->accept(&sv);
	used = sv.size;
	return size - used;
}

