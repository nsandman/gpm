/* main.h - Noah Sandman
   04/24/2015 */

static std::string curlResult;

std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

static size_t curlToVar(void *ptr, size_t size, size_t nmemb, ...) {
	curlResult.append((char*)ptr, size*nmemb);
	return size * nmemb;
}

/* This is just a quick function to clear cURL results */
void cc(void) {
	curlResult.clear();
}