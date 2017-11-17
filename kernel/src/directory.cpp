#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <inlow/dirent.h>
#include <inlow/stat.h>
#include <inlow/kernel/directory.h>
#include <inlow/kernel/print.h>

DirectoryVnode::DirectoryVnode(DirectoryVnode* parent, mode_t mode) : Vnode(S_IFDIR | mode)
{
	childCount = 0;
	childNodes = nullptr;
	fileNames = nullptr;
	this->parent = parent;
}

DirectoryVnode::~DirectoryVnode()
{
	free(childNodes);
	free(fileNames);
}

bool DirectoryVnode::addChildNode(const char* path, Vnode* vnode)
{
	Vnode** newChildNodes = (Vnode**) reallocarray(childNodes, childCount + 1, sizeof(Vnode*));
	if (!newChildNodes)
			return false;
	const char** newFileNames = (const char**) reallocarray(fileNames, childCount + 1, sizeof(const char*));
	if (!newFileNames)
			return false;
	childNodes = newChildNodes;
	fileNames = newFileNames;

	childNodes[childCount] = vnode;
	fileNames[childCount] = strdup(path);
	childCount++;
	return true;
}

Vnode* DirectoryVnode::openat(const char* path, int flags, mode_t mode)
{
	while (*path == '/')
		path++;

	size_t length = strcspn(path, "/");

	if (length == 0)
			return this;

	if (strncmp(path, ".", length) == 0)
			return openat(path + length, flags, mode);
	else if (length == 2 && strncmp(path, "..", length) == 0)
	{
			Vnode* dotdot = parent ? parent : this;
			return dotdot->openat(path + length, flags, mode);
	}

	for (size_t i = 0; i < childCount; i++)
	{
		if (strlen(fileNames[i]) == length && strncmp(path, fileNames[i], length) == 0)
		{
			if (path[length] == '\0')
				return childNodes[i];
			else
				return childNodes[i]->openat(path + length, flags, mode);			
		}
	}

	errno = ENOENT;
	return nullptr;
}

ssize_t DirectoryVnode::readdir(unsigned long offset, void* buffer, size_t size)
{
	const char* name;
	if (offset == 0)
			name = ".";
	else if (offset == 1)
			name = "..";
	else if (offset - 2 < childCount)
			name = fileNames[offset - 2];
	else if (offset - 2 == childCount)
			return 0;
	else
	{
		return -1;
	}

	size_t structSize = sizeof(struct dirent) + strlen(name) + 1;
	if (size >= structSize)
	{
		struct dirent* entry = (struct dirent*) buffer;
		entry->d_reclen = size;
		strcpy(entry->d_name, name);
	}

	return structSize;
}
