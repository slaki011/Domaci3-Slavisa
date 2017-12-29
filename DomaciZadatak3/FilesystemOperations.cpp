#include "FilesystemOperations.h"
#include "FilesystemObject.h"


void WriteFile::execute()
{
	f->write(content);
}
void CreateFile::execute()
{
	f = new File(fName, destFolder);
	destFolder->add(f);
}
void DeleteObject::execute()
{
	(objToDelete->parent)->remove(objToDelete);
}
void CreateFolder::execute()
{
	f = new Folder(fName, parentFolder);
	parentFolder->add(f);
}
void CopyPaste::execute()
{
	rez = objToCopy->copy();
	rez->parent = destFolder;
	destFolder->add(rez);
	rez->setName(name);
}
void ListDirectory::execute()
{	
	for (unsigned int i = 0; i < folder->containedObjects.size(); i++) {
		rez.push_back(folder->containedObjects.at(i));
	}
}
void Search::execute()
{
	SearchVisitor v(folderToSearch);
	root->accept(&v);
	found = v.foundObjects;
}
void ProtectedOperation::execute()
{
	wrappedOperation->execute();
}
bool ProtectedOperation::checkPrecondition()
{
	if (wrappedOperation->getName() == "DeleteObject") {
		CheckVisitor cv("Write");
		ff->accept(&cv);
		if (cv.check) { this->execute(); return cv.check; }
		else { throw AccessException("Neki od podfajlova/podfoldera nema Delete permition!"); return false; }
	}
	if (wrappedOperation->getName() == "CreateFile") {
		if (this->ff->getAccessDescriptor()->checkAccess("Write")) {
			this->execute();
			return true;
		}
		else {
			throw AccessException("Parent folder nema write permition!");
			return false;
		}
	}
	if (wrappedOperation->getName() == "CreateFolder") {
		if (this->ff->getAccessDescriptor()->checkAccess("Write")) {
			this->execute();
			return true;
		}
		else {
			throw AccessException("Parent folder nema write permition!");
			return false;
		}
	}
	if (wrappedOperation->getName() == "Search") {
		this->execute();
		return true;
	}
	if (wrappedOperation->getName() == "WriteFile") {
		if (this->ff->getAccessDescriptor()->checkAccess("Write")) {
			this->execute();
			return true;
		}
		else {
			throw AccessException("Fajl nema write permition!");
			return false;
		}
	}
	if (wrappedOperation->getName() == "ReadFile") {
		if (this->ff->getAccessDescriptor()->checkAccess("Read")) {
			this->execute();
			return true;
		}
		else {
			throw AccessException("Fajl nema read permition!");
			return false;
		}
	}
	if (wrappedOperation->getName() == "ListDirectory") {
		if (this->ff->getAccessDescriptor()->checkAccess("Read")) {
			this->execute();
			return true;
		}
		else {
			throw AccessException("Fajl nema read permition!");
			return false;
		}
	}
	if (wrappedOperation->getName() == "Move") {
		if (this->ff->getAccessDescriptor()->checkAccess("Read")&&
			this->ff->getAccessDescriptor()->checkAccess("Write")&&
			this->ff2->getAccessDescriptor()->checkAccess("Write")) {
			this->execute();
			return true;
		}
		else {
			throw AccessException("Destination folder nema write permition ili objToMove nema read/write permision!");
			return false;
		}
	}
	if (wrappedOperation->getName() == "CopyPaste") {
		if (this->ff->getAccessDescriptor()->checkAccess("Read") &&
			this->ff->getAccessDescriptor()->checkAccess("Write") &&
			this->ff2->getAccessDescriptor()->checkAccess("Write")&&
			this->ff2->getAccessDescriptor()->checkAccess("Read")) {
			this->execute();
			return true;
		}
		else {
			throw AccessException("Destination folder nema write/read permition ili objToMove nema read/write permision!");
			return false;
		}
	}

	else
		return false;
}
Text* FSOperation::getName()
{
	return this->opName;
}
void ReadFile::execute()
{
	stanje = f->read();
}
void Move::execute()
{
	destFolder->add(objToMove);
	objToMove->parent->remove(objToMove);
	objToMove->parent = destFolder;
}