// ------------------------------------------------------------------------------------------------
// Image class Implementation file
// ------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "Image.h"

// ------------------------------------------------------------------------------------------------
// get/set functions
// ------------------------------------------------------------------------------------------------

void Image::getPixel(int x, int y, byte& R, byte& G, byte& B)
{
	R = m_pRGB[getAddr(x, y) + 0];
	G = m_pRGB[getAddr(x, y) + 1];
	B = m_pRGB[getAddr(x, y) + 2];
}

void Image::setPixel(int x, int y, byte  R, byte  G, byte  B)
{
	m_pRGB[getAddr(x, y) + 0] = R;
	m_pRGB[getAddr(x, y) + 1] = G;
	m_pRGB[getAddr(x, y) + 2] = B;
}

// ------------------------------------------------------------------------------------------------
// Load from BMP file
//
// return  0 for success
// return -1 for error (file not found)
// ------------------------------------------------------------------------------------------------

// load from BMP
int Image::loadFromBMP(const char* fname)
{
	BMPHdr bmphdr;
	FILE*  fpt;
	byte*  temp;

	fpt = fopen(fname, "rb");
	if (fpt == NULL) return -1;
	fread(&bmphdr, sizeof(BMPHdr), 1, fpt);

	// free if already allocate
	if (m_pRGB) delete[] m_pRGB;

	// allocate memory
	m_pRGB = new byte[bmphdr.width*bmphdr.height * 3];
	m_iWidth = bmphdr.width;
	m_iHeight = bmphdr.height;

	// skip BMP header
	fseek(fpt, bmphdr.offset, 0);

	// read RGB data
	temp = new byte[m_iWidth*m_iHeight * 3];
	for (int i = 0; i < m_iHeight; i++)
	{
		if (fread(temp, sizeof(byte), m_iWidth * 3, fpt) != m_iWidth * 3) return -1;
		for (int j = 0; j < m_iWidth; j++)
		{
			setPixel(j, i, temp[j * 3 + 2], temp[j * 3 + 1], temp[j * 3 + 0]);
		}
	}
	delete[] temp;

	fclose(fpt);

	return 0;
}
