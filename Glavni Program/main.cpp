#include"FilesystemFacade.h"
#include<iostream>
int main() {

	try {


		Filesystem* f = Filesystem::getInstance();
		f->grantAccess(f->rootFolder,"Write");
		f->grantAccess(f->rootFolder, "Read");
		f->setSize(500);
		Folder * f1 = f->createFolder("Folder 1",f->rootFolder);
		f->grantAccess(f1, "Read");
		f->grantAccess(f1, "Write");
		Folder * f2 = f->createFolder("Folder 2", f->rootFolder);
		f->grantAccess(f2, "Read");
		f->grantAccess(f2, "Write");
		Folder * f3 = f->createFolder("Folder 3", f->rootFolder);
		Folder * f4 = f->createFolder("Folder 4", f->rootFolder);
		Folder * f5 = f->createFolder("Folder 5", f->rootFolder);
		File* fajl11 = f->createFile("Fajl 0", f->rootFolder);
		File* fajl12 = f->createFile("Fajl 1", f->rootFolder);
		File* fajl13 = f->createFile("Fajl 2", f->rootFolder);
		File* fajl14 = f->createFile("Fajl 3", f->rootFolder);
		File* fajl15 = f->createFile("Fajl 4", f->rootFolder);
		File* fajl16 = f->createFile("Fajl 5", f->rootFolder);
		f->grantAccess(fajl11, "Write");
		f->grantAccess(fajl12, "Write");
		f->grantAccess(fajl13, "Write");
		f->grantAccess(fajl14, "Write");
		f->grantAccess(fajl15, "Write");
		f->grantAccess(fajl16, "Write");
		Byte l = "KURCINAAAAA";
		f->writeFile(&l, fajl11);
		f->writeFile(&l, fajl12);
		f->writeFile(&l, fajl13);
		f->writeFile(&l, fajl14);
		f->writeFile(&l, fajl15);
		f->writeFile(&l, fajl16);
		f->copyPaste(f1, f2,"novo ime");
		f->move(f1, f2);

		long freespace = f->freeSpace();
		vector<FSObject*> ggg = f->listFolder(f->rootFolder);
		f->grantAccess(f1, "Write");
		File* fajl1 = f->createFile("Fajl 0", f->rootFolder);
		f->grantAccess(fajl1, "Write");
		File* fajl2 = f->createFile("Fajl 1", f1);
		f->grantAccess(fajl2, "Write");
		f->grantAccess(fajl2, "Write");
		Byte a = "nesto";
		f->writeFile(&a, fajl2);
		Byte *fff = f->readFile(fajl2);
		
		
		f->deleteF(f->rootFolder);
		

	}
	catch (exception) {}


}