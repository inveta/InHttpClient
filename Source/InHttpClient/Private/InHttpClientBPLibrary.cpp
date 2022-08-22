#include "InHttpClientBPLibrary.h"
#include "InHttpClient.h"


TMap<FString, FString> UInHttpClientBPLibrary::m_RequestHeaders{
	{"Accept","*/*"}
};

FString UInHttpClientBPLibrary::m_BaseUrl;
TArray<UInHttpClientBPLibrary*> UInHttpClientBPLibrary::m_ListInHttp;

void UInHttpClientBPLibrary::Activate()
{
	TSharedRef Request = FHttpModule::Get().CreateRequest();

	FString httpUrl = UInHttpClientBPLibrary::m_BaseUrl + m_SubRoute;
	if (false == m_UseBaseUrl)
	{
		httpUrl = m_LocalBaseUrl + m_SubRoute;
	}
	Request->SetURL(httpUrl);

	if (true == m_UseBaseUrl)
	{
		for (auto& item : UInHttpClientBPLibrary::m_RequestHeaders)
		{
			Request->SetHeader(item.Key, item.Value);
		}
	}
	else
	{
		for (auto& item : m_LocalRequestHeaders)
		{
			Request->SetHeader(item.Key, item.Value);
		}
	}
	
	Request->SetVerb(m_Verb);
	Request->SetContentAsString(m_Content);

	Request->OnProcessRequestComplete().BindUObject(this, &UInHttpClientBPLibrary::HttpResponse);
	auto isSuccess = Request->ProcessRequest();
	if (false == isSuccess)
	{
		UInHttpClientBPLibrary::m_ListInHttp.Remove(this);
		OnFailEvent.Broadcast("");
	}
}

void UInHttpClientBPLibrary::AddHeader(const FString& HeaderName, const FString& HeaderValue)
{
	m_RequestHeaders.Add(HeaderName, HeaderValue);
}
void UInHttpClientBPLibrary::SetBaseUrl(const FString& BaseUrl)
{
	m_BaseUrl = BaseUrl;
}
UInHttpClientBPLibrary* UInHttpClientBPLibrary::PostHttpRequest(const FString& SubRoute, const FString& Content)
{
	UInHttpClientBPLibrary* ptr = NewObject<UInHttpClientBPLibrary>();
	ptr->m_SubRoute = SubRoute;
	ptr->m_Content = Content;
	ptr->m_Verb = "POST";

	m_ListInHttp.Add(ptr);
	return ptr;
}
UInHttpClientBPLibrary* UInHttpClientBPLibrary::GetHttpRequest(const FString& SubRoute)
{
	UInHttpClientBPLibrary* ptr = NewObject<UInHttpClientBPLibrary>();
	ptr->m_SubRoute = SubRoute;
	ptr->m_Verb = "GET";

	m_ListInHttp.Add(ptr);
	return ptr;
}
UInHttpClientBPLibrary* UInHttpClientBPLibrary::PostHttpRequestWithBaseUrl(const FString& BaseUrl, const FString& SubRoute, const FString& Content,const TMap<FString, FString>& Headers)
{
	UInHttpClientBPLibrary* ptr = NewObject<UInHttpClientBPLibrary>();
	ptr->m_SubRoute = SubRoute;
	ptr->m_Content = Content;
	ptr->m_Verb = "POST";
	ptr->m_LocalBaseUrl = BaseUrl;
	ptr->m_LocalRequestHeaders = Headers;

	ptr->m_UseBaseUrl = false;
	m_ListInHttp.Add(ptr);
	return ptr;
}

UInHttpClientBPLibrary* UInHttpClientBPLibrary::GetHttpRequestWithBaseUrl(const FString& BaseUrl, const FString& SubRoute, const TMap<FString, FString>& Headers)
{
	UInHttpClientBPLibrary* ptr = NewObject<UInHttpClientBPLibrary>();
	ptr->m_SubRoute = SubRoute;
	ptr->m_Verb = "GET";
	ptr->m_LocalBaseUrl = BaseUrl;
	ptr->m_LocalRequestHeaders = Headers;
	ptr->m_UseBaseUrl = false;

	m_ListInHttp.Add(ptr);
	return ptr;
}

void UInHttpClientBPLibrary::HttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UInHttpClientBPLibrary::m_ListInHttp.Remove(this);

	if (false == bWasSuccessful)
	{
		OnFailEvent.Broadcast("");
		return;
	}
	if (false == Response.IsValid())
	{
		OnFailEvent.Broadcast("");
		return;
	}
	if (false == EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnFailEvent.Broadcast("");
		return;
	}
	FString JsonString = Response->GetContentAsString();
	OnSuccessEvent.Broadcast(JsonString);
}