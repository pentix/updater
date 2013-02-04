/*
 * Copyright (c) 2013 Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
*/


#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "updater.h"


/**
 * @brief Get a file from the internet
 * @param url The full url to the file
 * @param localpath The path where the file should be downloaded to
 * @return 0 if no errors occurred
 */
char getfile(char *url, char *localpath){
	if(url == NULL || strlen(url) == 0 || localpath == NULL || strlen(localpath) == 0)
		return 1;

	FILE *localfile = fopen(localpath, "w");
	if(localfile == NULL)
		return 1;

	CURL *instance = curl_easy_init();
	if(!instance){
		fclose(localfile);
		return 1;
	}
	
	curl_easy_setopt(instance, CURLOPT_URL, url);
	curl_easy_setopt(instance, CURLOPT_WRITEDATA, localfile) ;
	
	CURLcode returncode = curl_easy_perform(instance);
	curl_easy_cleanup(instance);
	fclose(localfile);
	
	if(returncode != CURLE_OK)
		return 1;
	
	return 0;
}


/**
 * @brief Main function
 * @return 0 if no errors occurred
 */
int main(void){
	
	return 0;
}