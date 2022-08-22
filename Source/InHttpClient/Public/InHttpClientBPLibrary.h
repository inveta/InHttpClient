#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "InHttpClientBPLibrary.generated.h"

UCLASS(BlueprintType, Blueprintable)
class  UInHttpClientBPLibrary : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInHttpEvent, const FString&, MessageString);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintAssignable, Category = InHttpClient)
	FInHttpEvent OnSuccessEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintAssignable, Category = InHttpClient)
	FInHttpEvent OnFailEvent;

	virtual void Activate();

public:
	UFUNCTION(BlueprintCallable, Category = "InHttpClient")
	static void AddHeader(const FString& HeaderName, const FString& HeaderValue);

	UFUNCTION(BlueprintCallable, Category = "InHttpClient")
	static void SetBaseUrl(const FString& BaseUrl);

	UFUNCTION(BlueprintCallable, Category = "InHttpClient")
	static UInHttpClientBPLibrary* PostHttpRequest(const FString& SubRoute, const FString& Content);

	UFUNCTION(BlueprintCallable, Category = "InHttpClient")
	static UInHttpClientBPLibrary* GetHttpRequest(const FString& SubRoute);

	UFUNCTION(BlueprintCallable, Category = "InHttpClient")
		static UInHttpClientBPLibrary* PostHttpRequestWithBaseUrl(const FString& BaseUrl, const FString& SubRoute, const FString& Content, const TMap<FString,  FString>& Headers);

	UFUNCTION(BlueprintCallable, Category = "InHttpClient")
		static UInHttpClientBPLibrary* GetHttpRequestWithBaseUrl(const FString& BaseUrl, const FString& SubRoute, const TMap<FString, FString>& Headers);

public:
	static TMap<FString, FString> m_RequestHeaders;
	static FString m_BaseUrl;
	static TArray<UInHttpClientBPLibrary*> m_ListInHttp;
public:
	FString m_SubRoute;
	FString m_Content;
	FString m_Verb;
	FString m_LocalBaseUrl;
	TMap<FString, FString> m_LocalRequestHeaders;
	bool m_UseBaseUrl = true;
private:
	void HttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

