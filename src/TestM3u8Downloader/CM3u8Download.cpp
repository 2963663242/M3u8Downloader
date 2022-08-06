#include "CM3u8Download.h"
#include "utils.h"
#include "log.h"

CM3u8Download::CM3u8Download()
{
	curl_global_init(CURL_GLOBAL_ALL); 
}
void CM3u8Download::download()
{
	LogD(Info, "============================ %s ==> Start Download ============================", this->guid.data());
	string str1;
	string str2;
	string str3;

	long long var_790 = 0;
	long var_7D4 = 0;
	long var_7D0 = 0;

	long long* var_668;
	if (var_668 = new __int64) {
		*var_668 = (long long)&var_668;
	}
	else {
		var_668 = 0;
	}
	__int64 * var_638 = sub_180009CC0();
	*((char*)var_638 + 0x69) = 1;
	var_638[1] = (__int64)var_638;
	var_638[0] = (__int64)var_638;
	var_638[2] = (__int64)var_638;
	__int64* var_630 = 0;

	string str_4E8;
	string str_388;
	string str_338;
	string str_360;
	string str_3D8;
	string str_428;
	string str_498;

	long long* var_6E8;
	if (var_6E8 = new __int64) {
		*var_6E8 = (long long)&var_6E8;
	}
	else {
		var_6E8 = 0;
	}


}

__int64* sub_180009CC0() {
	__int64* arg_8;
	
	if (arg_8 = new __int64[0x14]) {
		arg_8[0] = 0;
	}
	if (&arg_8[1]) {
		arg_8[1] = 0;
	}
	if (&arg_8[2]) {
		arg_8[2] = 0;
	}
	*((char*)arg_8 + 0x68) = 1;
	*((char*)arg_8 + 0x69) = 0;
	return arg_8;
}

void * write_callback=(void *)0x0000000180001F30;
void * progress_callback = (void *)0x0000000180001DF0;

CURLcode  CM3u8Download::downloadSegment(string& url,string * pCookie,string *pReceiveData){

	CURLcode retCode;
	CURL* pCurl = curl_easy_init();
    this->pCurl = pCurl;
	struct curl_slist *pCookie_list = NULL;

	LogD(Info,"%s ==> url : %s", this->guid.data(),url.data());

	curl_easy_setopt(pCurl,CURLOPT_URL,url.data());
	curl_easy_setopt(pCurl,CURLOPT_FOLLOWLOCATION,51);
	curl_easy_setopt(pCurl,CURLOPT_WRITEFUNCTION,write_callback);
	curl_easy_setopt(pCurl,CURLOPT_WRITEDATA,pReceiveData);
	curl_easy_setopt(pCurl,CURLOPT_SSL_VERIFYPEER,0);
	curl_easy_setopt(pCurl,CURLOPT_PROGRESSFUNCTION,progress_callback);
	curl_easy_setopt(pCurl,CURLOPT_PROGRESSDATA,this);
	curl_easy_setopt(pCurl,CURLOPT_NOPROGRESS,0);

	if(this->speedLimit){
		LogD(Info,"%s ==> limit speed(%lld)",this->guid.c_str(),speedLimit);
		curl_easy_setopt(pCurl,CURLOPT_MAX_RECV_SPEED_LARGE,speedLimit);
	}

	if(pCookie->size()!=0){
       LogD(Info,"%s ==> cookie : %s",this->guid.c_str(),pCookie->c_str());
		curl_easy_setopt(pCurl,CURLOPT_COOKIE,pCookie->c_str());
	}
	
	if(this->headerchain!=0){
	 curl_easy_setopt(pCurl,CURLOPT_HTTPHEADER,this->headerchain);
	}
	if(this->useragent !=0){
		LogD(Info,"%s ==> user agent : %s",this->guid.c_str(),this->useragent);
		curl_easy_setopt(pCurl,CURLOPT_USERAGENT,this->useragent);
	}

	retCode = curl_easy_perform(pCurl);
	if(retCode == CURLE_OK){

		char * lastUrl = 0;
		if(curl_easy_getinfo(pCurl,CURLINFO_EFFECTIVE_URL,&lastUrl) == CURLE_OK){
			std::vector<std::vector<std::string>> ipMatch;
			if(strlen(lastUrl) > 1){
				string strLastUrl;
				strLastUrl = lastUrl;   
				if(!RegexExec(strLastUrl,"[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}",ipMatch)){
					url = lastUrl;
				}

			}

		}


		if(curl_easy_getinfo(pCurl,CURLINFO_COOKIELIST,&pCookie_list) == CURLE_OK && pCookie_list!= 0){
			string cookies="";
			while(pCookie_list){
				string strCookie = pCookie_list->data;
				std::vector<std::string> vecCookie;
				if(strCookie.size()!=0){
					size_t lastPos =0;

					while(size_t pos = strCookie.find("\t",lastPos,1)!=string::npos){
						string subCookie;
						subCookie = strCookie.substr(lastPos,pos);
						vecCookie.push_back(subCookie);
						lastPos = pos+1;
					}

				}
				
				cookies+=vecCookie[5];
				cookies+="=";
				cookies+=vecCookie[6];
				cookies+=";";

			}
			*pCookie=cookies;
		}
	}
	curl_easy_cleanup(pCurl);

return retCode;

}