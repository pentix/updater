/*
 * Copyright (c) 2013 Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * 
 * License: MIT
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
 * @param url The full (HTTP) url to the file
 * @param localfile File stream in which the download should be written
 * @return 1 if no errors occurred; 0 otherwise 
 */
char getfile(char *url, FILE *localfile){
	if(url == NULL || strlen(url) == 0 || localfile == NULL){
		fprintf(stderr, "Could not start download!\n");
		return 0;
	}

	CURL *instance = curl_easy_init();
	if(!instance){
		fclose(localfile);
		return 0;
	}
	
	curl_easy_setopt(instance, CURLOPT_URL, url);
	curl_easy_setopt(instance, CURLOPT_WRITEDATA, localfile) ;
	CURLcode returncode = curl_easy_perform(instance);
	
	// Clean up
	curl_easy_cleanup(instance);
	
	if(returncode != CURLE_OK)
		return 0;
	
	return 1;
}


/**
 * @brief Main function
 * @return 0 if no errors occurred
 */
int main(void){
	// Print some informations
	printf("%s\n               Updater %s\n    %s\n%s\n\n", UPDATER_BOX, UPDATER_VERSION, UPDATER_WEB, UPDATER_BOX);

	
	// Open configuration file
	FILE *configfile = fopen(CONFIG_PATH, "r");
	if(configfile == NULL){
		fprintf(stderr, "Could not read updater configuration file!\n");
		return 1;
	}

	char localversion[64], latestversion[64];
	char url[1024];
	
	fscanf(configfile, "%64[^\n]", localversion);
	fscanf(configfile, "%s", url);
	fclose(configfile);
	
	if(strlen(localversion) == 0 || strlen(url) == 0){
		fprintf(stderr, "Corrupt updater configuration file!\n");
		return 1;
	}
	
	FILE *versionfile = fopen(TMP_FILE, "w");
	
	// Download versionfile
	if(!getfile(url, versionfile))
		return 1;
	
	// Make sure the cache is written to the disk!
	// Note: fflush() did not work here!?!
	freopen(TMP_FILE, "r", versionfile);
	
	// Now read the file
	fscanf(versionfile, "%64[^\n]", latestversion);
	
	if(strlen(latestversion) == 0){
		fprintf(stderr, "Corrupt updater version file!\n");
		fclose(versionfile);
		return 1;
	}
	
	// Print versions
	printf("Local version:   %s\n", localversion);
	printf("Latest version:  %s\n\n", latestversion);
	
	// Compare versions
	if(strcmp(localversion, latestversion) == 0)
		printf("No update available.");
	else{
		printf("An update is available!\n");
		
		if(CHECK_FOR_BETA && strlen(latestversion) > 3){
			if(latestversion[strlen(latestversion)-4] == 'b' && 
			   latestversion[strlen(latestversion)-3] == 'e' && 
			   latestversion[strlen(latestversion)-2] == 't' && 
			   latestversion[strlen(latestversion)-1] == 'a')
			{
				printf("Note: Latest version is a beta version. Update on your own risk!\n");
			}
		}
	}
	
	fclose(versionfile);
	remove(TMP_FILE);
	
	return 0;
}