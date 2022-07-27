#define CURL_STATICLIB
#define _CRT_SECURE_NO_WARNINGS
#include <curl.h>
#include <stdio.h>
#include <string.h>


void   parseinfo(char* info)
{
    size_t len = strlen(info);
    printf("These are 5 words that rhyme:");
    for (int i = 0; i < len; i++)
    {
        if (info[i] == 'w' && info[i + 1] == 'o' && info[i + 2] == 'r' && info[i + 3] == 'd')
            i += 4;
        if (info[i] == 's' && info[i + 1] == 'c' && info[i + 2] == 'o' && info[i + 3] == 'r' && info[i + 4] == 'e')
            i += 5;
        if (info[i] == 'n' && info[i + 1] == 'u' && info[i + 2] == 'm')
            i += 12;
        if (info[i] == ':')
            i++;
        if (info[i] == '"')
            printf(" ");
        if (info[i] >= 'a' && info[i] <= 'z')
            printf("%c", info[i]);
    }
    
}

char* getword(void)
{
    char word[15];
    printf("Pick a word to find rhymes\n");
    scanf_s("%s", &word,15);
    return (word);
}

char* makeFullUrl(char* s1, char* s2)
{
    char max[] = "&max=5";
    size_t apilen = strlen(s1);
    size_t wordlen = strlen(s2);
    size_t fullurllen = wordlen + apilen;
    size_t maxlen = strlen(max);
    char* fullurl = malloc(sizeof(char) * fullurllen + maxlen + 1);
    if (!fullurl)
    {
        printf("ERROR: url not created.\n");
        return (1);
    }
    strcat(s1, s2);
    strcpy(fullurl, s1);
    strcat(fullurl, max);
    return (fullurl);
}

size_t got_data(char* buffer, size_t itemsize, size_t nitems, void* ignorethis)
{
    size_t bytes = itemsize * nitems;
    parseinfo(buffer);
    return (bytes);
}

int main(int ac, char** av)
{
    char api[100] = "https://api.datamuse.com/words?rel_rhy=";
    char* word = getword();
    char* fullurl = makeFullUrl(api, word);
    
    if (!word)
    {
        printf("ERROR: Word not got or url not created\n");
        return (1);
    }
   
    // initializing curl
     CURL* curl = curl_easy_init();
    if (!curl)
    {
        printf("Curl init failed\n");
        return (EXIT_FAILURE);
    }
    // set options
    curl_easy_setopt(curl, CURLOPT_URL, fullurl);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);


    // perform action
    CURLcode result = curl_easy_perform(curl);

    if (result != CURLE_OK)
    {
        printf("Download problem: %s\n", curl_easy_strerror(result));
    }
    curl_easy_cleanup(curl);
    free(fullurl);
}


