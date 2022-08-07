#include "CM3u8Download.h"
#include "utils.h"
#include "log.h"


CM3u8Download::CM3u8Download()
{
	curl_global_init(CURL_GLOBAL_ALL); 
}
void CM3u8Download::download()
{
	LogD(Info, "============================ %s ==> Start Download ============================", this->guid.c_str());

	string url;
	string cookies;
	string receiveData;
	string strLastCookies;
	std::vector<std::vector<std::string>> regexResult;

	this->invalidDLCount = 0;
	 CURLcode dlProfileRet = CURLE_UNSUPPORTED_PROTOCOL;
	 if(this->savePath == 0 || this->url == 0)
		 return;
	url = this->url;
	if(this->strCookie!=0)
		cookies = this->strCookie;
	
	stateCallback stateinfo = {0};
	stateinfo.type = 2;
	this->stateType = 2;

	LogD(Info,"%s ==> connecting...",this->guid.c_str());

	this->setCallbackState(&stateinfo);
	
	while(true){
		receiveData = "";
		this->invalidDLCount = 0;
		if(strLastCookies.size() != 0)
			cookies = strLastCookies;
		this->dlBody = false;
		if(this->v78 == 0)
			dlProfileRet = this->downloadSegment(url,&cookies,&receiveData);
		
		 this->dlBody = true;
		if(strLastCookies.size() == 0)
			strLastCookies = cookies;

		if(dlProfileRet){
			stateinfo.type = 1;
			stateinfo.speed = dlProfileRet;
			this->setCallbackState(&stateinfo);
			return;
		}
		
		RegexExec(receiveData,"([^\n\r]+)",regexResult);

		int index =0;
		int index2 = 0;

		if(regexResult.size() == 0 ){
			stateinfo.type = 1;
			stateinfo.speed = dlProfileRet;
			this->setCallbackState(&stateinfo);
			return;
		}

		do{
			if(index2 >= regexResult.size())
				exit(1);

			if(regexResult[index2].size() <= 1)
				exit(1);

			string strItem = regexResult[index2][0];
	
		}while(index < regexResult.size());

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

//void *write_callback = (void *)0x0000000180001F30;
size_t write_callback(char *ptr, size_t size, size_t nmemb,string * pReceiveData){
	pReceiveData->append(ptr,nmemb);
	return nmemb*size;
}
//void * progress_callback = (void *)0x0000000180001DF0;
int progress_callback(CM3u8Download *downloader,
					  double dltotal,
					  double dlnow,
					  double ultotal,
					  double ulnow){
						  double speed;
						  time_t curtime;
						  downloader->stateType = 3;
						  curl_easy_getinfo(downloader->pCurl,CURLINFO_SPEED_DOWNLOAD,&speed);
						  curtime = time(0);
						  if(downloader->dlBody == true && curtime != downloader->time){
							  stateCallback stateinfo;
							  stateinfo.type = 3;
							  stateinfo.speed = speed;
							  stateinfo.downloadingSize = dlnow + downloader->downloadedSize;
							  stateinfo.totalSize = downloader->totalSize;
							  downloader->setCallbackState(&stateinfo);
							  downloader->time = curtime;
							  if(dlnow == downloader->dlnow){
								  downloader->invalidDLCount++;
							  }
							  downloader->dlnow = dlnow;

						  }

						  if(downloader-> v78 !=0  && downloader->invalidDLCount > 0x3C)
							  return 1;
						  return 0;
}
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