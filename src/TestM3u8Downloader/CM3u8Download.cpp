#include "CM3u8Download.h"
#include <string>
#include <map>
#include "polarssl/aes.h"
#include "utils.h"
#include "log.h"


CM3u8Download::CM3u8Download()
{
	curl_global_init(CURL_GLOBAL_ALL); 
}



void CM3u8Download::download()
{
	LogD(Info, "============================ %s ==> Start Download ============================", this->guid.c_str());

	string subUrl;
	string url;
	string cookies;
	string receiveData;
	string strLastCookies;
	string strIV;
	string videoData;
	std::vector<std::vector<std::string>> regexResult;
	std::vector<std::pair<int,std::string>> tsPairSet;
	std::vector<std::pair<std::string,std::string>> IVPairSet;
	std::map<std::string,std::string> urlInfos;
	http hUrl;
	bool findEXT_X_STREAM = 0;
	int RESOLUTION = 0;
	int BANDWIDTH = 0;
	bool EXTINF = 0;
	int resolution_url_index = 0;
	int bandWidth_url_index = 0;
	bool find_resolution = 0;
	int EXTINF_value = 0;
	long long totalEXTINF = 0;
	int acumExtInfo = 0;


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

	this->setCallbackState(stateinfo);

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
			this->setCallbackState(stateinfo);
			return;
		}

		RegexExec(receiveData,"([^\n\r]+)",regexResult);

		int index =0;
		int index2 = 0;

		if(regexResult.size() == 0 ){
			stateinfo.type = 1;
			stateinfo.speed = dlProfileRet;
			this->setCallbackState(stateinfo);
			return;
		}



		do{
			std::vector<std::vector<std::string>> regexEXTINF;
			std::vector<std::vector<std::string>> regexResolution;
			string strItem = regexResult[index2][0];

			if(strItem.size() != 0){
				if( strItem.size() >= 17){
					if(strstr(strItem.c_str(),"#EXT-X-STREAM-INF")!=0 && strItem.rfind("#EXT-X-STREAM-INF",0)==0){
						findEXT_X_STREAM =1;
						LogD(Info,"%s ==> %s",this->guid.c_str(),strItem.c_str());	
					}
				}
				if(strItem.size() != 0 && strItem.size() >= 11 &&strItem.find("#EXT-X-KEY:")==0){
					// 0x00000001800027B5
					if( strItem.find("METHOD=AES-128") != -1 &&  strItem.find("METHOD=AES-128") != 0){
						std::vector<std::vector<std::string>> urlRegex;
						string  subUrl;
						if(RegexExec(strItem,"URI=\"([^\"]+)\"",urlRegex)){
							subUrl = urlRegex[0][1];
						}
						std::vector<std::vector<std::string>> IVRegex;
						if(RegexExec(strItem,"IV=0x([0-9A-Za-z]+)",IVRegex)){
							strIV = IVRegex[0][1];
						}
						if(subUrl.size()!=0){
							if(urlInfos.find(subUrl) ==  urlInfos.end()){
								string selectUrl = subUrl;
								string strCookieTemp = strLastCookies;
								if(selectUrl.data()[0] == '/'){
									// 0x0000000180002CD1
									http hUrl = parseURL(url);
									selectUrl = hUrl.protocol+"://"+hUrl.hostname + selectUrl;
								}
								else if(selectUrl.find("http")!=0){
									// 0x0000000180002CD1
									string strUrlPath = url.substr(0,url.find_last_of("/")) ;
									selectUrl = strUrlPath + std::string("/") + selectUrl;
								}
								if(this->v78==0 && (dlProfileRet = downloadSegment(selectUrl,&strCookieTemp,&videoData))==0){

								}
								else{
									stateinfo.type = 1;
									stateinfo.speed = dlProfileRet;
									this->setCallbackState(stateinfo);
									return;
								}



							}
							urlInfos[subUrl] = videoData;
							
						}
						index++;
						index2++;
						continue;
					}

				}
				
			}

			if(strItem.find("#EXTINF:") == 0 && RegexExec(strItem,"#EXTINF:([0-9\\.]+)",regexEXTINF)){
				EXTINF = 1;

				EXTINF_value = atof(regexEXTINF[0][1].c_str()) * 1000;
				index++;
				index2++;
				continue;
			}
			else if(strItem.find("RESOLUTION") == std::string::npos && strItem.find("BANDWIDTH") == std::string::npos){
				if(strItem.find("#") != 0 && findEXT_X_STREAM == 0 && EXTINF!=0){
					if( strItem.c_str()[0] == '/'){
						// 0x000000018000331B
						http hUrl = parseURL(url);
						strItem = hUrl.protocol+"://"+hUrl.hostname + strItem;
					}
					else if(strItem.find("http")!=0){
						string strUrl = url;
						string strUrlPath = strUrl.substr(0,strUrl.find_last_of("/")) ;
						strItem = strUrlPath + std::string("/") + strItem;
					}

					tsPairSet.push_back(std::make_pair(EXTINF_value,strItem));
					IVPairSet.push_back(std::make_pair(videoData,strIV));

					totalEXTINF+=EXTINF_value;
					EXTINF = false;
				}
				index++;
				index2++;
				continue;
			}
			find_resolution = strItem.find("RESOLUTION")!= string::npos?1:0;

			std::vector<std::vector<std::string>> regexBANDWIDTH;

			if(find_resolution){
				// 0x000000018000385A
				if(RegexExec(strItem,"RESOLUTION=[0-9]+x([0-9]+)",regexResolution) && RESOLUTION < atoi(regexResolution[0][1].c_str())){
					RESOLUTION = atoi(regexResolution[0][1].c_str());
				resolution_url_index = index +1;
				}
		
			}
			else if(RegexExec(strItem,"BANDWIDTH=([0-9]+)",regexBANDWIDTH)){
				if(BANDWIDTH < atoi(regexBANDWIDTH[0][1].c_str())){
					BANDWIDTH = atoi(regexBANDWIDTH[0][1].c_str());
					bandWidth_url_index = index +1;
				}
			}
			index++;
			index2++;
		}while(index < regexResult.size());

		if(findEXT_X_STREAM==0){
			// 0x0000000180004223
			break;
		}
		string strLastCookies2 = strLastCookies;
		findEXT_X_STREAM =0;
		string strItem = "";
		if( find_resolution)
			strItem = regexResult[resolution_url_index][1];
		else
			strItem = regexResult[bandWidth_url_index][1];

		if(strItem.c_str()[0] == '/'){
			// 0x0000000180003D75
			 hUrl = parseURL(url);
			strItem = hUrl.protocol+"://"+hUrl.hostname + strItem;

		}
		else if(strItem.find("http")!=0){

			string strUrlPath = url.substr(0,url.find_last_of("/")) ;
			strItem = strUrlPath + std::string("/") + strItem;

		}

		this->dlBody = 0;
		if(this->v78 || this->downloadSegment(strItem,&strLastCookies2,&receiveData)!=0)
			break;
		this->dlBody=1;
		url = strItem;

	}
	if(tsPairSet.size() == 0){
		stateinfo.type = 1;
		stateinfo.speed = dlProfileRet;
		this->setCallbackState(stateinfo);
		return;
	}
	FILE * file = fopen(this->dsSavePath,"a+b");
	
	if(file == 0){
		// 0x0000000180004287
		stateCallback stateinfo;
		stateinfo.type=1;
		stateinfo.speed  =  23;
		this->setCallbackState(stateinfo);
		return ;
	}
	EndInfo endInfo = {0};
	if(this->flag1){
		fseek(file,0,2);
		int fpos = ftell(file);
		if( fpos > 24 ){
			// 0x0000000180004591
			

			fseek(file,fpos-24,0);
			fread(&endInfo,1,24,file);
			if(strstr(endInfo.sig,"WSDS")){
				if(endInfo.index > 0){
					if(endInfo.index < tsPairSet.size()){
						int i=0;
						while(i<endInfo.index){
							acumExtInfo += tsPairSet[i].first;
							 i++;
						}
						 this->downloadedSize = fpos-24;
						 this->totalSize += this->downloadedSize * totalEXTINF/acumExtInfo;
						
					}
				}	
			}
		}

	}
	if(endInfo.index==0){
		fseek(file,0,0);
		this->totalSize = totalEXTINF * 500 / 8;
	}

	this->time = 0;
	if(tsPairSet.size() > 0){
		int i=endInfo.index;
		while(true){
			EndInfo endinfo = {0};
			string strCookieTemp = strLastCookies;			
			string tsData="";
			int tsEXTInfo = tsPairSet[i].first;
			string tsUrl = tsPairSet[i].second;
			videoData = IVPairSet[i].first;
			string strItem = tsUrl;
			string protocol = GetProtocol(url);
			if(protocol == ""){
				protocol = "http";
			}
			if(strItem.c_str()[0] == '/'){
				// 0x00000001800049D1
				strItem = hUrl.protocol+"://"+hUrl.hostname + strItem;

			}
			else if(strItem.find("http")!=0){
				// 0x0000000180004AE7
				if(strItem.find("://")==0){
					strItem = protocol + strItem;
				}
				else if(strItem.find("//")==0){
					strItem = protocol +":"+strItem;
				}
				else{
					strItem =  url.substr(0,url.find_last_of("/")) + "/" + strItem;
				}

			}
			if(this->v78){
				return ;
			
			}
			int retCode = this->downloadSegment(strItem,&strCookieTemp,&tsData);
				if(retCode){
					// 0x000000018000533A
					if(this->v78 == 0){
						stateCallback stateinfo;
						stateinfo.type=1;
						stateinfo.speed  =  this->invalidDLCount > 60 ? 28 : retCode;
						this->setCallbackState(stateinfo);
						fclose(file);
						return ;
					}
				}
				this->downloadedSize += tsData.size();
				acumExtInfo+=	tsEXTInfo;
				this->totalSize = this->downloadedSize * totalEXTINF/ acumExtInfo;
				fseek(file,0,2);
				int fpos = ftell(file);
				if( fpos > 24 ){
					fseek(file,fpos-24,0);
					fread(&endinfo,1,24,file);
					fseek(file,-24,1);
				}
				size_t writeSize = 0;				
				if(videoData.size()!=0){
					// 0x0000000180004E9F
					 string IV = strIV;
					 char * decryptData = new char[tsData.size()];
					unsigned char * IVBin = new unsigned char[16];
					hex2Bin(IV,IVBin);

					aes_context aes_ctx = {0};
					aes_ctx.nr = 16;

					aes_setkey_dec(&aes_ctx, (const unsigned char *)videoData.c_str(), 128);
					aes_crypt_cbc(&aes_ctx,AES_DECRYPT,tsData.size(),IVBin,(const unsigned char *)tsData.c_str(),(unsigned char *)decryptData);
					writeSize = fwrite(decryptData,1,tsData.size(),file);
					delete [] decryptData;
					delete [] IVBin;
	
				}
				else{
					writeSize = fwrite(tsData.c_str(),1,tsData.size(),file);
				}
				if(writeSize != tsData.size()) {
					// 0x1800051E2
					stateCallback stateinfo;
					stateinfo.type=1;
					stateinfo.speed  =  retCode;
					this->setCallbackState(stateinfo);
					fclose(file);
					return ;
				
				}

				if(i+1<tsPairSet.size()){
					strcpy(endinfo.sig,"WSDS");
					 endinfo.index = i+1;
					 endinfo.size = acumExtInfo;
					fwrite(&endinfo,1,24,file);
					fflush(file);
				}
				i++;
			if(i>= tsPairSet.size()) break;
		}
	}


	fclose(file);
	rename(this->dsSavePath,this->savePath);
	stateinfo.type = 5;
	this->setCallbackState(stateinfo);
	LogD(Info, "============================ %s ==> End Of Download ============================", this->guid.c_str());
}


size_t write_callback(char *ptr, size_t size, size_t nmemb,string * pReceiveData){
	pReceiveData->append(ptr,nmemb);
	return nmemb*size;
}

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
							  downloader->setCallbackState(stateinfo);
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
				if(!RegexExec(strLastUrl,"[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}",ipMatch)){
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