#include"FilesystemObject.h"

Folder::Folder(Text* a, Folder* fn) :FSObject(a, fn) {
}
FSObject::FSObject(Text * nm, Folder * fn) : name(nm), parent(fn)
{
	accessDescriptor = new AccessDescriptor(this);
}
Text* FSObject::getName() {
	return name;
}
void FSObject::setName(Text * name)
{
	this->name = name;
}
FSObject::~FSObject() {
	/*if (name)
		name = "";
	name = nullptr;
	parent = nullptr;*/
}
AccessDescriptor::AccessDescriptor(FSObject* f)
{
	allowedOperations = new vector<Text*>;
	protectedObject = f;
}
void AccessDescriptor::add(Text* operationName) {
	if (allowedOperations == nullptr)
		allowedOperations = new vector<Text*>;
	allowedOperations->push_back(operationName);
}
void AccessDescriptor::remove(Text* operationName) {
	if (allowedOperations == nullptr)
		return;
	for (unsigned int i = 0; i < allowedOperations->size(); i++) {
		if (!strcmp(allowedOperations->at(i), operationName))
			allowedOperations->erase(allowedOperations->begin() + i);
	}
}
vector<Text*> AccessDescriptor::getAllowedOperations() {
	if (allowedOperations == nullptr) return *allowedOperations;
	return *allowedOperations;
}
bool AccessDescriptor::checkAccess(Text* operationName) {
	if (allowedOperations == nullptr)
		return false;
	for (unsigned int i = 0; i < allowedOperations->size(); i++) {
		if (!strcmp(allowedOperations->at(i), operationName))
			return true;
	}
	return false;
}
AccessDescriptor::~AccessDescriptor() {
	delete protectedObject;
	delete allowedOperations;
}
void Folder::accept(FilesystemVisitor* v) {
	v->visitFolder(this);
}
void Folder::add(FSObject* o) {
	containedObjects.push_back(o);
}

std::vector<FSObject*> Folder::getObjects() {
	return containedObjects;
}
void Folder::remove(FSObject* obj) {
	for (int i = 0; i < containedObjects.size(); i++) {
		if (containedObjects[i]->getName() == obj->getName()) {
			containedObjects.erase(containedObjects.begin() + i);
			return;
		}
	}
}
FSObject* Folder::copy() {

	if (containedObjects.size() == 0) {
		Folder *x = new Folder(this->getName(),this->parent);
		return x;
	}
	else {
		Folder *x = new Folder(this->getName(),this->parent);
		for (int i = 0; i < containedObjects.size(); i++)
		{

			x->add(containedObjects[i]->copy());
		}
		return x;
	}
}
Folder::~Folder() {
	for (int i = 0; i < containedObjects.size(); i++)
	{
		delete containedObjects[i];
	}

}
File::File(Text * a, Folder * fn) :FSObject(a, fn)
{
	content = nullptr;
	
}
void File::accept(FilesystemVisitor* v) {
	v->visitFile(this);
}
void File::write(Byte* content) {
		this->content = new Byte(*content);
}
FSObject* File::copy() {
	File* novi = new File(this->getName(), this->parent);
	novi->write(this->content);
	return novi;
}

long File::size()
{
	if(content!=nullptr)return sizeof(content);
}


Byte* File::read() {
	return this->content;
}
File::~File() {
	/**content = "";*/
}

SearchVisitor::SearchVisitor(Text* t) {
	filename = new Text(*t);
}
void SearchVisitor::visitFile(File * f)
{
	if (f->getName() == filename && f->getAccessDescriptor()->checkAccess("Read"))
		foundObjects.push_back(f);
}
void SearchVisitor::visitFolder(Folder * f)
{
	if (f->getName() == filename && f->getAccessDescriptor()->checkAccess("Read")) {
		foundObjects.push_back(f);
		folder = f;
	}
	for (unsigned int i = 0; i < f->containedObjects.size(); i++) {
			f->containedObjects[i]->accept(this);
	}

}
CheckVisitor::CheckVisitor(Text * t)
{	
	access = t;
}
void CheckVisitor::visitFile(File * f)
{
	AccessDescriptor* a = f->getAccessDescriptor();
	if (a->checkAccess(access)) {
		check = true;
	}
	else
		check = false;
}
void CheckVisitor::visitFolder(Folder * f)
{
	AccessDescriptor* a = f->getAccessDescriptor();
	if (a->checkAccess(access)) {
		check = true;
	}
	else check = false;
	for (unsigned int i = 0; i < f->containedObjects.size(); i++) {
		f->containedObjects[i]->accept(this);
	}
}

void SizeVisitor::visitFile(File * f)
{
	size += f->size();
}

void SizeVisitor::visitFolder(Folder * f)
{
	for (unsigned int i = 0; i < f->containedObjects.size(); i++) {
		f->containedObjects[i]->accept(this);
	}
}
