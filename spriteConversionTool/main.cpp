#include <fstream>
#include <iostream>
#include <string>

// g++ -o main main.cpp

int main()
{
	std::string filename = "steveleft"; // Filename without externsion

	std::fstream file;
	std::fstream ofile;

	const int imageWidth = 16;
	const int imageHeight = 16;
	const int pixelCount = imageWidth*imageHeight;
	
	int pixelReadCount = 0;
	int currentRow = imageHeight;
	unsigned short rgb = 0;
	unsigned short rgbArray[pixelCount];
	int paddingPerRow = (4 - ((imageWidth*3)%4))%4;
	int byteCount = pixelCount*3 + imageHeight*paddingPerRow;

	std::string path1 = "tile_textures/";
	std::string path2 = "converted_textures/";
	std::string string1 = ".bmp";
	std::string string2 = "_sprite.txt";
	std::string iFilePath = path1 + filename + string1;
	std::string oFilePath = path2 + filename + string2;




	file.open(iFilePath.c_str(), std::ios::in );
	ofile.open(oFilePath.c_str(), std::ios::out );

	file.seekg(0, file.end);
	int size = file.tellg();
	file.seekg(0, file.beg);

	char fileContent[size];
	file.read(fileContent, size);
	file.seekg(0, file.beg);

	
	
	for (int i = (size-1); i >= (size-byteCount); i -= 3)
	{
		if (i)
		rgb = ((((unsigned char)fileContent[i])/8));
		rgb = rgb << 6 | (((unsigned char)fileContent[i-1])/4);
		rgb = rgb << 5 | (((unsigned char)fileContent[i-2])/8);

		rgbArray[pixelReadCount++] = rgb;
		
		if (pixelReadCount%imageWidth == 0)
		{
			i -= paddingPerRow;
		}
	}
	
	std::cout << "Filesize in bytes: " << size << "\n";
	std::cout << "Byte padding each row: " << paddingPerRow << "\n";
	std::cout << "Imagesize in bytes without padding: " << pixelCount*3 << "\n";
	std::cout << "Imagesize in bytes with padding: " << byteCount << "\n";
	std::cout << "Pixel read count: " << pixelReadCount << "\n";
	

	ofile << "const unsigned short tile[" << pixelCount << "] = {\t";
	for (int j = 0; j < imageHeight; j++)
	{
		int index = 0;
		for(int i = 0; i < imageWidth; i++)
		{
			index = j*imageWidth + (imageWidth - i - 1);
			ofile << rgbArray[index];	
			if ((j*imageWidth + i) != (pixelCount-1))
				ofile << ", ";	
		}
	}

	ofile << "};";

	file.close();
	ofile.close();

	return 0;
}










