// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <mutex>
#include "NetworkServer.h"
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>		// Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>	// Video write
#include <stdio.h>
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StreamHelper.generated.h"
/**
 * 
 */
UCLASS()
class UNREALSTREAM_API UStreamHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	
	static FString filePath;
	static FILE* f;
	static long lSize;
	static FILE * pFile;
	static char * buffer;
	static size_t result;
	static size_t counter;
	static FColor *PixelBeginning;
	static cv::VideoWriter output_cap;

	//static NetworkServer NS;
	//UPROPERTY(Category = HeightMap, EditAnywhere)
	//UTextureRenderTarget2D* RenderTarget;
public:

	UFUNCTION(BlueprintCallable, Category = "HeightMap|GetColorBuffer")
		static TArray<FColor> GetColorBuffer(UTextureRenderTarget2D* RenderTarget);

	UFUNCTION(BlueprintCallable, Category = "HeightMap|EncodeColorBuffer")
		static void EncodeColorBuffer(TArray<FColor> ColorBuffer, int x, int y);

	UFUNCTION(BlueprintCallable, Category = "HeightMap|StartStream")
		static void StartStream(int x, int y);

	UFUNCTION(BlueprintCallable, Category = "HeightMap|StopStream")
		static void StopStream();



};
