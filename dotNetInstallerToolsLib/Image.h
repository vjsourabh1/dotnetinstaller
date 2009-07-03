#pragma once

#include "Path.h"

namespace DVLib
{
	inline HBITMAP LoadBitmapFromFile(LPCTSTR p_File)
	{
		HBITMAP l_hBitmap = (HBITMAP)(::LoadImage(NULL,  p_File,
											IMAGE_BITMAP,
											0, 0,
											LR_LOADFROMFILE |
											LR_CREATEDIBSECTION |
											LR_DEFAULTSIZE |
											LR_VGACOLOR));

		if (l_hBitmap == NULL)
        {
            throw std::exception("Error loading bitmap from file");
        }

		return l_hBitmap;
	}

	//Load a bitmap from a stream.
	// I can't use CImage or GDI+ functions because are not supported on operating system prior to WinXp
	inline HBITMAP LoadBitmapFromBuffer(void * p_Buffer, DWORD p_BufferLength)
	{
		//creao un file temporaneo
		CString l_TempFile = GetTempFullFileName();
		HANDLE l_hFile = CreateFile(l_TempFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (l_hFile == INVALID_HANDLE_VALUE)
        {
            throw std::exception("Error loading bitmap from buffer, CreateFile failed");
        }

		DWORD l_BytesWritten = 0;
		if (WriteFile(l_hFile, p_Buffer, p_BufferLength, &l_BytesWritten, NULL) == FALSE)
        {
            throw std::exception("Error writing bitmap to file");
        }

		if (l_BytesWritten != p_BufferLength)
        {
            throw std::exception("Error loading bitmap from file, invalid number of bytes written");
        }

		CloseHandle(l_hFile);

		HBITMAP l_hBitmap = LoadBitmapFromFile(l_TempFile);

		DeleteFile(l_TempFile);

		return l_hBitmap;
	}

    inline HBITMAP LoadBannerFromResource(HMODULE p_Module)
    {
	    try
	    {
		    //leggo la risorsa
		    HRSRC l_res = FindResource(p_Module, TEXT("RES_BANNER"), TEXT("CUSTOM"));
		    if (l_res == NULL)
                throw std::exception("Error locating RES_BANNER resource");

            HGLOBAL l_hRes = LoadResource(p_Module, l_res);
		    if (l_hRes == NULL)
			    throw std::exception("Error loading RES_BANNER resource");
		    DWORD l_size = SizeofResource(p_Module, l_res);

		    LPVOID l_buffer = LockResource(l_hRes);
		    if (l_buffer == NULL)
			    throw std::exception("Error locking RES_BANNER resource");

		    return LoadBitmapFromBuffer(l_buffer, l_size);
	    }
        catch(std::exception&)
	    {
		    return NULL;
	    }
    }
}