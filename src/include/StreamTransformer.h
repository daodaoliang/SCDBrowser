#ifndef StreamTransformer_H
#define StreamTransformer_H


#include <string>
#include <vector>
#include <fstream>
//#include "SliceSCLModelInterface.h"


#define FILE_BLOCK_SIZE 1024

using namespace std;


class StreamTransformer
{
public:
	StreamTransformer()
	{

	};

	void FromStreamToByteSeq(fstream &graphicStream,vector<unsigned char>& fileData)
	{
		char tempFileBlock[FILE_BLOCK_SIZE];
		int totalSize = 0;
		while(!graphicStream.eof())
		{
			graphicStream.read(tempFileBlock,FILE_BLOCK_SIZE);
			int count = graphicStream.gcount();

			if(count==0)
			{
				break;
			}

			for(int i=0;i<count;i++)
			{
				fileData.push_back(tempFileBlock[i]);
			}
			totalSize += count;
		}
	};
		
	void FromByteSeqToStream(vector<unsigned char>& fileData,fstream &fileStream)
	{
		char    tempFileBlock[FILE_BLOCK_SIZE];
		int     totalFileSize = fileData.size();
		int     resFileSize   = totalFileSize;
		int     fileBlockSize = FILE_BLOCK_SIZE;
		
		//文件块个数
		int fileBlockNum = (totalFileSize/FILE_BLOCK_SIZE)+1;
		if(totalFileSize%FILE_BLOCK_SIZE==0)
		{
			fileBlockNum = totalFileSize/FILE_BLOCK_SIZE;
		}
		
		for(int i=0;i<fileBlockNum;i++)
		{
			for(int j=0;j<fileBlockSize;j++)
			{
				tempFileBlock[j] = fileData[i*FILE_BLOCK_SIZE+j];
			}
			fileStream.write(tempFileBlock,fileBlockSize);
		
			resFileSize = resFileSize - fileBlockSize;
			if(resFileSize<FILE_BLOCK_SIZE)
			{
				fileBlockSize = resFileSize;
			}
		}
		return;
	};
};



#endif