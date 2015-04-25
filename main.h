/* main.h - Noah Sandman
   04/24/2014 */

static std::string curlResult;

static size_t curlToVar(void *ptr, size_t size, size_t nmemb, ...) {
	curlResult.append((char*)ptr, size*nmemb);
	return size * nmemb;
}

/* This is just a quick function to clear cURL results */
void cc(void) {
	curlResult.clear();
}