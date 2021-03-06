// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealStream.h"
#include "StreamHelper.h"
#include <HighResScreenshot.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <algorithm>


//#pragma comment (lib, "C:\\US\\OpenCV\\build\\x64\\vc14\\lib\\opencv_world310.lib")
//#pragma comment (lib, "C:\\US\\OpenCV\\build\\x64\\vc14\\lib\\opencv_world310d.lib")

FILE* UStreamHelper::f = nullptr;
FString UStreamHelper::filePath = "C:\\US\\";
long UStreamHelper::lSize;
FILE * UStreamHelper::pFile;
char * UStreamHelper::buffer;
size_t UStreamHelper::result;
size_t UStreamHelper::counter = 0;
FColor* UStreamHelper::PixelBeginning;
// Setup output video
cv::VideoWriter UStreamHelper::output_cap;
//NetworkServer UStreamHelper::NS;


TArray<FColor> UStreamHelper::GetColorBuffer(UTextureRenderTarget2D* RenderTarget)
{
	//ColorBuffer.Reset();
	TArray<FColor> ColorBuffer;
	FString Filename = filePath + "US.png";

	if (RenderTarget != NULL)
	{
		FIntRect SourceRect;
		FIntPoint DestSize(RenderTarget->GetSurfaceWidth(), RenderTarget->GetSurfaceHeight());
		FString ResultPath;
		FHighResScreenshotConfig& HighResScreenshotConfig = GetHighResScreenshotConfig();


		FTextureRenderTarget2DResource* textureResource = (FTextureRenderTarget2DResource*)RenderTarget->Resource;
		//textureResource->GetTextureRenderTarget2DResource()->GetRenderTargetTexture()->
		//textureResource->ReadPixelsPtr(ColorBuffer);
		if (textureResource->ReadPixels(ColorBuffer))
		{
			/*for (FColor& color : ColorBuffer)
			{
				color.A = 255;
			}*/
			// Save to PNG
			//HighResScreenshotConfig.SaveImage(Filename, ColorBuffer, DestSize, &ResultPath);
			return ColorBuffer;
		}
	}
	return ColorBuffer;
}

void UStreamHelper::StartStream(int x, int y) {
	// Setup output video
	output_cap.open("C:\\US\\outfile.avi",
		cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
		24,
		cv::Size(x, y),
		true);


	/*output_cap.open("",
		cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
		50,
		cv::Size(x, y),
		true);*/
	return;
/*
	std::ofstream file;
	file.open("C:\\US\\mergeList.US", std::ios::out);
	file << "# this is a comment" << std::endl;
	file << "file 'C:/US/outfile_crf20.mkv'" << std::endl;
	file << "file 'C:/US/tmp_crf20.mkv'" << std::endl;
	file.close();

	std::string resolution = std::to_string(x) + "x" + std::to_string(y);
	//f = _popen(("ffmpeg -f image2pipe -re -pix_fmt rgba -s " + resolution + " -i - -vcodec h264 -r 10 -f mp4 C:\\US\\US.mp4").c_str(), "wb");
	//f = _popen(("ffmpeg -re -s "+resolution+" -i C:\\US\\US.png -vcodec libx264 -crf 20 C:\\US\\outfile_crf20.mkv").c_str(), "w");
	f = _popen(("ffmpeg -f image2 -r 30 -s " + resolution + " -pix_fmt rgb8 -vcodec rawvideo -i - -vcodec huffyuv video.avi").c_str(), "w");
	//_pclose(f);
*/
}

void UStreamHelper::StopStream() {
	//system("taskkill /F /IM ffmpeg.exe");
	//_pclose(f);
	output_cap.release();
}

void UStreamHelper::EncodeColorBuffer(TArray<FColor> ColorBuffer, int x, int y) {
	cv::Mat tmpFrame = cv::Mat(y,x,CV_8UC4, ColorBuffer.GetData());
	cv::Mat frame;
	cv::cvtColor(tmpFrame, frame, CV_RGBA2BGRA);
	//cv::imwrite("C:/US/qqq.png", frame);
	output_cap.write(frame);

	std::vector<uchar> buf;
	buf.reserve(frame.total() * frame.elemSize());
	cv::imencode(".jpg", frame, buf, std::vector<int>());
	std::string content(buf.begin(), buf.end()); //this must be sent to the client

	NetworkServer NS;
	NS.sendMsg(content);

	/*std::ofstream file;
	file.open("C:\\US\\US.txt", std::ios::out);
	try
	{
	UE_LOG(LogBlueprintUserMessages, Log, TEXT("open: %d"), (int)file.is_open());
	}
	catch (const std::exception&)
	{

	}
	if (file.is_open()) {
	for (auto &color : ColorBuffer) {
	//file.write((char*)(&color), sizeof(color));
	uint32 u = color.ToPackedRGBA();
	file.write((char*)(&u), sizeof(u));
	//file << color.R << color.G << color.B << color.A;
	}
	}
	fwrite(PixelBeginning, 1, ColorBuffer.Num() * sizeof(FColor), f);
	fwrite((FColor*)(&ColorBuffer), 1, ColorBuffer.Num() * sizeof(FColor), f);
	*/
	
	return;
	/*
	std::string resolution = std::to_string(x) + "x" + std::to_string(y);
	f = _popen(("ffmpeg -re -s " + resolution + " -i C:\\US\\US.png -vcodec libx264 -crf 20 C:\\US\\tmp_crf20.mkv").c_str(), "w");
	_pclose(f);
	f = _popen(("ffmpeg -safe 0 -f concat -i C:\\US\\mergeList.US -c copy C:\\US\\outfile_crf20.mkv -y"), "w");
	*/
	// Would work if we were simontanesly reading and writing at the same time, but right now
	// we're only writing
	/*char buff[512];
	while (fgets(buff, sizeof(buff), f) != NULL) {
	FString Fs = FString(ANSI_TO_TCHAR(buff));
	UE_LOG(LogBlueprintUserMessages, Log, TEXT("Ch! %s"), *Fs);
	}*/
	_pclose(f);
	return;


	/*** Alternative method ***/
	/*
	* This alternative method reads the png image to some allocated memory
	* and then copies this to the pipe
	*/
	/*
	pFile = fopen(("C:\\US\\US.png"), "rb");
	if (pFile == NULL) { fputs("File error", stderr); exit(2); }

	// obtain file size of png:

	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);


	// allocate memory to contain the whole file:
	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL) { fputs("Memory error", stderr); exit(3); }

	// copy the file into the buffer:
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) { fputs("Reading error", stderr); exit(4); }

	// Write to pipe
	fwrite(buffer, 1, lSize, f);
	fflush(f);

	// Clean
	fclose(pFile);
	free(buffer);


	//std::stringstream ss;
	//for (int32 Index = 0; Index != ColorBuffer.Num(); ++Index) {
	//	std::string str = TCHAR_TO_UTF8(*ColorBuffer[Index].ToString());
	//	char * returnvalue = (char *)malloc(sizeof(char) * (str.length() + 1));
	//	strncpy_s(returnvalue, str.length(), str.c_str(), str.length());
	//	f << returnvalue;
	//}
	//f << (*ss.rdbuf()).c_str();
	//FMemory::StreamingMemcpy(f, ColorBuffer, ColorBuffer.GetAllocatedSize());

	//for (int32 Index = 0; Index != ColorBuffer.Num(); ++Index) {
	//f << ColorBuffer[Index].ToString();
	//fwrite(*ColorBuffer[Index], )
	//f << ColorBuffer[Index].R;
	//f << ColorBuffer[Index].G;
	//f << ColorBuffer[Index].B;
	//}
	//std::vector<FColor> tmp(ColorBuffer.Num());
	//std::transform(ColorBuffer.begin(), ColorBuffer.end(), tmp.begin());
	//fwrite((FColor*)(&tmp), 1, tmp.size() * sizeof(FColor), f)

	// A good potential - but yields no result?
	//fwrite((FColor*)(&ColorBuffer), 1, ColorBuffer.Num() * sizeof(FColor), f);
	*/

	/*** Just a failed attempt to print the results FFMPEG gives **/
	//char ch[1024] = "";
	//char* q = fgets(ch, sizeof(ch), f);
	//FString Fs = FString(ANSI_TO_TCHAR(q));
	//FString Fs = FString(UTF8_TO_TCHAR(ch));
	//if (f != 0)
	//UE_LOG(LogBlueprintUserMessages, Log, TEXT("Ch! %s"), *Fs);
	//UE_LOG(LogBlueprintUserMessages, Log, TEXT("Ch! %s"), q);
}
