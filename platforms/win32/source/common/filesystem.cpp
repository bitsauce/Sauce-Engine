//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "filesystem.h"

// Recursive directory traversal using the Win32 API
XScriptArray *FileSystem::listFiles(string &directory, const string &mask, const bool recursive) const
{
	// Create output array
	XScriptArray *files = CreateArray("string", 0);
	
	util::toAbsoluteFilePath(directory);
	util::toDirectoryPath(directory);

	// Folders to search
	vector<string> folders;
	folders.push_back(directory);

	do {
		// Get search path
		string searchPath = folders.back() + mask;
		folders.pop_back();
		
		// Find first file
		WIN32_FIND_DATA fdata;
		HANDLE hFind = FindFirstFile(searchPath.c_str(), &fdata);

		// Check if the search was successful
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do {
				wchar_t filename[MAX_PATH];
				mbstowcs_s(0, filename, fdata.cFileName, 1024);

				if(wcscmp(filename, L".") != 0 && wcscmp(filename, L"..") != 0)
				{
					if((fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
					{
						// Found a folder, add it to the list if we're doing a recursive search
						if(recursive) {
							folders.push_back(directory + fdata.cFileName + "/");
						}
					}
					else
					{
						// Found a file, add it to the output list
						string file(directory + fdata.cFileName);
						files->InsertLast(&file);
					}
				}
			}
			while (FindNextFile(hFind, &fdata) != 0);
		}

		FindClose(hFind);
	}
	while(!folders.empty());

	return files;
}

XScriptArray *FileSystem::listFolders(string &directory, const string &mask, const bool recursive) const
{
	// Create output array
	XScriptArray *folders = CreateArray("string", 0);
	
	util::toAbsoluteFilePath(directory);
	
	// Folders to search
	vector<string> folderList;
	folderList.push_back(directory);

	do {
		// Get search path
		string searchPath = folderList.back() + mask;
		folderList.pop_back();
		
		// Find first file
		WIN32_FIND_DATA fdata;
		HANDLE hFind = FindFirstFile(searchPath.c_str(), &fdata);

		// Check if the search was successful
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do {
				wchar_t filename[MAX_PATH];
				mbstowcs_s(0, filename, fdata.cFileName, 1024);

				if(wcscmp(filename, L".") != 0 && wcscmp(filename, L"..") != 0)
				{
					if((fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
					{
						// Found a folder, add it to the list if we're doing a recursive search
						if(recursive) {
							folderList.push_back(directory + fdata.cFileName + "/");
						}
						string folder = directory + fdata.cFileName;
						folders->InsertLast(&folder);
					}
				}
			}
			while (FindNextFile(hFind, &fdata) != 0);
		}

		FindClose(hFind);
	}
	while(!folderList.empty());

	return folders;
}

bool dirExists(const string& dirPath)
{
	DWORD attr = GetFileAttributes(dirPath.c_str());
	return attr != INVALID_FILE_ATTRIBUTES &&
		(attr & FILE_ATTRIBUTE_DIRECTORY);
}

bool FileSystem::makeDir(const string &path)
{
	if(!dirExists(path))
	{
		// Create all intermediate directories
		for(uint i = 2; i < path.size(); i++)
		{
			if(path[i] == '/')
			{
				string subPath = path.substr(0, i);
				if(!dirExists(subPath))
				{
					CreateDirectory(subPath.c_str(), 0);
				}
			}
		}
		return CreateDirectory(path.c_str(), 0) != 0;
	}
	return true;
}

bool FileSystem::remove(string &path)
{
	util::toAbsoluteFilePath(path);

	// Is it a directory?
	if(dirExists(path))
	{
		util::toDirectoryPath(path);

		// Perform recursive deletion
		WIN32_FIND_DATA fdata;
		HANDLE hFind = FindFirstFile((path + "*").c_str(), &fdata);

		// Check if the search was successful
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do {
				wchar_t filename[MAX_PATH];
				mbstowcs_s(0, filename, fdata.cFileName, 1024);
				if(wcscmp(filename, L".") != 0 && wcscmp(filename, L"..") != 0)
				{
					if(!remove(path + fdata.cFileName)) {
						return false;
					}
				}
			}
			while (FindNextFile(hFind, &fdata) != 0);
		}else
		{
			return false;
		}

		FindClose(hFind);
		return RemoveDirectory(path.c_str()) != 0;
	}else
	{
		return DeleteFile(path.c_str()) != 0;
	}
}